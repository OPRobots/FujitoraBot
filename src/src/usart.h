#ifndef __USART_H
#define __USART_H

#include <inttypes.h>
#include <libopencm3/stm32/usart.h>
#include <stdio.h>
#include <unistd.h>

int _write(int file, char *ptr, int len);

#endif