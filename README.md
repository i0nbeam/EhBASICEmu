# EhBASICEmu
This 6502 emulator has been tested against [Klaus Dormann's functional test](https://github.com/Klaus2m5/6502_65C02_functional_tests). Included in the asm/ folder is the EhBASIC code and monitor taken from [Klaus Dormann's EhBASIC repo](https://github.com/Klaus2m5/6502_EhBASIC_V2.22). 
## Building EhBASIC

Keyboard input is parsed through the Linux terminal in raw mode. As such, EhBASICEmu is only compatible with Linux at the moment. 

**Requirements:** CMake >3.17, compiler supporting C++17.

In the main directory, execute

```
mkdir build 
cd build
```

followed by

```
cmake ..
make
```

The executable gets dropped into ```../bin```. 

### Building the monitor

Tiny syntax changes were made to ```basic.asm``` and ```min_mon.asm``` to make it compile via ca65 with ```--feature labels_without_colons```. This consisted of changing ```*=``` directives with ```.SEGMENT``` directives. Also included in asm/ is my linker script to be used with ld65.

To compile EhBASIC, execute
```
ca65 min_mon.asm -o min_mon.o --feature labels_without_colons
ld65 -C linker.cfg min_mon.o -o min_mon.bin
mv min_mon.bin ../bin
```

EhBASIC must be run in the same directory as ```min_mon.bin``` for the program to work.

### Running EhBASIC
Simply execute ```./EhBASICEmu```. For help, execute ```./EhBASICEmu --help```. 
