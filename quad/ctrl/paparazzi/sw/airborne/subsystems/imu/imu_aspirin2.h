/*
 * Copyright (C) 2012 Christophe DeWagter
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

#ifndef IMU_ASPIRIN_2_H
#define IMU_ASPIRIN_2_H

#include "generated/airframe.h"
#include "subsystems/imu.h"


#ifdef IMU_ASPIRIN_VERSION_2_1
#if !defined IMU_MAG_X_SIGN & !defined IMU_MAG_Y_SIGN & !defined IMU_MAG_Z_SIGN
#define IMU_MAG_X_SIGN 1
#define IMU_MAG_Y_SIGN 1
#define IMU_MAG_Z_SIGN 1
#endif
#endif

#if !defined IMU_GYRO_P_SIGN & !defined IMU_GYRO_Q_SIGN & !defined IMU_GYRO_R_SIGN
#define IMU_GYRO_P_SIGN   1
#define IMU_GYRO_Q_SIGN   1
#define IMU_GYRO_R_SIGN   1
#endif
#if !defined IMU_ACCEL_X_SIGN & !defined IMU_ACCEL_Y_SIGN & !defined IMU_ACCEL_Z_SIGN
#define IMU_ACCEL_X_SIGN  1
#define IMU_ACCEL_Y_SIGN  1
#define IMU_ACCEL_Z_SIGN  1
#endif

/** default gyro sensitivy and neutral from the datasheet
 * MPU60X0 has 16.4 LSB/(deg/s) at 2000deg/s range
 * sens = 1/16.4 * pi/180 * 2^INT32_RATE_FRAC
 * sens = 1/16.4 * pi/180 * 4096 = 4.359066229
 */
#if !defined IMU_GYRO_P_SENS & !defined IMU_GYRO_Q_SENS & !defined IMU_GYRO_R_SENS
#define IMU_GYRO_P_SENS 4.359
#define IMU_GYRO_P_SENS_NUM 4359
#define IMU_GYRO_P_SENS_DEN 1000
#define IMU_GYRO_Q_SENS 4.359
#define IMU_GYRO_Q_SENS_NUM 4359
#define IMU_GYRO_Q_SENS_DEN 1000
#define IMU_GYRO_R_SENS 4.359
#define IMU_GYRO_R_SENS_NUM 4359
#define IMU_GYRO_R_SENS_DEN 1000
#endif
#if !defined IMU_GYRO_P_NEUTRAL & !defined IMU_GYRO_Q_NEUTRAL & !defined IMU_GYRO_R_NEUTRAL
#define IMU_GYRO_P_NEUTRAL 0
#define IMU_GYRO_Q_NEUTRAL 0
#define IMU_GYRO_R_NEUTRAL 0
#endif

/** default accel sensitivy from the datasheet
 * MPU60X0 has 2048 LSB/g
 * fixed point sens: 9.81 [m/s^2] / 2048 [LSB/g] * 2^INT32_ACCEL_FRAC
 * sens = 9.81 / 2048 * 1024 = 4.905
 */
#if !defined IMU_ACCEL_X_SENS & !defined IMU_ACCEL_Y_SENS & !defined IMU_ACCEL_Z_SENS
#define IMU_ACCEL_X_SENS 4.905
#define IMU_ACCEL_X_SENS_NUM 4905
#define IMU_ACCEL_X_SENS_DEN 1000
#define IMU_ACCEL_Y_SENS 4.905
#define IMU_ACCEL_Y_SENS_NUM 4905
#define IMU_ACCEL_Y_SENS_DEN 1000
#define IMU_ACCEL_Z_SENS 4.905
#define IMU_ACCEL_Z_SENS_NUM 4905
#define IMU_ACCEL_Z_SENS_DEN 1000
#endif
#if !defined IMU_ACCEL_X_NEUTRAL & !defined IMU_ACCEL_Y_NEUTRAL & !defined IMU_ACCEL_Z_NEUTRAL
#define IMU_ACCEL_X_NEUTRAL 0
#define IMU_ACCEL_Y_NEUTRAL 0
#define IMU_ACCEL_Z_NEUTRAL 0
#endif


