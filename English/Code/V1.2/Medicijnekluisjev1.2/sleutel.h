#include <Arduino.h>
class SleutelStore {
  private:
    char* sleutels[50];
    int nrOfSleutels = 50;
    
  public:
    SleutelStore(void);
    void voegHoofdSleutelToe(const char Sleutel[]);
    void voegSleutelToe(const char Sleutel[]);
    bool heeftSleutelToegang(const char Sleutel[]);
    void verwijderSleutel(const char Sleutel[]); 
    bool heeftMastercardToegang(const char Sleutel[]);
    char* getkey(int nr);
    void printSleutels();
};
