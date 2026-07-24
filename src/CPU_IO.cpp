#include "CPU.hpp"



// Some Notes: // 

// Functions starting with "Read" DO NOT increment the PC.
// Functions starting with "Fetch" DO increment the PC.



// Memory Map // 

// 0000 - 00FF : page 0/Zero page - single-byte addressing - fastest addressing mode!
// 0100 - 01FF : page 1/stack - interrupts and subroutines
// 0200 - 02FF : page 2 - RAM
// 0300 - 03FF  page 3 - RAM
// .
// .
// .
// .
// 8000 - FFFF : page 128 - page 256 - ROM
// .
// .
// .
// FFFA - FFFB : page 256 - NMI vector
// FFFC - FFFD : page 256 - Reset vector
// FFFE - FFFF : page 256 - IRQ vector




// Returns byte at the PC then increments the PC
uint8_t CPU::FetchByte()
{
    return bus.ReadByte(pc++);
}



// Public function, executes FetchByte(), increments PC
uint8_t CPU::Fetch()
{
    
    return FetchByte();
    
}


// Returns an absolute address constructed from the lo and hi bytes. 
// Does not increment PC
uint16_t CPU::ReadAbsAddr()
{
    uint8_t lo = bus.ReadByte(pc);
    uint8_t hi = bus.ReadByte(pc+1);
    return static_cast<uint16_t>(hi) << 8 | lo;
}

uint16_t CPU::ReadAbsAddr(uint16_t address)
{
    uint8_t lo = bus.ReadByte(address);
    uint8_t hi = bus.ReadByte(address+1);
    return static_cast<uint16_t>(hi) << 8 | lo;
}




// Returns an absolute address constructed from the lo and hi bytes. Increments the PC.
uint16_t CPU::FetchAbsAddr()
{
    // little endian!
    uint8_t lo = FetchByte(); 
    uint8_t hi = FetchByte(); 
    return static_cast<uint16_t>(hi) << 8 | static_cast<uint16_t>(lo);
}


// Returns the value at the absolute address + X
// Increments PC
uint16_t CPU::FetchAbsXAddr()
{
    uint16_t address = FetchAbsAddr() + static_cast<uint16_t>(indexRegX);
    return address;
}

// Returns the value at the absolute address + Y
// Increments PC
uint16_t CPU::FetchAbsYAddr()
{
    uint16_t address = FetchAbsAddr() + static_cast<uint16_t>(indexRegY);
    return address;
}



// Returns a value from a zeropage address + X
// Increments PC
uint16_t CPU::FetchZPXAddr()
{
    //uint16_t address = FetchByte();
    uint8_t zp_address = FetchByte() + indexRegX;
    zp_address = static_cast<uint16_t>(zp_address);
    return zp_address;

}

// Returns a value from a zeropage address + Y
// Increments PC
uint16_t CPU::FetchZPYAddr()
{
    uint8_t zp_address = FetchByte() + indexRegY;
    zp_address = static_cast<uint16_t>(zp_address);
    return zp_address;

}


// Increments PC
uint16_t CPU::FetchIndXAddr()
{
    uint8_t zp_address = FetchByte() + indexRegX;
    uint16_t address = ReadAbsAddr(static_cast<uint16_t>(zp_address));
    return address;
}


// Increments PC
uint16_t CPU::FetchIndYAddr()
{
    uint16_t zp_address = static_cast<uint16_t>(FetchByte());
    uint16_t address = ReadAbsAddr(zp_address) + static_cast<uint16_t>(indexRegY);
    return address;
}


// Returns a memory address thru an indirect address
// used for JMP
uint16_t CPU::ReadIndAddr()
{
    uint16_t indirectAddress = ReadAbsAddr();
    uint8_t lo = bus.ReadByte(indirectAddress);
    uint8_t hi = bus.ReadByte(indirectAddress+1);
    return static_cast<uint16_t>(hi) << 8 | lo;
}


