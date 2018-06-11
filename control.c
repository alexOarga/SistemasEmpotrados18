#include "derivative.h"

#include "encoder.h"
#include "pwm.h"
#include "ad.h"
#include "servos.h"
#include "clock.h"


static void InitSystem (void) ;
static void Init_Display();
static void Init_Button();
static void Init_LED();
static void display (unsigned char queDisplay, unsigned char valor);
	/* Muestra valor por el display queDisplay (0, 1, 2) */
static void led( unsigned char Salida );
static void led_cntf( unsigned char Salida );

unsigned int siguiente, periodo = 50 ;
float E, E_ant, U ;
float Y;
float Yd = 0.0;




#define PLM   0x04   // 0000_0100
#define PLP1   0x08   // 0000_1000
#define PLP2   0x10   // 0001_0000
#define PLP3   0x20   // 0010_0000
#define PLCN   0x40   // 0100_0000
#define PLCF  0x80   // 1000_0000


enum Estados{
	NP,	/* sin programa seleccionado / parado */    
	P1,	/* programa 1 */
	P2,	/* programa 2 */
	P3, /* programa 3 */
	P12, /* programa 1 espera soltar */
	P22, /* programa 2 espera soltar */
	P32,  /* programa 3 espera soltar */
	PM1,	/* programa 1 marcha */
	PM2,	/* programa 2 marcha */
	PM3		/* programa 3 marcha */
} ;
enum Estados estado_actual = NP;
unsigned int programa = 1;	   /* programa 1 2 3 */
unsigned int centrifugado = 0; /* centrifugado 0 / 1 */
/* el valor de centrifugado habra que cambiarlo al iniciar  */
float programa1_giro[5] = { 1, -1, 1, 0, 0 };
unsigned int programa1_duracion[5] = {4000, 4000, 4000, 4000, 10000};
float programa2_giro[4] = { -0.5, 1, 0, 0 };
unsigned int programa2_duracion[4] = {2000, 4000, 2000, 6000};
float programa3_giro[2] = { 0, 0 };
unsigned int programa3_duracion[2] = {3000 , 3000};
const unsigned int P1_LENGTH = 5;
const unsigned int P2_LENGTH = 4;
const unsigned int P3_LENGTH = 2;
unsigned int paso = 0; 		/*variable para almacenar paso actual del progama*/ 
unsigned int tiempo_paso = 0;

unsigned int pulsador_0 = 0;
unsigned int pulsador_1 = 0;
unsigned int pulsador_2 = 0;
unsigned int pulsador_3 = 0;
unsigned int pulsador_4 = 0;
unsigned int pulsador_5 = 0;


unsigned int muestreo = 0; 	/* 0 1 señal para muestreo cada 100ms */


void timer_fn(){
  
}

