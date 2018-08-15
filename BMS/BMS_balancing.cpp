
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
	//TODO: Entire Balancing Procedure for Test
}
