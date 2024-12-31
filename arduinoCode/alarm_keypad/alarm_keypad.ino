#include <Keypad.h>


// - Buzzer Configuration -
const int buzzerPin = 13;    // Buzzer connected to digital pin 8
const int buzzerDelay = 10;  // Delay between frequencies

// - Keypad Configuration -
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns

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

void setup() {
  pinMode(buzzerPin, OUTPUT); // Buzzer pin as an output
  Serial.begin(9600); //Serial for the keypad.
  Serial.println("The Pin is 4 digits. Press '*' to clear your entry. Press 'A' to confirm your entry. You have 3 attempts or the alarm will sound!"); // Instructions
  Serial.println("Enter PIN:"); // Asking for Pin
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
  char customKey = customKeypad.getKey();

    if (customKey){ // Displays Input
    Serial.println(customKey);
    
    if (customKey == 'A') { // A will act as enter key
      Serial.println("PIN Submitted: " + enteredPIN);
      
      // Verify the entered PIN
      if (enteredPIN == correctPIN) {
        Serial.println("Access Granted.");
        enteredPIN = ""; // Reset entered PIN
        attemptCount = 0; // Reset attempt count
      }
      else {
        Serial.println("Incorrect PIN.");
        attemptCount++;
        enteredPIN = ""; // Reset entered PIN
        
        // Check attempt amount
        if (attemptCount >= maxAttempts) {
          Serial.println("Maximum attempts exceeded. Alarm Activated!");
          alarmActive = true;
        }
      }
    }
    else if (customKey == '*') { // '*' Will clear entry
      Serial.println("PIN Cleared.");
      enteredPIN = "";
    }
    
    // append digits to entered pin.
    else {
      enteredPIN += customKey;
      Serial.print("Entered PIN: ");
      for (int i = 0; i < enteredPIN.length(); i++) {
        Serial.print("*"); // Print asterisks instead of actual digits
      }
      Serial.println();
    }
  }
  
  // If the alarm is active, play the alarm sound
  if (alarmActive) {
    playAlarm();
  }
}










