#pragma once

#define MCU_WAIT_TIME			2000
#define TRANSFER_DATA_BYTES		8

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

class SerialPort
{
private:
	HANDLE handleToCom;
	bool connected;
	COMSTAT status;
	DWORD errors;
public:
	SerialPort(char* portName);
	~SerialPort();

	int ReadSerialPort(char* buffer, unsigned int buf_size);
	bool WriteSerialPort(char* buffer, unsigned int buf_size);
	bool isConnected();
};

