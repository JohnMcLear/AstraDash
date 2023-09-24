#define SCK 18 //Clk
#define MOSI 23 //Data
#define SS 5 // Pulse seperating low and high speed packets

#include <ArduinoMqttClient.h>
int displayDelay = 34;
void setup() {
  Serial.begin(115200);
  pinMode(SS, OUTPUT);
  pinMode(SCK, OUTPUT);
  pinMode(MOSI, OUTPUT);

  digitalWrite(MOSI, HIGH);
  digitalWrite(SCK, HIGH);
  digitalWrite(SS, HIGH);
  delay(100);

  digitalWrite(MOSI, LOW);
  digitalWrite(SCK, LOW);
  digitalWrite(SS, LOW);
}

void loop () {

  // 12 display segments
  byte nulldata = 0b00000000;

  int i = 0; // 1 is 0b00011000
  // 1 on 3rd screen
  processPayload(0b00011000, 0b00001100, nulldata, nulldata, 0b00000000, 0b00010000, 0b10011000, 0b01000001, 0b00100000, 0b00010000, 0b00000000, 0b00000000);
  delay(1000); // 2
  processPayload(0b10110000, 0b00011011, nulldata, nulldata, nulldata, 0b00010000, 0b10110111, 0b10000010, 0b01000000, 0b01100000, 0b00000000, 0b00000000);
  delay(1000); // 3
  processPayload(0b10111100, 0b00011110, nulldata, nulldata, nulldata, 0b00010000, 0b10110101, 0b10000110, 0b11000001, 0b11100000, 0b00000001, 0b00000000); // 00100001
  delay(1000); // 4
  processPayload(0b11011000, 0b00101100, nulldata, nulldata, nulldata, 0b00010000, 0b10110001, 0b10001110, 0b11000011, 0b11100001, 0b00000011, 0b0000000);
  delay(1000); // 5
  processPayload(0b11101000, 0b00110110, nulldata, nulldata, nulldata, 0b00010000, 0b10110101, 0b10011110, 0b11000111, 0b11100011, 0b00000111, 0b00010000);
  delay(1000); // 6
  processPayload(0b11101000, 0b00110111, nulldata, nulldata, nulldata, 0b00010000, 0b10110111, 0b10111110, 0b11001111, 0b11100111, 0b00011111, 0b10100000);
  delay(1000); // 7 
  processPayload(0b00111000, 0b00011100, nulldata, nulldata, nulldata, 0b00010100, 0b10110000, 0b10111110, 0b11011111, 0b11101111, 0b111111111, 0b11100001);
  delay(1000); // 8 
  processPayload(0b11111000, 0b00111111, nulldata, nulldata, nulldata, 0b00010100, 0b10110111, 0b10111110, 0b11011111, 0b11101111, 0b111111111, 0b11100001);
  delay(1000); // 9
  processPayload(0b11111000, 0b00111110, nulldata, nulldata, nulldata, 0b00010100, 0b10110101, 0b10111110, 0b11011111, 0b11101111, 0b111111111, 0b11100001);
  delay(1000);

  i++;
}

