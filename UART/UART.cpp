// UART.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <iomanip>
#include <thread>
#include <string>
#include "SerialPort.h"

char rxData[TRANSFER_DATA_BYTES];
char txData[TRANSFER_DATA_BYTES] = {'n', 'g', 'h', 'i', 'a', 'd', 'z'};

char comport[] = "COM3";
char* port = comport;

int main()
{
    SerialPort portToMCU(port);
    if (portToMCU.isConnected())
    {
        std::cout << "Connect to " << port << std::endl;
    }
    else
    {
        std::cin.get();
    }

    // to read port only
    while (portToMCU.isConnected())
    {
        portToMCU.WriteSerialPort(txData, TRANSFER_DATA_BYTES);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        portToMCU.ReadSerialPort(rxData, TRANSFER_DATA_BYTES);
        std::string inputValStr(rxData);
        std::cout << inputValStr << '\n';
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}