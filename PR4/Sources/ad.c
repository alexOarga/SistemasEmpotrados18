/******************************************************************
* Area Ingenieria de Sistemas y Automatica
* Sistemas Empotrados
*******************************************************************
* nombre fichero : ad.c 
*******************************************************************
* descripcion    : Implementacion modulo conversor analogico -
*                  digital
* programador    : JL Villarroel (JLV) 
* lenguaje       : ANSI C para CodeWarrior
* fecha          : 24 marzo 2017
********************************************************************/


#include "derivative.h"
#include "ad.h"

#define A 0.0048876

void Init_AD (void)
{
  ADCSC1_ADCO = 0 ;       // One conversion
  ADCSC1_AIEN = 0 ;       // Interrupt disabled
  
  ADCSC2_ACFE = 0 ;        // Compare function disabled
  ADCSC2_ADTRG = 0 ;       // Software trigger enabled
  
  ADCCFG_ADIV = 1 ;       // Input clock divided by 2
  ADCCFG_MODE = 2 ;       // 10 bit conversion
  ADCCFG_ADICLK = 0 ;     // Bus clock source
  
  APCTL1_ADPC4 = 1 ;      // Pin PTB0/ADP4 controlled by ADC
  APCTL1_ADPC5 = 1 ;      // Pin PTB1/ADP5 controlled by ADC

}

unsigned int Read_Value_Int_1 (void) 
{
    ADCSC1 = 0x04;        // Inicio de la conversion AD
                          // Conversion canal ADP4/PTB0
    while(!ADCSC1_COCO);  // Espera fin conversion
    return ADCR ;         // Lectura dato, borrado COCO 
}

unsigned int Read_Value_Int_2 (void) 
{
    ADCSC1 = 0x05;        // Inicio de la conversion AD
                          // Conversion canal ADP5/PTB1
    while(!ADCSC1_COCO);  // Espera fin conversion
    return ADCR ;         // Lectura dato, borrado COCO 
}

