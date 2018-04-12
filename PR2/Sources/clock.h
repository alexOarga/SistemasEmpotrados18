/******************************************************************
* Area Ingenieria de Sistemas y Automatica
* Area Arquitectura de Computadores
* Sistemas Empotrados I
*******************************************************************
* nombre fichero : clock.h 
*******************************************************************
* descripcion    : Fichero de cabecera modulo reloj de sistema
*                  Tick del reloj 1 ms
* programador    : JL Villarroel (JLV) 
* lenguaje       : ANSI C para CodeWarrior
* fecha          : 20 febrero 2014
********************************************************************/

#ifndef clock_h
#define clock_h

void Reset_Clock (void) ;
   /* Instalacion del reloj y puesta a cero*/
   
void Start_Clock (void) ;
   /* Puesta en marcha del reloj */
   
void Stop_Clock (void) ;
   /* Parada del reloj */
   
unsigned int Get_Time (void) ;
   /* Obtención de la cuenta del reloj*/
   
void delay_until(unsigned int T);
    /* Espera bloqueante hasta que la cuenta del reloj llegue
       a T */
       
void set_timer(unsigned int T);


void Start_Clock_after (void) ;
   /* Puesta en marcha del reloj sin reset counter*/
#endif