
#ifndef MAIN_RMT_H_
#define MAIN_RMT_H_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include "esp_err.h"
#include "esp_log.h"
#include "driver/rmt.h"
#include "driver/periph_ctrl.h"
#include "soc/rmt_reg.h"

void RMTbegin(int pinTX, int pinRX);
void RMTsendRaw(rmt_item32_t* item, int num_bits);

void imprimir_ITEM(rmt_item32_t* item, int num_bits);

//Não implementado
void RMTsendNEC(rmt_item32_t* data, int num_bits);
//Não implementado
void RMTsendSony(rmt_item32_t* data, int num_bits);
//Não implementado
void RMTreceive(rmt_item32_t* data);

#ifdef __cplusplus
}
#endif

#endif /* MAIN_RMT_H_ */