void processPayload(byte displayOne, byte displayTwo, byte displayThree, byte displayFour, byte displayFive, byte displaySix, byte displaySeven, byte displayEight, byte displayNine, byte displayTen, byte displayEleven, byte displayTwelve) {
  Serial.print("Processing payload..  ");
  bitBangData(displayOne); // digit 3 and MPH/KMH
/*
              || ignore
                | - middle middle
                 | - top left
                  | - top middle
                   | - top right
                    | - bottom right
                     | - TBD - not digit 3
                      | - TBD - not digit 3
                       | - TBD - not digit 3 

*/
  bitBangData(displayTwo); // digit 2 and 1st 4 segs of revs and bits digit 2 of MPH/KPH
/*
              || ignore
                | - 6
                 | - TBD
                  | - Top left digit 2
                   | - Top middle digit 2
                    | - Top right digit 2
                     | - bottom right digit 2
                      | - bottom middle speedo digit 3
                       | - bottom left speedo digit 3

*/
  bitBangData(displayThree); // next 8 segs of revs
/*
              || ignore
                | - 22
                 | - TBD
                  | - TBD
                   | - 32k rpm
                    | - TBD
                     | - TBD
                      | - 25k rpm
                       | - 24k rpm

*/
  bitBangData(displayFour); // next 8 segs of revs
/*
              || ignore
                | - 38
                 | - TBD
                  | - TBD
                   | - 49k rpm
                    | - TBD
                     | - 42k rpm
                      | - TBD
                       | - 42k rpm

*/
  bitBangData(displayFive); // last 8 segs of revs
/*
              || ignore
                | - 50
                 | - 62
                  | - TBD
                   | - TBD
                    | - TBD
                     | - TBD
                      | - TBD
                       | - 80k rpm

*/
  bitBangData(displaySix); // revs colour and red bits (0XFF for Red bits on)
/*
              || ignore
                | - TBD
                 | - TBD
                  | - TBD
                   | - Speedo backlight
                    | - TBD
                     | - First Red line?
                      | - TBD
                       | - TBD - 

*/
  bitBangData(displaySeven); // digit 1 and 2(max value of 2 cause speedo) of MPH/KPH & All 4 of left indicators -
/*
              || ignore
                | - Left 4 icons
                 | - TBD
                  | - digit 1 > top middle, bottom left, middle middle, bottom middle
                   | - digit 1 > top right
                    | - digit 1 > bottom right
                     | - digit 2 > bottom middle
                      | - digit 2 > bottom left
                       | - digit 2 > middle section

*/
  bitBangData(displayEight); // Oil segments, battery alarm (0x15 == battery alarm off, 0x13 == battery alarm on) -- first bit of battery
/*
              || ignore
                | - 1st bit of battery
                 | - battery alarm
                  | - final section of oil
                   | - 4th section of oil
                    | - 3rd section of oil
                     | - 2nd section of oil
                      | - 1st section of oil
                       | - oil warning?

*/
  bitBangData(displayNine); // battery segments (6), temp segments(2), temp alarm (1) - 
/*
              || ignore
                | - 2nd bit of temp
                 | - 1st bit of temp
                  | - temp light
                   | - 6th bit of battery
                    | - 5th bit of battery
                     | - 4th bit of battery
                      | - 3rd bit of battery
                       | - 2nd bit of battery

*/
  bitBangData(displayTen); // temp segments, fuel alarm -- I'm not sure about this, I feel like it's fuel...
/*
              || ignore
                | - 2nd bit of fuel
                 | - 1st bit of fuel
                  | - TBD
                   | - fuel alarm
                    | - Final red bit of temp
                     | - 5th bit of temp
                      | - 4th bit of temp
                       | - 3rd bit of temp

*/
  bitBangData(displayEleven); // fuel segments
/*
              || ignore
                | - 9th
                 | - 8th
                  | - 7th
                   | - 6th bit of fuel
                    | - 5th bit of fuiel
                     | - 4th bit of Fuel?
                      | - 3rd bit of Fuel?  Hard to tell
                       | - 2nd bit of Fuel?  Hard to tell

*/
  bitBangData(displayTwelve); // fuel segment end
/*
              || ignore
                | - penultimate bit of fuel?
                 | - final bit of fuel
                  | - 7th bit of fuel
                   | - TBD
                    | - TBD
                     | - TBD?
                      | - TBD
                       | - final bit of fuel, maybe 8?

*/

  digitalWrite(SS, HIGH);
  delayMicroseconds(2);
  digitalWrite(SS, LOW);
  Serial.println();
}


byte bitBangData(byte _send) {
  for(int i=0; i<8; i++) {
    digitalWrite(MOSI, bitRead(_send, i));
    delayMicroseconds(1);
    digitalWrite(SCK, LOW);
    delayMicroseconds(1);
    digitalWrite(SCK, HIGH);
    // Pads this so each bit is approx 34ms
    delayMicroseconds(displayDelay); 
  }
  digitalWrite(MOSI, LOW);
  return false;
}

