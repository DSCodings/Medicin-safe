#include "tijdvooropenen.h" // // ensures that the functions called in "tijdvooropen.h" are used in it.
#include <String.h>



TijdStore::TijdStore(void) { // the setup, it is ensured that there is room for it in memory.
  for(int i=0; i< nrOfTijden; i++) {
    tijden[i] = NULL; // all times are put in memory empty.
  }
}


void TijdStore::voegtijdstiptoe(const char Tijden[]) { // for a time to add.
  int l = strlen(Tijden); // adds the time
  for(int i=0; i< nrOfTijden; i++) { // at the first empty place it finds.
    //Serial.println(i);
    if (tijden[i] == NULL) {
      tijden[i] = new char[l+1]; 
      strcpy(tijden[i], Tijden);
      tijden[i][l] = 0;
      break;
    }
  }
}

bool TijdStore::istijdstipgelijk(const char Tijden[]) { // checks for the time equal to the specified time.
  bool toegang = false; // is always not true at the beginning
  if (nrOfTijden > 0) { // if the times do equal
      for(int i=0; i< nrOfTijden; i++) {
        if (strcmp(tijden[i], Tijden) == 0) {
          toegang = true; // if it is equal it is true
        }
      }
  }
  return toegang; // sends back whether he has access,
}

void TijdStore::verwijdertijdstip(const char Tijden[]) { // for removing the times
  if (nrOfTijden > 0) { // looks at which time is equal to the specified time and removes it.
    for(int i=0; i< nrOfTijden; i++) {
      if (strcmp(tijden[i], Tijden) == 0) {
        delete(tijden[i]);
        tijden[i] = 0;
      }
    }
  }
}


char* TijdStore::getkey(int nr) { // if you want to retrieve a time
  char* key = NULL;
  if (nr < nrOfTijden) {
   key =  tijden[nr];
  }
  return key; // returns the time
}

void TijdStore::printtijden() { // for the times to print

  for(int i=0; i<nrOfTijden; i++) {
    if (tijden[i] != 0) {
      Serial.println(tijden[i]);
    }

    
  }
}

String TijdStore::printtijdenlcd() { // for the times to print on the LCD.
  String tttijden = " ";
  for(int i=0; i<nrOfTijden; i++) {
    if (tijden[i] != 0) {
      Serial.println(tijden[i]); // makes sure everything is in order
      tttijden =  tttijden + ", " + String(tijden[i]);
    }

    
  }
  return tttijden; // returns the times.
}
