#include "Bus.hpp"

#define ROM_START 0x8000

Bus::Bus(SerialPort& port) : serialPort(port)
{

}

Bus::~Bus()
{

}


// Returns byte from given address without incrementing PC

uint8_t Bus::ReadByte(uint16_t address)
{
   
    if(address == 0xF004) // Serial input port
    {
        uint8_t c = serialPort.ReadChar();
        memory[address] = c;
    }
    
    return memory[address];
}


// Writes a byte to a provided address
void Bus::WriteByte(uint16_t address, uint8_t data)
{
    memory[address] = data;

    if (address == 0xF001) // Serial output port
    {
        serialPort.WriteChar(data);
    }
}



void Bus::MemDump(uint16_t startAddress, uint16_t endAddress)
{

    if(startAddress >= endAddress)
    {
        std::cerr << "Error: start address is greater than or equal to end address!" << std::endl;
        return;
    }
    if(startAddress < 0x0000 || endAddress > 0xFFFF)
    {
        std::cerr << "Error: address out of range!" << std::endl;
        return;
    }

    uint16_t numAddresses = endAddress - startAddress;

    uint16_t address = startAddress;
    
    
    uint16_t startAddressLowNibble = startAddress & 0xF;
    uint16_t endAddressLowNibble = endAddress & 0xF;
    
    std::cout << "        ";
    for (int i = 0; i < 0x10; i++)
    {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << i << "  ";
        
    }
    std::cout << "\n" << std::endl;
    
    while(address <= endAddress && address >= startAddress)
    {
        uint16_t startAddressHighNibble;
        if(numAddresses < 15) startAddressHighNibble = address & 0xFFFF; 
        else startAddressHighNibble = address & 0xFFF0;
        std::cout << "0x" << std::hex << std::setw(4) << std::setfill('0') << startAddressHighNibble << "  ";
        for(uint16_t i = address; i < address + 0x10; i++)
        {

            std::cout << std::hex << std::setw(2) << std::setfill('0') << +ReadByte(i) << "  ";
            std::cout << std::flush;
            if(i == endAddress) break;
        }
        address += 0x10;
        std::cout << std::endl;
    }
        
    std::cout << std::endl;

    

}


bool Bus::LoadROM(std::string filepath)
{
    char byte;
    std::ifstream rom(filepath, std::ios::binary);

    if(!rom) // check to see if the rom opened successfully
    {
        std::cerr << "\033[31mERROR\033[0m : ./basic must be in the same folder as min_mon.bin.\n";
        return false;
    }

    rom.seekg(-2, rom.end);
    rom.seekg(0, rom.beg);
    uint16_t i = ROM_START;
    while(rom.get(byte))
    {
        byte = static_cast<uint8_t>(byte); 
        WriteByte(i++, byte);
        
    }

    return true;



} 

