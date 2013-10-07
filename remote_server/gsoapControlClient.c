#include "soapStub.h"
#include "ns.nsmap" 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
int led_control(const char *server, int led_id, int led_stat, int *led_result)
{ 
	printf("led_id:%d, led_stat:%d, server:%s\n", led_id,led_stat,server);
	struct soap add_soap; 
	int result = 0; 
	soap_init(&add_soap); 
	soap_set_namespaces(&add_soap, namespaces); 
	soap_call_ns__ledControl(&add_soap,server,NULL,led_id,led_stat,led_result);
	if (add_soap.error)   
	{ 
		printf("soap  error:  %d,  %s,  %s\n",  add_soap.error, 
				*soap_faultcode(&add_soap), *soap_faultstring(&add_soap)); 
		result = add_soap.error; 
	} 
	soap_end(&add_soap); 
	soap_done(&add_soap); 
	return result; 
} 
   
int main(int argc, char **argv) 
{ 
	int led_id;
	int led_stat;
	int led_result=0;
	int result = 0;
	char server[128] = {0}; 
	if (argc < 4) { 
		printf("usage: %s <ip:port> led_id led_stat \n", argv[0]); 
		exit(1); 
	} 
	strcpy(server,argv[1]); 
	led_id = atoi(argv[2]); 
	led_stat = atoi(argv[3]);
	printf("%d\t %d\n", led_id, led_stat);
	result = led_control(server, led_id, led_stat, &led_result); 
	if (result != 0)   
	{ 
		printf("soap error, errcode=%d\n", result); 
	}   
	else   
	{ 
		printf("led id: %d, status: %d\t result:%d\n", led_id, led_stat,led_result);
	} 
	return 0; 
} 
