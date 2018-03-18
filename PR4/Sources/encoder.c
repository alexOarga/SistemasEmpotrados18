/******************************************************************
* Area Ingenieria de Sistemas y Automatica
* Sistemas Electronicos Programables
*******************************************************************
* nombre fichero : encoder.c 
*******************************************************************
* descripcion    : Fichero de implementacion modulo encoder
* programador    : JL Villarroel (JLV) 
* lenguaje       : ANSI C para CodeWarrior
* fecha          : 12 septiembre 2016
********************************************************************/

#include "derivative.h"
#include "encoder.h"

static unsigned int Counter = 0 ;


void Init_Encoder (void) {

  TPM1C2SC_MS2A = 0 ;               // Input capture
  TPM1C2SC_MS2B = 0 ; 
  TPM1C2SC_ELS2A = 1 ;              // Rising and falling edge
  TPM1C2SC_ELS2B = 1 ;   
  TPM1C2SC_CH2IE = 1 ;              // Interrupt enable
  
 
  PTDDD_PTDDD2 = 0 ;                // PTD3 as input
  PTDDD_PTDDD3 = 0 ;                // PTD3 as input
  
  asm cli ;                         // CPU interrupt enable 
   
  Counter = 0 ;                     // Inicializacion contador
   
  PTDDD_PTDDD0 = 1 ;                // PTD0 as output

}

unsigned int Get_Counter (void) {

   unsigned int Counter_Copy ;
   
   asm sei ;
   Counter_Copy = Counter ;
   asm cli ;
   return Counter_Copy ;

}

void interrupt VectorNumber_Vtpm1ch2 Pulse (void) {

PTDD_PTDD0 = 1 ;
   if (TPM1C2SC_CH2F) TPM1C2SC_CH2F = 0 ;
   if (PTDD_PTDD2) { 
      if (PTDD_PTDD3) Counter -- ;
      else Counter ++ ;
   } else {
      if (PTDD_PTDD3) Counter ++ ;
      else Counter -- ;
   }
   PTDD_PTDD0 = 0 ;

   return ;
}
   
