#include "SerialPort.h"

SerialPort::SerialPort(char* portName)
{
	errors = 0;
	status = { 0 };
	connected = false;

	// Create & open the COM I/O device. This return a handle to COM device
	handleToCom = CreateFileA(static_cast<LPCSTR>(portName), GENERIC_READ | GENERIC_WRITE,
		0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	/* **************** Handle Errors **************** */
	
	/*
	* We care about 2 codes:
	*	- Is the device connected?
	*	- Is it already being used in another app?
	*/

	DWORD errMsg = GetLastError();
	if (errMsg == 2)
	{
		printf("Device is not connected. Please check the connection\n");
	}
	else if (errMsg == 5)
	{
		printf("Device is being used in other app\n");
	}

	/* **************** Configure Serial Port **************** */

	// If there are no Errors
	else if (errMsg == 0)
	{
		// DCB (Device Controll Block), a structure that holds the COM port setting
		DCB dcbSerialParameters = { 0 };
		if (!GetCommState(handleToCom, &dcbSerialParameters))
		{
			printf("Fail to get current Serial port parameters\n");
		}
		else
		{
			// Standard Serial Comunication parameters
			dcbSerialParameters.BaudRate = CBR_9600;
			dcbSerialParameters.ByteSize = 8;
			dcbSerialParameters.StopBits = ONESTOPBIT;
			dcbSerialParameters.Parity = NOPARITY;
			dcbSerialParameters.fDtrControl = DTR_CONTROL_ENABLE;
		}

		if (!SetCommState(handleToCom, &dcbSerialParameters))
		{
			printf("ERROR: could not set Serial port parameters\n");
		}
		else
		{
			connected = true;
			PurgeComm(handleToCom, PURGE_RXCLEAR | PURGE_TXCLEAR);
			Sleep(MCU_WAIT_TIME);
		}
	}
}

SerialPort::~SerialPort()
{
	if (connected == true)
	{
		connected = false;
		CloseHandle(handleToCom);
	}
}

int SerialPort::ReadSerialPort(char* buffer, unsigned int buf_size)
{
	DWORD bytesRead = 0;
	unsigned int toRead = 0;

	ClearCommError(handleToCom, &errors, &status);

	/*
	* Figure out how many bytes to read in the subsequent ReadFile() method.
	*
	* cbInQue is the number of bytes received in the Serial Port but not
	* yet read by a ReadFile operation
	*
	* Prepare to read any available bytes, not to exceed the requested number of bytes
	*/

	if (status.cbInQue > 0)
	{
		if (status.cbInQue > buf_size)
		{
			toRead = buf_size;
		}
		else toRead = status.cbInQue;
	}

	/*
	* Read requested bytes into buffer, and return number of bytes
	*/

	if (ReadFile(handleToCom, buffer, toRead, &bytesRead, NULL))
	{
		return bytesRead;
	}
	return 0;
}

bool SerialPort::WriteSerialPort(char* buffer, unsigned int buf_size)
{
	DWORD bytesWrite = 0;
	
	if (WriteFile(handleToCom, buffer, buf_size, &bytesWrite, NULL)) {
		return true;
	}
	return false;
}

bool SerialPort::isConnected()
{
	return connected;
}