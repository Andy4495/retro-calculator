
// Retro calculator using NSA 1166 Bubble Display and ICM7218 LED driver
// Note that the NSA1166 display only has 6 digits and a single fixed
// decimal point.
//
// https://github.com/Andy4495/retro-calculator
//
// 1.0.0    04/12/2019  A.T.   Original


#include <ICM7218.h>
#include <Keypad4495.h>

// Create a keypad object
#define NUM_ROWS 4
#define NUM_COLS 4
byte rowPins[NUM_ROWS] = {15, 14, 8, 7};
byte colPins[NUM_COLS] = {19, 18, 13, 12};
char keymap[NUM_COLS * NUM_ROWS] = { '1', '2', '3', '+', '4', '5', '6', '-', '7', '8', '9', '*', 'C', '0', '=', '/' };
Keypad4495 myKeypad(keymap, rowPins, colPins, NUM_ROWS, NUM_COLS, true);

// Configure the 10 OUTPUT pins used to interface with ICM7218: D0-D7, mode, write
ICM7218 myLED(19, 18, 13, 12, 11, 10, 9, 6, 2, 5);

// NSA1166 is a 6 digit display
#define MAXCHARS 6

// Debounce time in ms. Normally 20 ms is sufficient for most types of buttons
#define DEBOUNCE_TIME 20

enum STATE {ENTRY1, ENTRY2, EQUALS, ERROR};

enum OPERATOR {NONE, PLUS, MINUS, MULT, DIV};

char convertBuffer[MAXCHARS + 1];
char entryString[MAXCHARS + 1];
char c;
int i = 0;
long op1, op2;
STATE    state = ENTRY1;
OPERATOR op    = NONE;
int pos = 5; // Position of current digit in entryString, right justify, leave a space for terminator


void setup() {
  // Use DIRECT mode since we will display some special characters
  myLED.setMode(ICM7218::DIRECT);

  memset(entryString, 0, MAXCHARS + 1); // clear the entry string
  displayCalc();
}

void loop() {
  long answer;

  // Debounce: Waits for button to be released before returning
  c = getKeyWithDebounce();

  switch (c) {
    case Keypad4495::NO_KEY:
      // Do nothing and loop around and check again
      break;

    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      // Add to char array to max of 6 digits
      // Drop MSB as more digits entered
      if (i >= MAXCHARS) {
        for (i = 1; i < MAXCHARS; i++) {
          entryString[i - 1] = entryString[i];
        }
        i = 5;
      }
      entryString[i] = c;
      i++;
      convertToDisplayFormat(entryString);
      myLED.print(convertBuffer);
      break;

    case '+':
      // Set operation as plus
      // Set state as entry2
      // Save first value string to number
      // Clear out entry string
      state = ENTRY2;
      op = PLUS;
      op1 = atol(entryString);
      memset(entryString, 0, MAXCHARS + 1); // clear the entry string
      i = 0;
      sprintf(convertBuffer, "  Plus");
      myLED.convertToSegments(convertBuffer);
      myLED.print(convertBuffer);
      break;

    case '-':
      // Set operation as "subtraction"
      // Save first value string to number
      // Set state as entry2
      // Clear out entry string
      state = ENTRY2;
      op = MINUS;
      op1 = atol(entryString);
      memset(entryString, 0, MAXCHARS + 1); // clear the entry string
      i = 0;
      sprintf(convertBuffer, " Subtr");
      myLED.convertToSegments(convertBuffer);
      myLED.print(convertBuffer);
      break;

    case '*':
      // Set operation as "mult"
      // Save first value string to number
      // Set state as entry2
      // Clear out entry string
      state = ENTRY2;
      op = MULT;
      op1 = atol(entryString);
      memset(entryString, 0, MAXCHARS + 1); // clear the entry string
      i = 0;
      sprintf(convertBuffer, "  Prod");
      myLED.convertToSegments(convertBuffer);
      myLED.print(convertBuffer);
      break;

    case '/':
      // Set operation as "div"
      // Save first value string to number
      // Set state as entry2
      // Clear out entry string
      state = ENTRY2;
      op = DIV;
      op1 = atol(entryString);
      memset(entryString, 0, MAXCHARS + 1); // clear the entry string
      i = 0;
      sprintf(convertBuffer, "   Div");
      myLED.convertToSegments(convertBuffer);
      myLED.print(convertBuffer);
      break;

    case 'C':
      // Clear entry string and display zero
      memset(entryString, 0, 7); // clear the entry string
      i = 0;
      convertBuffer[MAXCHARS - 1] = c;
      memset(convertBuffer, ' ', MAXCHARS - 2);
      convertBuffer[MAXCHARS] = 0;
      myLED.convertToSegments(convertBuffer);
      myLED.print(convertBuffer);
      break;

    case '=':
      // Check for overflow, display error if needed
      //
      state = ENTRY1;
      op2 = atol(entryString);
      i = 0;
      switch (op) {
        case PLUS:
          answer = op1 + op2;
          if ((answer > 999999) || (answer < -99999)) state = ERROR;
          break;

        case MINUS:
          answer = op1 - op2;
          if ((answer > 999999) || (answer < -99999)) state = ERROR;
          break;

        case MULT:
          answer = op1 * op2;
          if ((answer > 999999) || (answer < -99999)) state = ERROR;
          break;

        case DIV:
          if (op2 == 0) state = ERROR;
          else answer = op1 / op2;
          if ((answer > 999999) || (answer < -99999)) state = ERROR;
          break;

        default:
          state = ERROR;
          break;
      }
      if (state != ERROR) {
        snprintf(entryString, MAXCHARS + 1, "%6ld", answer);
        convertToDisplayFormat(entryString);
        myLED.print(convertBuffer);
      } else {
        displayError();
        state = ENTRY1;
      }
      memset(entryString, 0, MAXCHARS + 1); // clear the entry string
      break;

    default:
      displayError();
      memset(entryString, 0, MAXCHARS + 1); // clear the entry string
      state = ENTRY1;
      break;

  }
}

void displayError() {
  snprintf(convertBuffer, MAXCHARS + 1, "Error");
  myLED.convertToSegments(convertBuffer);
  myLED.print(convertBuffer);
}

void displayCalc() {
  snprintf(convertBuffer, MAXCHARS + 1, " CALC ");
  myLED.convertToSegments(convertBuffer);
  myLED.print(convertBuffer);
}

char getKeyWithDebounce() {
  char button;
  unsigned long pressTime;

  button = myKeypad.getKey();
  if (button != Keypad4495::NO_KEY) {
    pressTime = millis();
    while (millis() - pressTime < DEBOUNCE_TIME)
      ; // Spin our wheels while we wait

    if (button == myKeypad.getKey()) {
      // Same button, now wait for it to be released
      while (myKeypad.getKey() == button)
        ; // Spin our wheels and wait for button to be released
    } else {
      // Different button, so return NO_KEY and ignore the keypress
      button = Keypad4495::NO_KEY;
    }
  }

  return button;
}

void convertToDisplayFormat(char* rawString) {
  int j;

  sprintf(convertBuffer, "%6ld", atol(rawString));

  convertBuffer[MAXCHARS] = 0;    // Add NULL terminator
  myLED.convertToSegments(convertBuffer);
}
