/******************************************************
Autor:
	Ezequiel Donhauser

Descrição:
	Este arquivo é o script que controla o linker
	para gerar um objeto espesifico para o
	microcontrolador STM32F407VG, este arquivo
	informa ao linker como colocar blocos de
	codigos em seus devidos lugares, a saida
	do objeto final linkado "output.elf" será
	usado para extrair um binario "firmware.bin"
	que será gravado no microcontrolador.
******************************************************/

OUTPUT_ARCH(arm)
ENTRY(user_main)

/* mapa de memoria STM32F407VG */
MEMORY
{
  FLASH (rx)      : ORIGIN = 0x08000000, LENGTH = 1024K
  RAM (xrw)       : ORIGIN = 0x20000000, LENGTH = 128K
  MEMORY_B1 (rx)  : ORIGIN = 0x60000000, LENGTH = 0K
}

/* ordem das sections na memoria */
SECTIONS
{
    .NVIC_Table :
    {
        KEEP(*(.NVIC_Table));
        . = ALIGN(4);
    } >FLASH
	
    .NVIC_Handler :
    {
        KEEP(*(.NVIC_Handler*));
        . = ALIGN(4);
    } >FLASH
	
    .text :
    {
        *(.text.*)
	. = ALIGN(4);
    } >FLASH

    .rodata :
    {
        *(.rodata*);
        . = ALIGN(4);
    } >FLASH

    _sidata = .;
    .data : AT (_sidata)
    {
        _sdata = .;
        *(.data*);
        . = ALIGN(4);
        _edata = .;
    } >RAM
	
    .bss :
    {
        _sbss = .; 
        *(.bss*)
        . = ALIGN(4);
        _ebss = .; 
    } >RAM

    .debug 0 : { *(.debug*) }
    .ARM.attributes 0 : { *(.ARM.attributes) }
    .comment 0 : { *(.comment) }
}
