/*
 * RMT.c
 *
 *  Created on: 20 de nov de 2017
 *      Author: Ewerton L. de Sousa
 */

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include "driver/rmt.h"
#include "driver/periph_ctrl.h"
#include "soc/rmt_reg.h"
#include "esp32-hal-rmt.h"
/*
 * @brief RMT transmitter initialization
 */
void RMTbegin(int pinTX, int pinRX)
{
    rmt_config_t rmt_tx;
    rmt_tx.rmt_mode = RMT_MODE_TX;
    rmt_tx.channel = 0; //canal zero
    rmt_tx.gpio_num = pinTX;
    rmt_tx.mem_block_num = 1; //1 block = 62 entradas de 32bits
    rmt_tx.clk_div = 80;   // divisor = 100 => 80.000.000 / 100 = 800k => 1.25 msec
    rmt_tx.tx_config.loop_en = 0; //false
    rmt_tx.tx_config.carrier_duty_percent = 33;
    rmt_tx.tx_config.carrier_freq_hz = 37900; //38000
    rmt_tx.tx_config.carrier_level = 1;
    rmt_tx.tx_config.carrier_en = 1;
    rmt_config(&rmt_tx);
    rmt_driver_install(rmt_tx.channel, 0, 0);
	
	rmt_config_t rmt_rx;
    rmt_rx.channel = 1;
    rmt_rx.gpio_num = pinRX;
    rmt_rx.clk_div = 80;
    rmt_rx.mem_block_num = 1; // 1 - original
    rmt_rx.rmt_mode = RMT_MODE_RX;
    rmt_rx.rx_config.filter_en = true;
    rmt_rx.rx_config.filter_ticks_thresh = 100;
    rmt_rx.rx_config.idle_threshold = 9000;
    rmt_config(&rmt_rx);
    rmt_driver_install(rmt_rx.channel, 1000, 0);
}

void imprimir_ITEM(rmt_item32_t* item, int num_bits){

	rmt_item32_t* item_temp = item;
	printf("       [HIGH]   [LOW] :%p\n", item);
	for(int i=0; i < num_bits; i++){
		printf( "[%2d] - %4dus | ", i, item_temp->duration0);
		printf( "%4dus \n", item_temp->duration1);
		item_temp++;
	}
}

void RMTsendRaw(rmt_item32_t* item, int num_bits){
	rmt_write_items(0, item, num_bits, true);
	rmt_wait_tx_done(0, portMAX_DELAY);
}


//Não implementado
void RMTsendNEC(rmt_item32_t* data, int num_bits){
	//Não implementado
}

//Não implementado
void RMTsendSony(rmt_item32_t* data, int num_bits){
	//Não implementado
}

//Não implementado
void RMTreceive(rmt_item32_t* data){
	//Não implementado
}
