# IoT tyúketető / IoT chicken feeder
The project's page: http://mmate.nhely.hu/feeder

## [Univerzalis.ino](https://github.com/MMate2007/IoT-chicken-feeder/blob/main/univerzalis.ino)
Ez a kód télen-nyáron használható, mert a reggel és este időpontokat lehet módosítani az alkalmazásból.

**Funkciók**:
- Csap vezérlése.
- Etetés az alkalmazásból manuálisan.
- Reggel-este időpontok beállítása.
- Alkalmankénti kiadott táplálék mennyiségének szabályzása.
- Autómatikus etetés.
------------------------------------------------------------------------------------------------------
This code could be use in the whole year because you can edit the morning and the evening time from the app.

**Features**:
- Controlling the tap.
- Feeding from the app manually.
- Morning-evening time settings.
- Food quantity edit.
- Auto feeding.
## [Tavaszbovitett.ino](https://github.com/MMate2007/IoT-chicken-feeder/blob/main/tavaszbovitett.ino)
Ezt a programot tavasszal érdemes használni.
*Vigyázat! Ezzel a programmal nem szabad az etetőt egyetlen etetőnek használni sokáig(3 nap), mert kevés gabonát ad naponta!*

**Funkciók**:
- Csap vezérlése.
- Etetés manuálisan.
  - Etetési fokozat választása (1-3).
- Etetőn lévő gombvezérlés.
  - Engedélyezés, letiltás a telefonos alkalmazásból.
- Automata etetés.
  - Automata etetés engedélyezése, tiltása a telefonos applikációból.

**Automata etetés ekkor**:
- 08:00 (3-as fokozat)
- 09:00 (1-es fokozat)
- 10:00 (2-es fokozat)
- 12:00 (3-as fokozat)
- 14:00 (1-es fokozat)
- 15:00 (2-es fokozat)
- 17:00 (1-es fokozat)
