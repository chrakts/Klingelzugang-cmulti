#ifndef UARTHARDWARE_H_INCLUDED
#define UARTHARDWARE_H_INCLUDED

#define EOL "\n\r"

#define USART_RXCINTLVL_0 USART_RXCINTLVL_LO_gc
#define USART_TXCINTLVL_0 USART_TXCINTLVL_LO_gc

#define  USE_RS485_0   // USB-RS232
#define  USE_RS485_1   // unbenutzt

#define USE_RS485_FEEDBACK_0
#undef USE_RS485_FEEDBACK_1

#define SERIAL_0		USARTC0
#define SERIAL_PORT_0	PORTC
#define SERIAL_TE_PIN_0 PIN0_bm
#define SERIAL_RE_PIN_0 PIN1_bm
#define SERIAL_TX_PIN_0 PIN3_bm
#define SERIAL_RX_PIN_0 PIN2_bm
#define T_COMPLETE_INT_0	USARTC0_TXC_vect
#define R_COMPLETE_INT_0	USARTC0_RXC_vect

#ifdef USE_RS485_0
	#define TE_ENABLE_0		((PORT_t *) &SERIAL_PORT_0)->OUTSET = SERIAL_TE_PIN_0
	#define TE_DISABLE_0	((PORT_t *) &SERIAL_PORT_0)->OUTCLR = SERIAL_TE_PIN_0
	#define RE_DISABLE_0	((PORT_t *) &SERIAL_PORT_0)->OUTSET = SERIAL_RE_PIN_0
	#define RE_ENABLE_0		((PORT_t *) &SERIAL_PORT_0)->OUTCLR = SERIAL_RE_PIN_0
#else
	#define TE_ENABLE_0
	#define TE_DISABLE_0
	#define RE_DISABLE_0
	#define RE_ENABLE_0
#endif

#define USART_RXCINTLVL_1 USART_RXCINTLVL_LO_gc
#define USART_TXCINTLVL_1 USART_TXCINTLVL_LO_gc


#define SERIAL_1		USARTC1
#define SERIAL_PORT_1	PORTC
#define SERIAL_TE_PIN_1 PIN0_bm
#define SERIAL_RE_PIN_1 PIN1_bm
#define T_COMPLETE_INT_1	USARTD0_TXC_vect // USARTD1_TXC_vect ##########
#define R_COMPLETE_INT_1	USARTD0_RXC_vect

#ifdef USE_RS485_1
	#define TE_ENABLE_1		((PORT_t *) &SERIAL_PORT_1)->OUTSET = SERIAL_TE_PIN_1
	#define TE_DISABLE_1	((PORT_t *) &SERIAL_PORT_1)->OUTCLR = SERIAL_TE_PIN_1
	#define RE_DISABLE_1	((PORT_t *) &SERIAL_PORT_1)->OUTSET = SERIAL_RE_PIN_1
	#define RE_ENABLE_1		((PORT_t *) &SERIAL_PORT_1)->OUTCLR = SERIAL_RE_PIN_1
#else
	#define TE_ENABLE_1
	#define TE_DISABLE_1
	#define RE_DISABLE_1
	#define RE_ENABLE_1
#endif


#define Busy_Control_Pin     PIN2_bm
#define Busy_Control_PinCtrl PIN2CTRL
#define Busy_Control_Port    PORTC
#define Busy_Control_IntVec  PORTC_INT0_vect


#define UART0_RING_BUFFER_SIZE 50
#define UART1_RING_BUFFER_SIZE 50

extern volatile uint8_t UART0_ring_received;
extern volatile  char UART0_ring_buffer[UART0_RING_BUFFER_SIZE];
extern volatile uint8_t UART1_ring_received;
extern volatile  char UART1_ring_buffer[UART1_RING_BUFFER_SIZE];

#endif // UARTHARDWARE_H_INCLUDED
