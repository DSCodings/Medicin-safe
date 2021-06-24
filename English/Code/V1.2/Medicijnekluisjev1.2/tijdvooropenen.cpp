#include "tijdvooropenen.h"
#include <String.h>



TijdStore::TijdStore(void) {
  for(int i=0; i< nrOfTijden; i++) {
    tijden[i] = NULL;
  }
}

/*void SleutelStore::voegHoofdSleutelToe(const char Sleutel[]) {
  if (sleutels[0] == NULL) {
    Serial.println("geen rfid tijdstippen gevoenden!");
  }
  int l = strlen(Sleutel);
  sleutels[0] = new char[l+1]; 
  strcpy(sleutels[0], Sleutel);
  sleutels[0][l] = 0;
}*/

void TijdStore::voegtijdstiptoe(const char Tijden[]) {
  int l = strlen(Tijden);
  for(int i=0; i< nrOfTijden; i++) {
    //Serial.println(i);
    if (tijden[i] == NULL) {
      tijden[i] = new char[l+1]; 
      strcpy(tijden[i], Tijden);
      tijden[i][l] = 0;
      break;
    }
  }
}

bool TijdStore::istijdstipgelijk(const char Tijden[]) {
//  Serial.println("11111111111111");
 // Serial.println(Tijden);
 // Serial.println("2222222222222222222");
  bool toegang = false;
  if (nrOfTijden > 0) {
      for(int i=0; i< nrOfTijden; i++) {
        if (strcmp(tijden[i], Tijden) == 0) {
          toegang = true;
        }
      }
  }
  //Serial.println(toegang);
  return toegang;
}
/*bool SleutelStore::heeftMastercardToegang(const char Tijden[]) {
 // Serial.println("11111111111111");
  Serial.println(Sleutel);
//  Serial.println("2222222222222222222");
  bool toegang = false;
  
      
  if (strcmp(sleutels[0], Sleutel) == 0) {
    toegang = true;
  }
      
  
  Serial.println(toegang);
  return toegang;
}*/

void TijdStore::verwijdertijdstip(const char Tijden[]) {
  if (nrOfTijden > 0) {
    for(int i=0; i< nrOfTijden; i++) {
      if (strcmp(tijden[i], Tijden) == 0) {
        delete(tijden[i]);
        tijden[i] = 0;
      }
    }
  }
}


char* TijdStore::getkey(int nr) {
  char* key = NULL;
  if (nr < nrOfTijden) {
   key =  tijden[nr];
  }
  return key;
}

void TijdStore::printtijden() {

  for(int i=0; i<nrOfTijden; i++) {
    if (tijden[i] != 0) {
      Serial.println(tijden[i]);
    }

    
  }
}

String TijdStore::printtijdenlcd() {
  String tttijden = " ";
  for(int i=0; i<nrOfTijden; i++) {
    if (tijden[i] != 0) {
      Serial.println(tijden[i]);
      tttijden =  tttijden + ", " + String(tijden[i]);
    }

    
  }
  return tttijden;
}
