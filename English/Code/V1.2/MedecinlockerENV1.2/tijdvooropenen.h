#include <Arduino.h>   // ensures that it can be used in the program
class TijdStore {  //there are all functions deployed those functions are worked out in "tijdvooropenen.cpp"
  private: // private functions
    char* tijden[20]; // number of times
    int nrOfTijden = 20;
    
  public: // public functions
    TijdStore(void);// for the setup
    void voegtijdstiptoe(const char Tijden[]); // for a time to add
    bool istijdstipgelijk(const char Tijden[]); // for seeing if a time is equal.
    void verwijdertijdstip(const char Tijden[]); // for a time to be removed.
    char* getkey(int nr); // for a time to be requested
   
    void printtijden(); // for printing in the serial console.
    String printtijdenlcd(); // for printing on the LCD.
};

// end
