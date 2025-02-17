#ifndef BARO_MS56111_I2C_H
#define BARO_MS56111_I2C_H

#include "std.h"

/* we use OSR=4096 for maximum resolution */
#define MS5611_SOFT_RESET       0x1E
#define MS5611_PROM_READ        0xA0
#define MS5611_START_CONV_D1    0x48
#define MS5611_START_CONV_D2    0x58
#define MS5611_ADC_READ         0x00

#define PROM_NB                 8

enum ms5611_stat{
  MS5611_UNINIT,
  MS5611_RESET,
  MS5611_RESET_OK,
  MS5611_PROM,
  MS5611_IDLE,
  MS5611_CONV_D1,
  MS5611_CONV_D1_OK,
  MS5611_ADC_D1,
  MS5611_CONV_D2,
  MS5611_CONV_D2_OK,
  MS5611_ADC_D2
};

void baro_ms5611_init(void);
void baro_ms5611_periodic(void);
void baro_ms5611_d1(void);
void baro_ms5611_d2(void);
void baro_ms5611_event(void);

#endif
