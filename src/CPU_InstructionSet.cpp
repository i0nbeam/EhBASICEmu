#include "CPU.hpp"


void CPU::LDA(AddressMode mode)
{
    acc = ReadByte(mode);
    SetNZFlags(acc);
}


void CPU::LDX(AddressMode mode)
{
    indexRegX = ReadByte(mode);
    SetNZFlags(indexRegX);
}


void CPU::LDY(AddressMode mode)
{
    indexRegY = ReadByte(mode);
    SetNZFlags(indexRegY);
}


void CPU::STA(AddressMode mode)
{
    WriteByte(mode, acc);
}

void CPU::STX(AddressMode mode)
{
    WriteByte(mode, indexRegX);
}


void CPU::STY(AddressMode mode)
{
    WriteByte(mode, indexRegY);
}


void CPU::INX()
{
    indexRegX++;
    SetNZFlags(indexRegX);

}

void CPU::DEX()
{
    indexRegX--;
    SetNZFlags(indexRegX);
}

void CPU::INY()
{
    indexRegY++;
    SetNZFlags(indexRegY);
}

void CPU::DEY()
{
    indexRegY--;
    SetNZFlags(indexRegY);
}

void CPU::INC(AddressMode mode)
{
    uint8_t zp_address;
    uint8_t operand;
    uint16_t address;

    switch(mode)
    {
        case(AddressMode::ZEROPAGE):
            address = static_cast<uint16_t>(bus.ReadByte(pc));
            break;

        case(AddressMode::ZEROPAGE_X):
            zp_address = bus.ReadByte(pc) + indexRegX;
            address = static_cast<uint16_t>(zp_address);
            break;
        case(AddressMode::ABSOLUTE):
            address = ReadAbsAddr();
            break;
        case(AddressMode::ABSOLUTE_X):
            address = ReadAbsAddr() + indexRegX;
            break;
    }

    operand = ReadByte(mode) + 1;
    bus.WriteByte(address,operand);
    SetNZFlags(operand);
}

void CPU::DEC(AddressMode mode)
{
    uint8_t zp_address;
    uint8_t operand;
    uint16_t address;

    switch(mode)
    {
        case(AddressMode::ZEROPAGE):
            address = static_cast<uint16_t>(bus.ReadByte(pc));
            break;

        case(AddressMode::ZEROPAGE_X):
            zp_address = bus.ReadByte(pc) + indexRegX;
            address = static_cast<uint16_t>(zp_address);
            break;
        case(AddressMode::ABSOLUTE):
            address = ReadAbsAddr();
            break;
        case(AddressMode::ABSOLUTE_X):
            address = ReadAbsAddr() + indexRegX;
            break;
    }

    operand = ReadByte(mode) - 1;
    bus.WriteByte(address,operand);
    SetNZFlags(operand);
}


void CPU::TAX()
{
    indexRegX = acc;
    SetNZFlags(indexRegX);
}

void CPU::TXA()
{
    acc = indexRegX;
    SetNZFlags(acc);
}

void CPU::TYA()
{
    acc = indexRegY;
    SetNZFlags(acc);
}

void CPU::TAY()
{
    indexRegY = acc;
    SetNZFlags(indexRegY);
}

void CPU::TXS()
{
    uint16_t stack_offset = 0x100;
    sp = stack_offset | static_cast<uint16_t>(indexRegX);
}

void CPU::TSX()
{
    uint8_t spLo = static_cast<uint8_t>(sp & 0x00FF);
    indexRegX = spLo;
    SetNZFlags(indexRegX);
}

void CPU::BEQ()
{
    int8_t offset = static_cast<int8_t>(FetchByte());

    if(GetStatusFlag(Flag::ZERO)) pc += offset;
}

void CPU::BNE()
{
    int8_t offset = static_cast<int8_t>(FetchByte());

    if(!GetStatusFlag(Flag::ZERO)) pc += offset;
}



void CPU::BMI()
{
    int8_t offset = static_cast<int8_t>(FetchByte());

    if(GetStatusFlag(Flag::NEG)) pc += offset;

}


