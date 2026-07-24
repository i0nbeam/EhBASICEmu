#include "Emulator.hpp"


Emulator::Emulator() : serialPort(), bus(serialPort), cpu(bus)
{

}


Emulator::~Emulator()
{

}


void Emulator::Reset()
{
    cpu.Reset();
}

bool Emulator::LoadROM(std::string filepath)
{
    bool loadSuccess = bus.LoadROM(filepath);
    return loadSuccess;
    
}

void Emulator::Run()
{
     
    bool executeSuccess;
    uint8_t opcode;

    Reset();
    
    while(!serialPort.ExitRequested())
    {
        serialPort.PollKeyboard();

        if(serialPort.ResetRequested())
        {
            Reset();
            serialPort.ResetServed();
        }
        
        opcode = cpu.Fetch();

        executeSuccess = cpu.Execute(opcode);

        if(!executeSuccess) break;
           
    }


}