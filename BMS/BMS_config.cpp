#include <BMS_config.h>
#include <stdint.h>
#include <LTC68041.h>
#include <LT_SPI.h>

//Set Under Voltage Threshold for Each IC
int setUnderVoltage(uint8_t (*CFGR1)[6], int thresholdVoltage, int stackNumber)
{
	uint16_t raw_threshold = thresholdVoltage*(1000000/(100*16))-1;
	uint16_t raw_threshold_final = (raw_threshold>>4) & (0x0FFF);
	(*(CFGR1+stackNumber))[1] = (uint8_t)(raw_threshold_final>>0) & (0x00FF);
	uint8_t initial_config = (*(CFGR1+stackNumber))[2] & 0x00F0;
	uint8_t final_uv_config = (raw_threshold_final>>8) & (0x000F);
	(*(CFGR1+stackNumber))[2] = (uint8_t)initial_config + final_uv_config;
		
}
//Set Over Voltage Threshold for Each IC
int setOverVoltage(uint8_t (*CFGR23)[6], int thresholdVoltage, int stackNumber)
{
	uint16_t raw_threshold = thresholdVoltage*(1000000/(100*16));
	uint16_t raw_threshold_final = (raw_threshold>>4) & (0x0FFF);
	(*(CFGR23+stackNumber))[2] = (uint8_t)(raw_threshold_final>>0) & (0x000F);
	(*(CFGR23+stackNumber))[3] = (uint8_t)(raw_threshold_final>>4) & (0x00FF);
	
}
//Toggle REFON bit
int ToggleReferencePower(uint8_t (*CFGR0)[6], int stackNumber, bool state)
{
	(*(CFGR0+stackNumber))[0] = (*(CFGR0+stackNumber))[0]^(1<<2);
	
}

//Toggle ADCOPT bit
int ToggleADCMode(uint8_t (*CFGR0)[6], int stackNumber,bool state)
{
	(*(CFGR0+stackNumber))[0] = (*(CFGR0+stackNumber))[0]^(1<<0);

}
//Set GPIO State
int setGPIOPins(uint8_t (*CFGR0)[6], int stackNumber, uint8_t GPIO, bool state)
{
	uint8_t initial_config = (*(CFGR0+stackNumber))[0] & 0x0007;
	uint8_t final_gpio_config = GPIO << 3;
	(*(CFGR0+stackNumber))[0] = (uint8_t)initial_config + final_gpio_config;
	
}
//Set Discharge Pattern
int setDischargeCell(uint8_t (*CFGR45)[6], uint16_t dischargePins, int stackNumber)
{
	uint16_t dischargeCode = (dischargePins & 0x0FFF);
	
	(*(CFGR45+stackNumber))[4] = (dischargeCode>>0) & 0x00FF;
	uint8_t initial_config = ((*(CFGR45+stackNumber)))[5] & 0xFFF0;
	uint8_t final_discharge_config = (dischargeCode>>8) & 0x003;
	(*(CFGR45+stackNumber))[5] = initial_config+final_discharge_config;	

}
//Set Discharge Timeout
int setDischargeTime(uint8_t (*CFGR5)[6], uint8_t dischargeMode, int stackNumber)
{
	uint8_t initial_config = ((*(CFGR5+stackNumber))[5]) & 0x000F;
	uint8_t final_discharge_time_config = ((dischargeMode>>0) & 0x000F)<<4;
	(*(CFGR5+stackNumber))[5] = initial_config+final_discharge_time_config;
}

int buildConfiguration(uint8_t *final_CFGR[6], uint8_t tx_cfg[TOTAL_STACK][6])
{
	for (int stackNumber = 0; stackNumber<8; stackNumber++)
	{
		for (int cfgrNumber = 0; cfgrNumber<6;cfgrNumber++)
		{
			tx_cfg[stackNumber][cfgrNumber] = (*(final_CFGR+stackNumber)[cfgrNumber]);			
		}
	}
	LTC6804_wrcfg(TOTAL_STACK,tx_cfg);
}
