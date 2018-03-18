/******************************************************************
* Area Ingenieria de Sistemas y Automatica
* Sistemas Electronicos Programable
*******************************************************************
* nombre fichero : prucon.c  prueba conexiones I/O practica 7
*******************************************************************
* proyecto       : PR7
* descripcion    : Prueba de las conexiones de las I/O 
* programador    : JL Villarroel (JLV) 
* fecha          : 25 marzo 2014
********************************************************************/
#include "derivative.h"
// #include "clock.h"

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

#define O_P0      0x04   // 0000_0100
#define O_P1      0x08   // 0000_1000
#define O_P2      0x10   // 0001_0000
#define O_SUBIR   0x20   // 0010_0000
#define O_BAJAR   0x40   // 0100_0000
#define O_PARADO  0x00   // 0000_0000



static void InitSystem (void) ;
static void ConfigurarEntradas(void);
static unsigned char LeerEntrada(void);
static void ConfigurarSalidas(void);
static void GenerarSalida(unsigned char Salida);
static void Temporizar(void);


void main (void){
  
  unsigned char Entrada;
  int i;
  InitSystem () ;
  ConfigurarEntradas();
  ConfigurarSalidas();
 
  for(i=0; i < 3; i++){
    GenerarSalida(O_P0);
    Temporizar();
    GenerarSalida(O_P1);
    Temporizar();
    GenerarSalida(O_P2);
    Temporizar();
    
    GenerarSalida(O_SUBIR);
    Temporizar();
    GenerarSalida(O_BAJAR);
    Temporizar();
  }
  
    
 
  while(1){
    Entrada = LeerEntrada();
    GenerarSalida(Entrada);
  }
}    

static void Temporizar(void){
  unsigned int i ;
  
  for (i=0; i < 0xFFFF; i++) asm NOP ;
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


static void ConfigurarEntradas(void)
{
    PTADD = 0x00;         // PTA pins as inputs
                  
    SOPT1_RSTPE = 0 ;     // PTA5 pin reset disable
    SOPT1_TCLKPEN = 0 ;   // PTA5 pin TCLK disable
    IRQSC_IRQPE = 0 ;     // PTA5 pin IRQ disable
}


static unsigned char LeerEntrada(void)
{
   return (~PTAD)&0xEE ;
}


static void ConfigurarSalidas(void)
{
     PTBDD |=0xFC;          // PTB2..7 as outputs
     PTCD = 0x70 ;
     PTCDD = 0x7F ;         // PTC0..6 as outputs

}

static void GenerarSalida(unsigned char Salida)
{
  PTBD = (Salida) | (PTBD & 0x03);
}


void display (unsigned char queDisplay, unsigned char valor) 
{

  PTCD = ((~(0x1 << queDisplay)) << 4) | (valor&0x0F) ;
  return ;
  
}  
