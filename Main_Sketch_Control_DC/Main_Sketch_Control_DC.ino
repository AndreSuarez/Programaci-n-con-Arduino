#include <Wire.h>

int Vt = 40;
int Bio_V = 800;
byte Bio_ValF;
byte Bio_ValS;
int Pot_Value= 90;
int Ciclo_Value = 2;
int Pass_Value = 1;

void setup() 
{
  Serial.begin(9600);
  Wire.begin(); // join i2c bus (address optional for master)
}

void loop() 
{
  for(int h=0; h<=10; h++)
  {    
  //  Bio_Comm(Bio_V); 
    Wire.beginTransmission(9); // transmit to device #9
    Wire.write(Vt);             // sends one byte
  //  Wire.write(Bio_ValF);              // sends one byte
  //  Wire.write(Bio_ValS);              // sends one byte
  //  Wire.write(Ciclo_Value);              // sends one byte
  //  Wire.write(Pass_Value);
    Wire.endTransmission();    // stop transmitting
    delay(10);
  }
}
void Bio_Comm(int Bio_Value)
{ 
  
  Bio_ValF = (Bio_Value >> 8)&(11111111);
  Bio_ValS = Bio_Value; 
  
}

