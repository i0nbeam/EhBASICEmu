#ifndef CPU_H
#define CPU_H


#include "Bus.hpp"


#define NEG_BIT 0x80
#define OVERFLOW_BIT 0x40
#define BRK_BIT 0x10
#define DEC_BIT 0x08
#define IRQ_BIT 0x04
#define ZERO_BIT 0x02
#define CARRY_BIT 0x01


// Enum for the status register indices. 
enum Flag
{
    CARRY,
    ZERO,
    INTDISABLE,
    DEC,
    BRK,
    UNUSED,
    OVERFLOW,
    NEG

};

enum class AddressMode 
{
    IMMEDIATE,
    ZEROPAGE,
    ZEROPAGE_X,
    ZEROPAGE_Y,
    ABSOLUTE,
    ABSOLUTE_X,
    ABSOLUTE_Y,
    INDIRECT,
    INDIRECT_X,
    INDIRECT_Y,
    ACCUMULATOR // for bitshift instructions
};



// Responsible for fetching and executing instructions from memory.
// Contains a Bus& to perform memory IO operations
class CPU 
{

    private:

        Bus& bus;

        uint8_t indexRegX{};
        uint8_t indexRegY{};
        uint16_t pc{};
        uint8_t acc{};
        uint8_t status{};
        uint8_t sp{};


        // functions that begin with "Fetch" increment the PC to the next instruction
        uint8_t FetchByte();
        uint16_t FetchAbsAddr();
        uint16_t FetchAbsXAddr();
        uint16_t FetchAbsYAddr();
        uint16_t FetchZPXAddr();
        uint16_t FetchZPYAddr();
        uint16_t FetchIndXAddr();
        uint16_t FetchIndYAddr();

        // functions that begin with "Read" do not increment the PC
        uint16_t ReadAbsAddr();
        uint16_t ReadAbsAddr(uint16_t address);
        uint16_t ReadIndAddr();

        
        uint8_t ReadByte(AddressMode mode);
        void WriteByte(AddressMode mode, uint8_t data);
        uint8_t GetStatusFlag(Flag index);
        

        void Push(uint8_t data);
        void PushPC();
        void PushStatus();
        uint8_t Pop();
        uint16_t PopPC();
        uint8_t PopStatus();

        void LDA(AddressMode mode);
        void LDX(AddressMode mode);
        void LDY(AddressMode mode);

        void STA(AddressMode mode);
        void STX(AddressMode mode);
        void STY(AddressMode mode);

        void INX();
        void DEX();
        void INY();
        void DEY();
        void INC(AddressMode mode);
        void DEC(AddressMode mode);

        void TAX();
        void TXA();
        void TYA();
        void TAY();
        void TXS();
        void TSX();

        void BEQ();
        void BNE();
        void BMI();
        void BPL();
        void BCC();
        void BCS();
        void BVC();
        void BVS();

        void JMP(AddressMode mode);
        void JSR();
        void RTS();
        void BRK();
        void RTI();

        void PHA();
        void PHP();
        void PLP();
        void PLA();

        void CMP(AddressMode mode);
        void CPX(AddressMode mode);
        void CPY(AddressMode mode);

        void CLC();
        void SEC();
        void CLD();
        void CLI();
        void SEI();
        void SED();
        void CLV();
        void BIT(AddressMode mode);

        void ADC(AddressMode mode);
        void SBC(AddressMode mode);
        void ASL(AddressMode mode);
        void LSR(AddressMode mode);
        void ROL(AddressMode mode);
        void ROR(AddressMode mode);

        void DecADC(AddressMode mode);
        void DecSBC(AddressMode mode);

        void EOR(AddressMode mode);
        void ORA(AddressMode mode);
        void AND(AddressMode mode);
        
        uint8_t BitReplace(uint8_t byte, uint8_t newBit, Flag index);
        void SetNegFlag(uint8_t value);
        void SetZeroFlag(uint8_t value);
        void SetNZFlags(uint8_t value);
        void SetOverflowFlag(uint8_t value1,uint8_t value2,uint8_t result);
        void SetCarryFlag(uint8_t value1, uint8_t value2, uint8_t result);
        void SetCarryFlag(uint8_t value1, uint8_t value2);

        
        
        
        
        
    public:
        CPU(Bus& bus);
        ~CPU();
        
        uint16_t GetPC();
        void SetPC(uint16_t addr);
        
        uint8_t Fetch();
        bool Execute(uint8_t opcode);
        void Reset();

        void NMI();
        void IRQ();
        
        void PrintRegisters();
        




    


};






#endif
















