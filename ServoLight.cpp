/*
 ServoLight.cpp - Interrupt driven Minimal Servo library for MSP430
 by Peter Brier 20-11-2012
 
 Derived from:
 Servo.cpp - Interrupt driven Servo library for Arduino using 16 bit timers- Version 2
 Copyright (c) 2009 Michael Margolis.  All right reserved.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "Energia.h"
#include "ServoLight.h"

// Globals
static volatile uint8_t servo_pin[MAX_SERVO] = {SERVO_INIT};
static volatile uint8_t servo_period[MAX_SERVO] = {SERVO_INIT};
static volatile int8_t servo_count = 0, servo_counter = -1;
static volatile uint16_t servo_time = 0; // total time 




/************ static functions and data structures common to all instances ***********************/
// Timer A0 interrupt service routine
#ifndef TIMERA0_VECTOR
#define TIMERA0_VECTOR TIMER0_A0_VECTOR
#endif 
__attribute__((interrupt(TIMERA0_VECTOR))) static void Timer_A_int(void)
{

  if (servo_counter >= 0) 
    digitalWrite(servo_pin[servo_counter], LOW);     /* Turn pulse off. */
  
  /* Service next servo, inactive records have a pin number 0, so this nothing happens */
  servo_counter++;

  /* Counter range is 0 .. ServoCount, the last count is used to complete the REFRESH_INTERVAL */
  if (servo_counter < servo_count) 
  {
    volatile uint16_t period = 1024 + ((uint16_t)servo_period[servo_counter]<<4); // assume FCLK/8 = 2Mhz, so we have 512..2560usec pulse
    digitalWrite(servo_pin[servo_counter], HIGH);     /* Turn pulse on for the next servo. */
    servo_time += period;    /* And hold! */
    TA0CCR0 = period;
  } 
  else 
  {     /* Wait for the remaining of REFRESH_INTERVAL. */
	volatile uint16_t time = (REFRESH_INTERVAL - servo_time);
	if ( time < 1024 ) 
	  time = 1024;
    TA0CCR0 = time; //time;
    servo_counter = -1;
	servo_time = 0;
  }
}

// Intiantiate Servo object
Servo::Servo(uint8_t pin)
{
  this->index = servo_count++;
  servo_period[this->index] = DEFAULT_PULSE_WIDTH;   
  servo_pin[this->index] = pin;
  pinMode( pin, OUTPUT) ; // set servo pin to output
  
  servo_counter = -1;
//  Timer_A_int(); // enable first servo
  TA0CCR0 = 10000;
  TA0CCTL0 = CCIE;                     // CCR0 interrupt enabled
  TA0CTL = TASSEL_2 + MC_1 + ID_3;     // prescale SMCLK/8, upmode
}

// Write a servo value
void Servo::write(uint8_t value)
{   
 // volatile uint8_t v = value;  // convert to ticks after compensating for interrupt overhead - 12 Aug 2009
  servo_period[this->index] = value; // this is atomic on a 16bit uC, no need to disable Interrupts 
}


