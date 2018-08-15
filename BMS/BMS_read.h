
#include <stdint.h>
#include <LTC68041.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <BMS_config.h>
//**********Read and Parse Data********************
#ifndef BMS_READ
#define BMS_READ

//Constants
#define DIRECT_PASS 0

#define SOC_CHANNEL 1
#define ITMP_CHANNEL 2
#define ANALOG_VOLTAGE_CHANNEL 3
#define DIGITAL_VOLTAGE_CHANNEL 4
#define UNDERVOLTAGE_CHANNEL 5
#define OVERVOLTAGE_CHANNEL 6
#define MUXFAIL_CHANNEL 7
#define THSD_CHANNEL 8


#define GPIO_STATUS_CHANNEL 1
#define REFON_CHANNEL 2
#define SWTRD_CHANNEL 3
#define ADCOPT_CHANNEL 4
#define DISCHARGE_CHANNEL 7
#define DCTO_CHANNEL 8

// Declare Variables

extern uint16_t rawCellReading[TOTAL_STACK][12];
extern uint16_t rawTempReading[TOTAL_STACK][6];
extern uint16_t rawStatReading[TOTAL_STACK][12];
extern uint8_t rawConfigReading[TOTAL_STACK][CONFIG_IN_STACK];

extern int cellReading[TOTAL_STACK][CELLS_IN_STACK];
extern int tempReading[TOTAL_STACK][GPIOS_IN_STACK];
extern int configReading[TOTAL_STACK][CONFIG_IN_STACK];
extern int dieReading[TOTAL_STACK];

extern int analogPowerSupplyReading[TOTAL_STACK];
extern int digitalPowerSupplyReading[TOTAL_STACK];

extern int underVoltageStatus[TOTAL_STACK];
extern int overVoltageStatus[TOTAL_STACK];

extern int muxFail[TOTAL_STACK];
extern int thermalShutdown[TOTAL_STACK];

extern int DischargeTimeOut[TOTAL_STACK];
extern int DischargeChannel[TOTAL_STACK][12];


// Functions

int readCellValue(uint16_t *rawCellReadingptr[CELLS_IN_STACK]);
int readTempValue(uint16_t *rawTempReadingptr[GPIOS_IN_STACK]);
int readStat(uint16_t *rawStatReadingptr[STATUS_IN_STACK]);
int readConfig(uint8_t *rawConfigReadingptr);

int checkUnderVoltageStatus(int *underVoltageStatusptr);
int checkOverVoltageStatus(int *underVoltageStatusptr);

int checkMuxFail(int *muxFailptr);
int checkThermalShutdown(int *thermalShutdownptr);

int readAnalogPower(int *analogPowerSupplyReadingptr);
int readDigitalPower(int *digitalPowerSupplyReadingptr);

int readInternalTemp(int *dieReadingptr);

int filterCellValue(int *rawCellReadingptr, int *cellReadingptr, int filter_mode);
int filterTempValue(int *rawTempReadingptr, int *tempReadingptr, int filter_mode);
int filterConfig(int *rawConfigReadingptr[6], int *configReadingptr[6]);

int readBalancingSetup(int *DischargeChannelptr);
int readBalancingTime(int *DischargeTimeptr);

#endif

