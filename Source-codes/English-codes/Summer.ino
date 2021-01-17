#define BLYNK_PRINT Serial
#include <Blynk.h>
#include <SPI.h>
#include <BlynkSimpleEthernet.h>
#include <Servo.h>
#include <TimeLib.h>
#include <WidgetRTC.h>

char auth[] = "XXXXXXXX"; //Please type in the auth token!

#define W5100_CS  10
#define SDCARD_CS 4

WidgetTerminal terminal(V5);
//WidgetLED blokkolas(V7);
BlynkTimer timer;
WidgetRTC rtc;

Servo csap;

String elozo_parancs;
String ido;
bool vilagos = true;
bool modosithato = false;
bool af = false;
bool pir = false;
int last_position;
bool ir = false;

BLYNK_WRITE(V4){
  if (param.asInt() == 1)
  {
    modosithato = true;
    terminal.println("Aut. mozg. kapcs. bekapcsolva!");
  }
  if (param.asInt() == 0)
  {
    modosithato = false;
   terminal.println("Aut. mozg. kapcs. kikapcsolva!");
  }
 terminal.flush();
}

BLYNK_WRITE(V0)
{
  if (param.asInt() == 1)
  {
    af = true;
    terminal.println("Aut. feeding enabled!");
  }
  if (param.asInt() == 0)
  {
    af = false;
    terminal.println("Aut. feeding disabled!");
  }
  terminal.flush();
}
BLYNK_WRITE(V1)
{
  if (param.asInt() == 1)
  {
   etetes(1);
  terminal.println("Feeding...");
  terminal.flush();
  }
}


BLYNK_WRITE(V2)
{
  if (param.asInt() == 0)
  {
    csap.write(-45);
    terminal.println("Close.");
  }
  if (param.asInt() == 1)
  {
    csap.write(45);
    terminal.println("Open.");
  }
  terminal.flush();
}


BLYNK_WRITE(V3)
{
  if (param.asInt() == 0)
  {
   digitalWrite(6, LOW);
    terminal.println("Motion decetion disabled.");
    pir = false;
  }else if(param.asInt() == 1)
  {
    mbe();
  }
  terminal.flush();
}


BLYNK_WRITE(V5)
{
  /*if (elozo_parancs == "Digitális PIN leolvasása")
  {
    terminal.println(digitalRead(param.asInt()));
    elozo_parancs = "";
  }*/
  /*if (elozo_parancs == "Analóg PIN leolvasása")
  {
    terminal.println(analogRead(param.asInt()));
    elozo_parancs = "";
  }*/
  if (elozo_parancs == "PIN bekapcsolása")
  {
    digitalWrite(param.asInt(), HIGH);
    elozo_parancs = "";
  }
  if (elozo_parancs == "PIN kikapcsolása")
  {
    digitalWrite(param.asInt(), LOW);
    elozo_parancs = "";
  }
  /*if (String("Read digital PIN") == param.asStr())
  {
    elozo_parancs = "Digitális PIN leolvasása";
    terminal.println("Add meg a PIN számát!");
  }*/
  /*if (String("Read analog PIN") == param.asStr())
  {
    elozo_parancs = "Analóg PIN leolvasása";
    terminal.println("Add meg a PIN számát! Pl.: A0");
  }*/
  if (String("PIN on") == param.asStr())
  {
    elozo_parancs = "PIN bekapcsolása";
    terminal.println("Please type in the PIN's numbe!");
  }
  if (String("PIN off") == param.asStr())
  {
    elozo_parancs = "PIN kikapcsolása";
    terminal.println("Please type in the PIN's number!");
  }
  /*if(String("Get time") == param.asStr())
  {
    orakiiras();
    terminal.println(ido);
  }*/
  if (String("Servo off") == param.asStr())
  {
    servo_off();
  }
  if (String("Servo on") == param.asStr())
  {
    servo_on();
  }
  /*if (String("Feed(2)") == param.asStr())
  {
    etetes(2);
    terminal.println("Feeding...");
  }
  if (String("Feed(3)") == param.asStr())
  {
    etetes(3);
    terminal.println("Feeding...");
  }*/
  terminal.flush();
}

BLYNK_CONNECTED() {
  rtc.begin();
}


void etetes(int merteke){
  if (csap.attached() == false)
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
    if (ido == "ki")
    {
      servo_off();
      orakiiras();
    }
}

void mbe() {
  digitalWrite(6, HIGH);
  //blokkolas.on();
  delay(5000);
  //blokkolas.off();
  pir = true;
  terminal.println("Motion decetion enabled.");
}

void orakiiras() {
  ido = String(hour()) + ":" + minute();
}
void servo_on() {
  csap.write(last_position);
  csap.attach(3);
  terminal.println("Servo turned on!");
  delay(10000);
}
void servo_off() {
  delay(10000);
  last_position = csap.read();
  csap.detach();
  terminal.println("Servo turned off!");
}

void setup() {
  Serial.begin(9600);
  pinMode(SDCARD_CS, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(2, INPUT);
  pinMode(6, OUTPUT);
  pinMode(5, INPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  digitalWrite(6, LOW);
  digitalWrite(SDCARD_CS, HIGH);
  csap.attach(3);
  Blynk.begin(auth);
  setSyncInterval(30*60);
  timer.setInterval(10000L, orakiiras);
  //csap.write(30);
  etetes(1);
  csap.write(-45);
}

void loop()
{
  Blynk.run();
  timer.run();
  orakiiras();
  if (pir == true)
  {
    if(digitalRead(2) == HIGH)
    {
      etetes(1);
      //blokkolas.on();
      delay(60000);
      delay(60000);
      delay(60000);
      delay(60000);
      delay(60000);
      //blokkolas.off();
    }
  }
  if (af == true)
  {
    orakiiras();
    if (ido == "9:0" || ido == "11:0" || ido == "19:0" || ido == "13:30")
    {
      etetes(2);
      ir = true;
    }
    if (ido == "10:0" || ido == "20:0")
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
      terminal.print("Automatic feeding at this time: ");
      terminal.println(ido);
      terminal.flush();
      delay(60000);
      ir = false;
    }
  }
  if (modosithato == true)
  {
    if (ido == "20:2")
    {
      vilagos = false;
      Blynk.virtualWrite(V3, 0);
      digitalWrite(6, LOW);
      pir = false;
      terminal.println("Motion detection disabled.");
    }
    if (ido == "7:2")
    {
      vilagos = true;
      Blynk.virtualWrite(V3, 1);
      mbe();
    }
  }
  if (ido == "20:5")
  {
    servo_off();
  }
  if (ido == "6:55")
  {
    servo_on();
  }
}
