#include <Keypad.h>
#include <LiquidCrystal.h> // for LCD

// - Buzzer Configuration -
const int buzzerPin = 13;    // Buzzer connected to digital pin 13
const int buzzerDelay = 10;  // Delay between frequencies

// - Keypad Configuration -
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns

// - LCD Configuration - 
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

// - Reed Switch Configuration -
const int reedSwitchPin = 11;
bool reedSwitchTriggered = true;

//define the symbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad

Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

// - PIN Configuration -
const String correctPIN = "1234"; 
String enteredPIN = "";            
const int maxAttempts = 3;         
int attemptCount = 0;              
bool alarmActive = false;

// Tone frequencies for different key types
const int DIGIT_TONE = 1000;   // Tone for numeric keys
const int SPECIAL_TONE = 1500; // Tone for A, *, #, etc.
const int CONFIRM_TONE = 2000; // Tone for successful actions
const int ERROR_TONE = 500;    // Tone for errors
const int DOOR_TONE = 2500;     // Tone for door open

void playKeyTone(int frequency) {
  tone(buzzerPin, frequency, 50); // Play tone for 50 milliseconds
  delay(50); // Small delay to prevent tone overlap
  noTone(buzzerPin);
}

void setup() {
  pinMode(buzzerPin, OUTPUT); // Buzzer pin as an output
  Serial.begin(9600); //Serial for the keypad.
  Serial.println("The Pin is 4 digits. Press '*' to clear your entry. Press 'A' to confirm your entry. You have 3 attempts or the alarm will sound!"); // Instructions
  Serial.println("Securitron Resting"); // Asking for Pin
  lcd.begin(16, 2); // LCD columns and rows
  lcd.clear(); // Clear data from previous session 
  lcd.print("**SECURITRON**"); // Display Pin
  pinMode(reedSwitchPin, INPUT_PULLUP);   // Configure the reed switch pin as input 
}

void playAlarm() {
  // Siren Sweep Logic
  for (int freq = 500; freq <= 2000; freq += 5) {
    tone(buzzerPin, freq);
    delay(buzzerDelay);
  }
  for (int freq = 2000; freq >= 500; freq -= 5) {
    tone(buzzerPin, freq);
    delay(buzzerDelay);
  }
}

void loop() {
  int switchState = digitalRead(reedSwitchPin);// Check Reed Switch State
  if (switchState ==  reedSwitchTriggered) {
    Serial.println("Door Opened!");
    lcd.clear();
    lcd.print("Door Opened!");
    playKeyTone(DOOR_TONE);
    reedSwitchTriggered = true;
    delay(1000); // Prevent multiple rapid triggers
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter PIN:");
  }

  char customKey = customKeypad.getKey();
    if (customKey){ // Displays Input
    Serial.println(customKey);
    
    // Play different tones based on key type
    if (isDigit(customKey)) {
      playKeyTone(DIGIT_TONE); // Numeric keys
    } else {
      playKeyTone(SPECIAL_TONE); // Special keys
    }
    
    if (customKey == 'A') { // A will act as enter key
      Serial.println("PIN Submitted: " + enteredPIN);
      playKeyTone(CONFIRM_TONE); // Confirmation tone
      
      // Verify the entered PIN
      if (enteredPIN == correctPIN) {
        Serial.println("Access Granted.");

         // Clear LCD and display success message
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Alarm Cleared.");
        lcd.setCursor(0, 1);
        lcd.print("Welcome!");

        delay(5000); // reset after 5 seconds 
        lcd.clear();
        lcd.print("**SECURITRON**"); 
        
        enteredPIN = ""; // Reset entered PIN
        attemptCount = 0; // Reset attempt count
      }
      else {
        Serial.println("Incorrect PIN.");
        lcd.print("Incorrect PIN.");
        playKeyTone(ERROR_TONE); // Error tone

        // Clear LCD and display error message
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Incorrect PIN.");
        lcd.setCursor(0, 1);
        lcd.print("Attempts: " + String(attemptCount + 1));

        // Incrementing the attempt count.
        attemptCount++;
        enteredPIN = ""; // Reset entered PIN

        delay(2000); // Display message for 2 seconds

        // Clear LCD and prompt for PIN again
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Enter PIN:");
        
        // Check attempt amount
        if (attemptCount >= maxAttempts) {
          Serial.println("Maximum attempts exceeded. Alarm Activated!");
          
          // Clear LCD and display error message
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Alarm Activated!");
          lcd.setCursor(0, 1);
          lcd.print("Unauthorized!");
          
          alarmActive = true;
        }
      }
    }
    else if (customKey == '*') { // '*' Will clear entry
      Serial.println("PIN Cleared.");
      playKeyTone(SPECIAL_TONE); // Special key tone

      // Clear entered PIN and LCD display
      enteredPIN = "";
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("PIN Cleared.");
      lcd.setCursor(0, 1);
      lcd.print("Enter PIN:");
      
      delay(1000); // Display message for 1 second
      
      // Clear LCD and prompt for PIN again
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Enter PIN:");
    }
    
    // append digits to entered pin.
    else { // If a digit is pressed
      enteredPIN += customKey;
      Serial.print("Entered PIN: ");
      
      // Display asterisks instead of actual digits
      lcd.setCursor(0, 1); // Move to second row
      for (int i = 0; i < enteredPIN.length(); i++) {
        lcd.print("*");
      } // Clear previous asterisks
      lcd.setCursor(0, 1); // Reset cursor to start of second row
      
      // Print asterisks equal to the number of entered digits
      for (int i = 0; i < enteredPIN.length(); i++) {
        lcd.print("*"); 
      }
      
      // Debugging in Serial Monitor
      for (int i = 0; i < enteredPIN.length(); i++) {
        Serial.print("*");
      }
      Serial.println();
    }
  }
  
  // If the alarm is active, play the alarm sound
  if (alarmActive) {
    playAlarm();
  }
}