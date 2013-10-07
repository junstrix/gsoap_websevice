/* soapProxy.h
   Generated by gSOAP 2.8.15 from gsoapControl.h

Copyright(C) 2000-2013, Robert van Engelen, Genivia Inc. All Rights Reserved.
The generated code is released under ONE of the following licenses:
GPL or Genivia's license for commercial use.
This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
*/

#ifndef soapProxy_H
#define soapProxy_H
#include "soapH.h"

class SOAP_CMAC Proxy : public soap
{ public:
	/// Endpoint URL of service 'Proxy' (change as needed)
	const char *soap_endpoint;
	/// Constructor
	Proxy();
	/// Construct from another engine state
	Proxy(const struct soap&);
	/// Constructor with endpoint URL
	Proxy(const char *url);
	/// Constructor with engine input+output mode control
	Proxy(soap_mode iomode);
	/// Constructor with URL and input+output mode control
	Proxy(const char *url, soap_mode iomode);
	/// Constructor with engine input and output mode control
	Proxy(soap_mode imode, soap_mode omode);
	/// Destructor frees deserialized data
	virtual	~Proxy();
	/// Initializer used by constructors
	virtual	void Proxy_init(soap_mode imode, soap_mode omode);
	/// Delete all deserialized data (with soap_destroy and soap_end)
	virtual	void destroy();
	/// Delete all deserialized data and reset to default
	virtual	void reset();
	/// Disables and removes SOAP Header from message
	virtual	void soap_noheader();
	/// Get SOAP Header structure (NULL when absent)
	virtual	const SOAP_ENV__Header *soap_header();
	/// Get SOAP Fault structure (NULL when absent)
	virtual	const SOAP_ENV__Fault *soap_fault();
	/// Get SOAP Fault string (NULL when absent)
	virtual	const char *soap_fault_string();
	/// Get SOAP Fault detail as string (NULL when absent)
	virtual	const char *soap_fault_detail();
	/// Close connection (normally automatic, except for send_X ops)
	virtual	int soap_close_socket();
	/// Force close connection (can kill a thread blocked on IO)
	virtual	int soap_force_close_socket();
	/// Print fault
	virtual	void soap_print_fault(FILE*);
#ifndef WITH_LEAN
	/// Print fault to stream
#ifndef WITH_COMPAT
	virtual	void soap_stream_fault(std::ostream&);
#endif

	/// Put fault into buffer
	virtual	char *soap_sprint_fault(char *buf, size_t len);
#endif

	/// Web service operation 'ledControl' (returns error code or SOAP_OK)
	virtual	int ledControl(int led_id, int led_stat, int *result) { return this->ledControl(NULL, NULL, led_id, led_stat, result); }
	virtual	int ledControl(const char *endpoint, const char *soap_action, int led_id, int led_stat, int *result);

	/// Web service operation 'motorControl' (returns error code or SOAP_OK)
	virtual	int motorControl(int motor_id, int motor_op, int *result) { return this->motorControl(NULL, NULL, motor_id, motor_op, result); }
	virtual	int motorControl(const char *endpoint, const char *soap_action, int motor_id, int motor_op, int *result);

	/// Web service operation 'spiReadId' (returns error code or SOAP_OK)
	virtual	int spiReadId(int *result) { return this->spiReadId(NULL, NULL, result); }
	virtual	int spiReadId(const char *endpoint, const char *soap_action, int *result);

	/// Web service operation 'lm75ReadTemp' (returns error code or SOAP_OK)
	virtual	int lm75ReadTemp(int *result) { return this->lm75ReadTemp(NULL, NULL, result); }
	virtual	int lm75ReadTemp(const char *endpoint, const char *soap_action, int *result);

	/// Web service operation 'adcReadValue' (returns error code or SOAP_OK)
	virtual	int adcReadValue(int *result) { return this->adcReadValue(NULL, NULL, result); }
	virtual	int adcReadValue(const char *endpoint, const char *soap_action, int *result);
};
#endif