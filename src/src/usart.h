#ifndef __USART_H
#define __USART_H

#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <unistd.h>

#include <libopencm3/stm32/usart.h>

int _write(int file, char *ptr, int len);

#endif