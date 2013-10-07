#include "soapH.h"
#include "ns.nsmap"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/fs.h>
#include <errno.h>
#include <string.h>
#include <termios.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>
#include <syslog.h>

#define handle_error(msg)\
	do {perror(msg); exit(EXIT_FAILURE);} while(0)
#define RECBUFSIZE 20
#define TRUE 1
#define FALSE 0

#define BACKLOG (100)	// Max. request backlog
#define MAX_THR (10)	// Max. threads to serve requests
int uart_fd = 0;
int overtime_flag = FALSE;
int control_flag = FALSE;
int read_flag = FALSE;
int receive_flag = FALSE;
int receive_len = 0;
char rec_data[RECBUFSIZE];

int http_get(struct soap * soap);
void receive_data(void);
int openSerialPort(char *devName);

int openSerialPort(char *devName)
{
	int fd;
	fd = open(devName, O_RDWR | O_NOCTTY);
	if (fd < 0) {
		handle_error("open");
	}
	struct termios serialAttr;//声明一个串口结构体
	memset(&serialAttr, 0, sizeof(serialAttr));//初始化serialAttr,相当于bzero();
	serialAttr.c_oflag = 0;
	serialAttr.c_iflag = IGNPAR;
	serialAttr.c_cflag = B9600 | HUPCL | CS8 | CREAD | CLOCAL;
	serialAttr.c_lflag = 0; //non-canonical
	serialAttr.c_cc[VMIN] = 0;
	serialAttr.c_cc[VTIME] = 10; // used inter-character timer  VTIME*0.1s
	if (tcsetattr(fd, TCSANOW, &serialAttr) != 0) {
		handle_error("tcsetattr");
	}
	return fd;
}

