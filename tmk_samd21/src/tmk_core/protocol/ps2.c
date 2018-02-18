/*
* ps2.c
*
* Created: 14/02/2018 8:30:15 PM
*  Author: Cong
*/

#include <asf.h>

#include "ps2.h"
#include "io_pins.h"

static bool isDataAvailable = false;
static int counter = 0;
static volatile uint8_t bitcount = 0;
static volatile uint8_t incoming = 0;
static volatile PS2Data_t ps2MouseData = {0,0,0};
static uint8_t val, n;

ps2_driver_t *driver;

//Get incoming data bit during interrupt caused by CLOCK_LINE change
uint8_t PS2_getBit(void) {
	if (!driver) return 0;
	isDataAvailable = false;

	val = (*driver->read_line)(DATA_LINE) ? 1 : 0;
	n = bitcount-1;
	if(n <= 7) {
		incoming |= (val << n);
	}
	bitcount++;

	if(bitcount == 11) {
		switch(counter) {
			case 0:
			ps2MouseData.state = incoming;
			counter++;
			break;
			case 1:
			ps2MouseData.x = incoming;
			counter++;
			break;
			case 2:
			ps2MouseData.y = incoming;
			counter = 0;
			isDataAvailable = 1;
			break;
		}
		bitcount = 0;
		incoming = 0;
	}
	return val;
}

bool PS2_dataAvailable(void) {
	return isDataAvailable;
}

PS2Data_t PS2_getStreamPackets(void) {
	return ps2MouseData;
}

void PS2_initialize(ps2_driver_t *d) {
	driver = d;
	
	//printf("Initializing...\r\n");
	(*driver->set_line)(CLOCK_LINE, HIGH);
	(*driver->set_line)(DATA_LINE, HIGH);
	//printf("Clock, data idle\r\n");
}

void PS2_transmit(uint8_t data) {
	//	printf("Transmitting: %x\r\n",data);
	//	uint8_t data_bak = data;	//only used when printf in this function is uncommented

	if (!driver) return;
	uint8_t parity = 1;
	uint8_t i;

	(*driver->set_line)(DATA_LINE, HIGH);
	(*driver->set_line)(CLOCK_LINE, HIGH);
	delay_us(300);
	(*driver->set_line)(CLOCK_LINE, LOW);
	delay_us(300);
	(*driver->set_line)(DATA_LINE, LOW);
	delay_us(10);
	(*driver->set_line)(CLOCK_LINE, HIGH);

	while((*driver->read_line)(CLOCK_LINE) == HIGH);

	for(i = 0; i < 8; i++) {
		if(data & 0x01) {
			(*driver->set_line)(DATA_LINE, HIGH);
			} else {
			(*driver->set_line)(DATA_LINE, LOW);
		}

		while((*driver->read_line)(CLOCK_LINE) == LOW);
		while((*driver->read_line)(CLOCK_LINE) == HIGH);
		parity ^= (data & 0x01);
		data >>= 1;
	}

	if(parity) {
		(*driver->set_line)(DATA_LINE, HIGH);
		} else {
		(*driver->set_line)(DATA_LINE, LOW);
	}

	while((*driver->read_line)(CLOCK_LINE) == LOW);
	while((*driver->read_line)(CLOCK_LINE) == HIGH);
	(*driver->set_line)(DATA_LINE, HIGH);
	delay_us(50);
	while((*driver->read_line)(CLOCK_LINE) == HIGH);
	while((*driver->read_line)(CLOCK_LINE) == LOW || (*driver->read_line)(DATA_LINE) == LOW);

	(*driver->set_line)(CLOCK_LINE, LOW);
	//	printf("Host: %x\r\n",data_bak);
}

uint8_t PS2_receive(void) {
	if (!driver) return 0;
	
	//printf("Receiving...\r\n");
	uint8_t data = 0;
	uint8_t i;
	uint8_t bit = 0x01;

	(*driver->set_line)(CLOCK_LINE, HIGH);
	(*driver->set_line)(DATA_LINE, HIGH);
	delay_us(50);
	while((*driver->read_line)(CLOCK_LINE) == HIGH);

	delay_us(5);
	while((*driver->read_line)(CLOCK_LINE) == LOW);

	for(i = 0; i < 8; i++) {
		while((*driver->read_line)(CLOCK_LINE) == HIGH);
		
		if ((*driver->read_line)(DATA_LINE)) {
			data |= bit;
		}
		while((*driver->read_line)(CLOCK_LINE) == LOW);
		bit <<= 1;
	}

	while((*driver->read_line)(CLOCK_LINE) == HIGH);
	while((*driver->read_line)(CLOCK_LINE) == LOW);
	while((*driver->read_line)(CLOCK_LINE) == HIGH);
	while((*driver->read_line)(CLOCK_LINE) == LOW);

	(*driver->set_line)(CLOCK_LINE, LOW);

	//printf("Mouse: %x\r\n",data);
	return data;
}

void PS2_setupClockLineForStream(void) {
	if (!driver) return;
	(*driver->enable_clock_interrupt)();
}

void PS2_revertClockLineStream(void) {
	if (!driver) return;
	(*driver->disable_clock_interrupt)();
	(*driver->set_line)(CLOCK_LINE, HIGH);
}

uint8_t PS2_setMode(uint8_t mode) {
	if (!driver) return 0;
	
	uint8_t success = 0;

	switch(mode) {
		case REMOTE_MODE:
		PS2_transmit(REMOTE_MODE);
		PS2_receive();
		break;
		case STREAM_MODE:
		//setup mouse
		PS2_transmit(STREAM_MODE);
		PS2_receive();
		PS2_transmit(ENABLE_REPORT);
		PS2_receive();

		(*driver->set_line)(CLOCK_LINE, HIGH);
		(*driver->set_line)(DATA_LINE, HIGH);
		break;
		case RESET_MODE:
		// set RESET high, wait 2.2us, set RESET low
		(*driver->toggle_reset_line)();
		//PS2_transmit(RESET_MODE);
		//PS2_receive();
		//PS2_receive();
		//PS2_receive();
		break;
	}

	return success;
}

PS2Data_t PS2_getRemotePackets(void) {
	PS2_transmit(READ_DATA);
	PS2_receive();
	ps2MouseData.state = PS2_receive();
	ps2MouseData.x = PS2_receive();
	ps2MouseData.y = PS2_receive();

	return ps2MouseData;
}

