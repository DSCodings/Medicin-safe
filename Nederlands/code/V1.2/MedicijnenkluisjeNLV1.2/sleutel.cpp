#include "sleutel.h"  // zorgt ervoor dat de functies die worden opgeroepen in "sleutel.h" hierin worden gebruikt.
#include <String.h> 

SleutelStore::SleutelStore(void) {  // de setup, er wordt gezorgt dat er plaats voor is in het geheugen.
  for(int i=0; i< nrOfSleutels; i++) { 
    sleutels[i] = NULL; // allen sleutels worden leeg in het geheugen gezet. 
  }
}

void SleutelStore::voegHoofdSleutelToe(const char Sleutel[]) { // voor de hoofdsleutel toe te voegen.
  if (sleutels[0] == NULL) { // als er geen plaats vrij is in het geheugen voor de sleutels
    Serial.println("geen rfid sleutel gevoenden!");
  }
  int l = strlen(Sleutel); // de sleutel wordt toegevoegd.
  sleutels[0] = new char[l+1]; 
  strcpy(sleutels[0], Sleutel);
  sleutels[0][l] = 0;
}

void SleutelStore::voegSleutelToe(const char Sleutel[]) { // voor een gewone sleutel toetevoegen.
  int l = strlen(Sleutel); // sleutel wordt toegevoegd
  for(int i=0; i< nrOfSleutels; i++) { // bij het eerste legen slot.
    if (sleutels[i] == NULL) {
      sleutels[i] = new char[l+1]; 
      strcpy(sleutels[i], Sleutel);
      sleutels[i][l] = 0;
      break;
    }
  }
}

bool SleutelStore::heeftSleutelToegang(const char Sleutel[]) { // als de sleutel toegang heeft.
  Serial.println(Sleutel);
  bool toegang = false; // toegang is bij het begin altijd niet waar
  if (nrOfSleutels > 0) { // als de sluetel wel gelijk is aan die van in het geheugen
      for(int i=0; i< nrOfSleutels; i++) {
        if (strcmp(sleutels[i], Sleutel) == 0) {
          toegang = true; // wordt de toegang waar
        }
      }
  }
  Serial.println(toegang);
  return toegang; // geeft de bool terug.
}
bool SleutelStore::heeftMastercardToegang(const char Sleutel[]) { // heeft de hoofdsleutel toegang.
  Serial.println(Sleutel); 
  bool toegang = false; // toegang is bij het begin niet waar
  
      
  if (strcmp(sleutels[0], Sleutel) == 0) { // als het wel gelijk is aan de hoofdsleutel in het geheugen is het wel waar.
    toegang = true;
  }
      
  
  Serial.println(toegang);
  return toegang; // geeft de bool terug,
}

void SleutelStore::verwijderSleutel(const char Sleutel[]) { // voor een sleutel te verwijderen.
  if (nrOfSleutels > 0) { // de sleutel wordt verweiderd die is opgegeven.
    for(int i=0; i< nrOfSleutels; i++) {
      if (strcmp(sleutels[i], Sleutel) == 0) {
        delete(sleutels[i]);
        sleutels[i] = 0;
      }
    }
  }
}

char* SleutelStore::getkey(int nr) { // voor te nummer op te vragen.
  char* key = NULL;
  if (nr < nrOfSleutels) {
   key =  sleutels[nr]; // als de nummer lager is dan het aantal sleutels wordt de sleutel code terug gestuurt.
  }
  return key; // stuurt het terug.
}

void SleutelStore::printSleutels() { // voor de sleutels te printen
  for(int i=0; i<nrOfSleutels; i++) { // print allen sleutels die in het geheugen zitten.
    if (sleutels[i] != 0) {
      Serial.println(sleutels[i]);
    }
  }
}
// einde