char *LedSendBufOn = NULL;
char *LedSendBufOff = NULL;
char *SendBuf=NULL;
int ns__ledControl(struct soap *soap, int led_id, int led_stat, int *result)
{
	switch (led_id) { // which led will be control
		case 1 :
			LedSendBufOn = "SL10000#\0";
			LedSendBufOff = "SL11000#\0";
			break;
		case 2 :
			LedSendBufOn = "SL20000#\0";
			LedSendBufOff = "SL21000#\0";
			break;
		case 3 :
			LedSendBufOn = "SL30000#\0";
			LedSendBufOff = "SL31000#\0";
			break;
		case 4 :
			LedSendBufOn = "SL40000#\0";
			LedSendBufOff = "SL41000#\0";
			break;
		case 5 :
			LedSendBufOn = "SL50000#\0";
			LedSendBufOff = "SL51000#\0";
			break;
		case 6 :
			LedSendBufOn = "SL60000#\0";
			LedSendBufOff = "SL61000#\0";
			break;
		case 7 :
			LedSendBufOn = "SL70000#\0";
			LedSendBufOff = "SL71000#\0";
			break;
		case 8 :
			LedSendBufOn = "SL80000#\0";
			LedSendBufOff = "SL81000#\0";
			break;
		case 9 :
			LedSendBufOn = "SL90000#\0";
			LedSendBufOff = "SL91000#\0";
			break;
		default : break;
	}
	if (led_stat == 1)
	{
		printf("The %d led will be Turn on.\n", led_id);
		SendBuf = LedSendBufOn;
	}
	else {
		printf("The %d led will be Turn off.\n", led_id);
		SendBuf = LedSendBufOff;
	}
	printf("write to uart complete  size = %d\n", write(uart_fd, SendBuf, 8));
	if(!receive_flag) {
		memset(rec_data, 0, strlen(rec_data));
	}
	while(!control_flag) { // receive_data
		char *temp;
		if ((read(uart_fd, temp, 1)) == 0) {
			printf("Time over\n");
			overtime_flag = TRUE;
			break;
		}
		if((*temp == 'S') && !receive_flag) {
			receive_flag = TRUE;
		}
		if(receive_flag) {
			rec_data[receive_len++] = *temp;
			if((*temp == '#') && (receive_len > 1)) {
				receive_flag = FALSE;
				if(receive_len == 8) {
					control_flag = TRUE;
				}
			}
		}
	}
	if (overtime_flag)
	{
		*result = -1;
		overtime_flag = FALSE;
	}
	else {
		*result = (char)rec_data[2]-48; // return back
	}
	receive_len = 0;
	control_flag = FALSE;
	return 0;
}
int ns__motorControl(struct soap *soap, int motor_id, int motor_op, int *result) // Control motor
{
	const char MotorSendBufZheng[9]="SM11000#\0";
	const char MotorSendBufFan[9]="SM12000#\0";
	const char MotorSendBufStop[9]="SM13000#\0";
	const char *MotorSendBuf=NULL;
	if (motor_op == 1)
	{
		MotorSendBuf = MotorSendBufZheng;
	}
	else if (motor_op == 2)
	{
		MotorSendBuf = MotorSendBufFan;
	}
	else {
		MotorSendBuf = MotorSendBufStop;
	}
	printf("write to uart complete  size = %d\n", write(uart_fd, MotorSendBuf, 8));
	receive_len = 0;
	control_flag = FALSE;
	return 0;
}
/*int ns__spiReadId(struct soap *soap, int spi_op, char *idWriteBuf, int *result) // TODO read/write spi id value*/
int ns__spiReadId(struct soap *soap, int *result) // read/write spi id value
{
	const char SpiReadIdBuf[9]="SS10000#\0";
	printf("write to uart complete  size = %d\n", write(uart_fd, SpiReadIdBuf, 8));
	if(!receive_flag) {
		memset(rec_data, 0, strlen(rec_data));
	}
	while(!control_flag) { // receive_data
		char *temp;
		if ((read(uart_fd, temp, 1)) == 0) {
			printf("Time over\n");
			overtime_flag = TRUE;
			break;
		}
		if((*temp == 'S') && !receive_flag) {
			receive_flag = TRUE;
		}
		if(receive_flag) { // SS1 1999xxxxxxx
			rec_data[receive_len++] = *temp;
			if((*temp == '#') && (receive_len > 1)) {
				receive_flag = FALSE;
				if(receive_len == 13) { // Sxxx...#     3 + 10
					control_flag = TRUE;
				}
			}
		}
	}
	printf("%s\n", rec_data);
	if(overtime_flag) {
		*result = -1;
		overtime_flag = FALSE;
	}
	//TODO receive SPI ID value and calculation it !!!
	/**result = (char)rec_data[3]-48; // return back*/
	else {
		*result = atoi(&rec_data[1]); // spi id : S10001130718
	}
	receive_len = 0;
	control_flag = FALSE;
	return 0;
}
int ns__lm75ReadTemp(struct soap *soap, int *result) // read lm75 temperature
{
	const char Lm75TempBuf[9]="ST10000#\0";
	printf("write to uart complete  size = %d\n", write(uart_fd, Lm75TempBuf, 8));
	/*timerOption(STARTTIMER); // start timer*/
	if(!receive_flag) {
		memset(rec_data, 0, strlen(rec_data));
	}
	while(!control_flag) { // receive_data
		char *temp;
		if ((read(uart_fd, temp, 1)) == 0) {
			printf("Time over\n");
			overtime_flag = TRUE;
			break;
		}
		if((*temp == 'S') && !receive_flag) {
			receive_flag = TRUE;
		}
		if(receive_flag) {
			rec_data[receive_len++] = *temp;
			if((*temp == '#') && (receive_len > 1)) {
				receive_flag = FALSE;
				if(receive_len == 8) {
					control_flag = TRUE;
				}
			}
		}
	}
	printf("%s\n", rec_data);
	if(overtime_flag) {
		*result = -1;
		overtime_flag = FALSE;
	}
	//TODO receive lm75 temperature and calculation it !!!
	/**result = (char)rec_data[3]-48; // return back*/
	else {
		*result = atoi(&rec_data[1]);
	}
	receive_len = 0;
	control_flag = FALSE;
	return 0;
}
int ns__adcReadValue(struct soap *soap, int *result) // read adc value
{
	char result_char[4]={0};
	const char AdcReadValueBuf[9]="SA10000#\0";
	printf("write to uart complete  size = %d\n", write(uart_fd, AdcReadValueBuf, 8));
	/*timerOption(STARTTIMER); // start timer*/
	if(!receive_flag) {
		memset(rec_data, 0, strlen(rec_data));
	}
	while(!control_flag) { // receive_data
		char *temp;
		if ((read(uart_fd, temp, 1)) == 0) {
			printf("Time over\n");
			overtime_flag = TRUE;
			break;
		}
		if((*temp == 'S') && !receive_flag) {
			receive_flag = TRUE;
		}
		if(receive_flag) {
			rec_data[receive_len++] = *temp;
			if((*temp == '#') && (receive_len > 1)) {
				receive_flag = FALSE;
				if(receive_len == 8) {
					control_flag = TRUE;
				}
			}
		}
	}
	printf("%s\n", rec_data);
	/*timerOption(STOPTIMER); // stop timer*/
	if(overtime_flag) {
		*result = -1;
		overtime_flag = FALSE;
	}
	else {
		result_char[0] = rec_data[1];
		result_char[1] = rec_data[2];
		result_char[2] = rec_data[3];
		result_char[3] = rec_data[4];
		result_char[4] = '\0';
		//TODO receive ADC value and calculation it !!!
		/**result = (char)rec_data[3]-48; // return back*/
		*result = atoi(result_char);
	}
	receive_len = 0;
	control_flag = FALSE;
	return 0;
}
int main(int argc, char **argv)
{

	daemon(1, 0);
	int master_socket,slave_socket;
	struct soap soap;
	int i;
	struct soap *soap_thr[MAX_THR]; // each thread needs a runtime context
	pthread_t tid[MAX_THR];
	if (argc < 3) {
		fprintf(stderr, "usage: %s <devices> <server_port> \n", argv[0]);
		exit(EXIT_FAILURE);
	}
  	if((uart_fd = openSerialPort(argv[1]))<0) { /*打开串口*/
		handle_error("openSerialPort");
  	}
	soap_init(&soap); //初始化soap
	soap.fget = http_get;
	master_socket = soap_bind(&soap, NULL, atoi(argv[2]), BACKLOG);
	if (!soap_valid_socket(master_socket))
		exit(1);
	syslog(LOG_INFO,"Socket connection successful %d\n", master_socket);
	fprintf(stderr, "Socket connection successful %d\n", master_socket);
	for (i = 0; i < MAX_THR; i++)
		soap_thr[i] = NULL;
	while(1) {
		for ( i = 0; i < MAX_THR; i++)
		{
			slave_socket = soap_accept(&soap);
			if (!soap_valid_socket(slave_socket))
			{
				if (soap.errnum)
				{
					soap_print_fault(&soap,stderr);
					continue; // retry
				}
				else {
					fprintf(stderr,"Server timed out\n");
					break;
				}
			}
	syslog(LOG_INFO,"Thread %d accepts socket %d connection from IP %d.%d.%d.%d\n", i, slave_socket, (soap.ip >> 24)&0xFF, (soap.ip >> 16)&0xFF, (soap.ip >> 8)&0xFF, soap.ip&0xFF);
			fprintf(stderr, "Thread %d accepts socket %d connection from IP %d.%d.%d.%d\n", i, slave_socket, (soap.ip >> 24)&0xFF, (soap.ip >> 16)&0xFF, (soap.ip >> 8)&0xFF, soap.ip&0xFF);
			if (!soap_thr[i]) // first time around
			{
				soap_thr[i] = soap_copy(&soap);
				if (!soap_thr[i])
					exit(1); // could not allocate
			}
			else// recycle soap context
			{
				pthread_join(tid[i], NULL);
				fprintf(stderr, "Thread %d completed\n", i);
				soap_destroy(soap_thr[i]); // deallocate C++ data of old thread
				soap_end(soap_thr[i]); // deallocate data of old thread
			}
			soap_thr[i]->socket = slave_socket; // new socket fd
			pthread_create(&tid[i], NULL, (void*(*)(void*))soap_serve, (void*)soap_thr[i]);
		}
	}
	for (i = 0; i < MAX_THR; i++)
		if (soap_thr[i])
		{
			soap_done(soap_thr[i]); // detach context
			free(soap_thr[i]); // free up
		}
	close(uart_fd);
	return 0;
}
int http_get(struct soap * soap)
{
	FILE *fd = NULL;
	char *s = strchr(soap->path, '?');
	if (!s || strcmp(s, "?wsdl"))
		return SOAP_GET_METHOD;
	fd = fopen("ns.wsdl", "rb");
	if (!fd)
		return 404;
	soap->http_content = "text/xml";
	soap_response(soap, SOAP_FILE);
	for (;;)
	{
		size_t r = fread(soap->tmpbuf, 1, sizeof(soap->tmpbuf), fd);
		if (!r)
			break;
		if (soap_send_raw(soap, soap->tmpbuf, r))
			break;
	}
	fclose(fd);
	soap_end_send(soap);
	return SOAP_OK;
}
