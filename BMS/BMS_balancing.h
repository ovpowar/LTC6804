#include <stdint.h>
#include <LTC68041.h>
#include <util/delay.h>
#include <BMS_config.h>

#ifndef BMS_BALANCING
#define BMS_BALANCING
#define DISCHARGE_ON 1
#define DISCHARGE_OFF 0


int dischargeCode;

//Functions declarations

int startBalancing(uint16_t *dischargeCode, int dischargePin, int stackNumber);

int stopBalancing(uint16_t *dischargeCode, int dischargePin, int stackNumber);

int holdBalancing(uint16_t *dischargeCode, int stackNumber);

int verifyBalancing(uint16_t *dischargeCode, int stackNumber);
#endif
