#define BLYNK_PRINT Serial
#include <Blynk.h>
#include <SPI.h>
#include <BlynkSimpleEthernet.h>
#include <Servo.h>
#include <TimeLib.h>
#include <WidgetRTC.h>

char auth[] = "XXXXXXXXXXXXXXXX"; //Type here your auth-token!

#define W5100_CS  10
#define SDCARD_CS 4

WidgetTerminal terminal(V5);
BlynkTimer timer;
WidgetRTC rtc;

Servo csap;

String ido;
int last_position;
String reg;
String est;
String ei[] = {"6:30", "7:0", "8:0", "9:0", "10:45", "12:0", "13:45", "14:55", "15:55", "16:55", "17:55", "18:55", "19:55"}; //Here you can change the feeding time.
int i;
int v = 1500;

BLYNK_WRITE(V8) {
  v = param.asInt();
  terminal.println("Value: OK.");
}

BLYNK_WRITE(V7) {
  TimeInputParam t(param);
  if (t.hasStartTime())
  {
    reg = String(t.getStartHour()) + ":" + t.getStartMinute();
  }

  if (t.hasStopTime())
  {
  est = String(t.getStopHour()) + ":" + t.getStopMinute();
  }
  terminal.println("Time: OK.");
  terminal.flush();
}

BLYNK_WRITE(V1)
{
  if (param.asInt() == 1)
  {
   etetes(v);
   terminal.println("Feeding...");
  }
}

BLYNK_WRITE(V2)
{
  if (param.asInt() == 0)
  {
    csap.write(-45);
  }
  if (param.asInt() == 1)
  {
    csap.write(45);
  }
  terminal.println("Tap: OK.");
  terminal.flush();
}

BLYNK_CONNECTED() {
  rtc.begin();
}

void etetes (int mertek) {
  csap.write(45);
  delay(mertek);
  csap.write(-45);
}

void orakiiras() {
  ido = String(hour()) + ":" + minute();
}

void servo_on() {
  csap.write(last_position);
  csap.attach(3);
  terminal.println("M. on!");
  delay(10000);
  terminal.flush();
}
void servo_off() {
  delay(10000);
  last_position = csap.read();
  csap.detach();
  terminal.println("M. off!");
  terminal.flush();
}

void setup() {
  Serial.begin(9600);
  pinMode(SDCARD_CS, OUTPUT);
  digitalWrite(SDCARD_CS, HIGH);
  pinMode(3, OUTPUT);
  csap.attach(3);
  Blynk.begin(auth);
  setSyncInterval(30*60);
  timer.setInterval(10000L, orakiiras);
  orakiiras();
  Blynk.syncVirtual(V0);
  Blynk.syncVirtual(V7);
  Blynk.syncVirtual(V8);
  etetes(500);
}

void loop()
{
  Blynk.run();
  timer.run();
  orakiiras();
  if (ido == reg)
  {
    servo_on();
    delay(60000);
  }
  i = 0;
  while (i < 14)
  {
    if (ido == ei[i])
    {
      etetes(v);
      terminal.print(ido);
      terminal.println(" -> Feeding.");
      terminal.flush();
      delay(60000);
    }
    i++;
  }
  if (ido == est)
  {
    servo_off();
    delay(60000);
  }
}
