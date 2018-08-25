
#include <BMS_config.h>
#include <LTC68041.h>

int startBalancing(uint16_t *dischargeCode, int dischargePin, int stackNumber)
{
	*(dischargeCode) = *(dischargeCode)|(1<<(dischargePin+0x0001));
	setDischargeCell(CFGR,*(dischargeCode),stackNumber);
	return 0;
}
int stopBalancing(uint16_t *dischargeCode, int dischargePin, int stackNumber)
{
	*(dischargeCode) = *(dischargeCode) & (1<<(dischargePin+0x0001));
	setDischargeCell(CFGR,*(dischargeCode),stackNumber);
	return 0;
}
int holdBalancing(uint16_t *dischargeCode, int stackNumber)
{
	*(dischargeCode) = (*(dischargeCode) & 0x0000);
	setDischargeCell(CFGR,*(dischargeCode),stackNumber);
	return 0;
}
int verifyBalancing(uint16_t *dischargeCode, int stackNumber)
{
	return 0;
	//TODO: Entire Balancing Procedure for Test as explained in Datasheet
}


uint16_t balancingAlgorithm(int status, int stackNumber)
{
	uint16_t rawCellReading[TOTAL_STACK][TOTAL_CELL_REG];
	if (status == CHARGING)
	{
		readCellValue(rawCellReading);
	    cellValueAverageinStack = calculateStackAverage(rawCellReading[][TOTAL_CELL_REG], stackNumber);
		for (int i = 0; i < CELLS_IN_STACK; i++)
		{
			uint16_t decider = compareCell(rawCellReading[stackNumber][i], cellValueAverageinStack);
			if (decider == 1)
			startBalancing(&dischargeCode, i ,stackNumber);													//Confirm Logic Once
			
		}
	
	}	
}


uint16_t compareCell(uint16_t cellValue, uint16_t cellValueAverage) 
{
	if (cellValue > (cellValueAverage + DISCHARGE_THRESHOLD))	
	return 1;
	else
	return 0;
}