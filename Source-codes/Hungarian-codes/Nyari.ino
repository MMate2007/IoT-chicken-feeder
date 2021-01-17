#define BLYNK_PRINT Serial
#include <Blynk.h>
#include <SPI.h>
#include <BlynkSimpleEthernet.h>
#include <Servo.h>
#include <TimeLib.h>
#include <WidgetRTC.h>

char auth[] = "XXXXXXXX";

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
    terminal.println("Aut. etetés bekapcsolva!");
  }
  if (param.asInt() == 0)
  {
    af = false;
    terminal.println("Aut. etetés kikapcsolva!");
  }
  terminal.flush();
}
BLYNK_WRITE(V1)
{
  if (param.asInt() == 1)
  {
   etetes(1);
  terminal.println("Etetés...");
  terminal.flush();
  }
}


BLYNK_WRITE(V2)
{
  if (param.asInt() == 0)
  {
    csap.write(-45);
    terminal.println("Csap zárása.");
  }
  if (param.asInt() == 1)
  {
    csap.write(45);
    terminal.println("Csap nyitása.");
  }
  terminal.flush();
}


BLYNK_WRITE(V3)
{
  if (param.asInt() == 0)
  {
   digitalWrite(6, LOW);
    terminal.println("Mozgásérzékelő kikapcsolva.");
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
  /*if (String("Digitális PIN leolvasása") == param.asStr())
  {
    elozo_parancs = "Digitális PIN leolvasása";
    terminal.println("Add meg a PIN számát!");
  }*/
  /*if (String("Analóg PIN leolvasása") == param.asStr())
  {
    elozo_parancs = "Analóg PIN leolvasása";
    terminal.println("Add meg a PIN számát! Pl.: A0");
  }*/
  if (String("PIN be") == param.asStr())
  {
    elozo_parancs = "PIN bekapcsolása";
    terminal.println("Add meg a PIN számát!");
  }
  if (String("PIN ki") == param.asStr())
  {
    elozo_parancs = "PIN kikapcsolása";
    terminal.println("Add meg a PIN számát!");
  }
  /*if(String("Idő lekérése") == param.asStr())
  {
    orakiiras();
    terminal.println(ido);
  }*/
  if (String("Servo ki") == param.asStr())
  {
    servo_off();
  }
  if (String("Servo be") == param.asStr())
  {
    servo_on();
  }
  /*if (String("Etetés(2)") == param.asStr())
  {
    etetes(2);
    terminal.println("Etetés...");
  }
  if (String("Etetés(3)") == param.asStr())
  {
    etetes(3);
    terminal.println("Etetés...");
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
  terminal.println("Mozgásérzékelő bekapcsolva.");
}

void orakiiras() {
  ido = String(hour()) + ":" + minute();
}
void servo_on() {
  csap.write(last_position);
  csap.attach(3);
  terminal.println("Szervo motor bekapcsolva!");
  delay(10000);
}
void servo_off() {
  delay(10000);
  last_position = csap.read();
  csap.detach();
  terminal.println("Szervo motor kikapcsolva!");
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
      terminal.print("Autómata etetés történt ekkor: ");
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
      terminal.println("Mozgásérzékelő kikapcsolva.");
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
