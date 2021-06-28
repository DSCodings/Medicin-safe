/*
  --------------------------------------------------------------------------------------------------------------------
  Medicijnekluisje
  --------------------------------------------------------------------------------------------------------------------
  Project Medicijnekluisje voor MSReva in pelt.
  --------------------------------------------------------------------------------------------------------------------
  Dit is een project voor het controleren van gebruik van medicijnen.

   @author Sem Boonen en Daan Swinters
   @creator Daan Swinters
   @license Verboden te gebruiken zonder toestemming van de auteurs.
  --------------------------------------------------------------------------------------------------------------------
  De aansluitingen op arduino(UNO):
  -----------------------------------------------------------------------------------------
                MFRC522      Arduino
                Reader/PCD   Uno/101
    Signaal     Pin          Pin
    -----------------------------------------------------------------------------------------
    RST/Reset   RST          9
    SPI SS      SDA(SS)      10
    SPI MOSI    MOSI         11 / ICSP-4
    SPI MISO    MISO         12 / ICSP-1
    SPI SCK     SCK          13 / ICSP-3

                LCD_I2C      Arduino
                Reader/PCD   Uno/101
    Signaal     Pin          Pin
    -----------------------------------------------------------------------------------------
    SPI SS      SDA(SS)      A4
    SPI SCL     SCL          A5

                Servo
    Signaal     Pin          Pin
    -----------------------------------------------------------------------------------------
    pwm         pwm          3

                Buzzer
    Signaal     Pin          Pin
    -----------------------------------------------------------------------------------------
    pin3        pin3          3


                knoppen
                Uno/101
    sinnaal     pin
    ------------------------------------------------------------------------------------------
    btnboven    7
    btnonder    5
    btnliks     4
    btnrechs    6

   © Sem Boonen en Daan Swinters Medicijnekluisje 2021.
*/



#include "sleutel.h" //  Dit is een zelfgemaakte libary waar je allen sleutels kan bewaren.
#include "tijdvooropenen.h" //  Dit is een zelfgemaakte libary waar je allen sleutels kan bewaren.

#include <Servo.h> //  de servo libary.
Servo s3; // een servo toevoegen met de naam.


#include <Wire.h>  // voor te communiceren met het I2C scherm.
#include <LiquidCrystal_I2C.h> // voor de te printen op de i2c.

// Stel het LCD-adres in op 0x27 voor een display met 16 tekens en 2 regels.
LiquidCrystal_I2C lcd(0x27, 16, 2);



#include <MsTimer2.h> // de timer voor elke second 



#include <SPI.h> // voor de te communiceren met de rfid.
#include <RFID.h>
#define SS_PIN 10 // de pinnen defineren rfid.
#define RST_PIN 9
RFID rfid(SS_PIN, RST_PIN);
String rfidCard; // de rfidcard code
String Data55; // hier in word elke keer opgeslagen wanneer het kluisje word geopend voor verpleger.
String Data1 = "Verpleger: ";

String Data66;// hier in word elke keer opgeslagen wanneer het kluisje word geopend voor patient.
String Data2 = "Patient: ";


String versie = "v1.2"; // de versie van het medicijne kluisje.


int schrikkeljaar = 2020; // eerste schrikkeljaar dat is ingesteld(de dag/maand/jaar dat bij het begin wordt gegeven.)
int dag = 1; //  // dagen
int maand = 1; // maanden
int jaar = 2020; // jaren


int h = 0; // uren
int m = 0; //  minuten
int s = 0; //  seconden

int mmedicijn = 0;

int Li = 16; // display naar links scrollen
int Lii = 0;


int servotimer = 0; // de timer voor de deur terug te sluiten, wordt bij het begin op 0 gezet.
int gradenservodict = 90; // de graden voor de servo dicht.
int gradenservoopen = 15; // de graden voor de servo open.
int hoelangopen = 30; // hoeveel seconden de deur moet open zijn.



const int buttonboven = 7;  // pins van de 4 knoppen
const int buttonrechts = 6;
const int buttononder = 5;
const int buttonlinks = 4;

bool updatetime = false; //  tijd updaten.
bool accesaccepted = false; // Als je toegang hebt als verpleger.
bool datechange = false; //voor de datum te veranderen.
bool datechange2 = false;
bool timechange = false; // voor de tijd te veranderen
bool Time = false;
bool seth = false; // de uren veranderen.
bool setm = false; // de minuten veranderen
bool pijl = false; // de pijl die wordt getoond bij de tijd te veranderen.
bool achtergrond = false; // achtergrond verlichting veranderen.
bool achtergrond1 = true; // als de achtergrond verlichting aan of uit is.
bool achtergrond2 = false; // achtergrond verlichting veranderen.
bool controldata = false; // de data controleren
bool controldata1 = false;
bool setjaar = false; // jaar veranderen
bool setmaand =  false; // maand veranderen
bool setdag = false; // dagen veranderen.
bool voegopeneningstijdentoe = false; // voor de tijden te openen in te stellen.
bool voegopeneningstijden2toe = false;
bool verwijderopeneningstijdentoe = false; // voor de tijden te openen in te stellen.
bool verwijderopeneningstijden2toe = false;

int hopeningtijden = 0; // voor de uren voor het openen.
int mopeningtijden = 0; // voor de minuten voor het openen.
int sopeningtijden = 0; // voor de seconden voor het openen.
bool sethopeningstijden = false; // voor de uren te veranderen.
bool setmopeningstijden = false; // voor de minuten te veranderen.
bool vergrendelingvoortijdstip = false; // Als de verpleger wilt instellen als de patient het kluisje pas kan openen het tijd is.
bool vergrendelingvoortijdstip2 = false;
bool vergrendelingvoortijdstip1 = false; // als dit op false is dan kan de patient het altijd openen anders niet, staat standaart op false.
bool bekijktijden = false; // voor te bekijken welke uren zijn ingesteld.
bool bekijktijden1 = false;

bool heeftmedicijngenomen = true; // als de patient zijn medicijn heeft genomen of niet.

bool addrfid = false;//verpleger rfid toevoegen
bool addrfid2 = false;
bool addrfid3 = false;//patient patient toevoegen.
bool addrfid4 = false;

bool removerfid = false;//rfid verpleger verwijderen.
bool removerfid2 = false;
bool removerfid3 = false;//rfid patient verwijderen.
bool removerfid4 = false;

bool buzzeron = false; // als de buzzer aan is is dit waar.
bool buzzeron2 = false;

bool schermvergrendeling = false; // scherm aan of uit bij vergrendeling.
bool schermvergrendeling2 = false;
bool schermvergrendelingaanuit = true; // het scherm aan of uit moet zijn bij de bergrendeling

bool deuropenclose = false; // is de deur open of dicht.


SleutelStore sleutelsPatienten; // bewaart de sleutals van de patient.
SleutelStore sleutelsVerpleger; // bewaart de sleutels van de verpleger.

TijdStore Tijdenvooropenen; //bewaart de tijden voor te openen.


