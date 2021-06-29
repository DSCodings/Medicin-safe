#include "tijdvooropenen.h" // // zorgt ervoor dat de functies die worden opgeroepen in "tijdvooropenen.h" hierin worden gebruikt.
#include <String.h>



TijdStore::TijdStore(void) { // de setup, er wordt gezorgt dat er plaats voor is in het geheugen.
  for(int i=0; i< nrOfTijden; i++) {
    tijden[i] = NULL; // allen tijden worden leeg in het geheugen gezet.
  }
}


void TijdStore::voegtijdstiptoe(const char Tijden[]) { // voor een tijdstip toetevoegen.
  int l = strlen(Tijden); // voegt de tijd toe
  for(int i=0; i< nrOfTijden; i++) { // bij het eerste legen plaats die hij vindt.
    //Serial.println(i);
    if (tijden[i] == NULL) {
      tijden[i] = new char[l+1]; 
      strcpy(tijden[i], Tijden);
      tijden[i][l] = 0;
      break;
    }
  }
}

bool TijdStore::istijdstipgelijk(const char Tijden[]) { // kijkt op het tijdstip gelijk is aan het opgegeven tijdstip.
  bool toegang = false; // is bij het begin altijd niet waar
  if (nrOfTijden > 0) { // als de tijden wel gelijk is
      for(int i=0; i< nrOfTijden; i++) {
        if (strcmp(tijden[i], Tijden) == 0) {
          toegang = true; // als het gelijk is is het waar
        }
      }
  }
  return toegang; // stuurt of hij toegang heeft terug,
}

void TijdStore::verwijdertijdstip(const char Tijden[]) { // voor het verwijderen van de tijdstippen
  if (nrOfTijden > 0) { // kijkt welke tijdstip gelijk is aan het opgegeven tijdstip en verwijderd het.
    for(int i=0; i< nrOfTijden; i++) {
      if (strcmp(tijden[i], Tijden) == 0) {
        delete(tijden[i]);
        tijden[i] = 0;
      }
    }
  }
}


char* TijdStore::getkey(int nr) { // als je een tijdstip wilt ophalen
  char* key = NULL;
  if (nr < nrOfTijden) {
   key =  tijden[nr];
  }
  return key; // stuurt het tijdstip terug
}

void TijdStore::printtijden() { // voor de tijden te printen

  for(int i=0; i<nrOfTijden; i++) {
    if (tijden[i] != 0) {
      Serial.println(tijden[i]);
    }

    
  }
}

String TijdStore::printtijdenlcd() { // voor de tijden te printen op het lcd.
  String tttijden = " ";
  for(int i=0; i<nrOfTijden; i++) {
    if (tijden[i] != 0) {
      Serial.println(tijden[i]); // zorgt dat alles achterelkaar staat
      tttijden =  tttijden + ", " + String(tijden[i]);
    }

    
  }
  return tttijden; // stuurt de tijden terug.
}
