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
const unsigned int SIZE = 5;	// tamanyo de la cola

/* Variables globales */
volatile unsigned char c = 0;
/********************************************/
volatile unsigned int pulsador_1 = 0;
volatile unsigned int pulsador_2 = 0;
volatile unsigned int pulsador_3 = 0;
/* cola circular */
volatile unsigned int cola[SIZE] = {0,0,0,0,0};
volatile unsigned int puntero = 0; // iterador sobre la cola  
/********************************************/

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
  
	asm SEI;
    c=digito1; 
    asm CLI;
    display (0,c) ;
    asm SEI; 
    c=digito2; 
    asm CLI;
    display (1,c) ;
/********************************************/
  while(1){
	if( pulsador_1 == 1 ){
		display(1, c);				// muestra validacion en 1
		cola[puntero] = c;			// almacena en la cola
		puntero = (puntero + 1)%SIZE-1;	// aumenta puntero de cola
		c = 0;						// valor a 0
		display(0, c);				// display 0 muestra 0
		pulsador_1 = 0;				//
	}
	if( pulsador_2 == 1 ){
		if( c > 0 ){
			c = c - 1;				// decrementa c
			display(0, c);				// display 0 muestra c
		}
		pulsador_2 = 0;				//
	}
	if( pulsador_3 == 1 ){
		if( c < 9 ){
			c = c + 1;				// aumenta c
			display(0, c);				// display 0 muestra c
		}
		pulsador_3 = 0;				//
	}
  }
/********************************************/
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
/********************************************/
/*  PAG 115
1. Mask keyboard interrupts by clearing KBIE in KBISC.
2. Enable the KBI polarity by setting the appropriate KBEDGn bits in KBIES.
3. If using internal pullup/pulldown device, configure the associated pullup enable bits in PTxPE.
4. Enable the KBI pins by setting the appropriate KBIPEn bits in KBIPE.
5. Write to KBACK in KBISC to clear any false interrupts.
6. Set KBIE in KBISC to enable interrupts.
*/
  KBIPE = 0x7				// Enable pin 2 1 0
  KBIES = 0x0				// Falling edge
  KBISC |= 0x2				// Enable interruptions
/********************************************/ 

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
		PTCDD |= ( 0x00 + valor);
	}
}  


void interrupt 4 isr_pio_handler(void){
/********************************************/
	KBISC |= 0x4;		//clear KBACK
	if( PTAD & 0x1 == 0x1 ){	// si se ha pulsado
		pulsador_1 = 1;
	}else if( PTAD & 0x2 == 0x2 ){
		pulsador_2 = 1;
	}else if( PTAD & 0x4 == 0x4 ){
		pulsador_3 = 1;
	}
/********************************************/
}
