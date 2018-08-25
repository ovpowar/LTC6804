#include <stdint.h>
#include <LTC68041.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <BMS_config.h>
#include <BMS_read.h>
//#include <atmega-timers.h>

/*!
------------Direct Access Functions---------------

The following 4 functions directly interact with the LTC6804 Code to read
the status register.

----------------------------------------------------
*/ 

int readCellValue(uint16_t rawCellReadingptr[][TOTAL_CELL_REG])
{
	wakeup_sleep();
	LTC6804_adcv(); //ADC Conversion start
	_delay_ms(3); // Wait for IC to power up
	int8_t error = LTC6804_rdcv(0, TOTAL_STACK, rawCellReadingptr);	//Read Cell Values
	if (error == -1)
		//TODO: Logic to retry
 	error = (int) 1;
	 return error;
}


int readTempValue(uint16_t rawTempReadingptr[][TOTAL_AUX_REG])
{
	wakeup_sleep();
	LTC6804_adax();//ADC Conversion Start
	_delay_ms(3); // Wait for IC to power up and complete conversion
	int8_t error = LTC6804_rdaux(0, TOTAL_STACK,rawTempReadingptr); //Read Aux Values
	if (error == -1)
	//TODO: Logic to retry
	error =(int) 1;
	return error;
}

int readStat(uint16_t rawStatReadingptr[][TOTAL_STAT_REG])
{
	wakeup_sleep();
	LTC6804_adstat();//ADC Conversion Start
	_delay_ms(3); // Wait for IC to power up and complete conversion
	int8_t error = LTC6804_rdstat(0, TOTAL_STACK, rawStatReadingptr); //Read Status Values
	if (error == -1)
	//TODO: Logic to retry
	error =(int) 1;
	return error;
}

int readConfig(uint8_t rawConfigReadingptr[][TOTAL_CONFIG_REG])
{
	wakeup_sleep();
	int8_t error = LTC6804_rdcfg(TOTAL_STACK, rawConfigReadingptr);// Read Configuration Register Values
	if (error == -1)
	//TODO: Logic to retry
	error =(int) 1;
	return error;
}

/*!
Parsing Functions

The following functions only parse the value read in the status register into
different variables based on type of status value

*/ 


int checkUnderVoltageStatus(int *underVoltageStatusptr, uint16_t rawStatReading[][STATUS_IN_STACK])
{
	for (int stackNumber = 0; stackNumber<TOTAL_STACK; stackNumber++)
	{
		*(underVoltageStatusptr+stackNumber) = (int) *(rawStatReading + (stackNumber*STATUS_IN_STACK)+UNDERVOLTAGE_CHANNEL);
	}
}

int checkOverVoltageStatus(int *overVoltageStatusptr, uint16_t rawStatReading[][STATUS_IN_STACK])
{
	for (int stackNumber = 0; stackNumber<TOTAL_STACK; stackNumber++)
	{
		*(overVoltageStatusptr+stackNumber) = (int) *(rawStatReading+(stackNumber*STATUS_IN_STACK)+OVERVOLTAGE_CHANNEL);
	}
}

int checkMuxFail(int *muxFailptr, uint16_t rawStatReading[][STATUS_IN_STACK])
{
	for (int stackNumber = 0; stackNumber<TOTAL_STACK; stackNumber++)
	{
		*(muxFailptr+stackNumber) = (int) *(rawStatReading+(stackNumber*STATUS_IN_STACK)+MUXFAIL_CHANNEL);		
	}
	
}
int checkThermalShutdown(int *thermalShutdownptr, uint16_t rawStatReading[][STATUS_IN_STACK])
{
	for (int stackNumber = 0; stackNumber<TOTAL_STACK; stackNumber++)
	{	
		*(thermalShutdownptr+stackNumber) = (int) *(rawStatReading+(stackNumber*STATUS_IN_STACK)+THSD_CHANNEL);
	}
}

