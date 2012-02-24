///
/// @file   isr.c
/// @author Kyeong Soo (Joseph) Kim <k.s.kim@swansea.ac.uk>
/// @date   2012-02-21
///
/// @brief  Implements interrupt service routines (ISRs).
///
/// @remarks Copyright (C) 2012 Swansea University. All rights reserved.
///
/// @remarks This software is written and distributed under the GNU General
///          Public License Version 2 (http://www.gnu.org/licenses/gpl-2.0.html).
///          You must not remove this notice, or any other, from this software.
///


#include "mouse.h"	// for the declaration of types, constants, variables and functions


interrupt VectorNumber_Vkeyboard1 void intSW3_4()
{
    byte sw3, sw4;
    
  sw3 = PTDD & 0x08;
  sw4 = PTDD & 0x04;

  // simple FSM for motor status handling
  if (sw3 == 0) {
    // for left motor
    switch (leftMotor) {
      case MOTOR_STATUS_STOP:
        ControlMotor(MOTOR_LEFT, MOTOR_ACTION_FORWARD);
        break;
      case MOTOR_STATUS_FORWARD:
        ControlMotor(MOTOR_LEFT, MOTOR_ACTION_REVERSE);
        break;
      case MOTOR_STATUS_REVERSE:
        ControlMotor(MOTOR_LEFT, MOTOR_ACTION_STOP);
        break;
    }
  } else if (sw4 == 0) {
    // for right motor
    switch (rightMotor) {
      case MOTOR_STATUS_STOP:
        ControlMotor(MOTOR_RIGHT, MOTOR_ACTION_FORWARD);
        break;
      case MOTOR_STATUS_FORWARD:
        ControlMotor(MOTOR_RIGHT, MOTOR_ACTION_REVERSE);
        break;
      case MOTOR_STATUS_REVERSE:
        ControlMotor(MOTOR_RIGHT, MOTOR_ACTION_STOP);
        break;
    }
  }
  
  // 'braking' hasn't been implemented yet
   
  KBI1SC_KBACK = 1; // clear KBI interrupt flag
}

// ISR to provide distance feedback to motor control functions
// based on tacho meter
interrupt VectorNumber_Vtpm2ovf void intTPM2OVF()
{
  TPM2C1SC = BitClear(7, KBI1SC); // clear TPM2 overflow
  
  if ((leftMotor != MOTOR_STATUS_STOP) && (rightMotor != MOTOR_STATUS_STOP)) {
    // balance the speeds of motors when both motors are moving
    ControlSpeed();
  }
}