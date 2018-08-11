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

byte readButtons() {
  byte raw = 0;

  digitalWrite(STB, LOW);
  shiftOut(DIO, CLK, LSBFIRST, 0x42);

  pinMode(DIO, INPUT);
  for (int i = 0; i < 4; i++)
    raw |= shiftIn(DIO, CLK, LSBFIRST) << i;
  pinMode(DIO, OUTPUT);
  digitalWrite(STB, HIGH);

  byte values;

  for (int i = 0; i < 8; i++) {
    values |= (raw & (0x1 << i) ? 1 : 0) << i;
  }

  return values;
}

void writeLed(byte pos, bool val) {
  digitalWrite(STB, LOW);
  shiftOut(DIO, CLK, LSBFIRST, 0xC1 + (pos << 1)); // 1st digit
  shiftOut(DIO, CLK, LSBFIRST, val);
  digitalWrite(STB, HIGH);
}

void writeDisplay(byte pos, byte character, bool dp = false) {
  digitalWrite(STB, LOW);
  shiftOut(DIO, CLK, LSBFIRST, 0xC0 + (pos << 1)); // 1st digit
  shiftOut(DIO, CLK, LSBFIRST, character + (0b10000000 * dp));
  digitalWrite(STB, HIGH);
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

  Serial.begin(9600);
}

void loop()
{
  sendCommand(0x44);  // set single address

  writeDisplay(0, numbers[3]);
  writeDisplay(1, numbers[9]);
  writeDisplay(2, numbers[9]);
  writeDisplay(3, numbers[0]);

  writeDisplay(4, numbers[2]);
  writeDisplay(5, numbers[0]);
  writeDisplay(6, numbers[1]);
  writeDisplay(7, numbers[8]);

  byte buttonValues = readButtons();
  for (int i = 0; i < 8; i++) {
    bool bit = bitRead(buttonValues, i);
    Serial.print(bit);
    writeLed(i, bit);
  }
  Serial.println();
}


