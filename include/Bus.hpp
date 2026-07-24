#ifndef BUS_H
#define BUS_H

#include <fstream>
#include <iomanip>
#include "SerialPort.hpp"


class Bus
{
    private:
        SerialPort& serialPort;
        uint8_t memory[0x10000] = {0};

        
    public:
        Bus(SerialPort& port);
        ~Bus();
        uint8_t ReadByte(uint16_t address);
        void WriteByte(uint16_t address, uint8_t data);
        void MemDump(uint16_t startAddress, uint16_t endAddress);
        bool LoadROM(std::string filepath);



};






#endif
































































