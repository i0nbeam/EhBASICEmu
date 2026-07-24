#ifndef EMULATOR_H
#define EMULATOR_H

#include "CPU.hpp"
#include "Bus.hpp"
#include "SerialPort.hpp"



class Emulator
{
    public:
        Emulator();
        ~Emulator();
        void Run();
        void Reset();
        bool LoadROM(std::string filepath);
        
        
    private:
        SerialPort serialPort;
        Bus bus;
        CPU cpu;

};








#endif