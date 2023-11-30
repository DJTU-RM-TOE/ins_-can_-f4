#include "FlashStorage.h"
#include <string.h>

#define FLAH_STORAGE_CAL_ADDR 0x08010000
#define FLASH_STORAGE_PARAM_ADDR 0x08007800 //����ṹ��������Flash��30ҳ
#define FLASH_STORAGE_ID_ADDR 0x08007C00 //���ID����Flash��31ҳ
#define FLASH_STORAGE_AVAIL_FLAG 0xA5A5A5A5 //Flash������Ч��־

// ���������У׼����
void Flash_SaveMagCal(float magBias[3], float magScale[3])
{
	FLASH_EraseInitTypeDef erase;
    uint32_t error = 0;

	erase.Sector = FLASH_SECTOR_4;
    erase.TypeErase = FLASH_TYPEERASE_SECTORS;
    erase.VoltageRange = FLASH_VOLTAGE_RANGE_3;
	erase.NbSectors = 1;

    HAL_FLASH_Unlock();
	HAL_FLASHEx_Erase(&erase, &error);

    uint32_t buf[8];
    buf[0] = FLASH_STORAGE_AVAIL_FLAG;
	buf[1] = *((uint32_t*)magBias);
	buf[2] = *((uint32_t*)(magBias+1));
	buf[3] = *((uint32_t*)(magBias+2));
    buf[4] = *((uint32_t*)magScale);
	buf[5] = *((uint32_t*)(magScale+1));
	buf[6] = *((uint32_t*)(magScale+2));
    for(uint8_t i=0; i<sizeof(buf); i++){
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, FLAH_STORAGE_CAL_ADDR+i*4, buf[i]);
    }
	HAL_FLASH_Lock();
}

int Flash_ReadMagCal(float magBias[3], float magScale[3])
{
    uint32_t *buf = (uint32_t*)FLAH_STORAGE_CAL_ADDR;
	if(buf[0] != FLASH_STORAGE_AVAIL_FLAG)
		return -1;
	magBias[0] = *((float*)&buf[1]);
	magBias[1] = *((float*)&buf[2]);
	magBias[2] = *((float*)&buf[3]);
    magScale[0] = *((float*)&buf[4]);
	magScale[1] = *((float*)&buf[5]);
	magScale[2] = *((float*)&buf[6]);
    return 0;
}
