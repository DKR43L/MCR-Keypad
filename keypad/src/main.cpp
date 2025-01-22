#include <Arduino.h>
#include <Keypad.h>

#define ROW_NUM 4    // four rows
#define COLUMN_NUM 4 // four columns

// Keymap for the 4x4 keypad
char keys[ROW_NUM][COLUMN_NUM] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};

// Define the row and column pins
byte pin_rows[ROW_NUM] = {19, 18, 5, 17};    // GPIO19, GPIO18, GPIO5, GPIO17 connect to the row pins
byte pin_column[COLUMN_NUM] = {16, 4, 0, 2}; // GPIO16, GPIO4, GPIO0, GPIO2 connect to the column pins

// Initialize the Keypad library
Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM);

// Define the password
const String password = "1234"; // Replace with your desired password
String input = "";              // String to store user input

void setup()
{
  Serial.begin(9600);
  Serial.println("Enter the password:");
}

void loop()
{
  char key = keypad.getKey(); // Get the key pressed

  if (key)
  { // If a key is pressed
    if (key == '#')
    { // '#' is used to submit the input
      if (input == password)
      {
        Serial.println("Access granted!");
      }
      else
      {
        Serial.println("Access denied!");
      }
      input = ""; // Reset the input after submission
    }
    else if (key == '*')
    { // '*' is used to clear the input
      input = "";
      Serial.println("Input cleared.");
    }
    else
    { // Append the key to the input
      input += key;
      Serial.println("Current input: " + input);
    }
  }
}
