#include "sleutel.h"  // ensures that the functions called in "sleutel.h" are used in this.
#include <String.h> 

SleutelStore::SleutelStore(void) {  // the setup, it makes sure there is room for it in memory.
  for(int i=0; i< nrOfSleutels; i++) { 
    sleutels[i] = NULL; // all keys are put in memory empty. 
  }
}

void SleutelStore::voegHoofdSleutelToe(const char Sleutel[]) { // for the master key to add.
  if (sleutels[0] == NULL) { // if there is no free space in the memory for the keys
    Serial.println("no rfid key found!");
  }
  int l = strlen(Sleutel); // the key is added.
  sleutels[0] = new char[l+1]; 
  strcpy(sleutels[0], Sleutel);
  sleutels[0][l] = 0;
}

void SleutelStore::voegSleutelToe(const char Sleutel[]) { // for an ordinary key to add.
  int l = strlen(Sleutel); // key is added
  for(int i=0; i< nrOfSleutels; i++) { // at the first empty slot.
    if (sleutels[i] == NULL) {
      sleutels[i] = new char[l+1]; 
      strcpy(sleutels[i], Sleutel);
      sleutels[i][l] = 0;
      break;
    }
  }
}

bool SleutelStore::heeftSleutelToegang(const char Sleutel[]) { // if the key has access.
  Serial.println(Sleutel);
  bool toegang = false; // access at the beginning is always not true
  if (nrOfSleutels > 0) { // if the lock is the same as the one in memory
      for(int i=0; i< nrOfSleutels; i++) {
        if (strcmp(sleutels[i], Sleutel) == 0) {
          toegang = true; // the access true
        }
      }
  }
  Serial.println(toegang);
  return toegang; // geeft de bool terug.
}
bool SleutelStore::heeftMastercardToegang(const char Sleutel[]) { // has the master key access.
  Serial.println(Sleutel); 
  bool toegang = false; // access is not true at the outset
  
      
  if (strcmp(sleutels[0], Sleutel) == 0) { // if it does equal the master key in memory it is true.
    toegang = true;
  }
      
  
  Serial.println(toegang);
  return toegang; // returns the bool,
}

void SleutelStore::verwijderSleutel(const char Sleutel[]) { // for a key to be removed.
  if (nrOfSleutels > 0) { // the key is substituted that is specified.
    for(int i=0; i< nrOfSleutels; i++) {
      if (strcmp(sleutels[i], Sleutel) == 0) {
        delete(sleutels[i]);
        sleutels[i] = 0;
      }
    }
  }
}

char* SleutelStore::getkey(int nr) { // for number to be requested.
  char* key = NULL;
  if (nr < nrOfSleutels) {
   key =  sleutels[nr]; // if the number is less than the number of keys the key code is returned.
  }
  return key; // sends it back.
}

void SleutelStore::printSleutels() { // to print the keys
  for(int i=0; i<nrOfSleutels; i++) { // prints all keys that are in memory.
    if (sleutels[i] != 0) {
      Serial.println(sleutels[i]);
    }
  }
}
// end
