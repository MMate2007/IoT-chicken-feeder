#define BLYNK_PRINT Serial
//Könyvtárak inicializálása
#include <Blynk.h>
#include <SPI.h>
#include <BlynkSimpleEthernet.h>
#include <Servo.h>
#include <TimeLib.h>
#include <WidgetRTC.h>

char auth[] = "XXXXXXXXXXXXXXXXXXXXXX";//Kérem írja be ide a Blynk-től kapott auth-token-ját!
//Inicializálás
#define W5100_CS  10
#define SDCARD_CS 4

WidgetTerminal terminal(V5);
BlynkTimer timer;
WidgetRTC rtc;
//Szervo motor inicializálása
Servo csap;
//Változók inicializálása
String ido;
int last_position;
bool ir = false;
//Ezek a metódusok arra valók, hogy ha a zárójelben lévő PIN értéke módosul akkor fut le a kapcsos zárójelek között lévő kód
BLYNK_WRITE(V2)
{
  if (param.asInt() == 0)//Ha 0 V2 értéke akkor,
  {
    csap.write(-45);//Szervo motor mozgatása és
    terminal.println("Zárva.");//Kiírás a terminálra.
  }
  if (param.asInt() == 1)
  {
    csap.write(45);
    terminal.println("Nyitva.");
  }
  terminal.flush();//Terminál frissítése(szöveg küldése a terminálra).
}

BLYNK_WRITE(V5)
{
  if (String("Servo ki") == param.asStr())//Ha terminál értéke "Servo ki"
  {
    servo_off();//Akkor fusson le ez a metódus
  }
  if (String("Servo be") == param.asStr())
  {
    servo_on();
  }
  terminal.flush();
}

BLYNK_CONNECTED() {
  rtc.begin();
}
//Saját metódus az etetésre
void etetes(int merteke){
  if (csap.attached() == false)//Ha a motor ki van kapcsolva akkor kapcsoljuk be!
  {
    servo_on();//Saját metódus, definiálás később
    ido = "ki";//Ezen változó segítségével tudjuk majd meg etetés után, hogy a motor ki volt-e kapcsolva
  }
    switch(merteke)//Etetés a megadott mértéknek megfelelően
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
    if (ido == "ki")//Ha ki volt kapcsolva a motor etetés előtt akkor most is kikapcsoljuk.
    {
      servo_off();//Saját metódus a motor kik, definiálás
      orakiiras();
    }
}

void orakiiras() {
  ido = String(hour()) + ":" + minute();//Egy változóba mentem az időt
}
void servo_on() {
  csap.write(last_position);
  csap.attach(3);
  terminal.println("Motor be!");
  delay(10000);
}
void servo_off() {
  delay(10000);
  last_position = csap.read();
  csap.detach();
  terminal.println("Motor ki!");
}

void setup() {
  Serial.begin(9600);
  pinMode(SDCARD_CS, OUTPUT);
  pinMode(3, OUTPUT);
  digitalWrite(SDCARD_CS, HIGH);
  csap.attach(3);
  Blynk.begin(auth);
  setSyncInterval(30*60);
  timer.setInterval(10000L, orakiiras);
  etetes(1);
  csap.write(-45);
  orakiiras();
}

void loop()
{
  Blynk.run();
  timer.run();
  orakiiras();
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
    if (ido == "7:0" || ido == "12:0" || ido == "15:0" || ido == "17:30")
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
  if (ido == "18:5")
  {
    servo_off();
    delay(60000);
  }
  if (ido == "6:55")
  {
    servo_on();
    delay(60000);
  }
}