int readAnalogPower(int *analogPowerSupplyReadingptr, uint16_t rawStatReading[][STATUS_IN_STACK])
{
	for (int stackNumber = 0; stackNumber<TOTAL_STACK; stackNumber++)
	{
		*(analogPowerSupplyReadingptr+stackNumber) = (int) *(rawStatReading+(stackNumber*STATUS_IN_STACK) + ANALOG_VOLTAGE_CHANNEL);
	}
}
int readDigitalPower(int *digitalPowerSupplyReadingptr)
{
	for (int stackNumber = 0; stackNumber<TOTAL_STACK; stackNumber++)
	{
		*(digitalPowerSupplyReadingptr+stackNumber) = (int) *(rawStatReading+(stackNumber*STATUS_IN_STACK) + DIGITAL_VOLTAGE_CHANNEL);
	}
}

int readSumVoltage(int *socReadingptr, uint16_t rawStatReading[][STATUS_IN_STACK])
{
	for (int stackNumber = 0; stackNumber<TOTAL_STACK; stackNumber++)
	{
		*(socReadingptr+stackNumber) = (int) *(rawStatReading+(stackNumber*STATUS_IN_STACK) + SOC_CHANNEL);
	}
}


int readInternalTemp(int *dieReadingptr, uint16_t rawStatReading[][STATUS_IN_STACK])
{
	for (int stackNumber = 0; stackNumber<TOTAL_STACK; stackNumber++)
	{
		*(dieReadingptr+stackNumber) = (int) *(rawStatReading+(stackNumber*STATUS_IN_STACK) + ITMP_CHANNEL);
	}
}

int readBalancingSetup(int *dischargeChannelptr, uint16_t ConfigReading[][CONFIG_IN_STACK])
{
	for (int stackNumber = 0; stackNumber<TOTAL_STACK;stackNumber++)
	{
		*(dischargeChannelptr+stackNumber) =(int) *(configReading+(stackNumber*CONFIG_IN_STACK)+DISCHARGE_CHANNEL);
	}	
	
}

int readBalancingTime(int *DischargeTimeOutptr, uint16_t configReading[][CONFIG_IN_STACK])
{
	for (int stackNumber = 0; stackNumber<TOTAL_STACK;stackNumber++)
	{
		*(DischargeTimeOutptr+stackNumber) =(int) *(configReading+(stackNumber*CONFIG_IN_STACK) + DCTO_CHANNEL);
	}
	
}

/*!

Filter Functions

Following Functions filter the raw cell, temperature and cofiguration reading

*/
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

/*!

Math Functions

These Functions perform various Math operations on the obtained data

*/

uint16_t calculateStackAverage(uint16_t rawCellReadingptr[][TOTAL_CELL_REG],int  stackNumber)
{
	int sum = 0;
	
	for (int i=0; i<CELLS_IN_STACK;i++)
	{
		sum = sum + rawCellReadingptr[stackNumber][i];
	}
	uint16_t averageCellValue = sum/CELLS_IN_STACK;
	return averageCellValue;
}


uint16_t calculatePackAverage(uint16_t rawCellReadingptr[][TOTAL_CELL_REG], int socReadingptr[TOTAL_STACK], int mode)
{
		
	int sum = 0;
	for (int i = 0; i<TOTAL_STACK; i++)
	{	
		if (mode == 0)													//Mode 0: Sum of individual cells
		{																//Function is slow but accurate
																		//Might fail if open cell value is read. Take care while calling function
			for (int j=0; j<CELLS_IN_STACK;j++)
			{
				sum = sum + rawCellReadingptr[i][j];		
			}
		}
		else if (mode == 1)												//Mode 1: Sum of individual stacks
		{																//Function is fast but can be inaccurate
			sum = sum + socReadingptr[i];								//Will fail if total value is wrong. Take care while calling function 
		}
		else
		return -1;
	}
	
	uint16_t averageCellValue;
	if (mode == 0)
		averageCellValue = sum/(TOTAL_STACK*CELLS_IN_STACK);
	else if (mode == 1)
		averageCellValue = sum/(TOTAL_STACK);
	
	return averageCellValue;
}
