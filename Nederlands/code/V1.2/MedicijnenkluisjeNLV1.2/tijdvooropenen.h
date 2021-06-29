#include <Arduino.h>   // zorgt ervoor dat het kan gebruikt worden in het programma
class TijdStore {  //hier worden allen functies ingezet die functies worden uitgewerkt in "tijdvooropenen.cpp"
  private: // private functies
    char* tijden[20]; // aantal tijden
    int nrOfTijden = 20;
    
  public: // public functies
    TijdStore(void);// voor de setup
    void voegtijdstiptoe(const char Tijden[]); // voor tijdstip toetevoegen
    bool istijdstipgelijk(const char Tijden[]); // voor te kijken of een tijdstip gelijk is.
    void verwijdertijdstip(const char Tijden[]); // voor een tijdstip te verwijderen.
    char* getkey(int nr); // voor een tijdstip op te vragen
   
    void printtijden(); // voor te printen in de serial console.
    String printtijdenlcd(); // voor te printen op het lcd.
};
// einde