void setup() {
  // elke second haalt hij de functie binnen.
  MsTimer2::set(1000, countDownOneSecond);  // elke 1000ms roept hij de functie op voor de tijd.
  MsTimer2::start(); // de timer wordt gestart.

  Serial.begin(9600); // de serialpoort starten.




  
  // Voegd een nog een kaart toe;
  char Card1[] = "12 177 28 181";
  char masterCard[] = "9 160 86 211"; // de kaart waarbij je sleutels kan toevoegen(hoofdkaard).
  sleutelsVerpleger.voegHoofdSleutelToe(masterCard);// voegd de hoofd kaart toe.
  sleutelsVerpleger.voegSleutelToe(masterCard); // voegd een kaart toe voor de verpleger.
  sleutelsPatienten.voegSleutelToe(Card1); // voegd een kaart toe voor de patient.
  sleutelsVerpleger.printSleutels(); // print alle sleutels die toegang hebben.
  sleutelsPatienten.printSleutels();

  Serial.println("Starting the RFID Reader...");
  SPI.begin(); // de rfid lezer wordt gestart.
  rfid.init();
  lcd.begin(); // de lcd wordt gesart.
  lcd.clear();
  lcd.print("Medicijn kluisje"); // print het op start scherm.
  lcd.setCursor(0, 1);
  lcd.print(versie);
  lcd.setBacklight(HIGH); // zet de achtergrond verlichting op hoog bij lcd.


  pinMode(2, OUTPUT); //uitgang voor de buzzer / maakt een geluidje bij het begin.
  for (int i = 0; i <= 3; i++) { // dit herhaalt hij 3 keer, het gaat aan, uit aan.
    if (buzzeron == false) { // als het uit is gaat het aan
      digitalWrite(2, HIGH); // zet de poort hoog.
      delay(150); // wacht 150 miliseconden
      buzzeron = true; // zet de bool op true.
    }
    else { // anders
      digitalWrite(2, LOW); // zet de buzzer laag.
      delay(150); // wacht 150 miliseconden
      buzzeron = false; //zet het op false.
    }

  }

  digitalWrite(2, HIGH); // zet het weer hoog.


  delay(500); // wacht 500 miliseconden.
  digitalWrite(2, LOW); // zet het laag.

  buzzeron = false;
  // de uren/minuten/seconden worden op 0 gezet.
  h = 0; // uren
  m = 0; //  minuten
  s = 0; //  seconden
  pinMode(buttonboven, INPUT); // de bovenste knop
  pinMode(buttonrechts, INPUT); // de rechtse knop
  pinMode(buttononder, INPUT); // de onderste knop
  pinMode(buttonlinks, INPUT); // de linkse knop
  Serial.println("TIME:" ); // print de tijd in de serial poort.
  Serial.println(h);
  Serial.println(":");
  Serial.println(m);
  Serial.println(":");
  Serial.println(s);





  s3.attach(3); // zet de servo op de juiste positie.
  s3.write(gradenservodict);




  lcd.clear();
  lcd.print("TIME:" );
  lcd.print(String(h, DEC)); // print de tijd op het lcd bij het opstarten, de string h wordt opgezet in decimalen.
  lcd.print(":");
  lcd.print(String(m, DEC));// print de tijd op het lcd bij het opstarten, de string m wordt opgezet in decimalen.
  lcd.print(":");
  lcd.print(String(s, DEC));// print de tijd op het lcd bij het opstarten, de string s wordt opgezet in decimalen.

}
void countDownOneSecond() {

  s = s + 1; // elke seconden komt er een second bij.


  if (s == 60) // elke 60 seconden komt er een minut bij.
  {
    s = 0; // zet de seconden terug op 0.
    m = m + 1; // als er 60 seconden zijn komt er een Minutje bij.
    mmedicijn = mmedicijn + 1; // de timer wordt upgedate. elke minut komt er eentje bij.
  }
  if (m == 60) // als er 60 minuten zijn komt er een uurtje bij.
  {

    m = 0; // de mintuen worden terug op nul gezet

    h = h + 1; // het uurtje wordt er bijgetelt.

  }
  if (h == 24)
  {
    h = 0; // de uren worden naar 0 gezet.
    dag = dag + 1; //  elke 24u komt er een dag bij.
  }
  updatetime = true; // elke second wordt dan de tijd upgedate.
  servotimer = servotimer + 1; // de servo timer wordt er 1 second bijgeteld.




}

void printtime() {
  if (accesaccepted == false && schermvergrendelingaanuit == true) { //  print de tijd.
    if (m < 10) {
      lcd.clear();

      lcd.print(String(h, DEC)); // de tijd printen op de lcd
      lcd.print(":");
      lcd.print("0" + String(m, DEC)); // een nul zetten als de minuten kleiner zijn 10.
      lcd.print(":");
      lcd.print(String(s, DEC));
    }
    else {
      lcd.clear();

      lcd.print(String(h, DEC)); //de tijd printen op de lcd.
      lcd.print(":");
      lcd.print(String(m, DEC));
      lcd.print(":");
      lcd.print(String(s, DEC));
    }
    lcd.setCursor(0, 1); // de cursor op de 2de regel zetten.
    lcd.print(String(dag, DEC)); // print de datum onder de tijd.
    lcd.print("/");
    lcd.print(String(maand, DEC));
    lcd.print("/");
    lcd.print(String(jaar, DEC));


  }
  if (achtergrond1 == true) { // achtergrond verliting aan of uitzetten, als het scherm uit staat bij vergrendeling.
    if (schermvergrendelingaanuit == false) { // als er bij vergrendeling uit moet zijn
      if (accesaccepted == false) { // als er geen toegang is.
        lcd.setBacklight(LOW); // zet de licht van de lcd uit.
      }
      else {
        lcd.setBacklight(HIGH); //anders hoog
      }
    }
    else {
      lcd.setBacklight(HIGH); // achtergrond verlichting aan
    }
  }
  else {
    lcd.setBacklight(LOW); // anders uit.
  }

}

void opendeur() { // opend de servo deur.
  servotimer = 0; // zet de timer op 0;
  s3.write(gradenservoopen); // zet de servo op de graden voor te openen.
}

void scrolldisplay() { // Print wanneer de patient de kluis heeft geopend.
  //lcd.clear();
  if (Data66 == "") { // als de data leeg is dan print hij "/".
    Data66 = " /";
  }

  if (Data55 == "") {
    Data55 = " /";
  }

  delay(100); // scrolt het display naar links.
  lcd.scrollDisplayLeft();
  delay(100);
  lcd.scrollDisplayLeft();
  delay(100);
  lcd.scrollDisplayLeft();
  delay(100);
  lcd.scrollDisplayLeft();
  delay(100);
  lcd.scrollDisplayLeft();
  delay(100);
  lcd.scrollDisplayLeft();
  delay(100);
  lcd.scrollDisplayLeft();
  delay(100);
  lcd.scrollDisplayLeft();
  delay(100);
  lcd.scrollDisplayLeft();
  delay(100);
  lcd.scrollDisplayLeft();


  if (Data66 == "/") { // als de data leeg was wordt hij terug leeg.
    Data66 = "";
  }

  if (Data55 == "/") {
    Data55 = "";
  }

}




byte customChar[] = { // de pijl voor de tijd te veranderen, costum charachter
  B00100, // bij de 1ste rij is 0 het lampje uit en 1 het lampje aan.
  B01110, // bij de 2de rij is 0 het lampje uit en 1 het lampje aan.
  B11111, // bij de 3de rij is 0 het lampje uit en 1 het lampje aan.
  B00100, // bij de 4de rij is 0 het lampje uit en 1 het lampje aan.
  B00100, // bij de 5de rij is 0 het lampje uit en 1 het lampje aan.
  B00100, // bij de 6de rij is 0 het lampje uit en 1 het lampje aan.
  B00100, // bij de 7de rij is 0 het lampje uit en 1 het lampje aan.
  B00100  // bij de 8ste rij is 0 het lampje uit en 1 het lampje aan.
};

void datumchange() { // Datum veranderen
  if (datechange2 == true) {
    lcd.clear();
    if (dag < 10) { // als de dag kleiner is dat de 10de van de maand.
      lcd.print("0" + String(dag, DEC)); // print de dag/maand/jaar voor die te veranderen..
      lcd.print("/");
      lcd.print(String(maand, DEC));
      lcd.print("/");
      lcd.print(String(jaar, DEC));
    }
    else { // anders
      lcd.print(String(dag, DEC));
      lcd.print("/");
      lcd.print(String(maand, DEC));
      lcd.print("/");
      lcd.print(String(jaar, DEC));
    }
    if (setdag == true) { // de pijl wordt op de juiste positie gebracht.
      if (pijl == true) { // plaatst de pijl.
        pijl = false;
        lcd.createChar(0, customChar); // maakt de pijl
        lcd.setCursor(0, 1); // zet het cursor op het juiste vakje.
        lcd.write(0); // plaats de pijl.
      }
      else {
        pijl = true;
      }
    }
    if (setmaand == true) { // maand veranderen.
      if (pijl == true) {
        pijl = false; // voor de pijl te laten flikeren.
        lcd.createChar(0, customChar); // maakt de pijl
        lcd.setCursor(3, 1); // zet de cursor op het juiste vakje.
        lcd.write(0); // plaats de pijl
      }
      else {
        pijl = true; 
      }
    }
    if (setjaar == true) { // jaar veranderen

      if (pijl == true) {
        pijl = false; // voor de pijl te laten veranderen
        lcd.createChar(0, customChar); // maakt de pijl
        lcd.setCursor(6, 1); //zet de cursor op het juiste vakje.
        lcd.write(0); // plaats de pijl.
      }
      else {
        pijl = true;
      }
    }


  }

}

