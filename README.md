# IoT tyúketető
The project's page: http://mmate.nhely.hu/feeder - it's hungarian.
The English text is below.

---------------------------------------------------------
## Az etetőről
A projekt oldala: http://mmate.nhely.hu/feeder
Ezen etető segítségével etethetjük a tyúkokat távolról, mindössze internetkapcsolat és egy böngésző vagy a Blynk applikácó kell hozzá. Ha meg szeretné építeni az etetőt kérem olvassa el a [Built-it](https://github.com/MMate2007/IoT-chicken-feeder/blob/main/Build-it.md) nevű fájlt!

## Forráskódok: 
### [Osz-automata.ino](https://github.com/MMate2007/IoT-chicken-feeder/blob/main/Source-codes/Hungarian-codes/Osz-automata.ino)
Ez a kód ősszel hasznos. A következő szolgáltatásokat, funkciókat nyújtja ez a kód:
* Etetés autómatán
* Etetés manuálisan
* PIN-ek kezelése
* Servo motor reggel be-, este kikapcsolása
* Servo motor ki- és bekapcsolása manuálisan

A következő időpontokkor etet autómatán az etető ha ez a kód fut rajta:
* 7:00
* 9:00
* 10:00
* 11:00
* 12:00
* 13:30
* 15:00
* 17:30

### [Nyari.ino](https://github.com/MMate2007/IoT-chicken-feeder/blob/main/Source-codes/Hungarian-codes/Nyari.ino)
Ezt a kódot nyáron, nyár vége felé érdemes használni. Reggel és este autómatikusan be-, illetve lekapcsolja a szervo motort, ezzel is megnövelve az élettartamát.
Funkciók:
* Etetés mozgásérzékeléskor
* Autómata etetés megadott időpontokkor
* Autómatikus mozgásérzékelő kapcsolás reggel és este
* Autómatikus mozgásérzékelő kapcsolás szabályozása távolról
* Etetés távolról
* Mozgásérzékelő kapcsolása manuálisan
* Csap nyitása, zárása manuálisan
* Servo motor be- és kikapcsolása manuálisan

**Figyelem! Elképzelhető, hogy ez a kód csak néhány napig fut az Arduino UNO-n! Utána újra kell indítani az etetőt!**

----------------------------------------------------------------------------------------------------------------------
# IoT chicken feeder
With this feeder you can feed the hens remotely, all you need is an internet connection and a browser or the Blynk app.
The feeder knows the following:
* Feeding fully automatically at specified times
* Feeding manually

## Source codes:
### [Autumn-automatic-feeding.ino](https://github.com/MMate2007/IoT-chicken-feeder/blob/main/Source-codes/English-codes/Autum-automatic-feeding.ino)
With this code the feeder is feeding the hens at this times:
* 7.00
* 9.00
* 10.00
* 11.00
* 12.00
* 13.30
* 15.00
* 17.30
