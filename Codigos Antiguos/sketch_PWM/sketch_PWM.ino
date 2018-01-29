// En esta pagina se encuentran los comandos para obtener las diferentes frecuencias disponibles en el arduino
// https://playground.arduino.cc/Main/TimerPWMCheatsheet

#define PWM_pin 6 // Pin de salida de la onda PWM

void setup() 
{

// Setting                          
TCCR0B = _BV(CS00);              

}

void loop()
{

analogWrite(PWM_pin,127);

}
