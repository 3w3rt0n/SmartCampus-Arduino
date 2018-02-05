/*
 *  IR_ESP32.h
 *
 *  Created on: 20 de nov de 2017
 *      Author: Ewerton L. de Sousa
 */

#ifndef IR_ESP32_H
#define IR_ESP32_H

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include "driver/rmt.h"
#include "driver/periph_ctrl.h"
#include "soc/rmt_reg.h"

#include "esp32-hal-rmt.h"


class IR_ESP32Class
{
	public: 
		void begin(int pinTX, int pinRX);

		void sendRaw(rmt_item32_t* data, int numBits);
		void sendNEC(rmt_item32_t* data, int numBits);
		void sendSony(rmt_item32_t* data, int numBits);

		void receive(rmt_item32_t* data);
	
};

extern IR_ESP32Class IR_ESP32;

#endif 