void CPU::BPL()
{
    int8_t offset = static_cast<int8_t>(FetchByte());

    if(!GetStatusFlag(Flag::NEG)) pc += offset;
}


void CPU::BCC()
{
    int8_t offset = static_cast<int8_t>(FetchByte());

    if(!GetStatusFlag(Flag::CARRY)) pc += offset;
}


void CPU::BCS()
{
    int8_t offset = static_cast<int8_t>(FetchByte());

    if(GetStatusFlag(Flag::CARRY)) pc += offset;
}


void CPU::BVC()
{
    int8_t offset = static_cast<int8_t>(FetchByte());

    if(!GetStatusFlag(Flag::OVERFLOW)) pc += offset;
}


void CPU::BVS()
{
    int8_t offset = static_cast<int8_t>(FetchByte());

    if(GetStatusFlag(Flag::OVERFLOW)) pc += offset;
}




void CPU::JMP(AddressMode mode)
{
    switch(mode)
    {
        case(AddressMode::ABSOLUTE):
            pc = ReadAbsAddr();
            break;
        case(AddressMode::INDIRECT):
            pc = ReadIndAddr();
            break;
    }
}

void CPU::JSR()
{
    uint16_t jumpAddress = ReadAbsAddr();

    PushPC();

    pc = jumpAddress;
    
}

void CPU::RTS()
{
   pc = PopPC() + 1;
}


void CPU::BRK()
{

    PushPC();
    PushStatus();
    status = BitReplace(status,1,Flag::INTDISABLE); // THIS MIGHT BECOME A PROBLEM !

    pc = 0xFFFE;

    pc = ReadAbsAddr();


}


void CPU::RTI()
{
    status = PopStatus();
    status = BitReplace(status,0,Flag::BRK);
    status = BitReplace(status,0,Flag::UNUSED);
    pc = PopPC();
}

void CPU::PHA()
{
    Push(acc);
}

void CPU::PHP()
{
    PushStatus();
}

void CPU::PLP()
{
    status = PopStatus();
}

void CPU::PLA()
{
    acc = Pop();
    SetNZFlags(acc);
}


void CPU::CMP(AddressMode mode)
{
    uint8_t operand = ReadByte(mode);
    uint8_t result = acc - operand;

    SetCarryFlag(acc,operand);
    SetNZFlags(result);

}


void CPU::CPX(AddressMode mode)
{
    uint8_t operand = ReadByte(mode);
    uint8_t result = indexRegX - operand;
    SetNZFlags(result);
    SetCarryFlag(indexRegX,operand);
}

void CPU::CPY(AddressMode mode)
{
    uint8_t operand = ReadByte(mode);
    uint8_t result = indexRegY - operand;
    SetNZFlags(result);
    SetCarryFlag(indexRegY,operand);
}



void CPU::CLC()
{
    status = BitReplace(status, 0, Flag::CARRY);
}

void CPU::SEC()
{
    status = BitReplace(status, 1, Flag::CARRY);
}


void CPU::CLD()
{
    status = BitReplace(status,0,Flag::DEC);
}

void CPU::CLI()
{
    status = BitReplace(status,0,Flag::INTDISABLE);
}


void CPU::SEI()
{
    status = BitReplace(status,1,Flag::INTDISABLE);
}

void CPU::SED()
{
    status = BitReplace(status,1,Flag::DEC);
}

void CPU::CLV()
{
    status = BitReplace(status,0,Flag::OVERFLOW);
}


void CPU::BIT(AddressMode mode)
{
    uint8_t operand = ReadByte(mode);
    SetZeroFlag(acc & operand);
    uint8_t N = (operand & NEG_BIT) >> Flag::NEG;
    uint8_t V = (operand & OVERFLOW_BIT) >>  Flag::OVERFLOW;
    status = BitReplace(status,N,Flag::NEG);
    status = BitReplace(status,V,Flag::OVERFLOW);
}


