/*
 * TMP_Manager.h
 *
 * Created: 05-Jun-21 2:31:41 PM
 *  Author: Refaey
 */ 


#ifndef TMP_MANAGER_H_
#define TMP_MANAGER_H_

#include "../Drivers/TC72.h"
#include "../Drivers/keypad_driver.h"
#include "Heater_Control.h"

void Update_CRT_Temperature();
void Update_SET_Temperature();
uint8_t check_off_key();

#endif /* TMP_MANAGER_H_ */