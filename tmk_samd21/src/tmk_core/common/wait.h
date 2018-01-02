#ifndef WAIT_H
#define WAIT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "asf.h"

#define wait_ms(ms) delay_ms(ms)
#define wait_us(us) delay_us(us)


#ifdef __cplusplus
}
#endif

#endif