void CPU::ADC(AddressMode mode)
{
    uint16_t sum;
    uint8_t carry = GetStatusFlag(Flag::CARRY);
    uint8_t operand = ReadByte(mode);
    
    sum = static_cast<uint16_t>(acc) + static_cast<uint16_t>(operand) + static_cast<uint16_t>(carry);
    
    SetCarryFlag(acc,operand,sum);
    SetOverflowFlag(acc,operand,sum);
    
    acc = static_cast<uint8_t>(sum);
    SetNZFlags(acc);
    
}

void CPU::SBC(AddressMode mode)
{
    uint16_t diff;
    uint8_t carry = GetStatusFlag(Flag::CARRY);
    uint8_t operand = ReadByte(mode);
    
    diff = static_cast<uint16_t>(acc) 
        + (~static_cast<uint16_t>(operand)) + (static_cast<uint16_t>(carry));
        
        
    // A - M = A - M - (~C)
    //       = A + (~M) + C
    // my SetOverflowFlag() and SetCarryFlag() assume addition
    // so we use the "addition" version of A - M
    SetOverflowFlag(acc,~operand,diff);
    SetCarryFlag(acc,~operand,diff);
    acc = static_cast<uint8_t>(diff);
    SetNZFlags(acc);
}



void CPU::DecADC(AddressMode mode)
{

    uint8_t C = GetStatusFlag(Flag::CARRY);
    
    uint8_t accLo = acc & 0x0F;
    uint8_t accHi = (acc & 0xF0) >> 4;
    
    uint8_t operand = ReadByte(mode);
    uint8_t operandLo = operand & 0x0F;
    uint8_t operandHi = (operand & 0xF0) >> 4;
    
    uint8_t ones = accLo + operandLo + C;
    uint8_t carry = ones > 9;
    if(carry) ones -= 10;
    
    uint8_t tens = accHi + operandHi + carry;
    uint8_t carryBit = tens > 9;
    if(carryBit) tens -= 10;
    
    status = BitReplace(status,carryBit,Flag::CARRY);
    
    acc = (tens << 4) | ones;

    uint8_t sum = acc + operand + C;

    SetOverflowFlag(acc,operand,sum);
    SetNZFlags(sum);



}

void CPU::DecSBC(AddressMode mode)
{

    uint8_t C = GetStatusFlag(Flag::CARRY);

    uint8_t accLo = acc & 0x0F;
    uint8_t accHi = (acc & 0xF0) >> 4;
    
    uint8_t operand = ReadByte(mode);

    uint8_t operandLo = operand & 0x0F;
    uint8_t operandHi = (operand & 0xF0) >> 4;

    int8_t ones = accLo - operandLo - (1 - C);
    int8_t borrow = ones < 0;
    if(borrow) ones += 10;

    int8_t tens = accHi - operandHi;
    if(borrow) tens -= 1;

    uint8_t carryBit = tens < 0;
    if(carryBit) tens += 10;

    
    uint16_t diff = static_cast<uint16_t>(acc) 
    + (~static_cast<uint16_t>(operand)) + (static_cast<uint16_t>(C));

    status = BitReplace(status,~carryBit,Flag::CARRY);
    SetOverflowFlag(acc,~operand,diff);
    SetNZFlags(diff);
    
    acc = (tens << 4) | ones;
}




void CPU::ASL(AddressMode mode)
{
    uint8_t operand;
    uint8_t C;
    uint16_t address;
    
    
    switch(mode)
    {
        case(AddressMode::ACCUMULATOR):
            // Bit 7 of operand goes into carry flag
            C = (acc & NEG_BIT) >> Flag::NEG;
            status = BitReplace(status,C,Flag::CARRY);
            acc = acc << 1;
            SetNZFlags(acc);
            break;
        case(AddressMode::ZEROPAGE):
            address = static_cast<uint16_t>(bus.ReadByte(pc));
            operand = ReadByte(mode);
            C = (operand & NEG_BIT) >> Flag::NEG;
            status = BitReplace(status,C,Flag::CARRY);
            operand = operand << 1;
            SetNZFlags(operand);
            bus.WriteByte(address,operand);
            break;
        case(AddressMode::ZEROPAGE_X):
            address = static_cast<uint16_t>(bus.ReadByte(pc)) + indexRegX;
            operand = ReadByte(mode);
            C = (operand & NEG_BIT) >> Flag::NEG;
            status = BitReplace(status,C,Flag::CARRY);
            operand = operand << 1;
            SetNZFlags(operand);
            bus.WriteByte(address,operand);
            break;
        case(AddressMode::ABSOLUTE):
            address = ReadAbsAddr();
            operand = ReadByte(mode);
            C = (operand & NEG_BIT) >> Flag::NEG;
            status = BitReplace(status,C,Flag::CARRY);
            operand = operand << 1;
            SetNZFlags(operand);
            bus.WriteByte(address,operand);
            break;
        case(AddressMode::ABSOLUTE_X):
            uint16_t address = ReadAbsAddr() + indexRegX;
            operand = ReadByte(mode);
            C = (operand & NEG_BIT) >> Flag::NEG;
            status = BitReplace(status,C,Flag::CARRY);
            operand = operand << 1;
            SetNZFlags(operand);
            bus.WriteByte(address,operand);
            break;
    }
    
}


