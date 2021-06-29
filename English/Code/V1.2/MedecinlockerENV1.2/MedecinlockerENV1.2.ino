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
/*
  -------------------------------------------------- -------------------------------------------------- ----------------
  medicine locker
  -------------------------------------------------- -------------------------------------------------- ----------------

   @author Sem Boonen and Daan Swinters
   @creator Daan Swinters
   @license Prohibited to use without the permission of the authors.
  -------------------------------------------------- -------------------------------------------------- ----------------
  The connections on arduino(UNO):
  -------------------------------------------------- ---------------------------------------
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

   © Sem Boonen and Daan Swinters Medicine safe 2021.
*/



#include "sleutel.h" //  This is a homemade libary where you can all store keys.
#include "tijdvooropenen.h" //  This is a homemade libary where you can all store keys.

#include <Servo.h> //  the servo libary.
Servo s3; // Add a servo with the name.


#include <Wire.h>  // for communicating with the I2C screen.
#include <LiquidCrystal_I2C.h> // for the to print on the i2c.

// Set the LCD address to 0x27 for a 16-character, 2-line display.
LiquidCrystal_I2C lcd(0x27, 16, 2);



#include <MsTimer2.h> // the timer for every second 



#include <SPI.h> // for the to communicate with the rfid.
#include <RFID.h>
#define SS_PIN 10 // the pins define rfid.
#define RST_PIN 9
RFID rfid(SS_PIN, RST_PIN);
String rfidCard; // of rfidcard code
String Data55; // here in is stored every time the safe is opened for nurse.
String Data1 = "Verpleger: ";

String Data66;// It stores every time the box is opened for the patient.
String Data2 = "Patient: ";


String versie = "v1.2"; // the version of the medicine safe.


int schrikkeljaar = 2020; // first leap year set(the day/month/year given at the start).
int dag = 1; //  // days
int maand = 1; // months
int jaar = 2020; // years


int h = 0; // hours
int m = 0; //  minutes
int s = 0; //  seconds

int mmedicijn = 0;

int Li = 16; // view scroll to the left
int Lii = 0;


int servotimer = 0; // the timer for closing the door back is set to 0 at the start.
int gradenservodict = 90; // the degrees for the servo close.
int gradenservoopen = 15; // the degrees for the servo close.
int hoelangopen = 30; // How many seconds the door must be open.



const int buttonboven = 7;  // pins of the 4 buttons
const int buttonrechts = 6;
const int buttononder = 5;
const int buttonlinks = 4;

bool updatetime = false; // time updater(happens 1 time per second).
bool accesaccepted = false; // If you have access as a nurse.
bool datechange = false; //before changing the date.
bool datechange2 = false;
bool timechange = false; // for the time to change
bool Time = false;
bool seth = false; // change the hours.
bool setm = false; // change the minutes
bool pijl = false; // the arrow shown at the time change.
bool achtergrond = false; // change background lighting.
bool achtergrond1 = true; // if the backlight is on or off.
bool achtergrond2 = false; // change background lighting.
bool controldata = false; // of date controleren
bool controldata1 = false;
bool setjaar = false; // change years
bool setmaand =  false; // month change
bool setdag = false; // days change.
bool voegopeneningstijdentoe = false; // for the times to open set.
bool voegopeneningstijden2toe = false;
bool verwijderopeneningstijdentoe = false; // for the times to open set.
bool verwijderopeneningstijden2toe = false;

int hopeningtijden = 0; // for opening hours.
int mopeningtijden = 0; // for opening minutes.
int sopeningtijden = 0; // for opening seconds.
bool sethopeningstijden = false; // for the hours to change.
bool setmopeningstijden = false; // voor de minuten te veranderen.
bool vergrendelingvoortijdstip = false; // If the nurse wants to set when the patient can only open the locker it is time.
bool vergrendelingvoortijdstip2 = false;
bool vergrendelingvoortijdstip1 = false; // if this is set to false then the patient can always open it otherwise not, defaults to false.
bool bekijktijden = false; // for to see what hours have been set.
bool bekijktijden1 = false;

bool heeftmedicijngenomen = true; // if the patient has taken his medicine or not.

bool addrfid = false;//add nurse rfid 
bool addrfid2 = false;
bool addrfid3 = false;//add rfid patient .
bool addrfid4 = false;

bool removerfid = false;//removal rfid nurse .
bool removerfid2 = false;
bool removerfid3 = false;// removal rfid patient.
bool removerfid4 = false;

bool buzzeron = false; // if the buzzer is on or not.
bool buzzeron2 = false;

bool schermvergrendeling = false; // screen on or off when locked.
bool schermvergrendeling2 = false;
bool schermvergrendelingaanuit = true; // the screen should be on or off at the time of the lockout

bool deuropenclose = false; // is the door open or closed.


SleutelStore sleutelsPatienten; // keeps the patient's keyals.
SleutelStore sleutelsVerpleger; // Keeps the keys from the nurse.

TijdStore Tijdenvooropenen; //saves the times for opening


