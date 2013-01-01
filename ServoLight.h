/*
  Servo.h
  Mini servo driver
*/

#ifndef Servo_H
#define Servo_H

#include <inttypes.h>
#define MAX_SERVO 3
#define SERVO_INIT 0,0,0 

#define DEFAULT_PULSE_WIDTH  128     // default pulse width when servo is attached [ticks]
#define REFRESH_INTERVAL    40000     // servos refresh period in ticks


class Servo
{
public:
  Servo(uint8_t pin);
  void write(uint8_t value);
  uint8_t index;
};

#endif;