void CPU::LSR(AddressMode mode)
{
    uint8_t operand;
    uint8_t C;
    uint16_t address;
    
    
    switch(mode)
    {
        case(AddressMode::ACCUMULATOR):
            // Bit 0 of operand goes into carry flag
            C = (acc & CARRY_BIT);
            status = BitReplace(status,C,Flag::CARRY);
            acc = acc >> 1;
            SetNZFlags(acc);
            break;
        case(AddressMode::ZEROPAGE):
            address = static_cast<uint16_t>(bus.ReadByte(pc));
            operand = ReadByte(mode);
            C = (operand & CARRY_BIT);
            status = BitReplace(status,C,Flag::CARRY);
            operand = operand >> 1;
            SetNZFlags(operand);
            bus.WriteByte(address,operand);
            break;
        case(AddressMode::ZEROPAGE_X):
            address = static_cast<uint16_t>(bus.ReadByte(pc)) + indexRegX;
            operand = ReadByte(mode);
            C = (operand & CARRY_BIT);
            status = BitReplace(status,C,Flag::CARRY);
            operand = operand >> 1;
            SetNZFlags(operand);
            bus.WriteByte(address,operand);
            break;
        case(AddressMode::ABSOLUTE):
            address = ReadAbsAddr();
            operand = ReadByte(mode);
            C = (operand & CARRY_BIT);
            status = BitReplace(status,C,Flag::CARRY);
            operand = operand >> 1;
            SetNZFlags(operand);
            bus.WriteByte(address,operand);
            break;
        case(AddressMode::ABSOLUTE_X):
            uint16_t address = ReadAbsAddr() + indexRegX;
            operand = ReadByte(mode);
            C = (operand & CARRY_BIT);
            status = BitReplace(status,C,Flag::CARRY);
            operand = operand >> 1;
            SetNZFlags(operand);
            bus.WriteByte(address,operand);
            break;
    }
    
}


void CPU::ROL(AddressMode mode)
{
    uint8_t operand;
    uint8_t C;
    uint8_t newCFlag;
    uint8_t zp_address;
    uint16_t address;
    
    C = GetStatusFlag(Flag::CARRY);
 
    switch(mode)
    {
        case(AddressMode::ACCUMULATOR):
            newCFlag = (acc & NEG_BIT) >> 7;
            status = BitReplace(status,newCFlag,Flag::CARRY);
            acc = acc << 1;
            acc = BitReplace(acc,C,Flag::CARRY);
            SetNZFlags(acc);
            break;
        case(AddressMode::ZEROPAGE):
            address = static_cast<uint16_t>(bus.ReadByte(pc));
            operand = ReadByte(mode);
            newCFlag = (operand & NEG_BIT) >> 7;
            status = BitReplace(status,newCFlag,Flag::CARRY);
            operand = operand << 1;
            operand = BitReplace(operand,C,Flag::CARRY);
            SetNZFlags(operand);
            bus.WriteByte(address,operand);
            break;
        case(AddressMode::ZEROPAGE_X):
            zp_address = (bus.ReadByte(pc) + indexRegX);
            address = static_cast<uint16_t>(zp_address);
            operand = ReadByte(mode);
            newCFlag = (operand & NEG_BIT) >> 7;
            status = BitReplace(status,newCFlag,Flag::CARRY);
            operand = operand << 1;
            operand = BitReplace(operand,C,Flag::CARRY);
            SetNZFlags(operand);
            bus.WriteByte(address,operand);
            break;
        case(AddressMode::ABSOLUTE):
            address = ReadAbsAddr();
            operand = ReadByte(mode);
            newCFlag = (operand & NEG_BIT) >> 7;
            status = BitReplace(status,newCFlag,Flag::CARRY);
            operand = operand << 1;
            operand = BitReplace(operand,C,Flag::CARRY);
            SetNZFlags(operand);
            bus.WriteByte(address,operand);
            break;
        case(AddressMode::ABSOLUTE_X):
            address = ReadAbsAddr() + indexRegX;
            operand = ReadByte(mode);
            newCFlag = (operand & NEG_BIT) >> 7;
            status = BitReplace(status,newCFlag,Flag::CARRY);
            operand = operand << 1;
            operand = BitReplace(operand,C,Flag::CARRY);
            SetNZFlags(operand);
            bus.WriteByte(address,operand);
            break;
    }
      
    

}


