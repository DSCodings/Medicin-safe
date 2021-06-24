#include "sleutel.h"
#include <String.h>

SleutelStore::SleutelStore(void) {
  for(int i=0; i< nrOfSleutels; i++) {
    sleutels[i] = NULL;
  }
}

void SleutelStore::voegHoofdSleutelToe(const char Sleutel[]) {
  if (sleutels[0] == NULL) {
    Serial.println("geen rfid sleutel gevoenden!");
  }
  int l = strlen(Sleutel);
  sleutels[0] = new char[l+1]; 
  strcpy(sleutels[0], Sleutel);
  sleutels[0][l] = 0;
}

void SleutelStore::voegSleutelToe(const char Sleutel[]) {
  int l = strlen(Sleutel);
  for(int i=0; i< nrOfSleutels; i++) {
    //Serial.println(i);
    if (sleutels[i] == NULL) {
      sleutels[i] = new char[l+1]; 
      strcpy(sleutels[i], Sleutel);
      sleutels[i][l] = 0;
      break;
    }
  }
}

bool SleutelStore::heeftSleutelToegang(const char Sleutel[]) {
//  Serial.println("11111111111111");
  Serial.println(Sleutel);
 // Serial.println("2222222222222222222");
  bool toegang = false;
  if (nrOfSleutels > 0) {
      for(int i=0; i< nrOfSleutels; i++) {
        if (strcmp(sleutels[i], Sleutel) == 0) {
          toegang = true;
        }
      }
  }
  Serial.println(toegang);
  return toegang;
}
bool SleutelStore::heeftMastercardToegang(const char Sleutel[]) {
 // Serial.println("11111111111111");
  Serial.println(Sleutel);
//  Serial.println("2222222222222222222");
  bool toegang = false;
  
      
  if (strcmp(sleutels[0], Sleutel) == 0) {
    toegang = true;
  }
      
  
  Serial.println(toegang);
  return toegang;
}

void SleutelStore::verwijderSleutel(const char Sleutel[]) {
  if (nrOfSleutels > 0) {
    for(int i=0; i< nrOfSleutels; i++) {
      if (strcmp(sleutels[i], Sleutel) == 0) {
        delete(sleutels[i]);
        sleutels[i] = 0;
      }
    }
  }
}

char* SleutelStore::getkey(int nr) {
  char* key = NULL;
  if (nr < nrOfSleutels) {
   key =  sleutels[nr];
  }
  return key;
}

void SleutelStore::printSleutels() {
  for(int i=0; i<nrOfSleutels; i++) {
    if (sleutels[i] != 0) {
      Serial.println(sleutels[i]);
    }
  }
}
