#include <stdint.h>
#include <LTC68041.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <BMS_config.h>
#include <BMS_read.h>
//#include <atmega-timers.h>

int readCellValue(uint16_t (*rawCellReadingptr)[12])
{
	wakeup_sleep();
	LTC6804_adcv();
	_delay_ms(3);
	int8_t error = LTC6804_rdcv(0, TOTAL_STACK, rawCellReadingptr);
	if (error == -1)
		//TODO: Logic to retry
 	error = 1;
}
int readTempValue(uint16_t (*rawTempReadingptr)[6])
{
	wakeup_sleep();
	LTC6804_adax();
	_delay_ms(3);
	int8_t error = LTC6804_rdaux(0, TOTAL_STACK,rawTempReadingptr);
	if (error == -1)
	//TODO: Logic to retry
	error = 1;
}

int readStat(uint16_t (*rawStatReadingptr)[12])
{
	wakeup_sleep();
	LTC6804_adstat();
	_delay_ms(3);
	int8_t error = LTC6804_rdstat(0, TOTAL_STACK, rawStatReadingptr);
	
}

int readConfig(uint8_t (*rawConfigReadingptr)[CONFIG_IN_STACK])
{
	wakeup_sleep();
	int8_t error = LTC6804_rdcfg(TOTAL_STACK, rawConfigReadingptr);
	
}


int checkUnderVoltageStatus(int *underVoltageStatusptr)
{
	for (int stackNumber = 0; stackNumber<TOTAL_STACK; stackNumber++)
	{
		*(underVoltageStatusptr+stackNumber) = rawStatReading[stackNumber][UNDERVOLTAGE_CHANNEL];
	}
}

int checkOverVoltageStatus(int *overVoltageStatusptr)
{
	for (int stackNumber = 0; stackNumber<TOTAL_STACK; stackNumber++)
	{
		*(overVoltageStatusptr+stackNumber) = rawStatReading[stackNumber][OVERVOLTAGE_CHANNEL];
	}
}

int checkMuxFail(int *muxFailptr)
{
	for (int stackNumber = 0; stackNumber<TOTAL_STACK; stackNumber++)
	{
		*(muxFailptr+stackNumber) = rawStatReading[stackNumber][MUXFAIL_CHANNEL];		
	}
	
}
int checkThermalShutdown(int *thermalShutdownptr)
{
	for (int stackNumber = 0; stackNumber<TOTAL_STACK; stackNumber++)
	{	
		*(thermalShutdownptr+stackNumber) = rawStatReading[stackNumber][THSD_CHANNEL];
	}
}

int readAnalogPower(int *analogPowerSupplyReadingptr)
{
	for (int stackNumber = 0; stackNumber<TOTAL_STACK; stackNumber++)
	{
		*(analogPowerSupplyReadingptr+stackNumber) = rawStatReading[stackNumber][ANALOG_VOLTAGE_CHANNEL];
	}
}
int readDigitalPower(int *digitalPowerSupplyReadingptr)
{
	for (int stackNumber = 0; stackNumber<TOTAL_STACK; stackNumber++)
	{
		*(digitalPowerSupplyReadingptr+stackNumber) = rawStatReading[stackNumber][DIGITAL_VOLTAGE_CHANNEL];
	}
}

int readInternalTemp(int *dieReadingptr)
{
	for (int stackNumber = 0; stackNumber<TOTAL_STACK; stackNumber++)
	{
		*(dieReadingptr+stackNumber) = rawStatReading[stackNumber][ITMP_CHANNEL];
	}
}

int readBalancingSetup(int *dischargeChannelptr)
{
	for (int stackNumber = 0; stackNumber<TOTAL_STACK;stackNumber++)
	{
		*(dischargeChannelptr+stackNumber) = configReading[stackNumber][DISCHARGE_CHANNEL];
	}	
	
}

int readBalancingTime(int *DischargeTimeOutptr)
{
	for (int stackNumber = 0; stackNumber<TOTAL_STACK;stackNumber++)
	{
		*(DischargeTimeOutptr+stackNumber) = configReading[stackNumber][DCTO_CHANNEL];
	}
	
}


int filterCellValue(uint16_t *rawCellReadingptr, int *cellReadingptr, int filter_mode)
{
	if (filter_mode == DIRECT_PASS)
	{
		for (int i = 0; i < TOTAL_STACK; i++)
		{
			for (int j = 0; j< CELLS_IN_STACK; j++)
					*(cellReadingptr + i*CELLS_IN_STACK +j) = *(rawCellReadingptr + i*12 +j);
		}
	}
	//TODO: New Filters
}
int filterTempValue(uint16_t *rawTempReadingptr, int *tempReadingptr, int filter_mode)
{
	if (filter_mode == DIRECT_PASS)
	{
		for (int i = 0; i < TOTAL_STACK; i++)
		{
			for (int j = 0; j< GPIOS_IN_STACK; j++)
			*(tempReadingptr + i*GPIOS_IN_STACK +j) = *(rawTempReadingptr +i*6 +j);
		}
	}
}
	//TODO: New Filters


int filterConfig(uint16_t *rawConfigReadingptr[6], int *configReadingptr[6])
{
	for(int stackNumber = 0; stackNumber<TOTAL_STACK; stackNumber++)
	{
		(*(configReadingptr+stackNumber))[GPIO_STATUS_CHANNEL] = (((*(rawConfigReadingptr+stackNumber))[0]>>3) & 0x01FFF);
		(*(configReadingptr+stackNumber))[REFON_CHANNEL] = (((*(rawConfigReadingptr+stackNumber))[0] & 0x04)>>2);
		(*(configReadingptr+stackNumber))[SWTRD_CHANNEL] = (((*(rawConfigReadingptr+stackNumber))[0] & 0x02)>>1);
		(*(configReadingptr+stackNumber))[ADCOPT_CHANNEL] = (((*(rawConfigReadingptr+stackNumber))[0] & 0x01)>>0);
		(*(configReadingptr+stackNumber))[UNDERVOLTAGE_CHANNEL] = (((*(rawConfigReadingptr+stackNumber))[1])+((*(rawConfigReadingptr+stackNumber))[2] & 0x0F)<<8);
		(*(configReadingptr+stackNumber))[OVERVOLTAGE_CHANNEL] = (((*(rawConfigReadingptr+stackNumber))[2] & 0x0F)>>4)+(*(rawConfigReadingptr+stackNumber)[3]<<4);
		(*(configReadingptr+stackNumber))[DISCHARGE_CHANNEL] = (((*(rawConfigReadingptr+stackNumber))[4])+(*(rawConfigReadingptr+stackNumber)[5]<<8));
		(*(configReadingptr+stackNumber))[DCTO_CHANNEL] = (((*(rawConfigReadingptr+stackNumber))[5] & 0x0F)>>4);
		
	}
}

