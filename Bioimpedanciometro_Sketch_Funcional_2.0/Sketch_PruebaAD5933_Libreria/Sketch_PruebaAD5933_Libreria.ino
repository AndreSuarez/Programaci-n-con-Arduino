
#include <AD5933.h>
#include <Wire.h>

int corte=0;
int coag=0;
AD5933 Bio(0);


void setup() 
{
  Serial.begin(9600);
  Wire.begin();
}

//////////////////   Ciclo de Sensado   ////////////////////////

void loop() 
{
  while((corte == 0)||(coag == 0))
  {
    int Impedance = Bio.impedance();
    Serial.println("La Bioimpedancia es: ");
    Serial.println(Impedance);
  } 
}


