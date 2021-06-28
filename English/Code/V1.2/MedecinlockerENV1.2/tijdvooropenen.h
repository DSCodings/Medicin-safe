
#include <Arduino.h>
class TijdStore {
  private:
    char* tijden[20];
    int nrOfTijden = 20;
    
  public:
    TijdStore(void);
    //void voegHoofdSleutelToe(const char Tijden[]);
    void voegtijdstiptoe(const char Tijden[]);
    bool istijdstipgelijk(const char Tijden[]);
    void verwijdertijdstip(const char Tijden[]); 
   // bool heeftMastercardToegang(const char Tijdentel[]);
    char* getkey(int nr);
   
    void printtijden();
    String printtijdenlcd();
};