void main(void) {
 
 InitSystem () ;
 Init_Display();
 Init_Encoder ();
 Init_PWM () ;
 Init_AD () ;
 Init_LED() ;
 Init_Servos (periodo) ;
 
 Reset_Clock () ;
 Start_Clock () ;
 
 siguiente = Get_Time () ;

  E_ant = 0.0 ;
  U=0.0;
  
   Set_Timer(periodo, timer_fn);
  
  while(1) {
  
  //display (0x00, paso); 
 // display(0x01, programa);
  
if( Time_Out() ){
  
    Remove_Timer();
    Set_Timer(periodo, timer_fn);

	if( muestreo == 1 ){
		muestreo = 0;
		Y = velocity();
		//Yd = Read_Value_Int_1();
		//Yd = 0.0;
		//Yd=8.0*(float)Yd/1024.0;
		//Yd-=4.0;
		E=R(Yd,Y);
		U = (0.572*E) - (0.286*E_ant) + U ;
		action (U) ;
		E_ant = E ;    
	}
	muestreo = muestreo + 1;

	pulsador_0 = 0;
	pulsador_1 = 0;
	pulsador_2 = 0;
	pulsador_3 = 0;
	pulsador_4 = 0;
	pulsador_5 = 0;

display(0x01, programa);
  display (0x00, paso); 
 

	if( PTAD_PTAD1 == 0 ){
		pulsador_0 = 1;
	}
	if( PTAD_PTAD2 == 0 ){
		pulsador_1 = 1;
	}
	if( PTAD_PTAD3 == 0 ){
		pulsador_2 = 1;
	}
	if( PTAD_PTAD5== 0 ){
		pulsador_3 = 1;
	}
	if( PTAD_PTAD6 == 0 ){
		pulsador_4 = 1;
	}
	if( PTAD_PTAD7 == 0 ){
		pulsador_5 = 1;
	}
	
  display (0x00, paso);
  display(0x01, programa); 
/**********************************************************************/
  
      
    
    	switch(estado_actual){
      	case	NP:
    			led(0x00);
    			paso = 0; 		/*variable para almacenar paso actual del progama*/ 	
    			tiempo_paso = 0;
    			Yd = 0.0;
    			if( pulsador_2 == 1 ){
    				programa = 2;
    				estado_actual = P2;
    			}else if( pulsador_3 == 1 ){
    				programa = 3;
    				estado_actual = P3;
    			}else if( pulsador_1 == 1 ){
    				programa = 1;
    				estado_actual = P1;
    			}
    			break;
    		case P1:
    			led(PLP1);
    			if( pulsador_0 == 1 ){
    				estado_actual = P12;
    			}
    			if( pulsador_4 == 1 ){
    				led_cntf(PLCN);
    				centrifugado = 0;
    			}
    			if( pulsador_5 == 1 ){
    				led_cntf(PLCF);
    				centrifugado = 1;
    			}
    			if( pulsador_2 == 1 ){
    				programa = 2;
    				estado_actual = P2;
    			}else if( pulsador_3 == 1 ){
    				programa = 3;
    				estado_actual = P3;
    			}
    				break;
    		case P2:
    			led(PLP2);
    			if( pulsador_0 == 1 ){
    				estado_actual = P22;
    			}
    			if( pulsador_4 == 1 ){
    				led_cntf(PLCN);
    				centrifugado = 0;
    			}
    			if( pulsador_5 == 1 ){
    				led_cntf(PLCF);
    				centrifugado = 1;
    			}
    			if( pulsador_1 == 1 ){
    				programa = 1;
    				estado_actual = P1;
    			}else if( pulsador_3 == 1 ){
    				programa = 3;
    				estado_actual = P3;
    			}
    				break;
    		case P3:
    			led(PLP3);
    			if( pulsador_0 == 1 ){
    				estado_actual = P32;
    			}
    			if( pulsador_4 == 1 ){
    				led_cntf(PLCN);
    				centrifugado = 0;
    			}
    			if( pulsador_5 == 1 ){
    				led_cntf(PLCF);
    				centrifugado = 1;
    			}
    			if( pulsador_1 == 1 ){
    				programa = 1;
    				estado_actual = P1;
    			}else if( pulsador_2 == 1 ){
    				programa = 2;
    				estado_actual = P2;
    			}
    				break;
    		case P12:
    			if( pulsador_0 == 0 ){
    			  if( centrifugado == 1 ){
    			      programa1_giro[4]  = 3.2;
    			  }else{
    			      programa1_giro[4]  = 2.8;
    			  }
    				estado_actual = PM1;
    				paso = 0;
    				tiempo_paso = 0;
    			}
    				break;
    		case P22:
    		  if( centrifugado == 1 ){
    			      programa2_giro[3]  = 3.2;
    			  }else{
    			      programa2_giro[3]  = 2.8;
    			  }
    			if( pulsador_0 == 0 ){
    				paso = 0;
    				tiempo_paso = 0;
    				estado_actual = PM2;
    			}
    				break;
    		case P32:
    			if( pulsador_0 == 0 ){
    			  if( centrifugado == 1 ){
    			      programa3_giro[0]  = -3.2;
    			      programa3_giro[1]  = 3.2;
    			  }else{
    			      programa3_giro[0]  = -2.8;
    			      programa3_giro[1]  = 2.8;
    			  }
    				paso = 0;
    				tiempo_paso = 0;
    				estado_actual = PM3;
    			}
    				break;
    		case PM1:
    			if( pulsador_0 == 1 ){
    				/* RESET TODO */
    				estado_actual = NP;
    			}else{
    				Yd = programa1_giro[ paso ];
    				tiempo_paso = tiempo_paso + 50;
      				if( tiempo_paso > programa1_duracion[paso] ){
      				    tiempo_paso = 0;
      					  paso = paso + 1;
        					if( paso == P1_LENGTH ){
        						/* end TODO */
        						estado_actual = NP;
        					}
      				}
    			}
    				break;
    		case PM2:
    			if( pulsador_0 == 1 ){
    				/* RESET TODO */
    				estado_actual = NP;
    			}else{
    				Yd = programa2_giro[ paso ];
    				tiempo_paso = tiempo_paso + 50;
      				if( tiempo_paso > programa2_duracion[paso] ){
      				    tiempo_paso = 0;
      					  paso = paso + 1;
        					if( paso == P2_LENGTH ){
        						/* end TODO */
        						estado_actual = NP;
        					}
      				}
    			}
    			break;
    		case PM3:
    			if( pulsador_0 == 1 ){
    				/* RESET TODO */
    				estado_actual = NP;
    			}else{
    				Yd = programa3_giro[ paso ];
    				tiempo_paso = tiempo_paso + 50;
      				if( tiempo_paso > programa3_duracion[paso] ){
      				    tiempo_paso = 0;
      					  paso = paso + 1;
        					if( paso == P3_LENGTH ){
        						/* end TODO */
        						estado_actual = NP;
        					}
      				}
    			}
    				break;					
     	}  // SWITCH
  } //if
     	
     display(0x01, programa); 
     		
   display (0x00, paso);
    
    // display(0x01, programa); 
    // display (0x00, paso);
    
     
     //	siguiente += periodo ;
		  //delay_until(siguiente);
     	
     	
     	
  }//WHILÑE
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

void Init_Display(){
	/* Inicializar PTC (visualizacion)  */
   PTCD = 0 ;
  PTCDS = 0xFF ;
  PTCDD = 0xFF ;
}

void Init_Button(){
	  /*Inicializar puerto A (entrada de datos) */ 		
  PTAPE |= (PTAPE_PTAPE5_MASK | PTAPE_PTAPE1_MASK | PTAPE_PTAPE2_MASK | PTAPE_PTAPE3_MASK | PTAPE_PTAPE6_MASK  | PTAPE_PTAPE7_MASK );
  PTADD &= ~(PTAPE_PTAPE5_MASK | PTAPE_PTAPE1_MASK | PTAPE_PTAPE2_MASK | PTAPE_PTAPE3_MASK  | PTAPE_PTAPE6_MASK  | PTAPE_PTAPE7_MASK);  
}

void Init_LED(){
   PTBDD_PTBDD0 = 1 ; 
   PTBDD_PTBDD1 = 1 ; 
   PTBDD_PTBDD2 = 1 ;  
	 PTBDD_PTBDD3 = 1 ; 
	 PTBDD_PTBDD4 = 1 ;      
   PTBDD_PTBDD5 = 1 ;
	 PTBDD_PTBDD6 = 1 ;
	 PTBDD_PTBDD7 = 1 ;   
	 
	 PTBD_PTBD0 = 0 ;   
   PTBD_PTBD1 = 0 ;        
   PTBD_PTBD2 = 0 ; 
	 PTBD_PTBD3 = 0 ;   
   PTBD_PTBD4 = 0 ;        
   PTBD_PTBD5 = 0 ;
	 PTBD_PTBD6 = 0 ;
	 PTBD_PTBD7 = 0 ;      
}

void led( unsigned char Salida ){
	PTBD &= (0xC0);
	PTBD = (Salida) | (PTBD & 0xFF);
}

void led_cntf( unsigned char Salida ){
  PTBD &= (0x3F);
	PTBD = (Salida) | (PTBD & 0xFF);
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
