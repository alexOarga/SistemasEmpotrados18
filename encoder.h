/******************************************************************
* Area Ingenieria de Sistemas y Automatica
* Sistemas Empotrados
*******************************************************************
* nombre fichero : encoder.h 
*******************************************************************
* descripcion    : Fichero de cabecera modulo encoder
* programador    : JL Villarroel (JLV) 
* lenguaje       : ANSI C para CodeWarrior
* fecha          : 30 marzo 20147
********************************************************************/

#ifndef encoder_h
#define encoder_h

void Init_Encoder (void) ;
   /* Inicializa el modulo encoder basado en el TPM1 */

unsigned int Get_Counter (void) ;
   /* Devuelve el contado del encoder. 1 pulso = 0.5 grados */

#endif