void setup() {
  
  MsTimer2::set(1000, countDownOneSecond);  // every 1000ms it calls the function for the time.
  MsTimer2::start(); // the timer is started.

  Serial.begin(9600); // start the serial port.

  

  char masterCard[] = "9 160 86 211"; // The map where you can add keys(main card).
  char Card1[] = "12 177 28 181";     // Add another card;
   
  sleutelsVerpleger.voegHoofdSleutelToe(masterCard);// add the main card.
  
  sleutelsPatienten.voegSleutelToe(Card1); // Adds a card for the nurse.
  sleutelsPatienten.voegSleutelToe(masterCard); // adds a card for the patient.
  sleutelsVerpleger.printSleutels(); // Print all keys that have access.
  sleutelsPatienten.printSleutels();

  Serial.println("Starting the RFID Reader...");
  SPI.begin(); // the rfid reader is started.
  rfid.init();
  lcd.begin(); // the lcd is booted.
  lcd.clear();
  lcd.print("Medicine safe"); // startup screen
  lcd.setCursor(0, 1);
  lcd.print(versie);
  lcd.setBacklight(HIGH); // set the backlight to high on lcd.


  pinMode(2, OUTPUT); //output for the buzzer / makes a sound at the start.
  for (int i = 0; i <= 3; i++) { // this he repeats 3 times, it goes on, off on.
    if (buzzeron == false) { // when it's off it goes on
      digitalWrite(2, HIGH); // set the gate high.
      delay(150); // wait 150 milliseconds
      buzzeron = true; // set the bool to true.
    }
    else { // otherwise
      digitalWrite(2, LOW); // Set the buzzer low.
      delay(150); // wait 150 milliseconds
      buzzeron = false; //set it to false.
    }

  }

  digitalWrite(2, HIGH); // set it high.


  delay(500); // wait 500 milliseconds.
  digitalWrite(2, LOW); // Set it low.

  buzzeron = false;
  // the hours/minutes/seconds are set to 0.
  h = 0; // hours
  m = 0; //  minutes
  s = 0; //  seconden
  
  pinMode(buttonboven, INPUT); // the top button
  pinMode(buttonrechts, INPUT); // the right button
  pinMode(buttononder, INPUT); // the bottom button
  pinMode(buttonlinks, INPUT); // the left button
  Serial.println("TIME:" ); // print the time in the serial port.
  Serial.println(h);
  Serial.println(":");
  Serial.println(m);
  Serial.println(":");
  Serial.println(s);





  s3.attach(3); // set the servo to the correct position.
  s3.write(gradenservodict);




  lcd.clear();
  lcd.print(String(h, DEC)); // prints the time on the LCD at startup, the string h is set up in decimal places.
  lcd.print(":");
  lcd.print(String(m, DEC));// prints the time on the LCD at startup, the string m is set up in decimal places.
  lcd.print(":");
  lcd.print(String(s, DEC));// prints the time on the LCD at startup, the string s is set up in decimal places.

}
void countDownOneSecond() {

  s = s + 1; // Every second adds a second.


  if (s == 60) // Every 60 seconds, a minute is added.
  {
    s = 0; // resets the seconds to 0.
    m = m + 1; // when there are 60 seconds a Minute is added.
    mmedicijn = mmedicijn + 1; // the timer is up-dated. one is added every minute.
  }
  if (m == 60) // when there are 60 minutes an hour is added.
  {

    m = 0; // the mintues are reset to zero

    h = h + 1; // the hour is added 1.

  }
  if (h == 24)
  {
    h = 0; // the hours are set to 0.
    dag = dag + 1; //  Every 24h a day is added.
  }
  updatetime = true; // every second the time is then up-dated on the lcd.
  servotimer = servotimer + 1; // 1 second is added for how long the door remains open.




}

void printtime() {
  if (accesaccepted == false && schermvergrendelingaanuit == true) { // print the time.
    if (m < 10) {
      lcd.clear();

      lcd.print(String(h, DEC)); // print the time on the lcd
      lcd.print(":");
      lcd.print("0" + String(m, DEC)); // put an extra zero if the minutes are smaller 10.
      lcd.print(":");
      lcd.print(String(s, DEC));
    }
    else {
      lcd.clear();

      lcd.print(String(h, DEC)); //print the time on the LCD.
      lcd.print(":");
      lcd.print(String(m, DEC));
      lcd.print(":");
      lcd.print(String(s, DEC));
    }
    lcd.setCursor(0, 1); // put the cursor on the 2nd line.
    lcd.print(String(dag, DEC)); // Print the date below the time.
    lcd.print("/");
    lcd.print(String(maand, DEC));
    lcd.print("/");
    lcd.print(String(jaar, DEC));


  }
  if (achtergrond1 == true) { // turn background verliting on or off, if the screen is off when locked.
    if (schermvergrendelingaanuit == false) { // if lockout should be off
      if (accesaccepted == false) { // if there is no access.
        lcd.setBacklight(LOW); // turns off the light of the lcd.
      }
      else {
        lcd.setBacklight(HIGH); //otherwise it stands ON
      }
    }
    else {
      lcd.setBacklight(HIGH); // backlight ON
    }
  }
  else {
    lcd.setBacklight(LOW); // backlight OFF
  }

}

void opendeur() { // opens the servo door.
  servotimer = 0; // set the timer to 0;
  s3.write(gradenservoopen); // set the servo to the degrees for opening.
}

void scrolldisplay() { // Print when the patient has opened the safe.
  if (Data66 == "") { // if the data is empty then it prints "/".
    Data66 = " /";
  }

  if (Data55 == "") {
    Data55 = " /";
  }

  delay(100); // scrolls the display to the left.
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


  if (Data66 == "/") { // if the data was empty it becomes empty again.
    Data66 = "";
  }

  if (Data55 == "/") {
    Data55 = "";
  }

}





byte customChar[] = { // change the arrow for the time, costum charachter
  B00100, // for the 1st row, 0 is the light off and 1 is the light on.
  B01110, // for the 2st row, 0 is the light off and 1 is the light on.
  B11111, // for the 3st row, 0 is the light off and 1 is the light on.
  B00100, // for the 4st row, 0 is the light off and 1 is the light on.
  B00100, // for the 5st row, 0 is the light off and 1 is the light on.
  B00100, // for the 6st row, 0 is the light off and 1 is the light on.
  B00100, // for the 7st row, 0 is the light off and 1 is the light on.
  B00100  // for the 8st row, 0 is the light off and 1 is the light on.
};

