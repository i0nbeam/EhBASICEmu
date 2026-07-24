
#include "Emulator.hpp"
#include <string_view>



int main(int argc, char **argv)
{
    if(argc >= 2)
    {
        std::string_view arg(argv[1]);

        if(argc > 2 || arg != "--help")
        {
            std::cerr << "\033[31mERROR\033[0m : Unrecognized argument.\n";
            std::cerr << "\033[33mUSAGE\033[0m : ./basic\n";
            std::cerr << "\t./basic --help\n";
            return 1;
        }
        else
        {
            std::cerr << "\n\033[32mStarting EhBASIC:\033[0m\n";
            std::cerr << "This program must be run in the same folder as min_mon.asm. If you have your own monitor, please rename it\n";
            std::cerr << "to min_mon.bin so it can be loaded.\n\n";
            std::cerr << "When executing ./basic, choose Cold Start. Warm Start can only be done after a reset from within the program.\n\n";
            std::cerr << "\033[32mKeyboard Commands:\033[0m\n";
            std::cerr << "CTRL+C - Exit\n";
            std::cerr << "CTRL+R - Reset EhBASIC\n\n";
            return 1;
        }
        
    }
    

    Emulator emulator;

    bool loadSuccess = emulator.LoadROM("min_mon.bin");
    if(!loadSuccess) return 1;

    emulator.Run();

    return 0;
    
}


