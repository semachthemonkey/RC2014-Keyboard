// ZX81 USB Keyboard for Leonardo
// (c) Dave Curran
// 2013-04-27

// Modified with Function keys by Tony Smith
// 2014-02-15

// Modified for serial output by Spencer Owen
// 2014-05-02

#define NUM_ROWS 8
#define NUM_COLS 5

// keymap for normal use
byte keyMap[NUM_ROWS][NUM_COLS] = {
  { '5', '4', '3', '2', '1' },
  { 't', 'r', 'e', 'w', 'q' },
  { '6', '7', '8', '9', '0' },
  { 'g', 'f', 'd', 's', 'a' },
  { 'y', 'u', 'i', 'o', 'p' },
  { 'v', 'c', 'x', 'z', 0 },
  { 'h', 'j', 'k', 'l', 13 },
  { 'b', 'n', 'm', 0, ' ' }
};

// keymap if Caps Shift is pressed
// # gives Â£, \\ gives #
byte keyMapShifted[NUM_ROWS][NUM_COLS] = {
  { 'L', '$', '\\', '@', 27 },
  { 'T', 'R', 'E', 'W', 'Q' },
  { 'D', 'U', 'R', '!', 127 },
  { 'G', 'F', 'D', 'S', 'A' },
  { 'Y', 'U', 'I', 'O', 'P' },
  { 'V', 'C', 'X', 'Z', 0 },
  { 'H', 'J', 'K', 'L', 13 },
  { 'B', 'N', 'M', 0, 3 }
};

// keymap if Symbol Shift is pressed
byte keyMapAlt[NUM_ROWS][NUM_COLS] = {
  {'%' , '$' , '#' , '@' , '!'},
  {'>' , '<' , '>' , '=' , '<'},
  {'&', '\'', '(', ')', '_'},
  {'}' , '{' , '\\' , '|' , '_'},
  {'[', ']', '#', ';', '\"'},
  {'/', '?', '£', ':', 0},
  {'^', '-', '+', '=', 13},
  {'*', ',', '.', 0, ' '}
};


// Global variables

int debounceCount[NUM_ROWS][NUM_COLS];
//int altKeyFlag;
//int ctrlKeyFlag;

// define the row and column pins
byte colPins[NUM_COLS] = {
  6, 5, 4, 3, 2
};
byte rowPins[NUM_ROWS] = {
  7, 8, 16, 11, 15, 9, 10, 14
};

// where is the shift key
#define SHIFT_COL 4
#define SHIFT_ROW 5

#define SS_COL 3
#define SS_ROW 7

// how many times does a key need to register as pressed?
#define DEBOUNCE_VALUE 200
#define REPEAT_DELAY 400

/*             */
/*    setup    */
/*             */
void setup()
{
  Serial.begin(115200);  // RC2014 runs at 115,200 baud
  // set all pins as inputs and activate pullups
  for (byte c = 0; c < NUM_COLS; c++)
  {
    pinMode(colPins[c], INPUT);
    digitalWrite(colPins[c], HIGH);

    // clear debounce counts
    for (byte r = 0; r < NUM_ROWS; r++)
    {
      debounceCount[r][c] = 0;
    }
  }

  // set all pins as inputs
  for (byte r = 0; r < NUM_ROWS; r++)
  {
    pinMode(rowPins[r], INPUT);
  }
  // initialize control over the keyboard:
  //Keyboard.begin();
}

/*             */
/*    loop     */
/*             */
void loop()
{


  bool shifted = false;
  bool keyPressed = false;
  bool ss = false;

  // Check for Caps Shift

  pinMode(rowPins[SHIFT_ROW], OUTPUT);

  if (digitalRead(colPins[SHIFT_COL]) == LOW)
  {
    shifted = true;
  }
  // Check for Symbol Shift
 
  pinMode(rowPins[SHIFT_ROW], INPUT);

  pinMode(rowPins[SS_ROW], OUTPUT);

  if (digitalRead(colPins[SS_COL]) == LOW)
  {
    ss = true;
  }
  pinMode(rowPins[SS_ROW], INPUT);

  //if (shifted == true && altKeyFlag == 255)
  if (shifted == true && 0 == 255)
  {
    // NOP in order to prevent Function selection from auto-repeating
  }

  //if (ss == true && altKeyFlag == 255)
  if (ss == true && 0 == 255)
  {
    // NOP in order to prevent Function selection from auto-repeating
  }


  else
  {
    pinMode(rowPins[SHIFT_ROW], INPUT);

    // For each row

    for (byte r = 0 ; r < NUM_ROWS ; r++)
    {
      // Turn the row on

      pinMode(rowPins[r], OUTPUT);
      digitalWrite(rowPins[r], LOW);

      for (byte c = 0 ; c < NUM_COLS ; c++)
      {
        if (digitalRead(colPins[c]) == LOW)
        {
          // Increase the debounce count

          debounceCount[r][c]++;

          // Has the switch been pressed continually for long enough?

          int count = debounceCount[r][c];
          if (count == DEBOUNCE_VALUE)
          {
            // First press

            keyPressed = true;
            pressKey(r, c, shifted, ss);
          }
          else if (count > DEBOUNCE_VALUE)
          {
            // Check for repeats

            count -= DEBOUNCE_VALUE;
            if (count % REPEAT_DELAY == 0)
            {
              // Send repeat press

              keyPressed = true;
              pressKey(r, c, shifted, ss);
            }
          }
        }
        else
        {
          // No key pressed, so reset debounce count

          debounceCount[r][c] = 0;
        }
      }

      // Turn the row back off

      pinMode(rowPins[r], INPUT);
    }

    digitalWrite(rowPins[SHIFT_ROW], LOW);
  }
}

void pressKey(byte r, byte c, bool shifted, bool ss)
{
  byte key = 0;

  if (shifted == true) {
    key = keyMapShifted[r][c];
  }

  if (ss == true) {
    key = keyMapAlt[r][c];
  }

  if (ss == false && shifted == false) {
    key = keyMap[r][c];
  }
  if (key > 0)
  {
    // send the key
    //Keyboard.write(key);
    Serial.write(key);
  }
}