enum Aspirin2Status
  { Aspirin2StatusUninit,
    Aspirin2StatusIdle,
    Aspirin2StatusReading
  };

struct ImuAspirin2 {
  volatile enum Aspirin2Status status;
  volatile uint8_t imu_spi_data_received;
  volatile uint8_t imu_tx_buf[64];
  volatile uint8_t imu_rx_buf[64];
};

extern struct ImuAspirin2 imu_aspirin2;


static inline uint8_t imu_from_buff(void)
{
  int32_t x, y, z, p, q, r, Mx, My, Mz;

#define MPU_OFFSET_STATUS 1
  if (!(imu_aspirin2.imu_rx_buf[MPU_OFFSET_STATUS] & 0x01)) {
    return 0;
  }

  // If the itg3200 I2C transaction has succeeded: convert the data
#define MPU_OFFSET_GYRO 10
  p = (int16_t) ((imu_aspirin2.imu_rx_buf[0+MPU_OFFSET_GYRO] << 8) | imu_aspirin2.imu_rx_buf[1+MPU_OFFSET_GYRO]);
  q = (int16_t) ((imu_aspirin2.imu_rx_buf[2+MPU_OFFSET_GYRO] << 8) | imu_aspirin2.imu_rx_buf[3+MPU_OFFSET_GYRO]);
  r = (int16_t) ((imu_aspirin2.imu_rx_buf[4+MPU_OFFSET_GYRO] << 8) | imu_aspirin2.imu_rx_buf[5+MPU_OFFSET_GYRO]);

#define MPU_OFFSET_ACC 2
  x = (int16_t) ((imu_aspirin2.imu_rx_buf[0+MPU_OFFSET_ACC] << 8) | imu_aspirin2.imu_rx_buf[1+MPU_OFFSET_ACC]);
  y = (int16_t) ((imu_aspirin2.imu_rx_buf[2+MPU_OFFSET_ACC] << 8) | imu_aspirin2.imu_rx_buf[3+MPU_OFFSET_ACC]);
  z = (int16_t) ((imu_aspirin2.imu_rx_buf[4+MPU_OFFSET_ACC] << 8) | imu_aspirin2.imu_rx_buf[5+MPU_OFFSET_ACC]);

#define MPU_OFFSET_MAG 16
  Mx = (int16_t) ((imu_aspirin2.imu_rx_buf[0+MPU_OFFSET_MAG] << 8) | imu_aspirin2.imu_rx_buf[1+MPU_OFFSET_MAG]);
  My = (int16_t) ((imu_aspirin2.imu_rx_buf[2+MPU_OFFSET_MAG] << 8) | imu_aspirin2.imu_rx_buf[3+MPU_OFFSET_MAG]);
  Mz = (int16_t) ((imu_aspirin2.imu_rx_buf[4+MPU_OFFSET_MAG] << 8) | imu_aspirin2.imu_rx_buf[5+MPU_OFFSET_MAG]);

#ifdef LISA_M_LONGITUDINAL_X
  RATES_ASSIGN(imu.gyro_unscaled, q, -p, r);
  VECT3_ASSIGN(imu.accel_unscaled, y, -x, z);
  VECT3_ASSIGN(imu.mag_unscaled, -Mx, -Mz, My);
#else
  RATES_ASSIGN(imu.gyro_unscaled, p, q, r);
  VECT3_ASSIGN(imu.accel_unscaled, x, y, z);
  VECT3_ASSIGN(imu.mag_unscaled, Mz, -Mx, My);
#endif

  return 1;
}


static inline void imu_aspirin2_event(void (* _gyro_handler)(void), void (* _accel_handler)(void), void (* _mag_handler)(void))
{
  if (imu_aspirin2.status == Aspirin2StatusUninit) return;

  if (imu_aspirin2.imu_spi_data_received) {
    imu_aspirin2.imu_spi_data_received = FALSE;
    if (imu_from_buff())
    {
      _gyro_handler();
      _accel_handler();
      _mag_handler();
    }
  }
}

#define ImuEvent(_gyro_handler, _accel_handler, _mag_handler) { \
  imu_aspirin2_event(_gyro_handler, _accel_handler, _mag_handler); \
}

#endif /* IMU_ASPIRIN_2_H */
