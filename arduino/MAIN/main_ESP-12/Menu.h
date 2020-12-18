#include "MyTFT.h"
#include "Buttons.h"

MyTFT myTFT;
Pushed pushed;


class Menu {
  private:
    byte x0 = 1;
    byte x = 1;

  public:
    void showMenu() {
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
      tft.print("exit");

    }

    void Cursor() {
      if (pushed.nextPrevious()) {
        x0 = x;
      }
      myTFT.Settings(1, 10, x0);
      tft.print(" ");
      x = pushed.state * 15 - 5;
      myTFT.Settings(1, 10, x);
      tft.print(">");
    }

    void select() {
      switch (pushed.state) {
        case 1:
          session();
          break;
        case 2:
          screens();
          break;
        case 3:
          saving();
          break;
        case 4:
          Stats();
          break;
        case 5:
          wifi();
          break;
        case 6:
          timeZone();
          break;
        case 7:
          memory();
          break;
        case 8:
          Exit();
          break;
        default:
          break;
      }
    }

    void session() {
      myTFT.Settings(2, 10, 15);
      tft.print("session");
    }

    void screens() {
      myTFT.Settings(2, 10, 15);
      tft.print("screens");
    }

    void saving() {
      myTFT.Settings(2, 10, 15);
      tft.print("saving");
    }

    void Stats() {
      myTFT.Settings(2, 10, 15);
      tft.print("stats");
    }

    void wifi() {
      myTFT.Settings(2, 10, 15);
      tft.print("wifi");
    }

    void timeZone() {
      myTFT.Settings(2, 10, 15);
      tft.print("time zone");
    }

    void memory() {
      myTFT.Settings(2, 10, 15);
      tft.print("memory");
    }

    void Exit() {
      pushed.menuState = 0;
      pushed.state = 1;
    }
};
