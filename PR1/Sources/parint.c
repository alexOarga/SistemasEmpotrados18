/******************************************************************
* Area Ingenieria de Sistemas y Automatica
* Area Arquitectura de Computadores
* Sistemas Empotrados I
*******************************************************************
* nombre fichero : parint.c
*******************************************************************
* proyecto       : PR1
* descripcion    : programacion E/S paralelo mediante interrupcion
* programador    : 
* lenguaje       : ANSI C para CodeWarrior 9S08
********************************************************************/

#include "derivative.h" /* include peripheral declarations */

/* Constantes */


/* Variables globales */
volatile unsigned char c = 2;

/* Funciones */

void isr_pio_handler(void);
	/* Manejador de la interrupcion generada por 
	   PTA1 funcionando como KBI 
		 lectura del dato, actualizacion del digito1 y digito2 */
		
void inicializacion (void);

void display (unsigned char queDisplay, unsigned char valor) ;
	/* Muestra valor por el display queDisplay (0, 1, 2) */
		

/* Programa principal */

void main(void){

  inicializacion ();
  
  while(1){
    asm SEI;
    c=digito1; 
    asm CLI;
    display (0,c) ;
    asm SEI; 
    c=digito2; 
    asm CLI;
    display (1,c) ;
  }
}


/* Implementacion funciones */


void inicializacion (void){

  /* Internal Clock Source Configuration */
	if(NVICSTRM == 0xFF)			// If programmed trim value is blank
		ICSTRM = 0x80;				  // load default value
	else
		ICSTRM = NVICSTRM;	  	// else load programmed trim value
  ICSC2_BDIV = 1 ;          // Divides selected clock by 2		
	                          // Bus clock = 5 MHz, CPU clock = 10MHz
  
  SOPT1_COPT = 0 ;          // WATCHDOG disable 

  /*Inicializar puerto A (entrada de datos) */ 		
  PTAPE |= (PTAPTE_PTAPE1_MASK | PTAPE_PTAPE2_MASK | PTAPE_PTAPE3_MASK);
  IRQSC_IRQPE = 0x1;
  IRQSC_IRQEDG = 0x0;

  		
  /* Inicializar PTC (visualizacion)  */
  PTCDD |= 0x3f;
 

  asm CLI;                  // Unmask interrupts

  return;
}



void display (unsigned char queDisplay, unsigned char valor) {
	if( queDisplay == 0 ){
		PTCDD &= ~(PTCPE_PTCPE5_MASK);
		PTCDD |= (PTCPE_PTCPE4_MASK);
	}  else if( queDisplay == 1 ){
		PTCDD &= ~(PTCPE_PTCPE4_MASK);
		PTCDD |= (PTCPE_PTCPE5_MASK);
	}
	if( valor >= 0 && valor <= 9 ){
		PTCDD &= ~(0x0f);
		PTCDD |= 0x02;
	}
}  


void interrupt 4 isr_pio_handler(void){
	
}
