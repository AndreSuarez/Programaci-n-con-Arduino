
#include <AD5933.h>
#include <Wire.h>

AD5933 Bio(0);


void setup() 
{
  Serial.begin(9600);
  Wire.begin();
}

//////////////////   Ciclo de Sensado   ////////////////////////

void loop() 
{
  for(int i=0; i<=10; i++)
  {
    int Impedance = Bio.impedance();
    Serial.println("La Bioimpedancia es: ");
    Serial.println(Impedance);
  } 
  delay(10000);
}


