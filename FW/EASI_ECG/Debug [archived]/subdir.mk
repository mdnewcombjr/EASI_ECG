################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ads1294.c \
../easi_gpio.c \
../easi_pll.c \
../easi_rtc.c \
../easi_spi.c \
../easi_uart.c \
../main.c 

CMD_SRCS += \
../lnkx.cmd 

OBJS += \
./ads1294.obj \
./easi_gpio.obj \
./easi_pll.obj \
./easi_rtc.obj \
./easi_spi.obj \
./easi_uart.obj \
./main.obj 

C_DEPS += \
./ads1294.pp \
./easi_gpio.pp \
./easi_pll.pp \
./easi_rtc.pp \
./easi_spi.pp \
./easi_uart.pp \
./main.pp 


# Each subdirectory must supply rules for building sources it contributes
ads1294.obj: ../ads1294.c $(GEN_SRCS) $(GEN_OPTS)
	@echo 'Building file: $<'
	@echo 'Invoking: C5500 Compiler'
	"C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c5500/bin/cl55" -v5505 -g --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c5500/include" --include_path="C:/c55xx_csl_3.00/inc" --include_path="" --diag_warning=225 --sat_reassoc=off --ptrdiff_size=32 --fp_reassoc=off --memory_model=large --asm_source=mnemonic $(GEN_OPTS_QUOTED) --preproc_with_compile --preproc_dependency="ads1294.pp" $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

easi_gpio.obj: ../easi_gpio.c $(GEN_SRCS) $(GEN_OPTS)
	@echo 'Building file: $<'
	@echo 'Invoking: C5500 Compiler'
	"C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c5500/bin/cl55" -v5505 -g --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c5500/include" --include_path="C:/c55xx_csl_3.00/inc" --include_path="" --diag_warning=225 --sat_reassoc=off --ptrdiff_size=32 --fp_reassoc=off --memory_model=large --asm_source=mnemonic $(GEN_OPTS_QUOTED) --preproc_with_compile --preproc_dependency="easi_gpio.pp" $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

easi_pll.obj: ../easi_pll.c $(GEN_SRCS) $(GEN_OPTS)
	@echo 'Building file: $<'
	@echo 'Invoking: C5500 Compiler'
	"C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c5500/bin/cl55" -v5505 -g --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c5500/include" --include_path="C:/c55xx_csl_3.00/inc" --include_path="" --diag_warning=225 --sat_reassoc=off --ptrdiff_size=32 --fp_reassoc=off --memory_model=large --asm_source=mnemonic $(GEN_OPTS_QUOTED) --preproc_with_compile --preproc_dependency="easi_pll.pp" $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

easi_rtc.obj: ../easi_rtc.c $(GEN_SRCS) $(GEN_OPTS)
	@echo 'Building file: $<'
	@echo 'Invoking: C5500 Compiler'
	"C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c5500/bin/cl55" -v5505 -g --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c5500/include" --include_path="C:/c55xx_csl_3.00/inc" --include_path="" --diag_warning=225 --sat_reassoc=off --ptrdiff_size=32 --fp_reassoc=off --memory_model=large --asm_source=mnemonic $(GEN_OPTS_QUOTED) --preproc_with_compile --preproc_dependency="easi_rtc.pp" $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

easi_spi.obj: ../easi_spi.c $(GEN_SRCS) $(GEN_OPTS)
	@echo 'Building file: $<'
	@echo 'Invoking: C5500 Compiler'
	"C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c5500/bin/cl55" -v5505 -g --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c5500/include" --include_path="C:/c55xx_csl_3.00/inc" --include_path="" --diag_warning=225 --sat_reassoc=off --ptrdiff_size=32 --fp_reassoc=off --memory_model=large --asm_source=mnemonic $(GEN_OPTS_QUOTED) --preproc_with_compile --preproc_dependency="easi_spi.pp" $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

easi_uart.obj: ../easi_uart.c $(GEN_SRCS) $(GEN_OPTS)
	@echo 'Building file: $<'
	@echo 'Invoking: C5500 Compiler'
	"C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c5500/bin/cl55" -v5505 -g --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c5500/include" --include_path="C:/c55xx_csl_3.00/inc" --include_path="" --diag_warning=225 --sat_reassoc=off --ptrdiff_size=32 --fp_reassoc=off --memory_model=large --asm_source=mnemonic $(GEN_OPTS_QUOTED) --preproc_with_compile --preproc_dependency="easi_uart.pp" $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

main.obj: ../main.c $(GEN_SRCS) $(GEN_OPTS)
	@echo 'Building file: $<'
	@echo 'Invoking: C5500 Compiler'
	"C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c5500/bin/cl55" -v5505 -g --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c5500/include" --include_path="C:/c55xx_csl_3.00/inc" --include_path="" --diag_warning=225 --sat_reassoc=off --ptrdiff_size=32 --fp_reassoc=off --memory_model=large --asm_source=mnemonic $(GEN_OPTS_QUOTED) --preproc_with_compile --preproc_dependency="main.pp" $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '


