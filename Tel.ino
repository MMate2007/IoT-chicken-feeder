#define BLYNK_PRINT Serial
//Könyvtárak inicializálása
#include <Blynk.h>
#include <SPI.h>
#include <BlynkSimpleEthernet.h>
#include <Servo.h>
#include <TimeLib.h>
#include <WidgetRTC.h>

char auth[] = "XXXXXXXXXXXX"; //Ide írja be a Blynk által küldött auth-token-t!

#define W5100_CS  10
#define SDCARD_CS 4

WidgetTerminal terminal(V5); //Terminál inicializálása
BlynkTimer timer;
WidgetRTC rtc;

Servo csap; //Szervo motor inicializálása
//Változók inicializálása
String ido;
int last_position;
bool ir = false;
bool af = false;
//A következő metódusok segítségével megadhatjuk, hogy mi történjen ha módosul a zárójelben levő PIN értéke.
BLYNK_WRITE(V2)
{
  if (param.asInt() == 0)//Ha a V2-es PIN értéke 0 akkor...
  {
    csap.write(-45);//Servo motor mozgatása
    terminal.println("Zárva.");//Írás a terminálra.
  }
  if (param.asInt() == 1)
  {
    csap.write(45);
    terminal.println("Nyitva.");
  }
  terminal.flush();//Kiírás a terminálra.
}

BLYNK_WRITE(V1)
{
  if (param.asInt() == 1)
  {
   etetes(1);//Etetés. Ez egy saját metódus melynek definíciója alul található.
   terminal.println("Etetés...");
  }
}

BLYNK_WRITE(V0)
{
  if (param.asInt() == 1)
  {
    af = true;
    terminal.println("Aut. et. be.");
  }
  if (param.asInt() == 0)
  {
    af = false;
    terminal.println("Aut. et. ki.");
  }
}

BLYNK_WRITE(V5)
{
  if (String("Servo ki") == param.asStr())
  {
    servo_off();//Saját metódus. Leírás alul.
  }
  if (String("Servo be") == param.asStr())
  {
    servo_on();//Saját metódus. Leírás alul.
  }
  terminal.flush();
}

BLYNK_CONNECTED() {
  rtc.begin();
}


void etetes(int merteke){ //Saját metódus definiálása az etetésre
  if (csap.attached() == false) //Ha a servo motor nincs csatlakoztatva akkor csatlakoztatjuk
  {
    servo_on();
    ido = "ki";
  }
    switch(merteke)
    {
      case 1 :
        csap.write(45);
        delay(500);
        csap.write(-45);
        break;
        
      case 2 :
      csap.write(45);
      delay(1500);
      csap.write(-45);
      break;
     case 3 :
     csap.write(45);
     delay(1500);
     delay(500);
     csap.write(-45);
     break;
    }
    if (ido == "ki")//Ha ki volt kapcsolva az etetés előtt a motor akkor etetés után is kikapcsoljuk.
    {
      servo_off();
      orakiiras();
    }
}

void orakiiras() {//Saját metódus mely kiírja egy változóba az órát és az időt.
  ido = String(hour()) + ":" + minute();
}
void servo_on() {//Szervo motor csatlakoztatása
  csap.write(last_position);//Szervo motor visszaállítása a kikapcsolás előtti pozícióba
  csap.attach(3);//Csatlakoztatás
  terminal.println("Motor be!");
  delay(10000);
}
void servo_off() {//Szervo motor lekapcsolása
  delay(10000);
  last_position = csap.read();
  csap.detach();//Lekapcsolás
  terminal.println("Motor ki!");
}
//A setup() metódus csak egyszer az Arduino indítása után fut le.
void setup() {
  Serial.begin(9600);//Soros monitor inicializálása
  pinMode(SDCARD_CS, OUTPUT);
  pinMode(3, OUTPUT);//Motor PIN-je a 3-mas ezen kapja az inpulzusokat, úgyhogy ez kimenet.
  digitalWrite(SDCARD_CS, HIGH);
  csap.attach(3);
  Blynk.begin(auth);
  setSyncInterval(30*60);
  timer.setInterval(10000L, orakiiras);
  //Servo motor mozgatása egy kicsit.
  etetes(1);
  csap.write(-45);
  orakiiras();
  //Applikáció szinkronizálása
  Blynk.virtualWrite(V2, 0);
  Blynk.virtualWrite(V0, 0);
}

void loop()
{
  Blynk.run();
  timer.run();
  orakiiras(); //Idő lekérése
  //Ha az autómata etetés be van kapcsolva akkor etetünk autómatán.
  if (af == true)
  {
    if (ido == "9:0" || ido == "11:0" || ido == "13:30")
    {
      etetes(2);
      ir = true;
    }
    if (ido == "10:0")
    {
      etetes(1);
      ir = true;
    }
    if (ido == "8:0" || ido == "12:0" || ido == "15:0" || ido == "16:0")
    {
      etetes(3);
      ir = true;
    }
    if (ir == true)
    {
      terminal.print("Aut. etetés ekkor: ");
      terminal.println(ido);
      terminal.flush();
      delay(60000);
      ir = false;
    }
  }
  //Szervo motor kikapcsolása este.
  if (ido == "16:5")
  {
    servo_off();
    delay(60000);
  }
  //Szervo motor bekapcsolása reggel.
  if (ido == "7:55")
  {
    servo_on();
    delay(60000);
  }
}
