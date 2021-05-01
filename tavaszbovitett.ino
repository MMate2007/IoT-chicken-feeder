#define BLYNK_PRINT Serial
#include <Blynk.h>
#include <SPI.h>
#include <BlynkSimpleEthernet.h>
#include <Servo.h>
#include <TimeLib.h>
#include <WidgetRTC.h>

char auth[] = "XXXXXXXXXXXXXXXXXXXX"; //Írja be ide a Blynk-től kapott kódot!

#define W5100_CS  10
#define SDCARD_CS 4

WidgetTerminal terminal(V5);
BlynkTimer timer;
WidgetRTC rtc;

Servo csap;

String ido;
int last_position;
bool nyitva = false;
int metetes = 1;
bool autetet = true;
BLYNK_WRITE(V2)
{
  terminal.print(ido);
  if (param.asInt() == 0)
  {
    csap.write(-45);
    terminal.println("-> Zárva.");
    nyitva = false;
  }
  if (param.asInt() == 1)
  {
    csap.write(45);
    terminal.println("-> Nyitva.");
    nyitva = true;
  }
  terminal.flush();
}
BLYNK_WRITE(V1)
{
  if (param.asInt() == 1)
  {
    etetes(metetes);
  }
}
BLYNK_WRITE(V6)
{
  switch (param.asInt())
  {
    case 1:
      metetes = 1;
      break;
    case 2:
      metetes = 2;
      break;
    case 3:
      metetes = 3;
      break;
  }
  terminal.print(ido);
  terminal.println("-> OK:");
  terminal.flush();
}

BLYNK_CONNECTED() {
  rtc.begin();
}

void etetes(int merteke) {
  switch (merteke)
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
  terminal.print(ido);
  terminal.println("-> Etetés");
  terminal.flush();
}
BLYNK_WRITE(V5)
{
  terminal.print(ido);
  terminal.print("-> ");
  if (String("Aut. etetés be") == param.asStr())
  {
    autetet = true;
    terminal.println("OK");
  }
  if (String("Aut. etetés ki") == param.asStr())
  {
    autetet = false;
    terminal.println("OK");
  }
  if (String("Motor be") == param.asStr())
  {
    terminal.println("...");
    terminal.flush();
    servo_on();
  }
  if (String("Motor ki") == param.asStr())
  {
    terminal.println("...");
    terminal.flush();
    servo_off();
  }
  if (String("Aut. etetés?") == param.asStr())
  {
    switch (autetet)
    {
      case true:
        terminal.println("Be van kapcsolva.");
        break;
      case false:
        terminal.println("Ki van kapcsolva.");
        break;
    }
  }
  terminal.flush();
}

void orakiiras() {
  ido = String(hour()) + ":" + minute();
}
void servo_on() {
  csap.write(last_position);
  csap.attach(3);
  terminal.println("Motor be!");
  delay(10000);
  terminal.flush();
}
void servo_off() {
  delay(10000);
  last_position = csap.read();
  csap.detach();
  terminal.println("Motor ki!");
  terminal.flush();
}

void setup() {
  Serial.begin(9600);
  pinMode(SDCARD_CS, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, INPUT_PULLUP);
  digitalWrite(5, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(SDCARD_CS, HIGH);
  csap.attach(3);
  Blynk.begin(auth);
  setSyncInterval(30 * 60);
  timer.setInterval(10000L, orakiiras);
  orakiiras();
}

void loop()
{
  Blynk.run();
  timer.run();
  orakiiras();
  if (digitalRead(9) == LOW)
  {
    terminal.print(ido);
    if (nyitva == true)
    {
      csap.write(-45);
      terminal.println("-> Zárva.");
      nyitva = false;
      Blynk.virtualWrite(V2, 0);
    } else if (nyitva == false)
    {
      csap.write(45);
      terminal.println("-> Nyitva.");
      nyitva = true;
      Blynk.virtualWrite(V2, 1);
    }
    terminal.flush();
    delay(1000);
  }
  if (autetet == true)
  {
    if (ido == "14:0" || ido == "9:0" || ido == "17:0")
    {
      etetes(1);
      delay(60000);
    }
    if (ido == "10:0" || ido == "15:0")
    {
      etetes(2);
      delay(60000);
    }
    if (ido == "8:0" || ido == "12:0")
    {
      etetes(3);
      delay(60000);
    }
  }
  if (ido == "17:5")
  {
    servo_off();
    delay(60000);
  }
  if (ido == "7:55")
  {
    servo_on();
    delay(60000);
  }
}
