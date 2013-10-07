#this is a Makefile to build client and server

OBJ_NAME := gsoapControl
GSOAP_ROOT := /media/embedded_CSIP/Tiny6410/FriendlyARM/build_src/gsoap-2.8/gsoap
INCLUDE := -I$(GSOAP_ROOT)

#CC := arm-linux-gcc
#GCC := arm-linux-g++
CC := gcc
GCC := gcc
#CC := arm-linux-gcc
#GCC := arm-linux-gcc

OBJ_SERVER := soapC.o stdsoap2.o soapServer.o $(OBJ_NAME)Server.o
OBJ_CLIENT := soapC.o stdsoap2.o soapClient.o $(OBJ_NAME)Client.o
OBJ_READ := soapC.o stdsoap2.o soapClient.o readClient.o

all:server

server: $(OBJ_SERVER)
#$(CC) $(INCLUDE) -lpthread $^ -o $@
	$(CC) $(INCLUDE) -lpthread $^ -o $@
client: $(OBJ_CLIENT)
	$(CC) $(INCLUDE) $^ -o $@
read  : $(OBJ_READ)
	$(CC) $(INCLUDE) $^ -o $@
soap:
	soapcpp2 -c $(OBJ_NAME).h

.PHONY:clean
clean:
	rm -f server client *.o
distclean:
	rm -f server client *.o ns* soap* *.xml *.nsmap
