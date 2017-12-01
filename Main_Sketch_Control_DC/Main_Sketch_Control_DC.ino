#include <Wire.h>

boolean State_DC = false;
int Bio_Val;
int Pot_Val;

void setup() 
{
  Wire.begin(); // join i2c bus (address optional for master)
}

void loop() 
{
  Wire.beginTransmission(9); // transmit to device #9
  Wire.write(Pot_Val);              // sends one byte
  Wire.endTransmission();    // stop transmitting


  Wire.beginTransmission(9); // transmit to device #9
  Wire.write(Bio_Val);              // sends one byte
  Wire.endTransmission();    // stop transmitting

 
  Wire.beginTransmission(9); // transmit to device #9
  Wire.write(Ciclo_Val);              // sends one byte
  Wire.endTransmission();    // stop transmitting

}
