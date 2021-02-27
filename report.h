#ifndef REPORT_H_INCLUDED
#define REPORT_H_INCLUDED


#define XSTR(x) STR(x)
#define STR(x) #x

#pragma message "----------------------------------------------------"
#pragma message( "Taster-Interrupt: " STR(TASTER_INT_VEC) ": " XSTR(TASTER_INT_VEC))
#pragma message "----------------------------------------------------"
#pragma message( "UART0-Busy-Pin-Interrupt: " STR(Busy_Control_IntVec_0) ": " XSTR(Busy_Control_IntVec_0))
#pragma message( "UART0-Busy-Time-Interrupt: " STR(Busy_Control_TimVec_0) ": " XSTR(Busy_Control_TimVec_0))
#pragma message( "UART1-Busy-Pin-Interrupt: " STR(Busy_Control_IntVec_1) ": " XSTR(Busy_Control_IntVec_1))
#pragma message( "UART1-Busy-Time-Interrupt: " STR(Busy_Control_TimVec_1) ": " XSTR(Busy_Control_TimVec_1))
#pragma message "----------------------------------------------------"


#endif // REPORT_H_INCLUDED
