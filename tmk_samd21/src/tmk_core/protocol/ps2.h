/*
* ps2.h
*
* Created: 14/02/2018 8:28:38 PM
*  Author: Cong
*/


#ifndef PS2_H_
#define PS2_H_

#include <asf.h>

#define	REMOTE_MODE		0xF0
#define	STREAM_MODE		0xEA
#define	RESET_MODE		0xFF

#define	READ_DATA		0xEB
#define	DISABLE_REPORT	0xF5
#define	SET_DEFAULTS	0xF6
#define	ENABLE_REPORT	0xF4
#define	SET_SAMPLE_RATE	0xF3
#define GET_ID			0xF2
#define STATUS_REQUEST	0xE9
#define	SET_RESOLUTION	0xE8
#define	SET_SCALING_2	0xE7
#define SET_SCALING_1	0xE6

typedef uint8_t line_t;

typedef struct {
	void (*set_line)(line_t, bool);
	bool (*read_line)(line_t);
	void (*enable_clock_interrupt)(void);
	void (*disable_clock_interrupt)(void);
	void (*toggle_reset_line)(void);
} ps2_driver_t;

typedef struct {
	uint8_t state;
	int8_t x;
	int8_t y;
} PS2Data_t;

enum lines {CLOCK_LINE, DATA_LINE};

void PS2_initialize(ps2_driver_t *d);
void PS2_transmit(uint8_t data);
uint8_t PS2_receive(void);
uint8_t PS2_getBit();
uint8_t PS2_setMode(uint8_t mode);

PS2Data_t PS2_getRemotePackets(void);

void PS2_setupClockLineForStream(void);
void PS2_revertClockLineStream(void);
bool PS2_dataAvailable(void);
PS2Data_t PS2_getStreamPackets(void);

#endif /* PS2_H_ */