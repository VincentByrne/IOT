// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins, using Analog to ran out of digital pins.
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Hello, World!");
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(millis() / 1000);
}
