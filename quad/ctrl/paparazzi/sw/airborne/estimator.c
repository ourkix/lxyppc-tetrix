/*
 * Paparazzi autopilot $Id$
 *
 * Copyright (C) 2004-2010 The Paparazzi Team
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
 *
 */

/** \file estimator.c
 * \brief State estimate, fusioning sensors
 */

#include <inttypes.h>
#include <math.h>

#include "estimator.h"
#include "mcu_periph/uart.h"
#include "ap_downlink.h"
#include "subsystems/gps.h"
#include "subsystems/nav.h"
#ifdef EXTRA_DOWNLINK_DEVICE
#include "core/extra_pprz_dl.h"
#endif

/* position in meters */
float estimator_x;
float estimator_y;
float estimator_z;

float estimator_z_dot;

/* attitude in radian */
float estimator_phi;
float estimator_psi;
float estimator_theta;

/* rates in radians per second */
float estimator_p;
float estimator_q;
float estimator_r;

/* flight time in seconds */
uint16_t estimator_flight_time;
/* flight time in seconds */
float estimator_t;

/* horizontal speed in module and dir */
float estimator_hspeed_mod;
float estimator_hspeed_dir;

/* wind */
float wind_east, wind_north;
float estimator_airspeed;
float estimator_AOA;

#define NORM_RAD_ANGLE2(x) { \
    while (x > 2 * M_PI) x -= 2 * M_PI; \
    while (x < 0 ) x += 2 * M_PI; \
  }


#define EstimatorSetSpeedCart(vx, vy, vz) { \
  estimator_vx = vx; \
  estimator_vy = vy; \
  estimator_vz = vz; \
}


void estimator_init( void ) {

  EstimatorSetPosXY(0., 0.);
  EstimatorSetAlt(0.);

  EstimatorSetAtt (0., 0., 0);

  EstimatorSetSpeedPol ( 0., 0., 0.);

  EstimatorSetRate(0., 0., 0.);

#ifdef USE_AOA
  EstimatorSetAOA( 0. );
#endif

  estimator_flight_time = 0;

  // FIXME? Set initial airspeed to zero if USE_AIRSPEED ?
  EstimatorSetAirspeed( NOMINAL_AIRSPEED );
}


bool_t alt_kalman_enabled;

#ifdef ALT_KALMAN

#ifndef ALT_KALMAN_ENABLED
#define ALT_KALMAN_ENABLED FALSE
#endif

#define GPS_SIGMA2 1.
#define GPS_DT 0.25
#define GPS_R 2.

#define BARO_DT 0.1

static float p[2][2];

void alt_kalman_reset( void ) {
  p[0][0] = 1.;
  p[0][1] = 0.;
  p[1][0] = 0.;
  p[1][1] = 1.;
}

void alt_kalman_init( void ) {
  alt_kalman_enabled = ALT_KALMAN_ENABLED;
  alt_kalman_reset();
}

void alt_kalman(float gps_z) {
  float DT;
  float R;
  float SIGMA2;

#if USE_BARO_MS5534A
  if (alt_baro_enabled) {
    DT = BARO_DT;
    R = baro_MS5534A_r;
    SIGMA2 = baro_MS5534A_sigma2;
  } else
#elif USE_BARO_ETS
  if (baro_ets_enabled) {
    DT = BARO_ETS_DT;
    R = baro_ets_r;
    SIGMA2 = baro_ets_sigma2;
  } else
#elif USE_BARO_BMP
  if (baro_bmp_enabled) {
    DT = BARO_BMP_DT;
    R = baro_bmp_r;
    SIGMA2 = baro_bmp_sigma2;
  } else
#endif
  {
    DT = GPS_DT;
    R = GPS_R;
    SIGMA2 = GPS_SIGMA2;
  }

  float q[2][2];
  q[0][0] = DT*DT*DT*DT/4.;
  q[0][1] = DT*DT*DT/2.;
  q[1][0] = DT*DT*DT/2.;
  q[1][1] = DT*DT;


  /* predict */
  estimator_z += estimator_z_dot * DT;
  p[0][0] = p[0][0]+p[1][0]*DT+DT*(p[0][1]+p[1][1]*DT) + SIGMA2*q[0][0];
  p[0][1] = p[0][1]+p[1][1]*DT + SIGMA2*q[0][1];
  p[1][0] = p[1][0]+p[1][1]*DT + SIGMA2*q[1][0];
  p[1][1] = p[1][1] + SIGMA2*q[1][1];

  /* error estimate */
  float e = p[0][0] + R;

  if (fabs(e) > 1e-5) {
    float k_0 = p[0][0] / e;
    float k_1 =  p[1][0] / e;
    e = gps_z - estimator_z;

    /* correction */
    estimator_z += k_0 * e;
    estimator_z_dot += k_1 * e;

    p[1][0] = -p[0][0]*k_1+p[1][0];
    p[1][1] = -p[0][1]*k_1+p[1][1];
    p[0][0] = p[0][0] * (1-k_0);
    p[0][1] = p[0][1] * (1-k_0);
  }

#ifdef DEBUG_ALT_KALMAN
  DOWNLINK_SEND_ALT_KALMAN(DefaultChannel,DefaultDevice,&(p[0][0]),&(p[0][1]),&(p[1][0]), &(p[1][1]));
#endif
}

#endif // ALT_KALMAN

void estimator_update_state_gps( void ) {
  float gps_east = gps.utm_pos.east / 100.;
  float gps_north = gps.utm_pos.north / 100.;

  /* Relative position to reference */
  gps_east -= nav_utm_east0;
  gps_north -= nav_utm_north0;

  EstimatorSetPosXY(gps_east, gps_north);
#if !USE_BARO_BMP && !USE_BARO_ETS && !USE_BARO_MS5534A
  float falt = gps.hmsl / 1000.;
  EstimatorSetAlt(falt);
#endif
  float fspeed = gps.gspeed / 100.;
  float fclimb = -gps.ned_vel.z / 100.;
  float fcourse = gps.course / 1e7;
  EstimatorSetSpeedPol(fspeed, fcourse, fclimb);

  // Heading estimation now in ahrs_infrared

}
