
#include <ina219.h>
#include <i2c_master.h>
#include <stdio.h>
#include <config.h>
#include <util/delay.h>

uint8_t INA219_readCalibrationReg(){
	uint8_t cal[2];
	uint8_t error = i2c_read2ByteRegister(INA219_ADDRESS, INA219_REG_CALIBRATION, cal);
	printf("Calib reg: %u\n", ((cal[1] << 8) | cal[0]));
	if (error) return 1;
	else return 0;
}

uint8_t INA219_readConfigReg(){
	uint8_t cal[2];
	uint8_t error = i2c_read2ByteRegister(INA219_ADDRESS, INA219_REG_CONFIG, cal);
	printf("Config reg: %x\n", ((cal[1] << 8) | cal[0]));
	if (error) return 1;
	else return 0;
}



uint8_t INA219_setCalibration_16V_400mA() {
	uint8_t error = 0;
	uint16_t ina219_calValue = 27264;  // Current LSB = 15uA per bit (1000/50 = 90.909)     Power LSB = 1mW per bit

	error += i2c_write2ByteRegister(INA219_ADDRESS, INA219_REG_CALIBRATION, ina219_calValue);
	
	uint16_t config =	INA219_CONFIG_BVOLTAGERANGE_16V |
						INA219_CONFIG_GAIN_1_40MV |
						INA219_CONFIG_BADCRES_12BIT |
						INA219_CONFIG_SADCRES_12BIT_1S_532US |
						INA219_CONFIG_MODE_SANDBVOLT_TRIGGERED;
	error += i2c_write2ByteRegister(INA219_ADDRESS, INA219_REG_CONFIG, config);
	if (error) return 1;
	else return 0;
}
static uint8_t INA219_trigger_oneshow_conversion(void);
uint8_t INA219_trigger_oneshow_conversion(void){
	uint8_t error = 0;
	uint16_t config =	INA219_CONFIG_BVOLTAGERANGE_16V |
	INA219_CONFIG_GAIN_1_40MV |
	INA219_CONFIG_BADCRES_12BIT |
	INA219_CONFIG_SADCRES_12BIT_1S_532US |
	INA219_CONFIG_MODE_SANDBVOLT_TRIGGERED;
	error += i2c_write2ByteRegister(INA219_ADDRESS, INA219_REG_CONFIG, config);
	if (error) return 1;
	else return 0;
}

int16_t INA219_readBusVoltageReg(){
	uint8_t value[2];
	INA219_trigger_oneshow_conversion();
	_delay_us(600);
	if (i2c_read2ByteRegister(INA219_ADDRESS, INA219_REG_BUSVOLTAGE, value)) return 0;
	uint16_t total = ((value[1] << 8) | value[0])>>3;
	#ifdef INA219_debug
	printf("vbus: %u\n", total);
	#endif
	if (total > 0x1000) return 0x00;
	return total;
}

uint16_t INA219_readShuntVoltageReg(){ // Not operational
	uint8_t value[2];
	if (i2c_read2ByteRegister(INA219_ADDRESS, INA219_REG_SHUNTVOLTAGE, value)) return 1;
	return ((value[1] << 8) | value[0]);
}
uint16_t INA219_readCurrentReg(){
	uint8_t value[2];
	INA219_trigger_oneshow_conversion();
	_delay_us(600);
	if (i2c_read2ByteRegister(INA219_ADDRESS, INA219_REG_CURRENT, value)) return 0;
	uint16_t total = (value[1] << 8) | value[0];
	#ifdef INA219_debug
	printf("curr: %u\n", total);
	#endif
	if(total>0x8000) return 0x00;  // Remove possibility of corruption with a bug where total is above 0xFFAE with no connected source.
	return total;
}


uint8_t INA219_init(){
	if (INA219_setCalibration_16V_400mA()) {
		#ifdef DEBUG_M
		printf("Setting calibration failed...\n");
		#endif
		return 1;
	}
	else{
		#ifdef DEBUG_M
		INA219_readCalibrationReg();
		INA219_readConfigReg();
		#endif
		return 0;
	}
}