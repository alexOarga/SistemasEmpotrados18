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
#define SIZE 5	  // tamanyo de la cola
#define digito1 1
#define digito2 2
#define displayer_0 1
#define displayer_1 2

/* Variables globales */
volatile unsigned char c = 0;
/********************************************/
volatile unsigned int pulsador_1 = 0;
volatile unsigned int pulsador_2 = 0;
volatile unsigned int pulsador_3 = 0;
/* cola circular */
volatile unsigned int cola[SIZE] ={0,1,0,0,0};
volatile unsigned int puntero = 0; // iterador sobre la cola
volatile unsigned int puntero_aux = 1; // iterador sobre la cola   
/********************************************/

/* Funciones */
void isr_pio_handler(void);
	/* Manejador de la interrupcion generada por 
	   PTA1 funcionando como KBI 
		 lectura del dato, actualizacion del digito1 y digito2 */
		
void inicializacion (void);

void display (unsigned char queDisplay, unsigned char valor);
	/* Muestra valor por el display queDisplay (0, 1, 2) */
		

/* Programa principal */

void main(void){

  inicializacion ();
    
	asm SEI;
  c=0;
  asm CLI;
  display (displayer_0,c);
  asm SEI; 
  c=1;
  asm CLI;
  display (displayer_1,c);
/********************************************/
  while(1){
	  if( pulsador_1 == 1 ){	  
	 	  
	 	  c = pulsador_2*2 + pulsador_3;
	 	  
	 	  puntero = puntero + 1;
	 	  puntero_aux = puntero + 1;
	 	  
	 	  if(puntero_aux == SIZE) puntero_aux = 0;
	 	  if(puntero == SIZE ) puntero =0;

	  	// almacena en la cola
	  	// aumenta puntero de cola
	    // display 0 muestra 0
	    
	    if(puntero==0) cola[SIZE-1] = c;
	    else cola[puntero-1] = c;
	    
	  	pulsador_1 = 0;
	  	pulsador_2 = 0;
	  	pulsador_3 = 0;	  	     	  	  	 
	  	}
	  	display(displayer_0,cola[puntero]);
	 	  display(displayer_1,cola[puntero_aux]);	  	
	  }
/********************************************/
}


/* Implementacion funciones */


void inicializacion (void){

  /* Internal Clock Source Configuration */
	if(NVICSTRM == 0xFF){
	  // If programmed trim value is blank
		ICSTRM = 0x80;
	} // load default value
	else{
		ICSTRM = NVICSTRM;
	} // else load programmed trim value
  ICSC2_BDIV = 1 ;          // Divides selected clock by 2		
	                          // Bus clock = 5 MHz, CPU clock = 10MHz
  
  SOPT1_COPT = 0 ;          // WATCHDOG disable
  
  
  
   

  /*Inicializar puerto A (entrada de datos) */ 		
  PTAPE |= (PTAPE_PTAPE1_MASK | PTAPE_PTAPE2_MASK | PTAPE_PTAPE3_MASK);
  PTADD &= ~(PTAPE_PTAPE1_MASK | PTAPE_PTAPE2_MASK | PTAPE_PTAPE3_MASK);   
  
  
  KBIPE |= 0x02;				  // Enable pin 2 1 0
  KBISC &= ~(0x01);
  KBIES &= ~(0x0d);
  KBISC |= 0x02;

/********************************************/
/*  PAG 115
1. Mask keyboard interrupts by clearing KBIE in KBISC.
2. Enable the KBI polarity by setting the appropriate KBEDGn bits in KBIES.
3. If using internal pullup/pulldown device, configure the associated pullup enable bits in PTxPE.
4. Enable the KBI pins by setting the appropriate KBIPEn bits in KBIPE.
5. Write to KBACK in KBISC to clear any false interrupts.
6. Set KBIE in KBISC to enable interrupts.
*/
/********************************************/ 

  /* Inicializar PTC (visualizacion)  */
   PTCD = 0 ;
   PTCDD |= 0x6f;
   PTCDS |= 0x6f;

 

  asm CLI;                  // Unmask interrupts
}



void display (unsigned char queDisplay, unsigned char valor) {
  if(queDisplay ==55){
    queDisplay = 1;
  }
	if( valor >= 0 && valor <= 9 ){
	    
	    PTCD |= 0xf0;
	    PTCD &= 0xf0;
	    PTCD |= valor;
	    
	    
      PTCD &= ~((1 << queDisplay) << 4);
		  
	}
}  


void interrupt 4 isr_pio_handler(void){
/********************************************/


		pulsador_1 = 1;
	
	if( PTAD_PTAD2 == 1 ){
		pulsador_2 = 0;
	}else{
	  pulsador_2 = 1;
	}
	if( PTAD_PTAD3 == 1  ){
		pulsador_3 = 0;
	}else{
	  pulsador_3 = 1;
	}
/********************************************/
  	KBISC_KBACK = 0x01;		//clear KBACK
  	return;
}