void datumchange() { // Date change
  if (datechange2 == true) {
    lcd.clear();
    if (dag < 10) { // if the day is smaller that the 10th of the month.
      lcd.print("0" + String(dag, DEC)); // print the day/month/year for that to change....
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
    if (setdag == true) { // the arrow is moved to the correct position.
      if (pijl == true) { // places the arrow.
        pijl = false;
        lcd.createChar(0, customChar); // makes the arrow
        lcd.setCursor(0, 1); // put the cursor on the right box.
        lcd.write(0); // place the arrow.
      }
      else {
        pijl = true;
      }
    }
    if (setmaand == true) { // month change.
      if (pijl == true) {
        pijl = false; // for the arrow to flicker.
        lcd.createChar(0, customChar); // makes the arrow
        lcd.setCursor(3, 1); // Put the cursor on the appropriate box.
        lcd.write(0); // place the arrow
      }
      else {
        pijl = true; 
      }
    }
    if (setjaar == true) { // change years

      if (pijl == true) {
        pijl = false; // for the arrow to change
        lcd.createChar(0, customChar); // makes the arrow
        lcd.setCursor(6, 1); //put the cursor on the appropriate box.
        lcd.write(0); // place the arrow.
      }
      else {
        pijl = true;
      }
    }


  }

}

void printtime1() { // print the time when it is changed

  if (seth == true) { // change hours
    if (h < 10 && m < 10) { // if the hours and minutes is less than 10.
      lcd.clear();
      lcd.print("0" + String(h, DEC));
      lcd.print(":");
      lcd.print("0" + String(m, DEC));
      lcd.print(":");
      lcd.print(String(s, DEC));
    }
    else if (m < 10) { // if the minutes are less than 10.
      lcd.clear();
      lcd.print(String(h, DEC));
      lcd.print(":");
      lcd.print("0" + String(m, DEC));
      lcd.print(":");
      lcd.print(String(s, DEC));
    }
    else if (h < 10) { //if the hours are less than 0.
      lcd.clear();
      lcd.print("0" + String(h, DEC));
      lcd.print(":");
      lcd.print(String(m, DEC));
      lcd.print(":");
      lcd.print(String(s, DEC));
    }

    else { // if the minutes and hours are greater than or equal to 10.
      lcd.clear();
      lcd.print(String(h, DEC));
      lcd.print(":");
      lcd.print(String(m, DEC));
      lcd.print(":");
      lcd.print(String(s, DEC));
    }

    if (pijl == true) { // print the arrow.
      pijl = false; 
      lcd.createChar(0, customChar); // makes the arrow
      lcd.setCursor(1, 1); 
      lcd.write(0); // print the arrow
    }
    else {
      pijl = true;
    }
  }
  if (setm == true) { // change minutes

    if (m < 10) { // if the minutes are less than 10.
      lcd.clear();
      lcd.print(String(h, DEC));
      lcd.print(":");
      lcd.print("0" + String(m, DEC));
      lcd.print(":");
      lcd.print(String(s, DEC));
    }
    else { // if the mintues are less than 10.
      lcd.clear();

      lcd.print(String(h, DEC));
      lcd.print(":");
      lcd.print(String(m, DEC));
      lcd.print(":");
      lcd.print(String(s, DEC));
    }

    if (pijl == true) { // print the arrow.
      pijl = false;
      lcd.createChar(0, customChar); // creates the arrow.
      lcd.setCursor(3, 1); // Set the cursor correctly.
      lcd.write(0); // print the arrow.
    }
    else {
      pijl = true;
    }

  }



}

void printtime2() { // print the time when it is changed

  if (sethopeningstijden == true) { // change hours
    if (hopeningtijden < 10 && mopeningtijden < 10) { // if the hours and minutes is less than 10.
      lcd.clear();
      lcd.print("0" + String(hopeningtijden, DEC));
      lcd.print(":");
      lcd.print("0" + String(mopeningtijden, DEC));
    }
    else if (mopeningtijden < 10) { // if the minutes are less than 10.
      lcd.clear();
      lcd.print(String(hopeningtijden, DEC));
      lcd.print(":");
      lcd.print("0" + String(mopeningtijden, DEC));
    }
    else if (sethopeningstijden < 10) { //if the hours are less than 10.
      lcd.clear();
      lcd.print("0" + String(hopeningtijden, DEC));
      lcd.print(":");
      lcd.print(String(mopeningtijden, DEC));
    }

    else { // if the minutes and hours are greater than or equal to 10.
      lcd.clear();
      lcd.print(String(hopeningtijden, DEC));
      lcd.print(":");
      lcd.print(String(mopeningtijden, DEC));
    }

    if (pijl == true) { // print the arrow.
      pijl = false; 
      lcd.createChar(0, customChar); // makes the arrow
      lcd.setCursor(1, 1);
      lcd.write(0); // print the arrow
    }
    else {
      pijl = true;
    }
  }
  if (setmopeningstijden == true) { // change minutes

    if (mopeningtijden < 10) { // if the minutes are less than 10.
      lcd.clear();
      lcd.print(String(hopeningtijden, DEC));
      lcd.print(":");
      lcd.print("0" + String(mopeningtijden, DEC));
    }
    else { // otherwise
      lcd.clear();
      lcd.print(String(hopeningtijden, DEC));
      lcd.print(":");
      lcd.print(String(mopeningtijden, DEC));
    }

    if (pijl == true) { // print the arrow.
      pijl = false;
      lcd.createChar(0, customChar); // Creates the arrow.
      lcd.setCursor(3, 1); // Set the cursor correctly.
      lcd.write(0); // print the arrow.
    }
    else {
      pijl = true;
    }

  }



}

void loop() { // is repeated infinitely.

  if (vergrendelingvoortijdstip1 == true && (Tijdenvooropenen.istijdstipgelijk((String(h) + ":" + String(m)  + ":"  + String(s)).c_str()) == true)) { // if the hours and minutes are equal to the set hours and minutes and the patient cannot open the safe before.
    mmedicijn = 0; // wordt terug gereset.
    heeftmedicijngenomen = false; // this is set to false because the patient has not yet taken any medicine
    digitalWrite(2, HIGH); // turn on the buzzor.
    //delay(300);

  }
  else { // otherwise
    //delay(300);
    digitalWrite(2, LOW); // turn off the buzzer.

  }

  if (heeftmedicijngenomen == false) { // als hij nog geen medicijn heeft genomen, dan biept hij om de 10 min.
    if (vergrendelingvoortijdstip1 == true && (mmedicijn == 10 || mmedicijn == 20 || mmedicijn == 30 || mmedicijn == 40 || mmedicijn == 50 || mmedicijn == 60 || mmedicijn == 70 || mmedicijn == 80 || mmedicijn == 90 || mmedicijn == 100 || mmedicijn == 110 || mmedicijn == 120 || mmedicijn == 130 || mmedicijn == 140 || mmedicijn == 150 || mmedicijn == 160 || mmedicijn == 170 || mmedicijn == 180 || mmedicijn == 190 || mmedicijn == 200 || mmedicijn == 210 || mmedicijn == 220 || mmedicijn == 230 || mmedicijn == 240 || mmedicijn == 250))
// Every 10 minutes the buzzer turns on.
      
      digitalWrite(2, HIGH); // turn on the buzzer.
    delay(1000);

    

  }

  if (hopeningtijden < 0){
    hopeningtijden = 0;
 }
 if (mopeningtijden < 0){
    mopeningtijden = 0;
 }

  if (maand == 1) { // the month/year/day is changed automatically.
    if (dag == 32) { // if the there are too many days in the month then a month is added and the day becomes 1 again.
      maand = maand + 1; // month is added.
      dag = 1; // day becomes back 1
    }
  }
  else if (maand == 2) { // if it is the 2nd month is also checked for a leap year.
    if (schrikkeljaar == jaar || schrikkeljaar + 4 == jaar || schrikkeljaar + 8 == jaar || schrikkeljaar + 12 == jaar || schrikkeljaar + 16 == jaar || schrikkeljaar + 20 == jaar || schrikkeljaar + 24 == jaar || schrikkeljaar + 28 == jaar || schrikkeljaar + 32 == jaar) {
      if (dag == 30) { // if the there are too many days in the month then a moon is added and the day becomes 1 again.
        maand = maand + 1; // month is added.
        dag = 1; // The day will be back 1.
      }
    }
    else {
      if (dag == 29) { // if the there are too many days in the month then a month is added and the day becomes 1 again.
        maand = maand + 1; // month is added.
        dag = 1; // The day will be back 1.
      }
    }
  }
  else if (maand == 3) { // als het de 3de maand is.
    if (dag == 32) { // if the there are too many days in the month then a month is added and the day becomes 1 again.
      maand = maand + 1; // er wordt  maand bijgeteld.
      dag = 1; // De dag wordt terug 1.
    }
  }
  else if (maand == 4) { //If it is the 4th month.
    if (dag == 31) { // if the there are too many days in the month then a month is added and the day becomes 1 again.
      maand = maand + 1; // month is added.
      dag = 1; // The day turns back 1.
    }
  }
  else if (maand == 5) { // if it is the 5th month.
    if (dag == 32) { // if the there are too many days in the month then a month is added and the day becomes 1 again.
      maand = maand + 1; // there is month added.
      dag = 1; // The day becomes back 1.
    }
  }
  else if (maand == 6) { // if it is the 6th month.
    if (dag == 31) { // if the there are too many days in the month then a month is added and the day becomes 1 again.
      maand = maand + 1; // month is added.
      dag = 1; // The day turns back 1.
    }
  }
  else if (maand == 7) { // if it is the 7th month.
    if (dag == 32) { // if the there are too many days in the month then a month is added and the day becomes 1 again.
      maand = maand + 1; // month is added.
      dag = 1; // The day turns back 1.
    }
  }
  else if (maand == 8) { // if it is the 8th month.
    if (dag == 32) { // if the there are too many days in the month then a month is added and the day becomes 1 again.
      maand = maand + 1; // month is added.
      dag = 1; // The day will be back 1.
    }
  }
  else if (maand == 9) { // if it is the 9th month.
    if (dag == 31) { // if the there are too many days in the month then a month is added and the day becomes 1 again.
      maand = maand + 1; // month is added.
      dag = 1; // The day turns back 1.
    }
  }
  else if (maand == 10) { // if it is the 10th month.
    if (dag == 32) { // if the there are too many days in the month then a month is added and the day becomes 1 again.

      maand = maand + 1; // there is month added.
      dag = 1; // The day becomes back 1.
    }
  }
  else if (maand == 11) { // if it is the 11th month.
    if (dag == 31) { // if the there are too many days in the month then a month is added and the day becomes 1 again.
      maand = maand + 1; // one month is added.
      dag = 1; // The day turns back 1.
    }
  }
  else if (maand == 12) { // if it is the 12 month.
    if (dag == 32) { // if the there are too many days in the month then a month is added, a yeart and the day and month become 1.
      maand = 1; // The month will be back 1.
      dag = 1; // The day turns back 1.
      jaar = jaar + 1; // one year is added.
    }
  }

  if (servotimer > hoelangopen && deuropenclose == true) { //the door will be closed
    s3.write(gradenservodict); // the servo is set to 0 degrees.
    deuropenclose = false; 
    servotimer = 0; // the servo timer is reset.
  }
  if (deuropenclose == true){ // when the door is closed in it stays closed.
      s3.write(gradenservoopen);
   }
    else{ // when the door is open it stays open.
      s3.write(gradenservodict);
   }



  if (accesaccepted == false) { // if the screen is to be off when locked, the backlight goes off.
    if (schermvergrendelingaanuit == false) {
      lcd.clear(); // the screen is emptied.
      lcd.setBacklight(LOW); // Backlight is turned off.
    }


  }
  if (achtergrond1 == true) { // when the backlight is on.
    if (schermvergrendelingaanuit == false) { // and the screen when locked is off.
      if (accesaccepted == false) { // and that there is no access.
        lcd.setBacklight(LOW); // background lighting off;
      }
      else { //If there is access.
        lcd.setBacklight(HIGH); // background lighting on.
      }
    }
    else { // If the screen is on when locked.
      lcd.setBacklight(HIGH); // background lighting is on.
    }
  }
  else { // when the backlight is off.
    lcd.setBacklight(LOW); // backlight is off;
  }
  if ((controldata1 == true || bekijktijden1 == true) && updatetime == true) { // if the menu control is open and timer is updated.

    scrolldisplay(); // scrolls the screen with the data.


  }


  if (updatetime == true) { // update de timer/
    printtime();   // print the time and print the date.
    printtime1();  // print the time when it is changed.
    printtime2();
    datumchange(); // change the date
    updatetime = false; // the time is updated.

  }


  if (digitalRead(buttonboven) == LOW) { // when the top button is pressed.
    Serial.println("buttonboven");
    delay(300);

    if (setdag == true) { // change day
      dag = dag + 1; // add a day.
    }
    else if (setmaand == true) { // month change
      maand = maand + 1; // one month is added
    }
    else if (setjaar == true) { // year change
      jaar = jaar + 1; // one year is added.
    }

    else if (timechange == true) { // when you see the word time on the main menu.
      datechange = true; // for the date to change
      timechange = false; // for the time to change
      lcd.clear();
      lcd.print("Date"); // print Date;
    }
    else if (removerfid3 == true) { // if the nurse wants to add an opening time.
      removerfid3 = false; // put the bools right.
      voegopeneningstijdentoe = true;
      lcd.clear(); // print what should be on the screen.
      lcd.print("Add Opening");
      lcd.setCursor(0, 1);
      lcd.print("Time");


    }
    else if (vergrendelingvoortijdstip == true) { // if you want to go to the menu to see the opening hours.
      vergrendelingvoortijdstip = false; // set the bools correctly.
      bekijktijden = true;
      lcd.clear(); // print the right one on the screen.
      lcd.print("View Opening");
      lcd.setCursor(0, 1);
      lcd.print("Times");
    }

    else if (verwijderopeneningstijdentoe == true) { // if you want to set when it is locked before the patient is allowed to open it.
      verwijderopeneningstijdentoe = false; // put the bools right.
      vergrendelingvoortijdstip = true;
      lcd.clear(); // print the right one on the screen.
      lcd.print("Lock For");
      lcd.setCursor(0, 1);
      lcd.print("Opening Hours");
    }


    else if (voegopeneningstijdentoe == true) { // if you want to remove the opening hours.
      voegopeneningstijdentoe = false; // put the bools right.
      verwijderopeneningstijdentoe = true;
      lcd.clear(); // print the right one on the screen.
      lcd.print("Remove Opening");
      lcd.setCursor(0, 1);
      lcd.print("Time");


    }
    else if (removerfid == true) { // If the menu to remove the rfid for the patient
      addrfid3 = false; // set the bools correctly.
      removerfid = false;
      removerfid3 = true;
      lcd.clear();
      lcd.print("Remove RFID"); // print what should be on the screen.
      lcd.setCursor(0, 1);
      lcd.print("Patient");

    }
    else if (addrfid3 == true) { // add the menu for the rfid for the nurse


      addrfid3 = false; // put the bools right.
      removerfid = true;
      lcd.clear();
      lcd.print("Remove RFID"); // print what to put on it.
      lcd.setCursor(0, 1);
      lcd.print("Nurse");

    }
    else if (datechange == true) { // If the menu for the date is there.
      if (sleutelsVerpleger.heeftMastercardToegang(rfidCard.c_str())) { // verifies that it is the master card accessed.
        datechange = false; // put the bools right.
        addrfid = true;
        lcd.clear();
        lcd.print("Add RFID"); // print the correct text.
        lcd.setCursor(0, 1);
        lcd.print("Nurse");
      }
    }

    else if (addrfid == true) { // if there is on the menu
      datechange = false; // set the bools right
      addrfid3 = true;
      addrfid = false;
      lcd.clear();
      lcd.print("Add RFID"); // print the text that should be on it.
      lcd.setCursor(0, 1);
      lcd.print("Patient");
    }

    else if (achtergrond == true) { // if he is in the menu background.
      if  (achtergrond2 == false) { // if he is not in the menu for the background lighting to adjust.
        timechange = true;
        achtergrond = false;
        lcd.clear();
        lcd.print("Time"); // print the text that should be on it.
      }


      if (achtergrond2 == true) { // if he does is in the menu for background lighting to adjust.
        if (achtergrond1 == true) { //  when the backlight is on.
          achtergrond1 = false; // Turn off the backlight.
          lcd.clear(); // put the right one on the LCD.
          lcd.print("OFF");

        }
        else { // when the backlight is on.
          achtergrond1 = true; // backlight is on
          lcd.clear(); // Print the right one on the screen.
          lcd.print("ON");
        }
      }
    }


    else if  (schermvergrendeling == true) {// if the screen is off when locked
      controldata = true; // put the bools right.
      schermvergrendeling = false;
      lcd.clear(); // put the right thing on the screen.
      lcd.print("Check");

    }



    else  if (controldata == true) { // If the menu for checking is there.
      timechange = false; // put the bools right.
      achtergrond = true;
      controldata = false;
      lcd.clear(); // Print the right one on the screen.

      lcd.print("Background");
      lcd.setCursor(0, 1);
      lcd.print("lighting");
    }



    if (schermvergrendeling2 == true) { // If the menu for the screen when locked is there.

      lcd.clear();
      if (schermvergrendelingaanuit == true) { // if it is on.
        lcd.print("OFF");
        schermvergrendelingaanuit = false; // it is turned off.
      }
      else { // when it's out.
        lcd.print("ON");
        schermvergrendelingaanuit = true; // it is turned on
      }

    }
    else if (vergrendelingvoortijdstip2 == true) { // for the locking of the time
      if (vergrendelingvoortijdstip1 == true) { // when it is on it becomes off.
        lcd.clear(); // print het juste
        lcd.print("OFF");
        vergrendelingvoortijdstip1 = false; // put the bools right
      }

      else {// Otherwise when it is off it becomes on.
        lcd.clear(); // print het juiste
        lcd.print("ON");
        vergrendelingvoortijdstip1 = true; // set the bools correctly.
      }

    }


    if (sethopeningstijden == true) { // if the time has to be changed for opening hours
      hopeningtijden = hopeningtijden + 1; // adds 1.
      if (hopeningtijden > 23) {
        hopeningtijden = 0; // if the hours are greater than 23 it goes to 0.
      }

    }

    if (setmopeningstijden == true) { //if the time is to be changed for opening minutes.
      mopeningtijden = mopeningtijden  + 1; // Adds 1.
      if (mopeningtijden > 59) { // als de minutes gorter are then 59 it becomes 0.
        mopeningtijden = 0; // set it to 0.
        hopeningtijden = hopeningtijden + 1; // Adds an hour.
      }

    }

    if (seth == true) { //if the time is changed that appears on the screen when it is locked.
      h = h + 1;  // add an hour

    }
    if (setm == true) { // if the time is changed that appears on the screen when it is locked.
      m = m + 1; // Adds a minute.
    }
  }
  else if (digitalRead(buttonrechts) == LOW) { // the right button.

    Serial.println("buttonrechts");
    delay(300);

    if (addrfid == true) { // If you want to add the rfid for the nurse.
      addrfid2 = true; // set the bools right
      addrfid = false;
      lcd.clear(); // print the right one on the screen.
      lcd.print("Place the RFID");
    }
    else if (addrfid2 == true) { // if you want to cancel it for the rfid add for the nurse.
      addrfid = true; // put the bools right.
      addrfid2 = false;
      lcd.clear(); // print the right one on the screen.
      lcd.print("Add RFID");
      lcd.setCursor(0, 1);
      lcd.print("Nurse");
    }
    else if (addrfid3 == true) { // If you want to add the rfid for the patient.
      addrfid4 = true; // put the bools right
      addrfid3 = false;
      lcd.clear(); // put the right one on the LCD.
      lcd.print("Place the RFID");
    }
    else if (addrfid4 == true) { // If you want to cancel it for the rfid add for the patient.
      addrfid3 = true; // put the bools right.
      addrfid4 = false;
      lcd.clear(); // Put the right thing on the screen.
      lcd.print("Add RFID");
      lcd.setCursor(0, 1);
      lcd.print("Patient");
    }
    else if (removerfid == true) { // if you want to remove the rfid for the nurse.
      removerfid2 = true; // put the bools right
      removerfid = false;
      lcd.clear(); // Put the right thing on the screen.
      lcd.print("Place the RFID");
    }
    else if (removerfid2 == true) { // If you want to cancel it for the rfid remove for the nurse.
      removerfid = true; // put the bools right.
      removerfid2 = false;
      lcd.clear(); // Print the right one on the screen.
      lcd.print("Remove RFID");
      lcd.setCursor(0, 1);
      lcd.print("Nurse");
    }
    else if (removerfid3 == true) { // if you want to remove the rfid before the patient.
      removerfid4 = true; // put the bools right.
      removerfid3 = false;
      lcd.clear(); // Put the right thing on the screen.
      lcd.print("Place the RFID");
    }
    else if (removerfid4 == true) { //if you want to cancel it for the rfid verijwderen for the patient.
      removerfid3 = true; // put the bools right.
      removerfid4 = false;
      lcd.clear(); // print the right one on the screen.
      lcd.print("Remove RFID");
      lcd.setCursor(0, 1);
      lcd.print("Patient");
    }
    else if (voegopeneningstijdentoe == true) { // for the opening hours to add.
      voegopeneningstijdentoe = false; // put the bools right.
      voegopeneningstijden2toe = true;
      setmopeningstijden = false;
      sethopeningstijden = true;
    }
    else if (voegopeneningstijden2toe == true) { // for the opening times to add.
      printtime2(); // print the time for the opening times,

      if (sethopeningstijden == true) { // if the minutes are entered
        setmopeningstijden = true;
        sethopeningstijden = false;
      } else if (setmopeningstijden == true) { // stores it.
        Tijdenvooropenen.voegtijdstiptoe((String(hopeningtijden) + ":" + String(mopeningtijden) + ":" + String(sopeningtijden)).c_str());
        voegopeneningstijdentoe = true; // put the bools back right.
        voegopeneningstijden2toe = false;
        setmopeningstijden = false;
        sethopeningstijden = false;
        lcd.clear(); // print the correct
        lcd.print("Opening Time");
        lcd.setCursor(0, 1);
        lcd.print("add");

      }
      else { // verander de bools voor de openingstijden.
        sethopeningstijden = true;
        setmopeningstijden = false;
        printtime2(); // is printed on the LCD.
      }

      printtime2(); // print the time for the opening times.


    }
    else if (verwijderopeneningstijdentoe == true) { // to remove the opening hours
      verwijderopeneningstijdentoe = false;
      verwijderopeneningstijden2toe = true;
      setmopeningstijden = false;
      sethopeningstijden = true;
    }
    else if (verwijderopeneningstijden2toe == true) { // for removal of opening hours.
      printtime2(); // for the time set that you want to delete.

      if (sethopeningstijden == true) { // for the minutes to adjust.
        setmopeningstijden = true; // put the bools right.
        sethopeningstijden = false;
      } else if (setmopeningstijden == true) { // if an opening hour needs to be removed.
        Tijdenvooropenen.verwijdertijdstip((String(hopeningtijden) + ":" + String(mopeningtijden) + ":" + String(sopeningtijden)).c_str()); // removed the opening time.
        verwijderopeneningstijdentoe = true; // put bools right
        verwijderopeneningstijden2toe = false;
        setmopeningstijden = false;
        sethopeningstijden = false;
        lcd.clear(); // print the right one on the screen.
        lcd.print("Opening Time");
        lcd.setCursor(0, 1);
        lcd.print("remove");

      }
      else { // otherwise for the hours change/
        sethopeningstijden = true; // set the bools correctly.
        setmopeningstijden = false;
      }


    }


    else if (vergrendelingvoortijdstip == true) { // for adjusting if the patient can open the safe earlier.
      vergrendelingvoortijdstip = false; // set the bools right
      vergrendelingvoortijdstip2 = true;
      if (vergrendelingvoortijdstip1 == true) { // print whether it is already on or off.
        lcd.clear();
        lcd.print("ON");
      }
      else {
        lcd.clear();
        lcd.print("OFF");
      }

    }
    else if (vergrendelingvoortijdstip2 == true) { // if you want to go out of the menu
      vergrendelingvoortijdstip2 = false; // set the bools right
      vergrendelingvoortijdstip = true;
      lcd.clear(); // print the right.
      lcd.print("Locking device for");
      lcd.setCursor(0, 1);
      lcd.print("opening hours");
    }





    else  if (datechange == true) { // if you want to change the date, the date is printed, for that to change.
      datechange = false; // set the bools correctly.
      datechange2 = true;
      setdag = true;

    }


    else if (datechange2 == true) { // for change from day to month to year.
      lcd.clear();
      if (setdag == true) { // of changing the day you are now going to change the month.
        setmaand = true; // set the bools right.
        setdag = false;
      }
      else if (setmaand == true) { // van de maand te verandren naar jaar te veranderen.
        setjaar = true; // put the bools right.
        setmaand = false;
      }
      else if (setjaar == true) { // as you are done with the month/year/day change.
        datechange2 = false;// put the bools right.
        datechange = true;
        setjaar = false;
        lcd.clear(); // print the right one.
        lcd.print("Date");
      } else {
        setdag = true; // if none of the 3 is true, setday automatically becomes true.
      }
    }
    else if (controldata == true) { // if you want to check the data
      controldata = false; // set the bools right
      controldata1 = true;
      lcd.clear();
      lcd.print(Data2 + Data66);
      scrolldisplay(); // causes the data to appear on the screen.

    }
    else if (controldata1 == true) { // if you want to go out of the menu to check the data.
      controldata = true; // set the bools right
      controldata1 = false;
      lcd.clear(); // print the right one on the screen.
      lcd.print("Check");
      Data66 = ""; // reset the data.

    }
    else if (bekijktijden == true) { // if you want to see the times
      bekijktijden1 = true; // put the bools right
      bekijktijden =  false;
      lcd.clear();
      lcd.print(Tijdenvooropenen.printtijdenlcd());  // Print the times on the LCD.
    }
    else if (bekijktijden1 == true) { // for opening hours to view.
      bekijktijden = true; // put the bools right
      bekijktijden1 = false;
      lcd.clear(); // Print it correctly on the LCD,
      lcd.print("Opening hours");
      lcd.setCursor(0, 1);
      lcd.print("view");
    }


    if (schermvergrendeling == true) { // if you want to turn the screen on or off when locking.
      schermvergrendeling2 = true; // put the bools right
      schermvergrendeling = false;
      lcd.clear();
      if (schermvergrendelingaanuit == true) { // if it is set to on, it prints on.
        lcd.print("ON");
      }
      else { // if it is set to off it prints out.
        lcd.print("OFF");
      }

    } else if (schermvergrendeling2 == true) { // for getting out of the menu for having the screen on or off when locked.
      schermvergrendeling2 = false; // set the bools correctly.
      schermvergrendeling = true;
      lcd.clear(); // print the right one on the screen.
      lcd.print("Screen by");
      lcd.setCursor(0, 1);
      lcd.print("lock");


    }

    if (timechange == true) { // if you want to change the time
      timechange = false; // set the bools right
      Time = true;
      seth = true;

    }

    else   if (seth == true) { // if you want to change the hours to minutes
      seth = false;
      setm = true;
    }
    else if (setm == true) { // if you want to go out of the menu for changing the time.
      seth = false; // set the bools correctly.
      setm = false;
      Time = false;
      timechange = true;
      lcd.clear(); // print the right one on the screen.
      lcd.print("Time");
    }
    if (achtergrond2 == true) // for exit from the backlight adjustment menu.
    {
      timechange = false; // set the bools correctly.
      achtergrond2 = false;
      achtergrond = true;
      lcd.clear(); // print the right thing on the screen
      lcd.print("Background");
      lcd.setCursor(0, 1);
      lcd.print("lighting");
    }
    else if (achtergrond == true) { // for enter the menu to adjust the backlight.
      achtergrond2 = true;
      // achtergrond  = false;

      if (achtergrond1 == true) { // if it is on it prints on.

        lcd.clear();
        lcd.print("ON");
      }
      else // otherwise it prints out.
      {

        lcd.clear();
        lcd.print("OFF");
      }

    }

  }
  else if (digitalRead(buttononder) == LOW) {// the bottom button.
    Serial.println("buttononder");


    delay(300);
    if (setdag == true) { // for the days to be adjusted.
      dag = dag - 1; // is counting down a day
      if (dag <= 0) { // if the days are less than or equal to zero, it becomes 1.
        dag = 1;
      }
    }
    else if (setmaand == true) { // for the months to change
      maand = maand - 1; // counts down 1 month.
      if (maand <= 0) { // if the monther is less than or equal to 0 that he makes it back 1.
        maand = 1;
      }

    }
    else if (setjaar == true) { // for the years to change
      jaar = jaar - 1; // counts down a year.
    }
    if (datechange == true) { // for a menu to go down, to the time menu
      datechange = false; // set the bools right
      timechange = true;
      lcd.clear(); // print what should be on the screen.
      lcd.print("Time");
    }
    else if (bekijktijden == true) {
      vergrendelingvoortijdstip = true;
      bekijktijden = false;
      lcd.clear(); // Print the right one on the screen.
      lcd.print("Lock for");
      lcd.setCursor(0, 1);
      lcd.print("Opening Hours");
    }


    else if (vergrendelingvoortijdstip == true) { // if you want to go to the menu for the opening time delete.
      verwijderopeneningstijdentoe = true; // put the bools right.
      vergrendelingvoortijdstip = false;
      lcd.clear(); // Print the right one on the screen.
      lcd.print("Opening Time");
      lcd.setCursor(0, 1);
      lcd.print("Remove");
    }
    else if (verwijderopeneningstijdentoe == true) { // if you want to go to the menu to add the opening hours.
      verwijderopeneningstijdentoe = false; // set the bools right
      voegopeneningstijdentoe = true;
      lcd.clear(); // Print the right one on the screen.
      lcd.print("Opening Time");
      lcd.setCursor(0, 1);
      lcd.print("Add");


    }
    else if (voegopeneningstijdentoe == true) { // to go to the menu for removing an rfid.
      voegopeneningstijdentoe = false;
      verwijderopeneningstijdentoe = false;
      removerfid3 = true; // put the bools right.
      lcd.clear(); // print the right one on the screen.
      lcd.print("Remove RFID");
      lcd.setCursor(0, 1);
      lcd.print("Patient");



    }


    else if (removerfid3 == true) { // if you want to go one to bottom, to rfid remove nurse.
      addrfid3 = false;// set the bools right
      removerfid = true;
      removerfid3 = false;
      lcd.clear(); // print the right one on the screen.
      lcd.print("Remove RFID");
      lcd.setCursor(0, 1);
      lcd.print("Nurse");

    }
    else if (removerfid == true) { // if you want to add a menu to the bottom for the rfid for the patient.
      addrfid3 = true; // set the bools correctly.
      removerfid = false;
      lcd.clear(); //  print the right one on the screen.
      lcd.print("Add RFID");
      lcd.setCursor(0, 1);
      lcd.print("Patient");

    }
    else if (addrfid3 == true) { // if you want to go down one menu again, to add an rfid for a patient.
      addrfid3 = false; // put the bools right;
      addrfid = true;
      lcd.clear(); // Print the right one on the screen.
      lcd.print("Add RFID");
      lcd.setCursor(0, 1);
      lcd.print("Nurse");
    }
    else if (addrfid == true) { // if you want to go down a menu to change the date.
      datechange = true; // put the bools right.
      addrfid = false;
      lcd.clear(); //Print the right one on the screen.
      lcd.print("Date");
    }



    else if (achtergrond == true) { // if you want to turn the backlight on or off.
      if (achtergrond2 == true) {
        if (achtergrond1 == true) { // if the on is on it goes off.
          achtergrond1 = false;
          lcd.clear();
          lcd.print("OFF");

        }
        else { //if it is off it goes on
          achtergrond1 = true;// the bools right
          lcd.clear(); // print the right.
          lcd.print("ON");
        }
      }
      else if (achtergrond == true) { // if you want to go down a menu to have the control menu,
        timechange = false; // put the bools right
        achtergrond = false;
        controldata = true;
        lcd.clear(); // print the correct
        lcd.print("Check");
      }


    }
    else if (timechange == true) { // if you want a menu down from the time change to the background lighting.
      timechange = false; // set the bools correctly.
      achtergrond = true;
      controldata = false;
      lcd.clear(); // print the right one on the screen.
      lcd.print("Background");
      lcd.setCursor(0, 1);
      lcd.print("Lighting");

    }


    else if  (controldata == true) { // if he wants to go from the control to the screen when locked.
      controldata = false; // set the bools right
      schermvergrendeling = true;
      lcd.clear(); // print the right one on the screen.
      lcd.print("Screen in");
      lcd.setCursor(0, 1);
      lcd.print("lock State");
    }


    if (schermvergrendeling2 == true) { // if you are in the menu of screen when locking

      lcd.clear();
      if (schermvergrendelingaanuit == true) { // when it's on it goes off;
        lcd.print("OFF"); // print uit:
        schermvergrendelingaanuit = false; // put the bool right
      }
      else { // when it is off it turns on;
        lcd.print("ON");  // print on:
        schermvergrendelingaanuit = true; // put the bool right
      }

    }

    else if (vergrendelingvoortijdstip2 == true) { // if it has to be locked before it can be opened
      if (vergrendelingvoortijdstip1 == true) { // when it is on it turns off
        lcd.clear(); // print the correct
        lcd.print("OFF");
        vergrendelingvoortijdstip1 = false; // set the bools correctly.
      }

      else { // when it is off it becomes on.
        lcd.clear(); // print the correct
        lcd.print("ON");
        vergrendelingvoortijdstip1 = true; // set the bools correctly.
      }

    }

    if (sethopeningstijden == true) { // als de openins uren moet gewijzigd worden
      hopeningtijden = hopeningtijden - 1; // Counts down 1.
      if (hopeningtijden < 0) {
        hopeningtijden = 23; // if the hours are less than 0 it goes to 23.
      }

    }

    if (setmopeningstijden == true) { // if the opening minutes need to be changed,
      mopeningtijden = mopeningtijden  - 1; // Counts down 1.
      if (mopeningtijden < 0) { // if the minutes are less than 0 it becomes 59.
        mopeningtijden = 59; // put it at 59.
        hopeningtijden = hopeningtijden - 1; // is counting down an hour.
      }

    }

    if (seth == true) { // if the time needs to be changed, one counts down.
      h = h - 1; // counts down 1.
      if (h < 0) {
        h = 23; // if the hours are less than 0 it goes to 23.
      }

    }

    if (setm == true) { // If you want to take a minute off.
      m = m - 1; // counts down 1.
      if (m < 0) { // if the minutes are less than 0 it becomes 59.
        m = 59; // put it at 59.
        h = h - 1; // is counting down an hour.
      }

    }
  }
  else if (digitalRead(buttonlinks) == LOW) { // Locked back the arduino.
    Serial.println("buttonlinks");
    delay(200);
    accesaccepted = false; // put all bools how it should be when locked.
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
    voegopeneningstijdentoe = false; 
    voegopeneningstijden2toe = false;
    verwijderopeneningstijdentoe = false;
    verwijderopeneningstijden2toe = false;
    sethopeningstijden = false;
    setmopeningstijden = false;
    bekijktijden = false;
    bekijktijden1 = false;
    vergrendelingvoortijdstip = false; 
    vergrendelingvoortijdstip2 = false;

  }



  if (rfid.isCard()) {  // looks for an rfid card.
    if (rfid.readCardSerial()) {// reads the rfid card number
      rfidCard = String(rfid.serNum[0]) + " " + String(rfid.serNum[1]) + " " + String(rfid.serNum[2]) + " " + String(rfid.serNum[3]); // Extracts the number from the rfid card.
      Serial.println(rfidCard); // print the number of the rfid.
      if (addrfid2 == true) { // if an rfid needs to be added for nurse.
        // when the rfid is added it goes back to the menu.
        addrfid2 = false;  // so set the bools correctly.
        addrfid = true;
        lcd.clear(); // print what should be on the screen
        lcd.print("Add RFID");
        lcd.setCursor(0, 1);
        lcd.print("Nurse");
        sleutelsVerpleger.voegSleutelToe(rfidCard.c_str());//The key is added
        sleutelsVerpleger.printSleutels(); // Keys are printed from the nurse



      } else if (addrfid4 == true) { // if an rfid needs to be added for patient.

        addrfid4 = false; // when the rfid is added it goes back to the menu. // so set the bools correctly.
        addrfid3 = true;
        lcd.clear(); // print what should be on it.
        lcd.print("Add RFID");
        lcd.setCursor(0, 1);
        lcd.print("Patient");
        sleutelsPatienten.voegSleutelToe(rfidCard.c_str()); // Adds the keys for the patient.
        sleutelsPatienten.printSleutels(); // Keys are printed from of the patient.

      }
      else if (removerfid2 == true) { // if you want to remove the rfid card for the nurse
        //RFIDs = rfidCard;
        removerfid2 = false; // set the bools correctly.
        removerfid = true;
        lcd.clear(); // print the right one on the screen.
        lcd.print("Remove RFID");
        lcd.setCursor(0, 1);
        lcd.print("Nurse");
        sleutelsVerpleger.verwijderSleutel(rfidCard.c_str()); // removed the rfid card.
        sleutelsVerpleger.printSleutels(); // print the keys

      } else if (removerfid4 == true) { // If you want to remove the rfid card for the patient

        removerfid4 = false; // put the bools right
        removerfid3 = true;
        lcd.clear(); // Print the right one on the screen.
        lcd.print("Remove RFID");
        lcd.setCursor(0, 1);
        lcd.print("Patient");
        sleutelsPatienten.verwijderSleutel(rfidCard.c_str()); // removed the rfid for the patient.
        sleutelsPatienten.printSleutels(); // print keys in the Serial port.

      }



      else if (sleutelsVerpleger.heeftSleutelToegang(rfidCard.c_str())) {/*|| rfidCard == RFIDs*/// verifies that the nurse has access;
        if (m < 10) { // als de minuten kleiner zijn als 10.
          Data55 =  String(h) + ":" + "0" + String(m)  + ", " + Data55; // When the nurse has opened it, it comes to the control.
        } else { // als de minuten groter of gelijk aan 10 zijn..
          Data55 =  String(h) + ":" + String(m)  + ", " + Data55; // when the nurse has opened it, it comes to the conrole.
        }
        Serial.println(Data55); // print the data
        Serial.println("Card Accepted!");
        opendeur();
        deuropenclose = true;


        delay(200);
        accesaccepted = true; // put the bools right in front of the unlocking of the safe for the nurse
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
        voegopeneningstijdentoe = false; 
        voegopeneningstijden2toe = false;
        verwijderopeneningstijdentoe = false; 
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
        lcd.clear(); // print what should be on the screen, starts at the menu of time.
        lcd.print("Time");





      } else if (sleutelsPatienten.heeftSleutelToegang(rfidCard.c_str())) { // verifies that the patient's key is correct.

        if (vergrendelingvoortijdstip1 == false) {
          opendeur();
          deuropenclose = true; // opent de deur
          if (m < 10) { // if the menuten is smaller than 10 then there is another zero for the minutes
            Data66 =  String(h) + ":" + "0" + String(m)  + ", " + Data66;
          } else { // als de minuten groter zijn
            Data66 =  String(h) + ":" + String(m)  + ", " + Data66;
          }
          heeftmedicijngenomen = true;
        }
        else if (heeftmedicijngenomen == false) {
          opendeur();
          deuropenclose = true; // opent de deur
          if (m < 10) { // if the menuten is smaller than 10 then there is another zero for the minutes
            Data66 =  String(h) + ":" + "0" + String(m)  + ", " + Data66;
          } else { // als de minuten groter zijn
            Data66 =  String(h) + ":" + String(m)  + ", " + Data66;
          }
          heeftmedicijngenomen = true;
        }
        // Put the bools right before the patient's release.
        accesaccepted = false; // put all bools how it should be when locked.
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
        voegopeneningstijdentoe = false; 
        voegopeneningstijden2toe = false;
        verwijderopeneningstijdentoe = false; 
        verwijderopeneningstijden2toe = false;
        sethopeningstijden = false;
        setmopeningstijden = false;
        bool bekijktijden = false;
        bool bekijktijden1 = false;
        vergrendelingvoortijdstip = false; 
    vergrendelingvoortijdstip2 = false;
        Serial.println(Data66);// print the data in the serial port
        Serial.println("Card Accepted!");


        delay(200); // wait 200ms
      }
      else { // If the card is invalid.
        lcd.clear();
        lcd.print("Invalid card!"); // print on the screen that the map is not correct
        Serial.println("Invalid card!");

        

        accesaccepted = false; // everything is locked
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

      delay(200); // wait 200 milliseconds.




    }
    rfid.halt(); // the rfid card can read back a others batch.
  }
}

// end
