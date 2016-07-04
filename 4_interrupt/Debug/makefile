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
4_interrupt.lss \

FLASH_IMAGE += \
4_interrupt.hex \

SIZEDUMMY += \
sizedummy \


# All Target
all: 4_interrupt.elf secondary-outputs

dependents:
	-cd C:/Users/Jiadong/OneDrive/Master/1.2/Embedded\ system/workspace/ses/Debug && $(MAKE) all

# Tool invocations
4_interrupt.elf: $(OBJS) $(USER_OBJS) C:/Users/Jiadong/OneDrive/Master/1.2/Embedded\ system/workspace/ses/Debug/libses.a
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,4_interrupt.map -L"C:\Users\Jiadong\OneDrive\Master\1.2\Embedded system\workspace\ses\Debug" -L"C:\Users\Jiadong\OneDrive\Master\1.2\Embedded system\workspace\ses" -mmcu=atmega128rfa1 -o "4_interrupt.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

4_interrupt.lss: 4_interrupt.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S 4_interrupt.elf  >"4_interrupt.lss"
	@echo 'Finished building: $@'
	@echo ' '

4_interrupt.hex: 4_interrupt.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex 4_interrupt.elf  "4_interrupt.hex"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: 4_interrupt.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega128rfa1 4_interrupt.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) 4_interrupt.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:
C:/Users/Jiadong/OneDrive/Master/1.2/Embedded\ system/workspace/ses/Debug/libses.a:

-include ../makefile.targets
