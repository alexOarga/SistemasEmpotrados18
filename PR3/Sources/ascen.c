/******************************************************************
* Area Ingenieria de Sistemas y Automatica
* Sistemas Empotrados
*******************************************************************
* nombre fichero : ascen.c  (Control ascensor)
*******************************************************************
* proyecto       : PR3
* descripcion    : Control del ascensor implementado como un automata
                    de estado finitos de Moore
* programador    : 
********************************************************************/

#include "derivative.h"

/* Las entradas se leen del puerto A
     PTA1 --> B0
     PTA2 --> B1
     PTA3 --> B2
     PTA5 --> S0
     PTA6 --> S1
     PTA7 --> S2
 Las salidas se envian por el puerto B 
     PTB2 --> P0
     PTB3 --> P1
     PTB4 --> P2
     PTB5 --> SUBIR
     PTB6 --> BAJAR 
     PTB7 --> ciclo */
     


#define B0      0x02     // 0000_0010
#define B1      0x04     // 0000_0100
#define B2      0x08     // 0000_1000
#define S0      0x20     // 0010_0000
#define S1      0x40     // 0100_0000
#define S2      0x80     // 1000_0000

#define O_SUBIR   0x10   // 0001_0000
#define O_BAJAR   0x20   // 0010_0000
#define O_PARADO  0x30   // 0011_0000

enum {inicial,P0, P1, P2, HACIA_P0, HACIA_P1, HACIA_P2} estado ;
unsigned char Entrada;     
/********************************/
volatile unsigned int ultima_planta = 2;
volatile unsigned int siguiente =0;
volatile static unsigned int periodo =20;
/********************************/


static void InitSystem (void) ;
static void ConfigurarEntradas(void);
static unsigned char LeerEntrada(void);
static void ConfigurarSalidas(void);
static void GenerarSalida(unsigned char Salida);
void display (unsigned char queDisplay, unsigned char valor) ;

static unsigned char posicion_bit(unsigned char entrada, unsigned char posicion)
{
   return ( entrada >> posicion )&0x1 ;

}

void setPiso(void){
    estado = HACIA_P0;
}

void main (void){
 
  InitSystem () ;
  
  ConfigurarEntradas();
  ConfigurarSalidas();
  estado = HACIA_P1;
  siguiente = Get_Time(); 
  
  if(  posicion_bit(Entrada,5)==0x1 ){
    ultima_planta = 0; 
  }else if(  posicion_bit(Entrada,6)==0x1 ){
    ultima_planta = 1; 
  }else if(  posicion_bit(Entrada,7)==0x1 ){
    ultima_planta = 2; 
  }
  
  while(1){
    PTBD_PTBD7 = 1 -  PTBD_PTBD7;
    Entrada = LeerEntrada();
    switch(estado){
/********************************/
      case inicial :
           estado = HACIA_P0;
      case P0 :
					GenerarSalida(O_PARADO);
					display(0, 0);
					 ultima_planta = 0;
                    if( posicion_bit(Entrada,2)==0x1 ){
                      estado = HACIA_P1;
					}else if( posicion_bit(Entrada,3)==0x1 ){
						 estado = HACIA_P2;
					}
          break;
		case P1 :
					GenerarSalida(O_PARADO);
					display(0, 1);
					 ultima_planta = 1;
                    if( posicion_bit(Entrada,1)==0x1 ){
                      estado = HACIA_P0;
					}else if( posicion_bit(Entrada,3)==0x1 ){
						 estado = HACIA_P2;
					}
					
          break;
		case P2 :
					GenerarSalida(O_PARADO);
					 ultima_planta = 2;
					display(0, 2);
                    if( posicion_bit(Entrada,1)==0x1 ){
                      estado = HACIA_P0;
					}else if( posicion_bit(Entrada,2)==0x1 ){
						 estado = HACIA_P1;
					}
          break;
		case HACIA_P0 :
					GenerarSalida(O_BAJAR);
					display(0, ultima_planta);
					if( posicion_bit(Entrada,6)==0x1 ){
                ultima_planta = 1;
					}
          if( posicion_bit(Entrada,5)==0x1 ){
                      estado = P0;
					}
					Remove_Timer ();        
          break;
		case HACIA_P1 :
		      if(ultima_planta==0){
		        GenerarSalida(O_SUBIR);
		      }else{
		        GenerarSalida(O_BAJAR);
		      }
					display(0, ultima_planta);
                    if( posicion_bit(Entrada,6)==0x1 ){
                      estado = P1;
					}
          break;
          Remove_Timer ();
		case HACIA_P2 :
					GenerarSalida(O_SUBIR);
					display(0, ultima_planta);
					if( posicion_bit(Entrada,6)==0x1 ){
                     ultima_planta = 1; 	
					}
          if( posicion_bit(Entrada,7)==0x1 ){
                      estado = P2;
                      Set_Timer(5000,setPiso);
					}
          break;
/*******************************/
    
    }
    siguiente = siguiente + periodo;
    delay_until(siguiente);
  }
}    



static void ConfigurarEntradas(void)
{
    PTADD = 0x00;         // PTA pins as inputs
    PTAPE = 0xEE ;        // Pull Up enable
    PTBDD = 0x80;
                  
    SOPT1_RSTPE = 0 ;     // PTA5 pin reset disable
    SOPT1_TCLKPEN = 0 ;   // PTA5 pin TCLK disable
    IRQSC_IRQPE = 0 ;     // PTA5 pin IRQ disable
}


void InitSystem (void) {

  /* Internal Clock Source Configuration */
	if(NVICSTRM == 0xFF)			// If programmed trim value is blank
		ICSTRM = 0x80;				  // load default value
	else
		ICSTRM = NVICSTRM;	  	// else load programmed trim value
  ICSC2_BDIV = 1 ;          // Divides selected clock by 2		
	                          // Bus clock = 5 MHz, CPU clock = 10MHz

  
  SOPT1_COPT = 0 ;          // WATCHDOG disable 
  Reset_Clock();
  Start_Clock();
}

static unsigned char LeerEntrada(void)
{
   return (~PTAD)&0xEE ;
}


static void ConfigurarSalidas(void)
{
     PTDDD_PTDDD4 = 1 ;      // PTD4..5 as outputs
     PTDDD_PTDDD5 = 1 ;  
     PTDD_PTDD4 = 1 ;        // Motor stopped
     PTDD_PTDD5 = 1 ;
     
     PTBDD_PTBDD4 = 1 ;      // PTB4..5 as outputs
     PTBDD_PTBDD5 = 1 ;  
     PTBD_PTBD4 = 1 ;        // Motor stopped
     PTBD_PTBD5 = 1 ;

     PTCD = 0x70 ;
     PTCDD = 0x7F ;          // PTC0..6 as outputs
            

}

static void GenerarSalida(unsigned char Salida)
{
  PTDD = (Salida) | (PTDD & 0xCF);
  PTBD = (Salida) | (PTBD & 0xCF);
}


void display (unsigned char queDisplay, unsigned char valor) 
{

  PTCD = ((~(0x1 << queDisplay)) << 4) | (valor&0x0F) ;
  return ;
  
}  

