#include <Arduino.h>  // zorgt ervoor dat het kan gebruikt worden in het programma
class SleutelStore { // here all functions are deployed those functions are worked out in "sleutel.cpp"
  private: // private functions
    char* sleutels[50]; // number of keys
    int nrOfSleutels = 50; 
    
  public: // public functions
    SleutelStore(void); // the setup of the keys
    void voegHoofdSleutelToe(const char Sleutel[]); // for the master key to add
    void voegSleutelToe(const char Sleutel[]); // to add a key
    bool heeftSleutelToegang(const char Sleutel[]); // to see if the key has access.
    void verwijderSleutel(const char Sleutel[]);  // to remove a key
    bool heeftMastercardToegang(const char Sleutel[]); //  see if the master key has access
    char* getkey(int nr); // to retrieve a key number
    void printSleutels(); // For the keys the print in the serial port.
};//end
