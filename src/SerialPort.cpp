#include "SerialPort.hpp"



SerialPort::SerialPort()
{
    EnableRawMode();
}

SerialPort::~SerialPort()
{
    DisableRawMode();
}

void SerialPort::EnableRawMode()
{
    tcgetattr(STDIN_FILENO, &original);

    termios raw = original;

    raw.c_lflag &= ~(ICANON | ECHO | ISIG);

    tcsetattr(STDIN_FILENO, TCSANOW, &raw);

    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
}

void SerialPort::DisableRawMode()
{
    tcsetattr(STDIN_FILENO, TCSANOW, &original);
}


uint8_t SerialPort::ReadChar()
{
    if(rxData.empty()) return 0;

    uint8_t c = rxData.front();
    rxData.pop();
    return c;
    
}

void SerialPort::WriteChar(uint8_t c)
{
    if(c == 0x08)
    {
        std::cout << "\b \b";
        std::cout.flush();
        return;
    }
    putchar(c);
    std::cout.flush();
}

void SerialPort::PollKeyboard()
{
    uint8_t c;
    while (read(STDIN_FILENO, &c, 1) == 1)
    {
        switch(c)
        {
            case('\n'): // EhBASIC expects \r not \n
                c = '\r';
                rxData.push(c);
                break;
            case(0x7F): // backspace
            c = 0x08;
            //std::cout << "\b \b";
                rxData.push(c);
                break;
            case(0x03): // CTRL+C
                RequestExit();
                break;
            case(0x12): // CTRL+R, using this keycombo to reset BASIC
                RequestReset();
                break;
            default:
                rxData.push(c);
                break;
        }

        
    }
}


void SerialPort::RequestExit()
{
    exit = true;
}

void SerialPort::RequestReset()
{
    reset = true;
}

bool SerialPort::ExitRequested()
{
    return exit;
}


bool SerialPort::ResetRequested()
{
    return reset;
}

void SerialPort::ResetServed()
{
    if(reset) reset = false;
}