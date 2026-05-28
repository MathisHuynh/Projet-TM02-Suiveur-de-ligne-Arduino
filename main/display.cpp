#include "Display.h"
#include "Config.h"

void gauche() {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("/   |");
    lcd.setCursor(1, 1);
    lcd.print("| ");
    lcd.write(byte(0));
    lcd.print(" /");
    lcd.setCursor(8, 0);
    lcd.print("/   |");
    lcd.setCursor(8, 1);
    lcd.print("| ");
    lcd.write(byte(0));
    lcd.print(" /");
}

void droit() {
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("|   |");
    lcd.setCursor(3, 1);
    lcd.print("| ");
    lcd.write(byte(0));
    lcd.print(" |");
    lcd.setCursor(10, 0);
    lcd.print("|   |");
    lcd.setCursor(10, 1);
    lcd.print("| ");
    lcd.write(byte(0));
    lcd.print(" |");
}

void dort() {
    lcd.clear();
    lcd.setCursor(3, 1);
    lcd.print("_____");
    lcd.setCursor(10, 1);
    lcd.print("_____");
}

void haut() {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("/ ");
    lcd.write(byte(0));
    lcd.print(" |");
    lcd.setCursor(1, 1);
    lcd.print("|   /");
    lcd.setCursor(8, 0);
    lcd.print("/ ");
    lcd.write(byte(0));
    lcd.print(" |");
    lcd.setCursor(8, 1);
    lcd.print("|   /");
}

void content() {
    lcd.clear();
    lcd.setCursor(3, 1);
    lcd.print("|");
    lcd.setCursor(4, 0);
    lcd.print("___");
    lcd.setCursor(7, 1);
    lcd.print("|");
    lcd.setCursor(10, 1);
    lcd.print("|");
    lcd.setCursor(11, 0);
    lcd.print("___");
    lcd.setCursor(14, 1);
    lcd.print("|");
}

void face() {
    if (count == 0) gauche();
    else if (count == 150) haut();
    else if (count == 300) gauche();
    else if (count == 450) droit();
    else if (count == 600) content();
    else if (count == 750) droit();
    else if (count == 900) count = 0;
    count++;
}