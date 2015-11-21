################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=
LINKER_SCRIPT_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS := 


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../main.c \
../MCEMapper.c


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
main.o \
MCEMapper.o

OBJS_AS_ARGS +=  \
main.o \
MCEMapper.o

C_DEPS +=  \
main.d \
MCEMapper.d

C_DEPS_AS_ARGS +=  \
main.d \
MCEMapper.d

OUTPUT_FILE_PATH +=PCRemote.elf

OUTPUT_FILE_PATH_AS_ARGS +=PCRemote.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+=  \
C:/Users/Bernd/Documents/Atmel\ Studio/7.0/DebugLogger/DebugLogger/Release/libDebugLogger.a \
C:/Users/Bernd/Documents/Atmel\ Studio/7.0/PCRemote/IRReceiver/Release/libIRReceiver.a \
C:/Users/Bernd/Documents/Atmel\ Studio/7.0/PCRemote/PS2KBDevice/Release/libPS2KBDevice.a

LINKER_SCRIPT_DEP+= 


# AVR32/GNU C Compiler





./%.o: .././%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.9.2
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DNDEBUG  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\Atmel\ATmega_DFP\1.0.91\include"  -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -mmcu=atmega328p -B "C:\Program Files (x86)\Atmel\Studio\7.0\Packs\Atmel\ATmega_DFP\1.0.91\gcc\dev\atmega328p" -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	



# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP) $(LINKER_SCRIPT_DEP)
	@echo Building target: $@
	@echo Invoking: AVR/GNU Linker : 4.9.2
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="PCRemote.map" -Wl,--start-group -Wl,-lm -Wl,-lprintf_min -Wl,-lscanf_min -Wl,-lDebugLogger -Wl,-lIRReceiver -Wl,-lPS2KBDevice  -Wl,--end-group -Wl,-L"C:\Users\Bernd\Documents\Atmel Studio\7.0\DebugLogger\DebugLogger\Release" -Wl,-L"C:\Users\Bernd\Documents\Atmel Studio\7.0\PCRemote\IRReceiver\Release" -Wl,-L"C:\Users\Bernd\Documents\Atmel Studio\7.0\PCRemote\PS2KBDevice\Release"  -Wl,--gc-sections -mmcu=atmega328p -B "C:\Program Files (x86)\Atmel\Studio\7.0\Packs\Atmel\ATmega_DFP\1.0.91\gcc\dev\atmega328p"  
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures  "PCRemote.elf" "PCRemote.hex"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "PCRemote.elf" "PCRemote.eep" || exit 0
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objdump.exe" -h -S "PCRemote.elf" > "PCRemote.lss"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures "PCRemote.elf" "PCRemote.srec"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-size.exe" "PCRemote.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "PCRemote.elf" "PCRemote.a" "PCRemote.hex" "PCRemote.lss" "PCRemote.eep" "PCRemote.map" "PCRemote.srec" "PCRemote.usersignatures"
	