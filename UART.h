#ifndef UART_H
#define UART_H

/*#define UART0_RING_BUFFER_SIZE 50*/
#define UART1_RING_BUFFER_SIZE 50
/*
#define SERIAL_0		USARTC0
#define SERIAL_PORT_0	PORTC
#define SERIAL_TE_PIN_0 RS485_0_TE_PIN
#define SERIAL_RE_PIN_0 RS485_0_RE_PIN
#define T_COMPLETE_INT_0	USARTC0_TXC_vect
#define R_COMPLETE_INT_0	USARTC0_RXC_vect
#define TE_ENABLE_0		((PORT_t *) &SERIAL_PORT_0)->OUTSET = SERIAL_TE_PIN_0
#define TE_DISABLE_0	((PORT_t *) &SERIAL_PORT_0)->OUTCLR = SERIAL_TE_PIN_0
#define RE_DISABLE_0	((PORT_t *) &SERIAL_PORT_0)->OUTSET = SERIAL_RE_PIN_0
#define RE_ENABLE_0		((PORT_t *) &SERIAL_PORT_0)->OUTCLR = SERIAL_RE_PIN_0

#define SERIAL_1		USARTD1
#define SERIAL_PORT_1	PORTD
#define SERIAL_TE_PIN_1 RS485_1_TERE_PIN
#define SERIAL_RE_PIN_1 RS485_1_TERE_PIN
#define T_COMPLETE_INT_1	USARTD1_TXC_vect
#define R_COMPLETE_INT_1	USARTD1_RXC_vect
#define TE_ENABLE_1		((PORT_t *) &SERIAL_PORT_1)->OUTSET = SERIAL_TE_PIN_1
#define TE_DISABLE_1	((PORT_t *) &SERIAL_PORT_1)->OUTCLR = SERIAL_TE_PIN_1
#define RE_DISABLE_1	((PORT_t *) &SERIAL_PORT_1)->OUTSET = SERIAL_RE_PIN_1
#define RE_ENABLE_1		((PORT_t *) &SERIAL_PORT_1)->OUTCLR = SERIAL_RE_PIN_1
*/
enum{CNET=0,KNET};

void uart_init_0();
void TransmitByte0( uint8_t data );
uint8_t ReceiveByte0(  void );

void uart_init_1();
void TransmitByte1( uint8_t data );
uint8_t ReceiveByte1(  void );

#endif /* UART_H */
