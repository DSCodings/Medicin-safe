#include <Arduino.h>  // zorgt ervoor dat het kan gebruikt worden in het programma
class SleutelStore { // hier worden allen functies ingezet die functies worden uitgewerkt in "sleutel.cpp"
  private: // private functies
    char* sleutels[50]; // aantal sleutels
    int nrOfSleutels = 50; 
    
  public: // public functies
    SleutelStore(void); // de setup van de sleutels
    void voegHoofdSleutelToe(const char Sleutel[]); // voor de hoofdsleutel toetevoegen
    void voegSleutelToe(const char Sleutel[]); // voor een sleutel toetevoegen
    bool heeftSleutelToegang(const char Sleutel[]); // voor te kijken of de sleutel toegang heeft.
    void verwijderSleutel(const char Sleutel[]);  // voor een sleutel te verwijderen
    bool heeftMastercardToegang(const char Sleutel[]); //kijken of de hoofdsleutel toegang heeft
    char* getkey(int nr); // voor een sleutel nummer op te vragen
    void printSleutels(); // voor de sleutels de printen in de serial poort.
};// einde
