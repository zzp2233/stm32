#ifndef __USART_H__
#define __USART_H__


struct usart_packet_t
{
	uint8_t rx_buf[64];
	uint8_t rx_len;
};

extern void uart_init(uint32_t baud);

#endif


