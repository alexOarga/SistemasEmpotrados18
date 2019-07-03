/******************************************************************
* Area Ingenieria de Sistemas y Automatica
* Sistemas Empotrados
*******************************************************************
* nombre fichero : pwm.h 
*******************************************************************
* descripcion    : Fichero de cabecera modulo PWM
* programador    : JL Villarroel (JLV) 
* lenguaje       : ANSI C para CodeWarrior
* fecha          : 30 marzo 2017
********************************************************************/

#ifndef pwm_h
#define pwm_h

void Init_PWM (void) ;
   /* Inicializa el TPM1 para generar un PWM (canal 4) y su negado (canal 5) */

void Set_Value_10b (unsigned int value) ;
   /* Estable la anchura de pulso. 0..1023 */

#endif
