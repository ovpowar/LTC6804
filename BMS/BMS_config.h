#include <LTC68041.h>
#include <stdint.h>


//CONFIGURATION PARAMETERS
#ifndef BMS_CONFIG
#define BMS_CONFIG
//CONFIGURATION REGISTERS

#define TOTAL_STACK 8
#define CELLS_IN_STACK 10
#define GPIOS_IN_STACK 5
#define STATUS_IN_STACK 8
#define CONFIG_IN_STACK 8

#define TOTAL_STAT_REG 8
#define TOTAL_CELL_REG 12
#define TOTAL_AUX_REG 6
#define TOTAL_CONFIG_REG 8

extern uint8_t CFGR[TOTAL_STACK][6];



//Functions


int setUnderVoltage(uint8_t (*CFGR1)[6], int thresholdVoltage, int stackNumber);  //Set Under Voltage Threshold for Each IC


int setOverVoltage(uint8_t (*CFGR23)[6], int thresholdVoltage, int stackNumber);  //Set Over Voltage Threshold for Each IC


int ToggleReferencePower(uint8_t (*CFGR0)[6], int stackNumber, bool state);  //Set REFON bit


int ToggleADCMode(uint8_t (*CFGR0)[6], int stackNumber,bool state);  //Set ADCOPT bit


int setGPIOPins(uint8_t (*CFGR0)[6], int stackNumber, uint8_t GPIO, bool state);  //Set GPIO State


int setDischargeCell(uint8_t (*CFGR45)[6], uint16_t dischargePins, int stackNumber);  //Set Discharge Pattern


int setDischargeTime(uint8_t (*CFGR5)[6], uint8_t dischargeMode, int stackNumber);  //Set Discharge Timeout


int buildConfiguration(uint8_t (*final_CFGR)[6], uint8_t tx_cfg[TOTAL_STACK][6]);  //Configuration Builder

#endif