void CPU::ROR(AddressMode mode)
{
    uint8_t operand;
    uint8_t C;
    uint8_t newCFlag;
    uint8_t zp_address;
    uint16_t address;
    
    C = GetStatusFlag(Flag::CARRY);
 
    switch(mode)
    {
        case(AddressMode::ACCUMULATOR):
            newCFlag = (acc & CARRY_BIT);
            status = BitReplace(status,newCFlag,Flag::CARRY);
            acc = acc >> 1;
            acc = BitReplace(acc,C,Flag::NEG);
            SetNZFlags(acc);
            break;
        case(AddressMode::ZEROPAGE):
            address = static_cast<uint16_t>(bus.ReadByte(pc));
            operand = ReadByte(mode);
            newCFlag = (operand & CARRY_BIT);
            status = BitReplace(status,newCFlag,Flag::CARRY);
            operand = operand >> 1;
            operand = BitReplace(operand,C,Flag::NEG);
            SetNZFlags(operand);
            bus.WriteByte(address,operand);
            break;
        case(AddressMode::ZEROPAGE_X):
            zp_address = (bus.ReadByte(pc) + indexRegX);
            address = static_cast<uint16_t>(zp_address);
            operand = ReadByte(mode);
            newCFlag = (operand & CARRY_BIT);
            status = BitReplace(status,newCFlag,Flag::CARRY);
            operand = operand >> 1;
            operand = BitReplace(operand,C,Flag::NEG);
            SetNZFlags(operand);
            bus.WriteByte(address,operand);
            break;
        case(AddressMode::ABSOLUTE):
            address = ReadAbsAddr();
            operand = ReadByte(mode);
            newCFlag = (operand & CARRY_BIT);
            status = BitReplace(status,newCFlag,Flag::CARRY);
            operand = operand >> 1;
            operand = BitReplace(operand,C,Flag::NEG);
            SetNZFlags(operand);
            bus.WriteByte(address,operand);
            break;
        case(AddressMode::ABSOLUTE_X):
            address = ReadAbsAddr() + indexRegX;
            operand = ReadByte(mode);
            newCFlag = (operand & CARRY_BIT);
            status = BitReplace(status,newCFlag,Flag::CARRY);
            operand = operand >> 1;
            operand = BitReplace(operand,C,Flag::NEG);
            SetNZFlags(operand);
            bus.WriteByte(address,operand);
            break;
    }
      
    

}




// LOGIC OPERATIONS

void CPU::EOR(AddressMode mode)
{
    uint8_t operand = ReadByte(mode);
    acc = acc ^ operand;
    SetNZFlags(acc); 
}



void CPU::ORA(AddressMode mode)
{
    uint8_t operand = ReadByte(mode);
    acc = acc | operand;
    SetNZFlags(acc);
}


void CPU::AND(AddressMode mode)
{
    uint8_t operand = ReadByte(mode);
    acc = acc & operand;
    SetNZFlags(acc);
}