uint8_t CPU::ReadByte(AddressMode mode)
{

    switch(mode)
    {
        case(AddressMode::ACCUMULATOR):  return acc;
        case(AddressMode::IMMEDIATE):    return FetchByte();
        case(AddressMode::ZEROPAGE):     return bus.ReadByte(FetchByte());
        case(AddressMode::ZEROPAGE_X):   return bus.ReadByte(FetchZPXAddr());
        case(AddressMode::ZEROPAGE_Y):   return bus.ReadByte(FetchZPYAddr());
        case(AddressMode::ABSOLUTE):     return bus.ReadByte(FetchAbsAddr());
        case(AddressMode::ABSOLUTE_X):   return bus.ReadByte(FetchAbsXAddr());
        case(AddressMode::ABSOLUTE_Y):   return bus.ReadByte(FetchAbsYAddr());
        case(AddressMode::INDIRECT_X):   return bus.ReadByte(FetchIndXAddr());
        case(AddressMode::INDIRECT_Y):   return bus.ReadByte(FetchIndYAddr());
        default:            
            std::cerr << "FetchOperand() recieved invalid AddressMode.\n";
            return 0;
    }       

}

void CPU::WriteByte(AddressMode mode, uint8_t data)
{
    switch(mode)
    {
        case(AddressMode::ZEROPAGE):
            //WriteZeroPage(data);
            bus.WriteByte(FetchByte(), data);
            break;
        case(AddressMode::ZEROPAGE_X):
            //WriteZeroPageX(data);
            bus.WriteByte(FetchZPXAddr(), data);
            break;
        case(AddressMode::ZEROPAGE_Y):
            //WriteZeroPageY(data);
            bus.WriteByte(FetchZPYAddr(), data);
            break;
        case(AddressMode::ABSOLUTE):
            //WriteAbsolute(data);
            bus.WriteByte(FetchAbsAddr(), data);
            break;
        case(AddressMode::ABSOLUTE_X):
            //WriteAbsoluteX(data);
            bus.WriteByte(FetchAbsXAddr(), data);
            break;
        case(AddressMode::ABSOLUTE_Y):
            //WriteAbsoluteY(data);
            bus.WriteByte(FetchAbsYAddr(), data);
            break;
        case(AddressMode::INDIRECT_X):
            //WriteIndirectX(data);
            bus.WriteByte(FetchIndXAddr(), data);
            break;
        case(AddressMode::INDIRECT_Y):
            //WriteIndirectY(data);
            bus.WriteByte(FetchIndYAddr(), data);
            break;
        default:
            std::cerr << "WriteMemory() recieved invalid AddressMode.\n";
            break;
    } 
}


////////////////////////////
// Stack functions
////////////////////////////


// Push a value onto the stack, decrementing the stack pointer by 1.
void CPU::Push(uint8_t data)
{
    uint16_t stackPtr = 0x100 + static_cast<uint16_t>(sp); 
    bus.WriteByte(stackPtr,data);
    sp--;

}

void CPU::PushPC()
{
    uint16_t returnAddress = pc + 1;
    uint8_t returnHi = static_cast<uint8_t>((returnAddress & 0xFF00) >> 8);
    uint8_t returnLo = static_cast<uint8_t>(returnAddress & 0x00FF);
    Push(returnHi);
    Push(returnLo);
}

void CPU::PushStatus()
{
    uint8_t stackStatus = BitReplace(status,1,Flag::BRK);
    stackStatus = BitReplace(stackStatus,1,Flag::UNUSED);
    Push(stackStatus);
}

// Pop a value off the stack, returning the value and incrementing the stack pointer by 1.
uint8_t CPU::Pop()
{
    uint16_t stackPtr = 0x100 | static_cast<uint16_t>(++sp);
    return bus.ReadByte(stackPtr);

    

}

uint16_t CPU::PopPC()
{
    uint8_t pcLo = Pop();
    uint8_t pcHi = Pop();
    return (static_cast<uint16_t>(pcHi) << 8 | pcLo);
}

uint8_t CPU::PopStatus()
{
    uint8_t stackStatus = Pop();
    stackStatus = BitReplace(stackStatus,0,Flag::BRK);
    stackStatus = BitReplace(stackStatus,0,Flag::UNUSED);
    return stackStatus;
}
