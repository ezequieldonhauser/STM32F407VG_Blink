########################################################
# Autor:
#	Ezequiel Donhauser
#
# Descrição:
#	Este arquivo é o script de compilaçao
#	para o microcontrolador STM32F407VG,
#	este arquivo inclui o arquivo "compile.conf"
#	onde esta listado os arquivos .c a serem
#	compilados. Para adicionar arquivos a serem
#	compilados, ou diretorios de includes, coloqueos
#	na lista dentro do arquivo "compile.conf"
########################################################

#captura o diretorio atual do projeto
DIR=$(CURDIR)

#nome do arquivo que vai ser gravado no microcontrolador
BIN=build/firmware.bin

#nome do objeto final ja linkado
ELF=build/output.elf

#nome do arquivo dump para analise
DMP=build/output.dmp

#nome da ferramenta de compilacao
TOOLCHAIN=arm-none-eabi

#nome do arquivo linker script
LD_SCRIPT=stm32f407vg.ld

#opçoes de compilação para arquivos individualmente (gcc)
#-O2
GCC_OPTIONS=-Wall -mcpu=cortex-m4 -mthumb -mthumb-interwork -mlittle-endian \
	    -std=gnu89 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -Wno-pointer-sign \
	    -fno-strict-aliasing -fconserve-stack -Werror=implicit-int	    \
	     -fno-strict-overflow -fno-optimize-sibling-calls		    \
	    -fno-var-tracking-assignments -Wno-unused-but-set-variable	    \
	    -fno-stack-protector --param=allow-store-data-races=0	    \
	    -Wframe-larger-than=1024 -mno-sched-prolog -Wno-format-security \
	    -fomit-frame-pointer

#opçoes de compilação para arquivos individualmente (as)
ASM_OPTIONS=-mcpu=cortex-m4 -mthumb -mthumb-interwork -mlittle-endian

#opçoes de compilação para objeto final (linker)
LD_OPTIONS= -EL --gc-sections -Bstatic -Lsrc/libs			    \

LD_LIBS=

#cria lista de objetos apartir dos arquivos .c
OBJECT_FILES=$(patsubst %.c,%.o,$(SOURCE_FILES))
COMPILED_FILES=$(notdir $(OBJECT_FILES))
INC_DIR=$(addprefix -I,$(INCLUDES))

#inclui o arquivos de configuração
include compile.conf

DEFS=-DUSE_STDPERIPH_DRIVER -DSTM32F4XX -DHSE_VALUE=8000000UL \
	-DUSE_FULL_ASSERT -DMOD_MTHOMAS_STMLIB -DVECT_TAB_FLASH \
	-DFLASH_RUN -DSTM32F4xx_1024k -DSTM32F4DISCOVERY -DARM_MATH_CM4 \
	-D__FPU_PRESENT=1 -D__FPU_USED=1

#inicia processo de compilação
all: teste startup $(OBJECT_FILES)
	$(TOOLCHAIN)-ld -T$(LD_SCRIPT) $(LD_OPTIONS) -o $(ELF) build/obj/startup.o $(addprefix build/obj/,$(COMPILED_FILES)) $(LD_LIBS)
	$(TOOLCHAIN)-objcopy -j.NVIC_Table -j.NVIC_Handler -j.text -j.rodata -j.data -O binary $(ELF) $(BIN)
	$(TOOLCHAIN)-objdump -a -f -h -x -D $(ELF) > $(DMP)
	@echo "----------------------------------------------------------------"
	$(TOOLCHAIN)-size $(ELF)
	@echo "----------------------------------------------------------------"
	@echo "Compilado com Sucesso!"

teste:
	@echo "----------------------------------------------------------------"
	@echo $(OBJECT_FILES)
	@echo "----------------------------------------------------------------"

checkdir:
	mkdir -p build/obj

#compila o arquivo startup.s
startup:
	mkdir -p build/obj
	$(TOOLCHAIN)-as $(ASM_OPTIONS) -o build/obj/startup.o src/system/startup.s
	
#compila todos os arquivos .c
%.o:
	$(TOOLCHAIN)-gcc $(INC_DIR) $(GCC_OPTIONS) $(DEFS) -c -o build/obj/$(notdir $*.o) $*.c

#remove todos os arquivos gerados pela compilação
clean:
	rm -f build/obj/*.o
	rm -f build/$(ELF)
	rm -f build/$(BIN)
