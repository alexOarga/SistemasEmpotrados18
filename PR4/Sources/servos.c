/******************************************************************************/
/*                                                                            */
/* project  : PRACTICAS SE UNIZAR                                             */
/* filename : servos.c                                                        */
/* version  : 3                                                               */
/* date     : 16/09/2016                                                      */
/* author   : Jose Luis Villarroel                                            */
/* description : Servocontrol of LEGO robot motors                            */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*                        Used modules                                        */
/******************************************************************************/

#include "pwm.h"
#include "servos.h"
#include "encoder.h"

/******************************************************************************/
/*                        Local variables                                     */
/******************************************************************************/

const unsigned int dNmax = 200 ;

static unsigned int sampling_period ;

float El = 0.0, El_ant = 0.0, Ul = 0.0 ;


/******************************************************************************/
/*                        Exported functions                                  */
/******************************************************************************/

void Init_Servos (unsigned int T) {
	sampling_period = T ; // sampling period in ms
}


float velocity (void) {
	static unsigned int previous_counter = 0 ;
	unsigned int counter ;
	float dN ;

	counter = Get_Counter () ;
	if (counter < previous_counter)
		if ((previous_counter - counter) > dNmax) dN = (float)(counter - previous_counter) ;
		else dN = - (float)(previous_counter - counter) ;
	else
		if ((counter - previous_counter) > dNmax) dN = - (float)(previous_counter - counter) ;
		else dN = (float)(counter - previous_counter) ;
	previous_counter = counter ;
	return (dN * 8.725 / (float)sampling_period) ;
}


void action (float U) {
	unsigned int PWM ;

	if (U > 5.0) U = 5.0 ;
	if (U < -5.0) U = -5.0 ;
//	PWM =  ;
	Set_Value_10b (PWM) ;
}

float R (float Wref, float W) {

	El = Wref - W ;

	return Ul ;
}


