const int STB = 7;
const int CLK = 9;
const int DIO = 8;

/*
    7 segment
    the numbers indicate the bit

      +-0-+
      5   1
      +-6-+
      4   2
      +-3-+  7
*/

const byte dp = 0b10000000;

const byte numbers[10] = {
  0b00111111,
  0b00000110,
  0b01011011,
  0b01001111,
  0b01100110,
  0b01101101,
  0b01111101,
  0b00000111,
  0b01111111,
  0b01101111,
};

const byte sevenSegmentsId[8] = {
  0xCE, // right
  0xCC,
  0xCA,
  0xC8,

  0xC6,
  0xC4,
  0xC2,
  0xC0  // left
};

const byte ledId[8] = {
  0xC1, // left
  0xC3,
  0xC5,
  0xC7,

  0xC9,
  0xCB,
  0xCD,
  0xCF  // right
};

void writeLed(byte pos, bool val) {
  digitalWrite(STB, LOW);
  shiftOut(DIO, CLK, LSBFIRST, ledId[pos]); // 1st digit
  shiftOut(DIO, CLK, LSBFIRST, val);
  digitalWrite(STB, HIGH);
}

void displayChar(byte pos, byte character) {
  digitalWrite(STB, LOW);
  shiftOut(DIO, CLK, LSBFIRST, sevenSegmentsId[pos]); // 1st digit
  shiftOut(DIO, CLK, LSBFIRST, character);
  digitalWrite(STB, HIGH);
}

void displayDigit(byte pos, int digit){
  displayChar(pos, numbers[digit]);
}

void sendCommand(uint8_t value)
{
  digitalWrite(STB, LOW);
  shiftOut(DIO, CLK, LSBFIRST, value);
  digitalWrite(STB, HIGH);
}

void reset()
{
  sendCommand(0x40); // set auto increment mode
  digitalWrite(STB, LOW);
  shiftOut(DIO, CLK, LSBFIRST, 0xc0);   // set starting address to 0
  for (uint8_t i = 0; i < 16; i++)
  {
    shiftOut(DIO, CLK, LSBFIRST, 0x00);
  }
  digitalWrite(STB, HIGH);
}

void setup()
{
  pinMode(STB, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(DIO, OUTPUT);

  sendCommand(0b10001001); // 1000ABBB where A is activate and B is brightness

  reset();
}

void loop()
{
  sendCommand(0x44);  // set single address


  displayDigit(7, 3);
  displayDigit(6, 9);
  displayDigit(5, 9);
  displayDigit(4, 0);


  displayDigit(3, 3);
  displayDigit(2, 9);
  displayDigit(1, 9);
  displayDigit(0, 0);


  delay(1000);

  reset();

  delay(1000);
}
