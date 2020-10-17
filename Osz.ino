#define BLYNK_PRINT Serial
#include <Blynk.h>
#include <SPI.h>
#include <BlynkSimpleEthernet.h>
#include <Servo.h>
#include <TimeLib.h>
#include <WidgetRTC.h>

char auth[] = "XXXXXXXXXXXXXXXXX";

#define W5100_CS  10
#define SDCARD_CS 4

WidgetTerminal terminal(V5);
BlynkTimer timer;
WidgetRTC rtc;

Servo csap;

String ido;
int last_position;
bool ir = false;
bool af = false;
BLYNK_WRITE(V2)
{
  if (param.asInt() == 0)
  {
    csap.write(-45);
    terminal.println("Zárva.");
  }
  if (param.asInt() == 1)
  {
    csap.write(45);
    terminal.println("Nyitva.");
  }
  terminal.flush();
}

BLYNK_WRITE(V5)
{
  if (String("Servo ki") == param.asStr())
  {
    servo_off();
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

void orakiiras() {
  ido = String(hour()) + ":" + minute();
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
  Blynk.virtualWrite(V2, 0);
  Blynk.virtualWrite(V0, 0);
}

void loop()
{
  Blynk.run();
  timer.run();
  orakiiras();
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
