/******************************************************************
* Area Ingenieria de Sistemas y Automatica
* Sistemas Electronicos Programables
*******************************************************************
* nombre fichero : pwm.h 
*******************************************************************
* descripcion    : Fichero de implementacion modulo PWM
* programador    : JL Villarroel (JLV) 
* lenguaje       : ANSI C para CodeWarrior
* fecha          : 12 septiembre 2016
********************************************************************/

#include "derivative.h"
#include "pwm.h"

void Init_PWM(void){
  
  TPM1SC_PS = 0 ;                   // Preescaler divide by 1
  TPM1SC_CPWMS = 0 ;                // Edge alligned PWM
  
  TPM1MOD = 0x03FF ;                // 10 bits PWM

  TPM1C4SC_MS4A = 0 ;               // Edge-aligned PWM
  TPM1C4SC_MS4B = 1 ; 
  TPM1C4SC_ELS4A = 0 ;              // High-tru pulses
  TPM1C4SC_ELS4B = 1 ;

  TPM1C5SC_MS5A = 0 ;               // Edge-aligned PWM
  TPM1C5SC_MS5B = 1 ; 
  TPM1C5SC_ELS5A = 1 ;              // High-tru pulses
  TPM1C5SC_ELS5B = 0 ;

  TPM1SC_CLKSA = 1 ;                // Bus clock 5MHz
  TPM1SC_CLKSB = 0 ;

}



void Set_Value_10b (unsigned int value)
{
   if (value > 0x03FF) value = 0x03FF ;
   TPM1C4V = value ;
   TPM1C5V = value ;
}
 
 