//gsoap ns service name: ns
//gsoap ns service protocol: SOAP1.1
//gsoap ns service style: rpc
//gsoap ns service encoding: encoded
//gsoap ns service namespace:	http://localhost:8888
//gsoap ns service location:	http://localhost:8888
//gsoap ns service port:	http://localhost:8888

int ns__ledControl(int led_id, int led_stat, int *result); // Control LPC11C15 led throught uart
int ns__motorControl(int motor_id, int motor_op, int *result); // Control motor
int ns__spiReadId(int *result); // read spi id value
int ns__lm75ReadTemp(int *result); // read lm75 temperature
int ns__adcReadValue(int *result); // read adc value
