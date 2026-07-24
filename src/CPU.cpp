#include "CPU.hpp"



// Constructor
CPU::CPU(Bus& busObject) : bus(busObject)
{

}    


// Destructor
CPU::~CPU()
{
    
}    


void CPU::SetPC(uint16_t addr)
{
    pc = addr;
}

uint16_t CPU::GetPC()
{
    return pc;
}

void CPU::Reset()
{
    

    status = BitReplace(status, 1, Flag::INTDISABLE);
    sp = 0xFD;

    pc = 0xFFFC;
    pc = ReadAbsAddr();

    
}



void CPU::IRQ()
{
    if (GetStatusFlag(Flag::INTDISABLE)) return;

    PushPC();
    uint8_t stackStatus = BitReplace(status,0,Flag::BRK);
    PushStatus();

    pc = ReadAbsAddr(0xFFFE);

}

void CPU::NMI()
{

    PushPC();
    uint8_t stackStatus = BitReplace(status,0,Flag::BRK);
    PushStatus();

    pc = ReadAbsAddr(0xFFFA);

}

void CPU::PrintRegisters()
{
    std::cout << "================" << std::endl;
    std::cout << "CPU Registers" << std::endl;
    std::cout << "================" << std::endl;
    std::cout << "Program counter = 0x" << std::hex << pc << std::endl;
    std::cout << "Accumulator = 0x" << std::hex << +acc << std::endl;
    std::cout << "X = 0x" << std::hex << +indexRegX << std::endl;
    std::cout << "Y = 0x" << std::hex << +indexRegY << std::endl;
    std::cout << "(N V - B D I Z C) = 0x" 
    << std::hex << +status << std::endl;
    std::cout << "Stack pointer = 0x" << std::hex << +sp << std::endl;
}


