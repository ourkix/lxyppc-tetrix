/*
 * Copyright (C) 2008-2012 The Paparazzi Team
 *
 * This file is part of paparazzi.
 *
 * paparazzi is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * paparazzi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with paparazzi; see the file COPYING.  If not, write to
 * the Free Software Foundation, 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "firmwares/rotorcraft/autopilot.h"

#include "subsystems/radio_control.h"
#include "firmwares/rotorcraft/commands.h"
#include "firmwares/rotorcraft/navigation.h"
#include "firmwares/rotorcraft/guidance.h"
#include "firmwares/rotorcraft/stabilization.h"
#include "led.h"

uint8_t  autopilot_mode;
uint8_t  autopilot_mode_auto2;

bool_t   autopilot_in_flight;
uint32_t autopilot_in_flight_counter;
uint16_t autopilot_flight_time;

bool_t   autopilot_motors_on;
bool_t   kill_throttle;

bool_t   autopilot_rc;
bool_t   autopilot_power_switch;

bool_t   autopilot_detect_ground;
bool_t   autopilot_detect_ground_once;

#define AUTOPILOT_IN_FLIGHT_TIME    40

#ifndef AUTOPILOT_DISABLE_AHRS_KILL
#include "subsystems/ahrs.h"
static inline int ahrs_is_aligned(void) {
  return (ahrs.status == AHRS_RUNNING);
}
#else
static inline int ahrs_is_aligned(void) {
  return TRUE;
}
#endif

#if USE_KILL_SWITCH_FOR_MOTOR_ARMING
#include "autopilot_arming_switch.h"
#elif USE_THROTTLE_FOR_MOTOR_ARMING
#include "autopilot_arming_throttle.h"
#else
#include "autopilot_arming_yaw.h"
#endif

void autopilot_init(void) {
  autopilot_mode = AP_MODE_KILL;
  autopilot_motors_on = FALSE;
  kill_throttle = ! autopilot_motors_on;
  autopilot_in_flight = FALSE;
  autopilot_in_flight_counter = 0;
  autopilot_mode_auto2 = MODE_AUTO2;
  autopilot_detect_ground = FALSE;
  autopilot_detect_ground_once = FALSE;
  autopilot_flight_time = 0;
  autopilot_rc = TRUE;
  autopilot_power_switch = FALSE;
#ifdef POWER_SWITCH_LED
  LED_ON(POWER_SWITCH_LED); // POWER OFF
#endif
  autopilot_arming_init();
}


void autopilot_periodic(void) {

  RunOnceEvery(NAV_PRESCALER, nav_periodic_task());
#ifdef FAILSAFE_GROUND_DETECT
  if (autopilot_mode == AP_MODE_FAILSAFE && autopilot_detect_ground) {
    autopilot_set_mode(AP_MODE_KILL);
    autopilot_detect_ground = FALSE;
  }
#endif

  /* set failsafe commands, if in FAILSAFE or KILL mode */
#ifndef FAILSAFE_GROUND_DETECT
  if (autopilot_mode == AP_MODE_KILL ||
      autopilot_mode == AP_MODE_FAILSAFE) {
#else
  if (autopilot_mode == AP_MODE_KILL) {
#endif
    SetCommands(commands_failsafe,
		autopilot_in_flight, autopilot_motors_on);
  }
  else {
    guidance_v_run( autopilot_in_flight );
    guidance_h_run( autopilot_in_flight );
    SetCommands(stabilization_cmd,
        autopilot_in_flight, autopilot_motors_on);
  }

}


