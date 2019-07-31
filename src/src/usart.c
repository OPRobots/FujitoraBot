#include "usart.h"

int _write(int file, char *ptr, int len)
{
	int i = 0;

	/*
	 * Write "len" of char from "ptr" to file id "file"
	 * Return number of char written.
	 *
	 * Only work for STDOUT, STDIN, and STDERR
	 */
	if (file > 2) {
		return -1;
	}
	while (*ptr && (i < len)) {
		usart_send_blocking(USART3, *ptr);
		// if (*ptr == '\n') {
		// 	usart_send_blocking(USART3, '\r');
		// }
		i++;
		ptr++;
	}
	return i;
}