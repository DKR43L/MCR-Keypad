#include <Keypad.h>
#include <Arduino.h> 

#define ROW_NUM     4 // four rows
#define COLUMN_NUM  4 // four columns

// Keymap for the 4x4 keypad
char keys[ROW_NUM][COLUMN_NUM] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// Define the row and column pins
uint8_t pin_rows[ROW_NUM]      = {19, 18, 5, 17}; // GPIO19, GPIO18, GPIO5, GPIO17 connect to the row pins
uint8_t pin_column[COLUMN_NUM] = {16, 4, 0, 2};   // GPIO16, GPIO4, GPIO0, GPIO2 connect to the column pins

// Initialize the Keypad library
Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

// Define the password
const String password = "1234";
String input = ""; // String to store user input

// Define possible states
enum DoorState {
  STATE_LOCKED,
  STATE_UNLOCKED,
  STATE_OPEN,
  STATE_CLOSED
};

// Define possible transitions
enum Transition {
  CODE_CORRECT,
  CODE_INCORRECT,
  OPEN_DOOR,
  CLOSE_DOOR,
  AUTO_LOCK
};

// Initialize the state
DoorState currentState = STATE_LOCKED;

// State machine function
void state_machine(Transition transition) {
  switch (currentState) {
    case STATE_LOCKED:
      if (transition == CODE_CORRECT) {
        currentState = STATE_UNLOCKED;
        Serial.println("State changed to: UNLOCKED");
      }
      break;

    case STATE_UNLOCKED:
      if (transition == OPEN_DOOR) {
        currentState = STATE_OPEN;
        Serial.println("State changed to: OPEN");
      } else if (transition == AUTO_LOCK) {
        currentState = STATE_LOCKED;
        Serial.println("State changed to: LOCKED");
      }
      break;

    case STATE_OPEN:
      if (transition == CLOSE_DOOR) {
        currentState = STATE_CLOSED;
        Serial.println("State changed to: CLOSED");
      }
      break;

    case STATE_CLOSED:
      if (transition == AUTO_LOCK) {
        currentState = STATE_LOCKED;
        Serial.println("State changed to: LOCKED");
      }
      break;
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("Enter the password:");
}

void loop() {
  char key = keypad.getKey(); // Get the key pressed

  if (key) { // If a key is pressed
    if (key == '#') { // '#' is used to submit the input
      if (input == password) {
        Serial.println("Access granted!");
        state_machine(CODE_CORRECT); // Transition to UNLOCKED state
      } else {
        Serial.println("Access denied!");
        state_machine(CODE_INCORRECT); // Stay in LOCKED state
      }
      input = ""; // Reset the input after submission
    } else if (key == '*') { // '*' is used to clear the input
      input = "";
      Serial.println("Input cleared.");
    } else { // Append the key to the input
      input += key;
      Serial.println("Current input: " + input);
    }
  }

  // Simulate door actions based on current state
  if (currentState == STATE_UNLOCKED) {
    // Example: Automatically lock the door after some time
    delay(5000); // Simulate delay for auto-lock
    state_machine(AUTO_LOCK);
  }
}
