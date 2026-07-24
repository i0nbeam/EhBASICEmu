#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdint>
#include <cstdio>
#include <queue>


class SerialPort
{
    public:
        SerialPort();
        ~SerialPort();

        bool exit = false;
        bool reset = false;
        
        uint8_t ReadChar();
        void WriteChar(uint8_t c);
        void PollKeyboard();
        bool ExitRequested();
        void RequestExit();
        void RequestReset();
        bool ResetRequested();
        void ResetServed();
        
        
    private:
        termios original;
        std::queue<uint8_t> rxData;

        
        void EnableRawMode();
        void DisableRawMode();
        


};


#endif