bool CPU::Execute(uint8_t opcode)
{
    switch(opcode)
    {

        //////////////////////
        // LDA INSTRUCTIONS // 
        //////////////////////

        case(0xA9): // load acc imm
            LDA(AddressMode::IMMEDIATE);
            return true;
        case(0xA5): // load acc zpg    
            LDA(AddressMode::ZEROPAGE);
            return true;
        case(0xB5): // load acc zpg,X    
            LDA(AddressMode::ZEROPAGE_X);
            return true;
        case(0xAD): // load acc abs    
            LDA(AddressMode::ABSOLUTE);
            return true;
        case(0xBD): // load acc abs,X    
            LDA(AddressMode::ABSOLUTE_X);
            return true;
        case(0xB9): // load acc abs,Y    
            LDA(AddressMode::ABSOLUTE_Y);
            return true;
        case(0xA1): // load acc AddressMode::INDIRECT,X    
            LDA(AddressMode::INDIRECT_X);
            return true;
        case(0xB1): // load acc AddressMode::INDIRECT,Y    
            LDA(AddressMode::INDIRECT_Y);
            return true;


        //////////////////////    
        // LDX INSTRUCTIONS // 
        //////////////////////


        case(0xA2): // load X imm
            LDX(AddressMode::IMMEDIATE);
            return true;
        case(0xA6): // load X from zp    
            LDX(AddressMode::ZEROPAGE);
            return true;
        case(0xB6): // load X from zp,Y    
            LDX(AddressMode::ZEROPAGE_Y);
            return true;
        case(0xAE):    
            LDX(AddressMode::ABSOLUTE);
            return true;
        case(0xBE):    
            LDX(AddressMode::ABSOLUTE_Y);
            return true;



        //////////////////////    
        // LDY INSTRUCTIONS // 
        //////////////////////


        case(0xA0): // load Y imm
            LDY(AddressMode::IMMEDIATE);
            return true;
        case(0xA4): // load Y from zp    
            LDY(AddressMode::ZEROPAGE);
            return true;
        case(0xB4): // load Y from zp,X    
            LDY(AddressMode::ZEROPAGE_X);
            return true;
        case(0xAC):    
            LDY(AddressMode::ABSOLUTE);
            return true;
        case(0xBC):    
            LDY(AddressMode::ABSOLUTE_X);
            return true;



        //////////////////////    
        // STA INSTRUCTIONS // 
        //////////////////////

        case(0x85):
            STA(AddressMode::ZEROPAGE);
            return true;
        case(0x95):    
            STA(AddressMode::ZEROPAGE_X);
            return true;
        case(0x8D):    
            STA(AddressMode::ABSOLUTE);
            return true;
        case(0x9D):    
            STA(AddressMode::ABSOLUTE_X);
            return true;
        case(0x99):    
            STA(AddressMode::ABSOLUTE_Y);
            return true;
        case(0x81):    
            STA(AddressMode::INDIRECT_X);
            return true;
        case(0x91):    
            STA(AddressMode::INDIRECT_Y);
            return true;


        //////////////////////    
        // STX INSTRUCTIONS // 
        //////////////////////

        case(0x86):
            STX(AddressMode::ZEROPAGE);
            return true;
        case(0x96):    
            STX(AddressMode::ZEROPAGE_Y);
            return true;
        case(0x8E):    
            STX(AddressMode::ABSOLUTE);
            return true;


        //////////////////////    
        // STY INSTRUCTIONS // 
        //////////////////////

        case(0x84):
            STY(AddressMode::ZEROPAGE);
            return true;
        case(0x94):    
            STY(AddressMode::ZEROPAGE_X);
            return true;
        case(0x8C):    
            STY(AddressMode::ABSOLUTE);
            return true;


        // INX //    
        case(0xE8):
            INX();
            return true;

        // DEX //     
        case(0xCA):
            DEX();
            return true;

        // INY //    
        case(0xC8):
            INY();
            return true;

        // DEY //     
        case(0x88):
            DEY();
            return true;


        // INC //
        case(0xE6):
            INC(AddressMode::ZEROPAGE);
            return true;
        case(0xF6):
            INC(AddressMode::ZEROPAGE_X);
            return true;
        case(0xEE):
            INC(AddressMode::ABSOLUTE);
            return true;
        case(0xFE):
            INC(AddressMode::ABSOLUTE_X);
            return true;

        // DEC //
        case(0xC6):
            DEC(AddressMode::ZEROPAGE);
            return true;
        case(0xD6):
            DEC(AddressMode::ZEROPAGE_X);
            return true;
        case(0xCE):
            DEC(AddressMode::ABSOLUTE);
            return true;
        case(0xDE):
            DEC(AddressMode::ABSOLUTE_X);
            return true;
        
        //////////////////////////
        // TRANSFER ACCUMULATOR //
        //////////////////////////

        // TAX //    
        case(0xAA):
            TAX();
            return true;

        // TXA //    
        case(0x8A):
            TXA();
            return true;

        // TYA //
        case(0x98):
            TYA();
            return true;

        // TAY //
        case(0xA8):
            TAY();
            return true;

        // TXS //
        case(0x9A):
            TXS();
            return true;

        // TSX //
        case(0xBA):
            TSX();
            return true;


        ////////////////////////////
        // BRANCHING INSTRUCTIONS //
        ////////////////////////////
        
        // BEQ //    
        case(0xF0):
            BEQ();
            return true;
            
        // BNE //    
        case(0xD0):
            BNE();
            return true;

        // BMI //    
        case(0x30):
            BMI();
            return true;
        // BPL //    
        case(0x10):
            BPL();
            return true;

        // BCC // 
        case(0x90):
            BCC();
            return true;
        
        // BCS // 
        case(0xB0):
            BCS();
            return true;
        
        // BVC //
        case(0x50):
            BVC();
            return true;
        
        // BVS //
        case(0x70):
            BVS();
            return true;

        ///////////////////////
        // JUMP INSTRUCTIONS //
        ///////////////////////

        // JMP //     
        case(0x4C):
            JMP(AddressMode::ABSOLUTE);
            return true;
        case(0x6C):    
            JMP(AddressMode::INDIRECT);
            return true;

        // JSR //    
        case(0x20):
            JSR();
            return true;

        
        // RTS //    
        case(0x60):
            RTS();
            return true;

        // BRK //
        case(0x00):
            BRK();
            return true;
        
        case(0x40):
            RTI();
            return true;
        
        // PHA //    
        case(0x48):
            PHA();
            return true;
        // PHP //    
        case(0x08):
            PHP();
            return true;
        // PLP //     
        case(0x28):
            PLP();
            return true;
        // PLA //    
        case(0x68):
            PLA();
            return true;

        // CMP //
        case(0xC9):
            CMP(AddressMode::IMMEDIATE);
            return true;
        case(0xC5):
            CMP(AddressMode::ZEROPAGE);
            return true;
        case(0xD5):
            CMP(AddressMode::ZEROPAGE_X);
            return true;
        case(0xCD):
            CMP(AddressMode::ABSOLUTE);
            return true;
        case(0xDD):
            CMP(AddressMode::ABSOLUTE_X);
            return true;
        case(0xD9):
            CMP(AddressMode::ABSOLUTE_Y);
            return true;
        case(0xC1):
            CMP(AddressMode::INDIRECT_X);
            return true;
        case(0xD1):
            CMP(AddressMode::INDIRECT_Y);
            return true;

        
        // CPX //
        case(0xE0):
            CPX(AddressMode::IMMEDIATE);
            return true;
        case(0xE4):
            CPX(AddressMode::ZEROPAGE);
            return true;
        case(0xEC):
            CPX(AddressMode::ABSOLUTE);
            return true;

            
        // CPY //
        case(0xC0):
            CPY(AddressMode::IMMEDIATE);
            return true;
        case(0xC4):
            CPY(AddressMode::ZEROPAGE);
            return true;
        case(0xCC):
            CPY(AddressMode::ABSOLUTE);
            return true;


        // CLC //
        case(0x18):
            CLC();
            return true;
        
        // SEC //
        case(0x38):
            SEC();
            return true;

        // CLD //
        case(0xD8):
            CLD();
            return true;

        // CLI //
        case(0x58):
            CLI();
            return true;
        
        case(0x78):
            SEI();
            return true;

        case(0xF8):
            SED();
            return true;
        
        // CLV //
        case(0xB8):
            CLV();
            return true;
    
        // BIT //
        case(0x24):
            BIT(AddressMode::ZEROPAGE);
            return true;
        case(0x2C):
            BIT(AddressMode::ABSOLUTE);
            return true;
        
        // ADC //    
        case(0x69):
            if(GetStatusFlag(Flag::DEC)) DecADC(AddressMode::IMMEDIATE);
            else ADC(AddressMode::IMMEDIATE);
            return true;
        case(0x65):
            if(GetStatusFlag(Flag::DEC)) DecADC(AddressMode::ZEROPAGE);    
            else ADC(AddressMode::ZEROPAGE);
            return true;
        case(0x75):    
            if(GetStatusFlag(Flag::DEC)) DecADC(AddressMode::ZEROPAGE_X);
            else ADC(AddressMode::ZEROPAGE_X);
            return true;
        case(0x6D):
            if(GetStatusFlag(Flag::DEC)) DecADC(AddressMode::ABSOLUTE);    
            else ADC(AddressMode::ABSOLUTE);
            return true;
        case(0x7D):
            if(GetStatusFlag(Flag::DEC)) DecADC(AddressMode::ABSOLUTE_X);    
            else ADC(AddressMode::ABSOLUTE_X);
            return true;
        case(0x79):
            if(GetStatusFlag(Flag::DEC)) DecADC(AddressMode::ABSOLUTE_Y);    
            else ADC(AddressMode::ABSOLUTE_Y);
            return true;
        case(0x61):
            if(GetStatusFlag(Flag::DEC)) DecADC(AddressMode::INDIRECT_X);    
            else ADC(AddressMode::INDIRECT_X);
            return true;
        case(0x71):
            if(GetStatusFlag(Flag::DEC)) DecADC(AddressMode::INDIRECT_Y);
            else ADC(AddressMode::INDIRECT_Y);
            return true;

        // SBC //    
        case(0xE9):
            if(GetStatusFlag(Flag::DEC)) DecSBC(AddressMode::IMMEDIATE);
            else SBC(AddressMode::IMMEDIATE);
            return true;
        case(0xE5):
            if(GetStatusFlag(Flag::DEC)) DecSBC(AddressMode::ZEROPAGE);    
            else SBC(AddressMode::ZEROPAGE);
            return true;
        case(0xF5):
            if(GetStatusFlag(Flag::DEC)) DecSBC(AddressMode::ZEROPAGE_X);    
            else SBC(AddressMode::ZEROPAGE_X);
            return true;
        case(0xED):
            if(GetStatusFlag(Flag::DEC)) DecSBC(AddressMode::ABSOLUTE);    
            else SBC(AddressMode::ABSOLUTE);
            return true;
        case(0xFD):
            if(GetStatusFlag(Flag::DEC)) DecSBC(AddressMode::ABSOLUTE_X);    
            else SBC(AddressMode::ABSOLUTE_X);
            return true;
        case(0xF9):
            if(GetStatusFlag(Flag::DEC)) DecSBC(AddressMode::ABSOLUTE_Y);    
            else SBC(AddressMode::ABSOLUTE_Y);
            return true;
        case(0xE1):
            if(GetStatusFlag(Flag::DEC)) DecSBC(AddressMode::INDIRECT_X);    
            else SBC(AddressMode::INDIRECT_X);
            return true;
        case(0xF1):
            if(GetStatusFlag(Flag::DEC)) DecSBC(AddressMode::INDIRECT_Y);    
            else SBC(AddressMode::INDIRECT_Y);
            return true;


        // ASL //
        case(0x0A):
            ASL(AddressMode::ACCUMULATOR);
            return true;
        case(0x06):
            ASL(AddressMode::ZEROPAGE);
            return true;
        case(0x16):
            ASL(AddressMode::ZEROPAGE_X);
            return true;
        case(0x0E):
            ASL(AddressMode::ABSOLUTE);
            return true;
        case(0x1E):
            ASL(AddressMode::ABSOLUTE_X);
            return true;
        
        // LSR //
        case(0x4A):
            LSR(AddressMode::ACCUMULATOR);
            return true;
        case(0x46):
            LSR(AddressMode::ZEROPAGE);
            return true;
        case(0x56):
            LSR(AddressMode::ZEROPAGE_X);
            return true;
        case(0x4E):
            LSR(AddressMode::ABSOLUTE);
            return true;
        case(0x5E):
            LSR(AddressMode::ABSOLUTE_X);
            return true;

        // ROL //
        case(0x2A):
            ROL(AddressMode::ACCUMULATOR);
            return true;
        case(0x26):
            ROL(AddressMode::ZEROPAGE);
            return true;
        case(0x36):
            ROL(AddressMode::ZEROPAGE_X);
            return true;
        case(0x2E):
            ROL(AddressMode::ABSOLUTE);
            return true;
        case(0x3E):
            ROL(AddressMode::ABSOLUTE_X);
            return true;


        // ROR //
        case(0x6A):
            ROR(AddressMode::ACCUMULATOR);
            return true;
        case(0x66):
            ROR(AddressMode::ZEROPAGE);
            return true;
        case(0x76):
            ROR(AddressMode::ZEROPAGE_X);
            return true;
        case(0x6E):
            ROR(AddressMode::ABSOLUTE);
            return true;
        case(0x7E):
            ROR(AddressMode::ABSOLUTE_X);
            return true;




        // EOR //
        case(0x49):
            EOR(AddressMode::IMMEDIATE);
            return true;
        case(0x45):
            EOR(AddressMode::ZEROPAGE);
            return true;
        case(0x55):
            EOR(AddressMode::ZEROPAGE_X);
            return true;
        case(0x4D):
            EOR(AddressMode::ABSOLUTE);
            return true;
        case(0x5D):
            EOR(AddressMode::ABSOLUTE_X);
            return true;
        case(0x59):
            EOR(AddressMode::ABSOLUTE_Y);
            return true;
        case(0x41):
            EOR(AddressMode::INDIRECT_X);
            return true;
        case(0x51):
            EOR(AddressMode::INDIRECT_Y);
            return true;

        // ORA //
        case(0x09):
            ORA(AddressMode::IMMEDIATE);
            return true;
        case(0x05):
            ORA(AddressMode::ZEROPAGE);
            return true;
        case(0x15):
            ORA(AddressMode::ZEROPAGE_X);
            return true;
        case(0x0D):
            ORA(AddressMode::ABSOLUTE);
            return true;
        case(0x1D):
            ORA(AddressMode::ABSOLUTE_X);
            return true;
        case(0x19):
            ORA(AddressMode::ABSOLUTE_Y);
            return true;
        case(0x01):
            ORA(AddressMode::INDIRECT_X);
            return true;
        case(0x11):
            ORA(AddressMode::INDIRECT_Y);
            return true;


        // AND // 
        case(0x29):
            AND(AddressMode::IMMEDIATE);
            return true;
        case(0x25):
            AND(AddressMode::ZEROPAGE);
            return true;
        case(0x35):
            AND(AddressMode::ZEROPAGE_X);
            return true;
        case(0x2D):
            AND(AddressMode::ABSOLUTE);
            return true;
        case(0x3D):
            AND(AddressMode::ABSOLUTE_X);
            return true;
        case(0x39):
            AND(AddressMode::ABSOLUTE_Y);
            return true;
        case(0x21):
            AND(AddressMode::INDIRECT_X);
            return true;
        case(0x31):
            AND(AddressMode::INDIRECT_Y);
            return true;
        

        // NOP //
        case(0xEA):
            return true;
        


        default:
            std::cerr << std::endl;    
            std::cerr << "\033[31mInvalid opcode!\033[0m" << std::endl;
            std::cerr << "PC = " << std::hex << pc-1 << std::endl;
            std::cerr << "Opcode = " << std::hex << +bus.ReadByte(pc-1) << std::endl;
            std::cerr << std::endl;
            return false;
    }        
}    



























































































