void printtime1() { // de tijd printen wanneer het word veranderd

  if (seth == true) { // uren veranderen
    if (h < 10 && m < 10) { // als de uren en minuten kleiner is dan 10.
      lcd.clear();
      // lcd.print("TIME:" );
      lcd.print("0" + String(h, DEC));
      lcd.print(":");
      lcd.print("0" + String(m, DEC));
      lcd.print(":");
      lcd.print(String(s, DEC));
    }
    else if (m < 10) { // als de minuten minder zijn dan 10.
      lcd.clear();
      // lcd.print("TIME:" );
      lcd.print(String(h, DEC));
      lcd.print(":");
      lcd.print("0" + String(m, DEC));
      lcd.print(":");
      lcd.print(String(s, DEC));
    }
    else if (h < 10) { //als de uren kleiner zijn dan 0.
      lcd.clear();
      // lcd.print("TIME:" );
      lcd.print("0" + String(h, DEC));
      lcd.print(":");
      lcd.print(String(m, DEC));
      lcd.print(":");
      lcd.print(String(s, DEC));
    }

    else { // als de minuten en uren groter of gelijk aan 10 zijn.
      lcd.clear();
      // lcd.print("TIME:" );
      lcd.print(String(h, DEC));
      lcd.print(":");
      lcd.print(String(m, DEC));
      lcd.print(":");
      lcd.print(String(s, DEC));
    }

    if (pijl == true) { // pijl printen.
      pijl = false; 
      lcd.createChar(0, customChar); // maakt de pijl
      lcd.setCursor(1, 1);
      lcd.write(0); // print de pijl
    }
    else {
      pijl = true;
    }
  }
  if (setm == true) { // minuten veranderen

    if (m < 10) { // als de minuten kleiner zijn dan 10.
      lcd.clear();
      //  lcd.print("TIME:" );
      lcd.print(String(h, DEC));
      lcd.print(":");
      lcd.print("0" + String(m, DEC));
      lcd.print(":");
      lcd.print(String(s, DEC));
    }
    else { // als de mintuten kleiner zijn dan 10.
      lcd.clear();

      lcd.print(String(h, DEC));
      lcd.print(":");
      lcd.print(String(m, DEC));
      lcd.print(":");
      lcd.print(String(s, DEC));
    }

    if (pijl == true) { // de pijl priten.
      pijl = false;
      lcd.createChar(0, customChar); // creeert de pijl.
      lcd.setCursor(3, 1); // zet de cursor juist.
      lcd.write(0); // print de pijl.
    }
    else {
      pijl = true;
    }

  }



}

void printtime2() { // de tijd printen wanneer het word veranderd

  if (sethopeningstijden == true) { // uren veranderen
    if (hopeningtijden < 10 && mopeningtijden < 10) { // als de uren en minuten kleiner is dan 10.
      lcd.clear();
      // lcd.print("TIME:" );
      lcd.print("0" + String(hopeningtijden, DEC));
      lcd.print(":");
      lcd.print("0" + String(mopeningtijden, DEC));
      //lcd.print(":");
      //lcd.print(String(s, DEC));
    }
    else if (mopeningtijden < 10) { // als de minuten minder zijn dan 10.
      lcd.clear();
      // lcd.print("TIME:" );
      lcd.print(String(hopeningtijden, DEC));
      lcd.print(":");
      lcd.print("0" + String(mopeningtijden, DEC));
      //lcd.print(":");
      //lcd.print(String(s, DEC));
    }
    else if (sethopeningstijden < 10) { //als de uren kleiner zijn dan 10.
      lcd.clear();
      // lcd.print("TIME:" );
      lcd.print("0" + String(hopeningtijden, DEC));
      lcd.print(":");
      lcd.print(String(mopeningtijden, DEC));
      //lcd.print(":");
      //lcd.print(String(s, DEC));
    }

    else { // als de minuten en uren groter of gelijk aan 10 zijn.
      lcd.clear();
      // lcd.print("TIME:" );
      lcd.print(String(hopeningtijden, DEC));
      lcd.print(":");
      lcd.print(String(mopeningtijden, DEC));
      //lcd.print(":");
      //lcd.print(String(s, DEC));
    }

    if (pijl == true) { // pijl printen.
      pijl = false; 
      lcd.createChar(0, customChar); // maakt de pijl
      lcd.setCursor(1, 1);
      lcd.write(0); // print de pijl
    }
    else {
      pijl = true;
    }
  }
  if (setmopeningstijden == true) { // minuten veranderen

    if (mopeningtijden < 10) { // als de minuten kleiner zijn dan 10.
      lcd.clear();
      //  lcd.print("TIME:" );
      lcd.print(String(hopeningtijden, DEC));
      lcd.print(":");
      lcd.print("0" + String(mopeningtijden, DEC));
      // lcd.print(":");
      // lcd.print(String(s, DEC));
    }
    else { //anders
      lcd.clear();

      lcd.print(String(hopeningtijden, DEC));
      lcd.print(":");
      lcd.print(String(mopeningtijden, DEC));
      //  lcd.print(":");
      //  lcd.print(String(s, DEC));
    }

    if (pijl == true) { // de pijl printen.
      pijl = false;
      lcd.createChar(0, customChar); // creeert de pijl.
      lcd.setCursor(3, 1); // zet de cursor juist.
      lcd.write(0); // print de pijl.
    }
    else {
      pijl = true;
    }

  }



}