void autopilot_set_mode(uint8_t new_autopilot_mode) {

  /* force kill mode as long as AHRS is not aligned */
  if (!ahrs_is_aligned())
    new_autopilot_mode = AP_MODE_KILL;

  if (new_autopilot_mode != autopilot_mode) {
    /* horizontal mode */
    switch (new_autopilot_mode) {
    case AP_MODE_FAILSAFE:
#ifndef KILL_AS_FAILSAFE
      stab_att_sp_euler.phi = 0;
      stab_att_sp_euler.theta = 0;
      guidance_h_mode_changed(GUIDANCE_H_MODE_ATTITUDE);
      break;
#endif
    case AP_MODE_KILL:
      autopilot_set_motors_on(FALSE);
      autopilot_in_flight = FALSE;
      autopilot_in_flight_counter = 0;
      guidance_h_mode_changed(GUIDANCE_H_MODE_KILL);
      break;
    case AP_MODE_RC_DIRECT:
      guidance_h_mode_changed(GUIDANCE_H_MODE_RC_DIRECT);
      break;
    case AP_MODE_RATE_DIRECT:
    case AP_MODE_RATE_Z_HOLD:
      guidance_h_mode_changed(GUIDANCE_H_MODE_RATE);
      break;
    case AP_MODE_ATTITUDE_DIRECT:
    case AP_MODE_ATTITUDE_CLIMB:
    case AP_MODE_ATTITUDE_Z_HOLD:
      guidance_h_mode_changed(GUIDANCE_H_MODE_ATTITUDE);
      break;
    case AP_MODE_HOVER_DIRECT:
    case AP_MODE_HOVER_CLIMB:
    case AP_MODE_HOVER_Z_HOLD:
      guidance_h_mode_changed(GUIDANCE_H_MODE_HOVER);
      break;
    case AP_MODE_NAV:
      guidance_h_mode_changed(GUIDANCE_H_MODE_NAV);
      break;
    default:
      break;
    }
    /* vertical mode */
    switch (new_autopilot_mode) {
    case AP_MODE_FAILSAFE:
#ifndef KILL_AS_FAILSAFE
      guidance_v_zd_sp = SPEED_BFP_OF_REAL(0.5);
      guidance_v_mode_changed(GUIDANCE_V_MODE_CLIMB);
      break;
#endif
    case AP_MODE_KILL:
      guidance_v_mode_changed(GUIDANCE_V_MODE_KILL);
      break;
    case AP_MODE_RC_DIRECT:
    case AP_MODE_RATE_DIRECT:
    case AP_MODE_ATTITUDE_DIRECT:
    case AP_MODE_HOVER_DIRECT:
      guidance_v_mode_changed(GUIDANCE_V_MODE_RC_DIRECT);
      break;
    case AP_MODE_RATE_RC_CLIMB:
    case AP_MODE_ATTITUDE_RC_CLIMB:
      guidance_v_mode_changed(GUIDANCE_V_MODE_RC_CLIMB);
      break;
    case AP_MODE_ATTITUDE_CLIMB:
    case AP_MODE_HOVER_CLIMB:
      guidance_v_mode_changed(GUIDANCE_V_MODE_CLIMB);
      break;
    case AP_MODE_RATE_Z_HOLD:
    case AP_MODE_ATTITUDE_Z_HOLD:
    case AP_MODE_HOVER_Z_HOLD:
      guidance_v_mode_changed(GUIDANCE_V_MODE_HOVER);
      break;
    case AP_MODE_NAV:
      guidance_v_mode_changed(GUIDANCE_V_MODE_NAV);
      break;
    default:
      break;
    }
    autopilot_mode = new_autopilot_mode;
  }

}


static inline void autopilot_check_in_flight( bool_t motors_on ) {
  if (autopilot_in_flight) {
    if (autopilot_in_flight_counter > 0) {
      if (THROTTLE_STICK_DOWN()) {
        autopilot_in_flight_counter--;
        if (autopilot_in_flight_counter == 0) {
          autopilot_in_flight = FALSE;
        }
      }
      else {	/* !THROTTLE_STICK_DOWN */
        autopilot_in_flight_counter = AUTOPILOT_IN_FLIGHT_TIME;
      }
    }
  }
  else { /* not in flight */
    if (autopilot_in_flight_counter < AUTOPILOT_IN_FLIGHT_TIME &&
        motors_on) {
      if (!THROTTLE_STICK_DOWN()) {
        autopilot_in_flight_counter++;
        if (autopilot_in_flight_counter == AUTOPILOT_IN_FLIGHT_TIME)
          autopilot_in_flight = TRUE;
      }
      else { /*  THROTTLE_STICK_DOWN */
        autopilot_in_flight_counter = 0;
      }
    }
  }
}


void autopilot_set_motors_on(bool_t motors_on) {
  if (ahrs_is_aligned() && motors_on)
    autopilot_motors_on = TRUE;
  else
    autopilot_motors_on = FALSE;
  kill_throttle = ! autopilot_motors_on;
  autopilot_arming_set(autopilot_motors_on);
}


void autopilot_on_rc_frame(void) {

  if (kill_switch_is_on())
    autopilot_set_mode(AP_MODE_KILL);
  else {
    uint8_t new_autopilot_mode = 0;
    AP_MODE_OF_PPRZ(radio_control.values[RADIO_MODE], new_autopilot_mode);
    autopilot_set_mode(new_autopilot_mode);
  }

  /* if not in FAILSAFE mode check motor and in_flight status, read RC */
  if (autopilot_mode > AP_MODE_FAILSAFE) {

    /* an arming sequence is used to start/stop motors */
    autopilot_arming_check_motors_on();
    kill_throttle = ! autopilot_motors_on;

    autopilot_check_in_flight(autopilot_motors_on);

    guidance_v_read_rc();
    guidance_h_read_rc(autopilot_in_flight);
  }

}
