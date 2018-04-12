/******************************************************************
* Area Ingenieria de Sistemas y Automatica
* Sistemas Electronicos Programable
*******************************************************************
* nombre fichero : ascen10.c  (Control ascensor con 10 estados)
*******************************************************************
* proyecto       : Ascensor LEGO
* descripcion    : Control del ascensor implementado como un automata
                    de estado finito con 10 estados. Automata de Moore
* programador    : JL Villarroel (JLV) 
* fecha          : 11 mayo 2017
********************************************************************/

#include "derivative.h"
#include "clock.h"

/* Las entradas se leen del puerto A
     PTA1 --> B0
     PTA2 --> B1
     PTA3 --> B2
     PTA5 --> S0
     PTA6 --> S1
     PTA7 --> S2
 Las salidas se envian por el puerto B y el D 
     PTB4 / PTD4 --> SUBIR
     PTB5 / PTD5 --> BAJAR 
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

enum {INICIAL, ...} estado ;
unsigned char Entrada;    

unsigned int siguiente ; 

static void InitSystem (void) ;
static void ConfigurarEntradas(void);
static unsigned char LeerEntrada(void);
static void ConfigurarSalidas(void);
static void GenerarSalida(unsigned char Salida);
void display (unsigned char queDisplay, unsigned char valor) ;


void main (void){
 
  InitSystem () ;
  
  ConfigurarEntradas();
  ConfigurarSalidas();
  
  estado = INICIAL;
  while(1){
    Entrada = LeerEntrada();
    switch(estado){
      case INICIAL :GenerarSalida(...);
                    if(...)
                      estado=...;
                    break;
                    
      ...
    }
  }
}    



static void ConfigurarEntradas(void)
{
    PTADD = 0x00;         // PTA pins as inputs
    PTAPE = 0xEE ;        // Pull Up enable
                  
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
