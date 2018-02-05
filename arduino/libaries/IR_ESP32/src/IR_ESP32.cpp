/*
 *  IR_ESP32.cpp
 *
 *  Created on: 20 de nov de 2017
 *      Author: Ewerton L. de Sousa
 */

#include "IR_ESP32.h"
#include "esp32-hal-rmt.h" //Biblioteca escrita em ESP-IDF
#include "Arduino.h"
 
extern "C" {
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>	
}

//Configurações temporarias (send)
#define DATA_ITEM_NUM   34  /*!< NEC code item number: header + 32bit data + end */
#define RMT_TX_DATA_NUM  100    /*!< NEC tx test data number */

void IR_ESP32Class::begin(int pinTX, int pinRX){
	
	RMTbegin(pinTX, pinRX);
	
}

/*
*
*
*
*/
void IR_ESP32Class::sendRaw(rmt_item32_t* data, int numBits){
	
	RMTsendRaw(data, numBits);
	
}

/*
*
*
*
*/
void IR_ESP32Class::sendNEC(rmt_item32_t* data, int numBits){
	//Fazer as configurações enviar pelo protocolo NEC
	RMTsendNEC(data, numBits);
	
}

/*
*
*
*
*/
void IR_ESP32Class::sendSony(rmt_item32_t* data, int numBits){
	//Fazer as configurações enviar pelo protocolo Sony
	RMTsendSony(data, numBits);
	
}

 void IR_ESP32Class::receive(rmt_item32_t* data){
					
    RMTreceive(data);
			
}



IR_ESP32Class IR_ESP32;
