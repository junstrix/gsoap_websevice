#ifndef __LPC_CONTROL__
#define __LPC_CONTROL__
extern void wm_op(void);
extern void con_led(void);
extern void read_adc(void);
extern void read_temp(void);
extern void read_spid(void);
extern void con_motor(void);
extern unsigned int motor_flags;
#endif

