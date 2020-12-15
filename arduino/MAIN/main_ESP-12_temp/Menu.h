#include "MyTFT.h"
#include "Buttons.h"

MyTFT myTFT;
Pushed pushed;


class Menu {
  private:
    byte x0 = 1;
    byte x = 1;

  public:
    void showMenu(){
      myTFT.Settings(1, 20, 10);
      tft.print("session");

      myTFT.Settings(1, 20, 25);
      tft.print("zobrazenie");

      myTFT.Settings(1, 20, 40);
      tft.print("ukladanie");

      myTFT.Settings(1, 20, 55);
      tft.print("statistiky");

      myTFT.Settings(1, 20, 70);
      tft.print("wifi");

      myTFT.Settings(1, 20, 85);
      tft.print("casove pasmo");

      myTFT.Settings(1, 20, 100);
      tft.print("pamat");

      myTFT.Settings(1, 20, 115);
      tft.print("reset");

      myTFT.Settings(1, 20, 130);
      tft.print("exit");
    } 

    void Cursor(){
      x = pushed.state*15-5;
      
      if(x0 != x){
        myTFT.Settings(1, 10, x0);
        tft.print(" ");
        myTFT.Settings(1, 10, x);
        tft.print(">");
        x0 = x;
      }
    }

    void session(){
      
    }
};
