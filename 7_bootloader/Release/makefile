################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
7_bootloader.lss \

FLASH_IMAGE += \
7_bootloader.hex \

EEPROM_IMAGE += \
7_bootloader.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: 7_bootloader.elf secondary-outputs

dependents:
	-cd C:/Users/Jiadong/OneDrive/Master/1.2/Embedded\ system/workspace/ses/Debug && $(MAKE) all

# Tool invocations
7_bootloader.elf: $(OBJS) $(USER_OBJS) C:/Users/Jiadong/OneDrive/Master/1.2/Embedded\ system/workspace/ses/Debug/libses.a
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,7_bootloader.map -Wl,-section-start=.text=0x1E000 -L"C:\Users\Jiadong\OneDrive\Master\1.2\Embedded system\workspace\ses\Debug" -L"C:\Users\Jiadong\OneDrive\Master\1.2\Embedded system\workspace\ses" -mmcu=atmega128rfa1 -o "7_bootloader.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

7_bootloader.lss: 7_bootloader.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S 7_bootloader.elf  >"7_bootloader.lss"
	@echo 'Finished building: $@'
	@echo ' '

7_bootloader.hex: 7_bootloader.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex 7_bootloader.elf  "7_bootloader.hex"
	@echo 'Finished building: $@'
	@echo ' '

7_bootloader.eep: 7_bootloader.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex 7_bootloader.elf  "7_bootloader.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: 7_bootloader.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega128rfa1 7_bootloader.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) 7_bootloader.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:
C:/Users/Jiadong/OneDrive/Master/1.2/Embedded\ system/workspace/ses/Debug/libses.a:

-include ../makefile.targets
