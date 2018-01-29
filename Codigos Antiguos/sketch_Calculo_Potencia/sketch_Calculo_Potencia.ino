
#define Switch_DDS  7
#define Switch_ECG  8 

int Select;                                      // Entrada del tipo de corte, Puro, Blend 1 o Blend 2   
int Bio;                                         // Entrada otorgada por la salida de la Etapa del bioimpedanciometro
int Power;                                       // Entrada otorgada por la seleccion en la pantalla         
int Fac_Use;                                     // Valor obtenido por formula a partir del ciclo util que se usara       
long Val;                                        // Valor de tension requerido en RMS       
int Vrms;                                        // Valor de la amplitud requerido, es decir nuestro valor DC de la fuente conmutada   
int Vout;                                        // Ciclo util seleccionado por el tipo de corte que se requiera               
int Duty_Cycle;
                                       
void setup() {

}

void loop() {
  
  Power_CutStage(Select);                       // Calculo previo de acuerdo al valor de ciclo util 
  CalcPower(Power, Bio);                        // Calculo de tension a colocar en eel control DC   
  Rst_State();                                  // Tipo de corte por defecto, es decir reset de variables  

}


void Power_CutStage (int Selection)
{
  if(Selection == 1)
  {
    Duty_Cycle = 100;
    Fac_Use = sqrt(2);
    digitalWrite(Switch_DDS, HIGH);         // Estas dos señales son opcionales para activar el paso de la modulacion por DDS o por el ECG
    digitalWrite(Switch_ECG, LOW);          // Estas dos señales son opcionales para activar el paso de la modulacion por DDS o por el ECG
  }
  else if(Selection == 2)
  {
    Duty_Cycle = 90;
    Fac_Use = sqrt(20/9);
    digitalWrite(Switch_DDS, HIGH);         // Estas dos señales son opcionales para activar el paso de la modulacion por DDS o por el ECG
    digitalWrite(Switch_ECG, LOW);          // Estas dos señales son opcionales para activar el paso de la modulacion por DDS o por el ECG
  }
  else if(Selection == 3)
  {
    Duty_Cycle = 85;
    Fac_Use = sqrt(40/17);
    digitalWrite(Switch_DDS, HIGH);         // Estas dos señales son opcionales para activar el paso de la modulacion por DDS o por el ECG
    digitalWrite(Switch_ECG, LOW);          // Estas dos señales son opcionales para activar el paso de la modulacion por DDS o por el ECG
  }
  else if(Selection == 4)
  {
    digitalWrite(Switch_DDS, LOW);          // Estas dos señales son opcionales para activar el paso de la modulacion por DDS o por el ECG
    digitalWrite(Switch_ECG, HIGH);         // Estas dos señales son opcionales para activar el paso de la modulacion por DDS o por el ECG
  }
}

void CalcPower (long Pin, long Z)
{
  Val= Z*Pin;                               // Si no se quiere manejar el valor del Bioimpedanciometro Z se coloca como un valor estandar en lugar de como una entrada 
  Vrms = sqrt(Val);
  Vout = Fac_Use*Vrms;                       // Los valores de Factor de uso Fac_Use se calcularon al obtener la formula Vrms = Vamp/(sqrt(2*Tciclo util))      
}

void Rst_State ()
{
  Duty_Cycle = 100;                          // Por defecto el estado inicial es con ciclo util al 100%
  Fac_Use = sqrt(2);  
}