void loop() { // wordt oneindig herhaalt.

  if (vergrendelingvoortijdstip1 == true && (Tijdenvooropenen.istijdstipgelijk((String(h) + ":" + String(m)  + ":"  + String(s)).c_str()) == true)) { // als de uren en de minuten gelijk zijn aan de ingestelde uren en minuten en de patient kan het kluisje niet eerder openen.
    mmedicijn = 0; // wordt terug gereset.
    heeftmedicijngenomen = false; // dit wordt op false gezet omdat de patient nog geen medicijn heeft genomen
    digitalWrite(2, HIGH); // zet de buzzor aan.
    //delay(300);

  }
  else { // als het niet waar is.
    //delay(300);
    digitalWrite(2, LOW); // zet de buzzer uit.

  }

  if (heeftmedicijngenomen == false) { // als hij nog geen medicijn heeft genomen, dan biept hij om de 10 min.
    if (vergrendelingvoortijdstip1 == true && (mmedicijn == 10 || mmedicijn == 20 || mmedicijn == 30 || mmedicijn == 40 || mmedicijn == 50 || mmedicijn == 60 || mmedicijn == 70 || mmedicijn == 80 || mmedicijn == 90 || mmedicijn == 100 || mmedicijn == 110 || mmedicijn == 120 || mmedicijn == 130 || mmedicijn == 140 || mmedicijn == 150 || mmedicijn == 160 || mmedicijn == 170 || mmedicijn == 180 || mmedicijn == 190 || mmedicijn == 200 || mmedicijn == 210 || mmedicijn == 220 || mmedicijn == 230 || mmedicijn == 240 || mmedicijn == 250))
// elke 10 minuten wordt gaat de buzzer aan.
      
      digitalWrite(2, HIGH); // zet de buzzer aan.
    delay(1000);

    

  }

  if (hopeningtijden < 0){
    hopeningtijden = 0;
 }
 if (mopeningtijden < 0){
    mopeningtijden = 0;
 }

  if (maand == 1) { // de maand/jaar/dag wordt automatisch veranderd.
    if (dag == 32) { // als de er te veel dagen in de maand zijn dan komt er een maantje bij en de dag wordt weer 1.
      maand = maand + 1; // er wordt  maand bijgeteld.
      dag = 1; // dag wordt terug 1
    }
  }
  else if (maand == 2) { // als het de 2de maand is.  // er wordt ook gecontroleerd op een schrikkel jaar.
    if (schrikkeljaar == jaar || schrikkeljaar + 4 == jaar || schrikkeljaar + 8 == jaar || schrikkeljaar + 12 == jaar || schrikkeljaar + 16 == jaar || schrikkeljaar + 20 == jaar || schrikkeljaar + 24 == jaar || schrikkeljaar + 28 == jaar || schrikkeljaar + 32 == jaar) {
      if (dag == 30) { // als de er te veel dagen in de maand zijn dan komt er een maantje bij en de dag wordt weer 1.
        maand = maand + 1; // er wordt  maand bijgeteld.
        dag = 1; // De dag wordt terug 1.
      }
    }
    else {
      if (dag == 29) { // als de er te veel dagen in de maand zijn dan komt er een maantje bij en de dag wordt weer 1.
        maand = maand + 1; // er wordt  maand bijgeteld.
        dag = 1; // De dag wordt terug 1.
      }
    }
  }
  else if (maand == 3) { // als het de 3de maand is.
    if (dag == 32) { // als de er te veel dagen in de maand zijn dan komt er een maantje bij en de dag wordt weer 1.
      maand = maand + 1; // er wordt  maand bijgeteld.
      dag = 1; // De dag wordt terug 1.
    }
  }
  else if (maand == 4) { //als het de 4de maand is.
    if (dag == 31) { // als de er te veel dagen in de maand zijn dan komt er een maantje bij en de dag wordt weer 1.
      maand = maand + 1; // er wordt  maand bijgeteld.
      dag = 1; // De dag wordt terug 1.
    }
  }
  else if (maand == 5) { // als het de 5de maand is.
    if (dag == 32) { // als de er te veel dagen in de maand zijn dan komt er een maantje bij en de dag wordt weer 1.
      maand = maand + 1; // er wordt  maand bijgeteld.
      dag = 1; // De dag wordt terug 1.
    }
  }
  else if (maand == 6) { // als het de 6de maand is.
    if (dag == 31) { // als de er te veel dagen in de maand zijn dan komt er een maantje bij en de dag wordt weer 1.
      maand = maand + 1; // er wordt  maand bijgeteld.
      dag = 1; // De dag wordt terug 1.
    }
  }
  else if (maand == 7) { // als het de 7de maand is.
    if (dag == 32) { // als de er te veel dagen in de maand zijn dan komt er een maantje bij en de dag wordt weer 1.
      maand = maand + 1; // er wordt  maand bijgeteld.
      dag = 1; // De dag wordt terug 1.
    }
  }
  else if (maand == 8) { // als het de 8ste maand is.
    if (dag == 32) { // als de er te veel dagen in de maand zijn dan komt er een maantje bij en de dag wordt weer 1.
      maand = maand + 1; // er wordt  maand bijgeteld.
      dag = 1; // De dag wordt terug 1.
    }
  }
  else if (maand == 9) { // als het de 9de maand is.
    if (dag == 31) { // als de er te veel dagen in de maand zijn dan komt er een maantje bij en de dag wordt weer 1.
      maand = maand + 1; // er wordt  maand bijgeteld.
      dag = 1; // De dag wordt terug 1.
    }
  }
  else if (maand == 10) { // als het de 10ste maand is.
    if (dag == 32) { // als de er te veel dagen in de maand zijn dan komt er een maantje bij en de dag wordt weer 1.

      maand = maand + 1; // er wordt  maand bijgeteld.
      dag = 1; // De dag wordt terug 1.
    }
  }
  else if (maand == 11) { // als het de 11de maand is.
    if (dag == 31) { // als de er te veel dagen in de maand zijn dan komt er een maantje bij en de dag wordt weer 1.
      maand = maand + 1; // er wordt een maand bijgeteld.
      dag = 1; // De dag wordt terug 1.
    }
  }
  else if (maand == 12) { // als het de 12 maand is.
    if (dag == 32) { // als de er te veel dagen in de maand zijn dan komt er een maantje bij en een jaarte en de dag en de maand wordt weer 1.
      maand = 1; // De maand wordt terug 1.
      dag = 1; // De dag wordt terug 1.
      jaar = jaar + 1; // er wordt een jaar bijgeteld.
    }
  }

  if (servotimer > hoelangopen && deuropenclose == true) { // de deur wordt gesloten.
    s3.write(gradenservodict); // de servo wordt op 0 graden gezet.
    deuropenclose = false; // de deur wordt gesloten.
    servotimer = 0; // de servotimer wordt gereset.
  }



  if (accesaccepted == false) { // als het scherm bij vergrendeling uit moet zijn, gaat de achtergrond verlichting uit.
    if (schermvergrendelingaanuit == false) {
      lcd.clear(); // het scherm wordt leeggehaalt.
      lcd.setBacklight(LOW); // achtergrondverlichting wordt uitgezet.
    }


  }
  if (achtergrond1 == true) { // als de achtergrond verlichting aan is.
    if (schermvergrendelingaanuit == false) { // en de scherm bij vergrendeling uit is.
      if (accesaccepted == false) { // en dat er geen toegang is.
        lcd.setBacklight(LOW); // achtergrond verlichiting uit;
      }
      else { //Als er wel toegang is.
        lcd.setBacklight(HIGH); // achtergrond verliching aan.
      }
    }
    else { // Als het scherm aan is bij vergrendeling.
      lcd.setBacklight(HIGH); // achtergrond verlichting is aan.
    }
  }
  else { // als de achergrond verlichitn uit is.
    lcd.setBacklight(LOW); // achergrondverlichting uit;
  }
  if ((controldata1 == true || bekijktijden1 == true) && updatetime == true) { // als het menu controle is geopend en timer is geupdate.

    scrolldisplay(); // scrolt de scherm met de data.


  }


  if (updatetime == true) { // update de timer/
    printtime();   // de tijd printen en de datum printen.
    printtime1();  // de tijd printen als het veranderd wordt.
    printtime2();
    datumchange(); // de datum veranderen
    updatetime = false; // de tijd is geupdate.

  }


  if (digitalRead(buttonboven) == LOW) { // als er op de bovenste knop wordt gedrukt.
    Serial.println("buttonboven");
    delay(300);

    if (setdag == true) { // dag veranderen
      dag = dag + 1; // een dag bijtellen.
    }
    else if (setmaand == true) { // maand veranderen
      maand = maand + 1; // er wordt een maand bijgeteld
    }
    else if (setjaar == true) { // jaar veranderen
      jaar = jaar + 1; // er wordt een jaar bijgeteld.
    }

    else if (timechange == true) { // als je op het hoofdmenu het woord tijd ziet.
      datechange = true; // voor de datum te veranderen
      timechange = false; // voor de tijd te veranderen
      lcd.clear();
      lcd.print("Datum"); // print Datum;
    }
    else if (removerfid3 == true) { // als de verpleger een openings tijd wilt toevoegen.
      removerfid3 = false; // zet de bools juist.
      voegopeneningstijdentoe = true;
      lcd.clear(); // print wat er op het scherm moet staan.
      lcd.print("Openingstijd");
      lcd.setCursor(0, 1);
      lcd.print("toevoegen");


    }
    else if (vergrendelingvoortijdstip == true) { // als je naar het menu wilt gaan voor de openingstijden te bekijken.
      vergrendelingvoortijdstip = false; // zet de bools juist.
      bekijktijden = true;
      lcd.clear(); // print het juiste op het scherm.
      lcd.print("Openingstijden");
      lcd.setCursor(0, 1);
      lcd.print("bekijken");
    }

    else if (verwijderopeneningstijdentoe == true) { // als je wilt instellen als het vergrendeld is voor dat de patient het mag openen.
      verwijderopeneningstijdentoe = false; // zet de bools juist.
      vergrendelingvoortijdstip = true;
      lcd.clear(); // print het juiste op het scherm.
      lcd.print("Vergrendeling");
      lcd.setCursor(0, 1);
      lcd.print("openingstijden");
    }


    else if (voegopeneningstijdentoe == true) { // als je de openingstijden wilt verwijderen.
      voegopeneningstijdentoe = false; // zet de bools juist.
      verwijderopeneningstijdentoe = true;
      lcd.clear(); // print het juiste op het scherm.
      lcd.print("Openingstijd");
      lcd.setCursor(0, 1);
      lcd.print("verwijderen");


    }
    else if (removerfid == true) { // Als het menu voor de rfid  te verwijderen voor de patient
      addrfid3 = false; // zet de bools juist.
      removerfid = false;
      removerfid3 = true;
      lcd.clear();
      lcd.print("RFID Verwijderen"); // print wat er op het scherm moet komen.
      lcd.setCursor(0, 1);
      lcd.print("Patient");

    }
    else if (addrfid3 == true) { // Als het menu voor de rfid  toe te voegen voor de verpleger


      addrfid3 = false; // zet de bools juist.
      removerfid = true;
      lcd.clear();
      lcd.print("RFID Verwijderen"); // print wat er op moet kommen.
      lcd.setCursor(0, 1);
      lcd.print("Verpleger");

    }
    else if (datechange == true) { // Als het menu voor de datum er is.
      if (sleutelsVerpleger.heeftMastercardToegang(rfidCard.c_str())) { // controleet of het de masterkaard is toegang heefft gekregen.
        datechange = false; // zet de bools juist.
        addrfid = true;
        lcd.clear();
        lcd.print("RFID toevoegen"); // print de juiste tekst.
        lcd.setCursor(0, 1);
        lcd.print("Verpleger");
      }
    }

    else if (addrfid == true) { // als er op het menu staat
      datechange = false; // zet de bools juist
      addrfid3 = true;
      addrfid = false;
      lcd.clear();
      lcd.print("RFID toevoegen"); // print de tekst die er op moet staan.
      lcd.setCursor(0, 1);
      lcd.print("Patient");
    }

    else if (achtergrond == true) { // als hij op de menu achtergrond staat.
      if  (achtergrond2 == false) { // als hij niet in het menu is voor de achtergrond verlichtin aan te passen.
        timechange = true;
        achtergrond = false;

        lcd.clear();

        lcd.print("Tijd"); // print de tekst die er op moet staan.
      }


      if (achtergrond2 == true) { // als hij wel in het menu is.
        if (achtergrond1 == true) { //  als de achergrond verlichting aan is.
          achtergrond1 = false; // zet de achtergrond verlichting uit.
          lcd.clear(); // zet het juiste op de lcd.
          lcd.print("Uit");

        }
        else { // als de achtergrondverlichtin aan is.
          achtergrond1 = true; // voor de achtergrond verlichting terug aan te zetten.
          lcd.clear(); // print het juiste op de scherm.
          lcd.print("Aan");
        }
      }
    }


    else if  (schermvergrendeling == true) {// alshet scherm bij vergrendeling uit is
      controldata = true; // zet de bools juist.
      schermvergrendeling = false;
      lcd.clear(); // zet het juiste op het scherm.
      lcd.print("Controle");

    }



    else  if (controldata == true) { // als het menu voor de controler er staat.
      timechange = false; // zet de bools juist.
      achtergrond = true;
      controldata = false;
      lcd.clear(); // print het juiste op het scherm.

      lcd.print("Achtergrond");
      lcd.setCursor(0, 1);
      lcd.print("verlichting");
    }



    if (schermvergrendeling2 == true) { // als het menu voor de scherm bij vergrendeling er is.

      lcd.clear();
      if (schermvergrendelingaanuit == true) { // als het aan is.
        lcd.print("Uit");
        schermvergrendelingaanuit = false; // wordt het uit gezet
      }
      else { // als het uit is.
        lcd.print("Aan");
        schermvergrendelingaanuit = true; // wordt het aangezet.
      }

    }
    else if (vergrendelingvoortijdstip2 == true) { // voor de vergrendeling van het tijdstip
      if (vergrendelingvoortijdstip1 == true) { // als het aan is wordt het uit.
        lcd.clear(); // print het juste
        lcd.print("Uit");
        vergrendelingvoortijdstip1 = false; // zet de bools juist
      }

      else {// anders als het uit is wordt het aan.
        lcd.clear(); // print het juiste
        lcd.print("Aan");
        vergrendelingvoortijdstip1 = true; // zet de bools juist.
      }

    }


    if (sethopeningstijden == true) { // als de tijd moet gewijzigd worden voor openings uren.
      hopeningtijden = hopeningtijden + 1; // telt er 1tje bij.
      if (hopeningtijden > 23) {
        hopeningtijden = 0; // als de uren groter dan 23 zijn gaat hij naar 0.
      }

    }

    if (setmopeningstijden == true) { //als de tijd moet gewijzigd worden voor openings minuten.
      mopeningtijden = mopeningtijden  + 1; // telt er 1tje bij.
      if (mopeningtijden > 59) { // als de minuten gorter zijn dan 59 wordt het 0.
        mopeningtijden = 0; // zet het op 0.
        hopeningtijden = hopeningtijden + 1; // telt er een uurtje bij.
      }

    }

    if (seth == true) { //als de tijd wordt veranderd die op het scherm komt als hij vergrendeld is.
      h = h + 1;  // zet er een uurtje bij

    }
    if (setm == true) { //als de tijd wordt veranderd die op het scherm komt als hij vergrendeld is.
      m = m + 1; // telt er een minutje bij.
    }
  }
  else if (digitalRead(buttonrechts) == LOW) { // de rechts knop.

    Serial.println("buttonrechts");
    delay(300);

    if (addrfid == true) { // als je de rfid wilt toevoegen voor de verpleger.
      addrfid2 = true; // zet de bools juist
      addrfid = false;
      lcd.clear(); // print het juiste op het scherm.
      lcd.print("Plaats de RFID");
    }
    else if (addrfid2 == true) { // als je het wilt annuleren voor de rfid toevoegen voor de verpleger.
      addrfid = true; // zet de bools juist.
      addrfid2 = false;
      lcd.clear(); // print het juiste op het scherm.
      lcd.print("RFID toevoegen");
      lcd.setCursor(0, 1);
      lcd.print("Verpleger");
    }
    else if (addrfid3 == true) { // als je de rfid wilt toevoegen voor de patient.
      addrfid4 = true; // zet de bools juist
      addrfid3 = false;
      lcd.clear(); // zet het juiste op de lcd.
      lcd.print("Plaats de RFID");
    }
    else if (addrfid4 == true) { // als je het wilt annuleren voor de rfid toevoegen voor de patient.
      addrfid3 = true; // zet de bools juist.
      addrfid4 = false;
      lcd.clear(); // zet het juiste op het scherm.
      lcd.print("RFID toevoegen");
      lcd.setCursor(0, 1);
      lcd.print("Patient");
    }
    else if (removerfid == true) { // als je de rfid wilt verwijderen voor de verpleger.
      removerfid2 = true; // zet de bools juist
      removerfid = false;
      lcd.clear(); // zet het juiste op het scherm.
      lcd.print("Plaats de RFID");
    }
    else if (removerfid2 == true) { // als je het wilt annuleren voor de rfid verwijderen voor de verpleger.
      removerfid = true; // zet de bools juist.
      removerfid2 = false;
      lcd.clear(); // print het juiste op het scherm.
      lcd.print("RFID verwijderen");
      lcd.setCursor(0, 1);
      lcd.print("Verpleger");
    }
    else if (removerfid3 == true) { // als je de rfid wilt verwijderen voor de patient.
      removerfid4 = true; // zet de bools juist.
      removerfid3 = false;
      lcd.clear(); // zet het juiste op het scherm.
      lcd.print("Plaats de RFID");
    }
    else if (removerfid4 == true) { //als je het wilt annuleren voor de rfid verijwderen voor de patient.
      removerfid3 = true; // zet de bools juist.
      removerfid4 = false;
      lcd.clear(); // print het juiste op het scherm.
      lcd.print("RFID verwijderen");
      lcd.setCursor(0, 1);
      lcd.print("Patient");
    }
    else if (voegopeneningstijdentoe == true) { // voor de openingstijden toetevoegen.
      voegopeneningstijdentoe = false; // zet de bools juist.
      voegopeneningstijden2toe = true;
      setmopeningstijden = false;
      sethopeningstijden = true;
    }
    else if (voegopeneningstijden2toe == true) { // voor de openings tijden toetevoegen.
      printtime2(); // print de tijd voor de openings tijden,
      //sethopeningstijden = true;
      //setmopeningstijden = false;

      if (sethopeningstijden == true) { // als de minuten worden ingegeven
        setmopeningstijden = true;
        sethopeningstijden = false;
      } else if (setmopeningstijden == true) { // slaat het op.
        Tijdenvooropenen.voegtijdstiptoe((String(hopeningtijden) + ":" + String(mopeningtijden) + ":" + String(sopeningtijden)).c_str());
        voegopeneningstijdentoe = true; // zet de bools terug juist.
        voegopeneningstijden2toe = false;
        setmopeningstijden = false;
        sethopeningstijden = false;
        lcd.clear(); // print het juiste
        lcd.print("Openingstijd");
        lcd.setCursor(0, 1);
        lcd.print("toevoegen");

      }
      else { // veranderd de bools voor de openings uren te veranderen.
        sethopeningstijden = true;
        setmopeningstijden = false;
        printtime2(); // wordt op het lcd geprint.
      }

      printtime2(); // print de tijd voor de openings tijden.


    }
    else if (verwijderopeneningstijdentoe == true) { // voor het verwijderen van de openingstijden
      verwijderopeneningstijdentoe = false;
      verwijderopeneningstijden2toe = true;
      setmopeningstijden = false;
      sethopeningstijden = true;
    }
    else if (verwijderopeneningstijden2toe == true) { // voor het verwijderen van de openingstijden.
      printtime2(); // voor de tijd intestellen die je wilt verwijderen.

      if (sethopeningstijden == true) { // voor de minuten aan te passen.
        setmopeningstijden = true; // zet de bools juist.
        sethopeningstijden = false;
      } else if (setmopeningstijden == true) { // als een openings uur verwijderd moet worden.
        Tijdenvooropenen.verwijdertijdstip((String(hopeningtijden) + ":" + String(mopeningtijden) + ":" + String(sopeningtijden)).c_str()); // verwijderd de openingstijd.
        verwijderopeneningstijdentoe = true; // zet bools juist
        verwijderopeneningstijden2toe = false;
        setmopeningstijden = false;
        sethopeningstijden = false;
        lcd.clear(); // print het juiste op het scherm.
        lcd.print("Openingstijd");
        lcd.setCursor(0, 1);
        lcd.print("verwijderen");

      }
      else { // anders voor de uren veranderen/
        sethopeningstijden = true; // zet de bools juist.
        setmopeningstijden = false;
      }


    }


    else if (vergrendelingvoortijdstip == true) { // voor aan te passen als de patient het kluisje eerder kan openen.
      vergrendelingvoortijdstip = false; // zet de bools juist
      vergrendelingvoortijdstip2 = true;
      if (vergrendelingvoortijdstip1 == true) { // print of het al aan staat of uit.
        lcd.clear();
        lcd.print("Aan");
      }
      else {
        lcd.clear();
        lcd.print("Uit");
      }

    }
    else if (vergrendelingvoortijdstip2 == true) { // als je uit het menu wilt gaan
      vergrendelingvoortijdstip2 = false; // zet de bools juist
      vergrendelingvoortijdstip = true;
      lcd.clear(); // print het juiste..
      lcd.print("Vergrendeling voor");
      lcd.setCursor(0, 1);
      lcd.print("openingstijden");
    }





    else  if (datechange == true) { // als de datum wilt veranderen wordt de datum geprint, voor die te veranderen.
      datechange = false; // zet de bools juist.
      datechange2 = true;
      setdag = true;

    }


    else if (datechange2 == true) { // voor te veranderen van dag naar maand naar jaar.
      lcd.clear();
      if (setdag == true) { // van de dag te veranderen ga je nu de maand veranderen.
        setmaand = true; // de bools juist.
        setdag = false;
      }
      else if (setmaand == true) { // van de maand te verandren naar jaar te veranderen.
        setjaar = true; // zet de bools juist.
        setmaand = false;
      }
      else if (setjaar == true) { // als je gedaan hebt met de maand/jaar/dag te veranderen.
        datechange2 = false;// zet de bools juist.
        datechange = true;
        setjaar = false;
        lcd.clear(); // print het juiste.
        lcd.print("Datum");
      } else {
        setdag = true; // als er geen van de 3 waar is, wordt setdag automatisch waar.
      }
    }
    else if (controldata == true) { // als je de data wilt controleren
      controldata = false; // zet de bools juist
      controldata1 = true;
      lcd.clear();
      lcd.print(Data2 + Data66);
      scrolldisplay(); // zorgt ervoor dat de data op het scherm staat.

    }
    else if (controldata1 == true) { // als je uit het menu wilt gaan voor de data te controleren.
      controldata = true; // zet de bools juist
      controldata1 = false;
      lcd.clear(); // print het juiste op het scherm.
      lcd.print("Controle");
      Data66 = ""; // reset de data.

    }
    else if (bekijktijden == true) { // als je de tijden wilt bekijken
      bekijktijden1 = true; // zet de bools juist
      bekijktijden =  false;
      lcd.clear();
      lcd.print(Tijdenvooropenen.printtijdenlcd());  // print de tijden op het lcd.
    }
    else if (bekijktijden1 == true) { // voor openingsuren te bekijken
      bekijktijden = true; // zet de bools juist
      bekijktijden1 = false; 
      lcd.clear(); // print het juisten op het lcd.
      lcd.print("Openingstijden");
      lcd.setCursor(0, 1);
      lcd.print("bekijken");
    }


    if (schermvergrendeling == true) { // als je het scherm bij vergrendelinging wilt aanpassen.
      schermvergrendeling2 = true; // zet de bools juist
      schermvergrendeling = false;
      lcd.clear();
      if (schermvergrendelingaanuit == true) { // als het op aan staat, print hij aan.
        lcd.print("Aan");
      }
      else { // als het op uit staat print hij uit.
        lcd.print("Uit");
      }

    } else if (schermvergrendeling2 == true) { // voor uit het menu te gaan voor het scherm aan of uit te hebben bij vergrendeling.
      schermvergrendeling2 = false; // zet de bools juist.
      schermvergrendeling = true;
      lcd.clear(); // print het juiste op het scherm.
      lcd.print("Scherm bij");
      lcd.setCursor(0, 1);
      lcd.print("vergrendeling");


    }

    if (timechange == true) { // als je de tijd wilt verandern
      timechange = false; // zet de bools juist
      Time = true;
      seth = true;

    }

    else   if (seth == true) { // als je van de uren de minuten wilt veranderen
      seth = false;
      setm = true;
    }
    else if (setm == true) { // als je uit het menu wilt gaan voor de tijd te veranderen.
      seth = false; // zet de bools juist.
      setm = false;
      Time = false;
      timechange = true;
      lcd.clear(); // print het juiste op het scherm.
      lcd.print("Tijd");
    }
    if (achtergrond2 == true) // voor uit het menu te gaan van de achergrondverlichting aan te passen.
    {
      timechange = false; // zet de bools juist.
      achtergrond2 = false;
      achtergrond = true;
      lcd.clear(); // print het juiste op het scherm
      lcd.print("Achtergrond");
      lcd.setCursor(0, 1);
      lcd.print("verlichting");
    }
    else if (achtergrond == true) { // voor in het menu te gaan voor de achtergrond verlichting aan te passen.
      achtergrond2 = true;
      // achtergrond  = false;

      if (achtergrond1 == true) { // als het aan is print hij aan.

        lcd.clear();
        lcd.print("Aan");
      }
      else // anders print hij uit.
      {

        lcd.clear();
        lcd.print("Uit");
      }






    }

  }
  else if (digitalRead(buttononder) == LOW) {// de onderste knop.
    Serial.println("buttononder");


    delay(300);
    if (setdag == true) { //voor de dagen aan te passen.
      dag = dag - 1; // telt er een dagje af
      if (dag <= 0) { // als de dagen kleiner of gelijk aan nul zijn, wordt het 1.
        dag = 1;
      }
    }
    else if (setmaand == true) { // voor de maanden te veranderen
      maand = maand - 1; // telt er 1 maand af.
      if (maand <= 0) { // als de maander kleiner of gelijk aan 0 zijn dat maakt hij het terug 1.
        maand = 1;
      }

    }
    else if (setjaar == true) { //voor de jaren te veranderen.
      jaar = jaar - 1; // telt er een jaar af.
    }
    if (datechange == true) { // voor naar een menu naar onder te gaan, naar de tijd menu
      datechange = false; // zet de bools juist
      timechange = true;
      lcd.clear(); // print wat er op het scherm moet staan.
      lcd.print("Tijd");
    }
    else if (bekijktijden == true) {
      vergrendelingvoortijdstip = true;
      bekijktijden = false;
      lcd.clear(); // print het juiste op het scherm.
      lcd.print("Vergrendeling");
      lcd.setCursor(0, 1);
      lcd.print("openingstijden");
    }


    else if (vergrendelingvoortijdstip == true) { // als je naar het menu voor de openingstijd te verwijderen wilt gaan.
      verwijderopeneningstijdentoe = true; // zet de bools juist.
      vergrendelingvoortijdstip = false;
      lcd.clear(); // print het juiste op het scherm.
      lcd.print("Openingstijd");
      lcd.setCursor(0, 1);
      lcd.print("verwijderen");
    }
    else if (verwijderopeneningstijdentoe == true) { // als je naar het menu wilt gaan voor de openingstijden toe te voegen.
      verwijderopeneningstijdentoe = false; // zet de bools juist
      voegopeneningstijdentoe = true;
      lcd.clear(); // print het juiste op het scherm.
      lcd.print("Openingstijd");
      lcd.setCursor(0, 1);
      lcd.print("toevoegen");


    }
    else if (voegopeneningstijdentoe == true) { // voor naar het menu te gaan voor een rfid te verwijderen.
      voegopeneningstijdentoe = false;
      verwijderopeneningstijdentoe = false;
      removerfid3 = true; // zet de bools juist.
      lcd.clear(); // print het juiste op het scherm.
      lcd.print("RFID verwijderen");
      lcd.setCursor(0, 1);
      lcd.print("Patient");



    }


    else if (removerfid3 == true) { // als je eentje naar onder wilt gaan, naar rfid verwijdere verpleger.
      addrfid3 = false;// zet de bools juist
      removerfid = true;
      removerfid3 = false;
      lcd.clear(); // print het juiste op het scherm.
      lcd.print("RFID Verwijderen");
      lcd.setCursor(0, 1);
      lcd.print("Verpleger");

    }
    else if (removerfid == true) { // als je een menu naar onder wilt voor de rfid toetevoegen voor de patient.
      addrfid3 = true; // zet de bools juist.
      removerfid = false;
      lcd.clear(); // print het juite op het scherm.
      lcd.print("RFID toevoegen");
      lcd.setCursor(0, 1);
      lcd.print("Patient");

    }
    else if (addrfid3 == true) { // als je weer een menu naar onder wilt gaan, voor een rfid toetevoegen voor een patient.
      addrfid3 = false; // zet de bools juist;
      addrfid = true;
      lcd.clear(); // print het juiste op het scherm.
      lcd.print("RFID toevoegen");
      lcd.setCursor(0, 1);
      lcd.print("Verpleger");
    }
    else if (addrfid == true) { // als je een menu naar onder wilt gaan voor de datum te veranden.
      datechange = true; // zet de bools juist.
      addrfid = false;
      lcd.clear(); //print het juiste op het scherm.
      lcd.print("Datum");
    }



    else if (achtergrond == true) { // als je de achtergrond verlichting aan of uit wilt zetten.
      if (achtergrond2 == true) {
        if (achtergrond1 == true) { // als de aan is gaat hij het uit doen.
          achtergrond1 = false;
          lcd.clear();
          lcd.print("Uit");

        }
        else { // als het uit is gaat hij het uit doen.
          achtergrond1 = true;// de bools juist
          lcd.clear(); // print het juiste.
          lcd.print("Aan");
        }
      }
      else if (achtergrond == true) { // als je een menu naar onder wilt gaan voor de controle menu te hebben,
        timechange = false; // zet de bools juist
        achtergrond = false;
        controldata = true;
        lcd.clear(); // print het juiste
        lcd.print("Controle");
      }


    }
    else if (timechange == true) { // als je een menu naar onder wilt van de tijd te veranderen naar de achtergrond verlichting.
      timechange = false; // zet de bools juist.
      achtergrond = true;
      controldata = false;
      lcd.clear(); // print het juiste op het scherm.
      lcd.print("Achtergrond");
      lcd.setCursor(0, 1);
      lcd.print("Verlichting");

    }


    else if  (controldata == true) { // als hij van de controle naar de scherm bij vergrendeling wilt gaan.
      controldata = false; // zet de bools juist
      schermvergrendeling = true;
      lcd.clear(); // print het juiste op het scherm.
      lcd.print("Scherm bij");
      lcd.setCursor(0, 1);
      lcd.print("vergrendeling");
    }


    if (schermvergrendeling2 == true) { // als je in het menu zit van scherm bij vergrendeling

      lcd.clear();
      if (schermvergrendelingaanuit == true) { // als het aan is is gaat hij uit;
        lcd.print("Uit"); // print uit:
        schermvergrendelingaanuit = false; // zet de bool juist
      }
      else { // als hij uit is gaat hij aan;
        lcd.print("Aan");  // print aan:
        schermvergrendelingaanuit = true; // zet de bool juist
      }

    }

    else if (vergrendelingvoortijdstip2 == true) { // als hij moet vergredeldt worden voor het te open.
      if (vergrendelingvoortijdstip1 == true) { // als het aan is wordt het uit
        lcd.clear(); // print het juiste
        lcd.print("Uit");
        vergrendelingvoortijdstip1 = false; // zet de bools juist.
      }

      else { // als het uit is wordt het aan.
        lcd.clear(); // print het juiste
        lcd.print("Aan");
        vergrendelingvoortijdstip1 = true; // zet de bools juist.
      }

    }

    if (sethopeningstijden == true) { //als de openins uren moet gewijzigd worden
      hopeningtijden = hopeningtijden - 1; // telt er 1tje af.
      if (hopeningtijden < 0) {
        hopeningtijden = 23; // als de uren kleiner dan 0 zijn gaat hij naar 23.
      }

    }

    if (setmopeningstijden == true) { //als de openins minuten moet gewijzigd worden
      mopeningtijden = mopeningtijden  - 1; // telt er 1tje af.
      if (mopeningtijden < 0) { // als de minuten kleiner zijn dan 0 wordt het 59.
        mopeningtijden = 59; // zet het op 59.
        hopeningtijden = hopeningtijden - 1; // telt er een uurtja af.
      }

    }

    if (seth == true) { // als de tijd moet gewijzigd worden, telt er eentje af.
      h = h - 1; // telt er 1tje af.
      if (h < 0) {
        h = 23; // als de uren kleiner dan 0 zijn gaat hij naar 23.
      }

    }

    if (setm == true) { // als je er een minutje af wilt.
      m = m - 1; // telt er 1tje af.
      if (m < 0) { // als de minuten kleiner zijn dan 0 wordt het 59.
        m = 59; // zet het op 59.
        h = h - 1; // telt er een uurtja af.
      }

    }
  }
  else if (digitalRead(buttonlinks) == LOW) { // vergrendeld terug de arduino.
    Serial.println("buttonlinks");
    delay(200);
    accesaccepted = false; // zet allen bools hoe het moet zijn als hij vergrendeld is.
    timechange = false;

    seth = false;
    setm = false;
    pijl = false;
    achtergrond = false;
    achtergrond2 = false;
    controldata = false;
    controldata1 = false;
    schermvergrendeling = false;
    schermvergrendeling2 = false;
    datechange2 = false;
    datechange = false;
    addrfid = false;
    addrfid3 = false;
    removerfid = false;
    removerfid3 = false;
    addrfid2 = false;
    addrfid4 = false;
    removerfid2 = false;
    removerfid4 = false;
    voegopeneningstijdentoe = false; // voor de tijden te openen in te stellen.
    voegopeneningstijden2toe = false;
    verwijderopeneningstijdentoe = false; // voor de tijden te openen in te stellen.
    verwijderopeneningstijden2toe = false;
    sethopeningstijden = false;
    setmopeningstijden = false;
    bekijktijden = false;
    bekijktijden1 = false;
    vergrendelingvoortijdstip = false; 
    vergrendelingvoortijdstip2 = false;

  }



  if (rfid.isCard()) {  // kijkt of er een rfid kaart is.
    if (rfid.readCardSerial()) {// leest de rfidkaar nummer
      rfidCard = String(rfid.serNum[0]) + " " + String(rfid.serNum[1]) + " " + String(rfid.serNum[2]) + " " + String(rfid.serNum[3]); // haalt de nummer van de rfid kaart.
      Serial.println(rfidCard); // print de nummer van de rfid.
      if (addrfid2 == true) { // als er een rfid moet worden toegevoegd voor verpleger.

        addrfid2 = false; // als de rfid is toegevoegd gaat hij terug naar het menu. // zet de bools dus juist.
        addrfid = true;
        lcd.clear(); // print wat op het scherm moet staat;
        lcd.print("RFID toevoegen");
        lcd.setCursor(0, 1);
        lcd.print("Verpleger");
        sleutelsVerpleger.voegSleutelToe(rfidCard.c_str());// voegd de sluetel toe voor de verpleger.
        sleutelsVerpleger.printSleutels(); // de sleutels worden geprint van de verpleger;



      } else if (addrfid4 == true) { // als er een rfid moet worden toegevoegd voor patient.

        addrfid4 = false; // als de rfid is toegevoegd gaat hij terug naar het menu. // zet de bools dus juist.
        addrfid3 = true;
        lcd.clear(); // print wat er op moet staan.
        lcd.print("RFID toevoegen");
        lcd.setCursor(0, 1);
        lcd.print("Patient");
        sleutelsPatienten.voegSleutelToe(rfidCard.c_str()); // voegd de sleutels voor de patient toe.
        sleutelsPatienten.printSleutels(); // sleutels worden geprint van van de patient.

      }
      else if (removerfid2 == true) { // als je de rfid kaar wilt verwijderen voor de verpleger
        //RFIDs = rfidCard;
        removerfid2 = false; // zet de bools juist.
        removerfid = true;
        lcd.clear(); // print het juiste op het scherm.
        lcd.print("RFID verwijderen");
        lcd.setCursor(0, 1);
        lcd.print("Verpleger");
        sleutelsVerpleger.verwijderSleutel(rfidCard.c_str()); // verwijderd de rfid kaart.
        sleutelsVerpleger.printSleutels(); // print de sleutels;

      } else if (removerfid4 == true) { // als je de rfid kaar wilt verwijderen voor de patient;

        removerfid4 = false; // zet de bools juist
        removerfid3 = true;
        lcd.clear(); // print het juiste op het scherm.
        lcd.print("RFID verwijderen");
        lcd.setCursor(0, 1);
        lcd.print("Patient");
        sleutelsPatienten.verwijderSleutel(rfidCard.c_str()); // verwijderd de rfid voor de patient.
        sleutelsPatienten.printSleutels(); // print sleutels

      }



      else if (sleutelsVerpleger.heeftSleutelToegang(rfidCard.c_str())) {/*|| rfidCard == RFIDs*/// controleert of de verpleger toegang heeft;
        if (m < 10) { // als de minuten kleiner zijn als 10.
          Data55 =  String(h) + ":" + "0" + String(m)  + ", " + Data55; // als de verpleger het heeft geopend, komt het bij de conrole.
        } else { // als de minuten groter of gelijk aan 10 zijn..
          Data55 =  String(h) + ":" + String(m)  + ", " + Data55; // als de verpleger het heeft geopend, komt het bij de conrole.
        }
        Serial.println(Data55); // print de data
        Serial.println("Card Accepted!");
        opendeur();
        deuropenclose = true;


        delay(200);
        accesaccepted = true; // zet de bools juist voor de ontgrendeling van de verpleger;
        timechange = true;


        seth = false;
        setm = false;
        pijl = false;
        achtergrond = false;
        achtergrond2 = false;
        controldata = false;
        controldata1 = false;
        schermvergrendeling = false;
        schermvergrendeling2 = false;
        datechange2 = false;
        datechange = false;
        addrfid = false;
        addrfid3 = false;
        removerfid = false;
        removerfid3 = false;
        addrfid2 = false;
        addrfid4 = false;
        removerfid2 = false;
        removerfid4 = false;
        voegopeneningstijdentoe = false; // voor de tijden te openen in te stellen.
        voegopeneningstijden2toe = false;
        verwijderopeneningstijdentoe = false; // voor de tijden te openen in te stellen.
        verwijderopeneningstijden2toe = false;
        sethopeningstijden = false;
        setmopeningstijden = false;
        achtergrond2 = false;
        achtergrond = false;
        controldata = false;
        controldata1 = false;
        schermvergrendeling = false;
        schermvergrendeling2 = false;
        datechange2 = false;
        datechange = false;
        addrfid = false;
         bekijktijden = false;
         bekijktijden1 = false;
         vergrendelingvoortijdstip = false; 
    vergrendelingvoortijdstip2 = false;
        lcd.clear(); // print wat er op het scherm moet komen, begint bij het menu van tijd.
        lcd.print("Tijd");





      } else if (sleutelsPatienten.heeftSleutelToegang(rfidCard.c_str())) { // controleert of de sleutel van de patient klopt.

        if (vergrendelingvoortijdstip1 == false) {
          opendeur();
          deuropenclose = true; // opent de deur
          if (m < 10) { // als de menuten kleiner zijn als 10 dan is er nog een nul voor de minuten
            Data66 =  String(h) + ":" + "0" + String(m)  + ", " + Data66;
          } else { // als de minuten groter zijn
            Data66 =  String(h) + ":" + String(m)  + ", " + Data66;
          }
          heeftmedicijngenomen = true;
        }
        else if (heeftmedicijngenomen == false) {
          opendeur();
          deuropenclose = true; // opent de deur
          if (m < 10) { // als de menuten kleiner zijn als 10 dan is er nog een nul voor de minuten
            Data66 =  String(h) + ":" + "0" + String(m)  + ", " + Data66;
          } else { // als de minuten groter zijn
            Data66 =  String(h) + ":" + String(m)  + ", " + Data66;
          }
          heeftmedicijngenomen = true;
        }
        // zet de bools juist voor de ontgrendeling van de patient.
        accesaccepted = false; // zet allen bools hoe het moet zijn als hij vergrendeld is.
        timechange = false;

        seth = false;
        setm = false;
        pijl = false;
        achtergrond = false;
        achtergrond2 = false;
        controldata = false;
        controldata1 = false;
        schermvergrendeling = false;
        schermvergrendeling2 = false;
        datechange2 = false;
        datechange = false;
        addrfid = false;
        addrfid3 = false;
        removerfid = false;
        removerfid3 = false;
        addrfid2 = false;
        addrfid4 = false;
        removerfid2 = false;
        removerfid4 = false;
        voegopeneningstijdentoe = false; // voor de tijden te openen in te stellen.
        voegopeneningstijden2toe = false;
        verwijderopeneningstijdentoe = false; // voor de tijden te openen in te stellen.
        verwijderopeneningstijden2toe = false;
        sethopeningstijden = false;
        setmopeningstijden = false;
        bool bekijktijden = false;
        bool bekijktijden1 = false;
        vergrendelingvoortijdstip = false; 
    vergrendelingvoortijdstip2 = false;
        Serial.println(Data66);// print de data in de serialpoort
        Serial.println("Card Accepted!");


        delay(200); // een pauze anders gaat het te snel achter elkaar.
      }
      else { // Als de kaart ongeldig is.
        lcd.clear();
        lcd.print("Ongeldige kaart!"); // print het juiste op het scherm.
        Serial.println("Ongeldige kaart!");

        

        accesaccepted = false; // alles wordt vergrendeld;
        timechange = false;
        achtergrond = false;
        controldata = false;
        controldata1 = false;
        schermvergrendeling = false;
        schermvergrendeling2 = false;
        datechange2 = false;
        datechange = false;
        addrfid = false;
        addrfid3 = false;
        removerfid = false;
        removerfid3 = false;
        addrfid2 = false;
        addrfid4 = false;
        removerfid2 = false;
        removerfid4 = false;
         bekijktijden = false;
         bekijktijden1 = false;
         vergrendelingvoortijdstip = false; 
    vergrendelingvoortijdstip2 = false;
      }

      delay(200); // wacht 200 miliseconden.




    }
    rfid.halt(); // de rfid kaart kan terug een anderen lezen.
  }
}

// einde
