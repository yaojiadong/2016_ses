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
3_button.lss \

FLASH_IMAGE += \
3_button.hex \

SIZEDUMMY += \
sizedummy \


# All Target
all: 3_button.elf secondary-outputs

dependents:
	-cd C:/Users/Jiadong/OneDrive/Master/1.2/Embedded\ system/workspace/ses/Debug && $(MAKE) clean all

# Tool invocations
3_button.elf: $(OBJS) $(USER_OBJS) C:/Users/Jiadong/OneDrive/Master/1.2/Embedded\ system/workspace/ses/Debug/libses.a
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,3_button.map -L"C:\Users\Jiadong\OneDrive\Master\1.2\Embedded system\workspace\ses\Debug" -L"C:\Users\Jiadong\OneDrive\Master\1.2\Embedded system\workspace\ses" -mmcu=atmega128rfa1 -o "3_button.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

3_button.lss: 3_button.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S 3_button.elf  >"3_button.lss"
	@echo 'Finished building: $@'
	@echo ' '

3_button.hex: 3_button.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex 3_button.elf  "3_button.hex"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: 3_button.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega128rfa1 3_button.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) 3_button.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:
C:/Users/Jiadong/OneDrive/Master/1.2/Embedded\ system/workspace/ses/Debug/libses.a:

-include ../makefile.targets
