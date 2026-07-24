#include "CPU.hpp"



////////////////////////////
// Update Flags
////////////////////////////


// Given byte, returns byte with the bit at index 'index' replaced with bit newBit.
// The function takes in a Flag enum element, ensuring no overshifting.
uint8_t CPU::BitReplace(uint8_t byte, uint8_t newBit, Flag index)
{
    
    return (byte & ~(1 << index)) | (newBit << index);
}

// Checks if a value is negative or positive based on the value in the 7th bit.
// 1 = Negative, 0 = Positive
void CPU::SetNegFlag(uint8_t value)
{
    uint8_t negFlag = (value & NEG_BIT) >> NEG;
    status = BitReplace(status, negFlag, NEG);

}

// Checks if a value is equal to zero
void CPU::SetZeroFlag(uint8_t value)
{
    uint8_t zeroFlag = (value == 0x00);
    status = BitReplace(status, zeroFlag, ZERO);
}

// Check if an operation led to overflow
// ie.) operands are both negative but result is positive or operands are negative and result is positive
void CPU::SetOverflowFlag(uint8_t value1,uint8_t value2, uint8_t result)
{

    uint8_t overflowFlag = (~(value1 ^ value2) & (value1 ^ result) & NEG_BIT) != 0;

    status = BitReplace(status, overflowFlag, OVERFLOW);

}



// Sets carry flag for arithmetic operations
void CPU::SetCarryFlag(uint8_t value1, uint8_t value2, uint8_t result)
{
    uint16_t carry = GetStatusFlag(Flag::CARRY);
    uint16_t sum = static_cast<uint16_t>(value1) + static_cast<uint16_t>(value2) + carry;
    uint8_t carryFlag = static_cast<uint8_t>(sum > 0xFF);
    status = BitReplace(status, carryFlag, CARRY);
    
}


// Sets carry for comparison operations
// value1 >= value2 -> carry
void CPU::SetCarryFlag(uint8_t value1, uint8_t value2)
{
    uint8_t carryFlag = (value1 >= value2);
    status = BitReplace(status,carryFlag,CARRY);
}

// Sets the negative and zero flags
void CPU::SetNZFlags(uint8_t value)
{
    SetNegFlag(value);
    SetZeroFlag(value);
}



uint8_t CPU::GetStatusFlag(Flag index)
{
    return (status >> index) & 0x1;
    
}