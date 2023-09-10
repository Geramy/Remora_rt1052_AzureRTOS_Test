################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include xip/subdir.mk
-include utilities/subdir.mk
-include startup/subdir.mk
-include source/drivers/nx_driver/gnu/subdir.mk
-include source/drivers/nx_driver/subdir.mk
-include source/subdir.mk
-include drivers/subdir.mk
-include device/subdir.mk
-include component/uart/subdir.mk
-include component/silicon_id/subdir.mk
-include component/serial_manager/subdir.mk
-include component/phyksz8081/subdir.mk
-include component/phy/subdir.mk
-include component/lists/subdir.mk
-include board/subdir.mk
-include azure-rtos/threadx/ports/cortex_m7/gnu/src/subdir.mk
-include azure-rtos/threadx/ports/common/subdir.mk
-include azure-rtos/netxduo/addons/web/subdir.mk
-include azure-rtos/netxduo/addons/tftp/subdir.mk
-include azure-rtos/netxduo/addons/telnet/subdir.mk
-include azure-rtos/netxduo/addons/sntp/subdir.mk
-include azure-rtos/netxduo/addons/snmp/subdir.mk
-include azure-rtos/netxduo/addons/smtp/subdir.mk
-include azure-rtos/netxduo/addons/ptp/subdir.mk
-include azure-rtos/netxduo/addons/pppoe/subdir.mk
-include azure-rtos/netxduo/addons/ppp/subdir.mk
-include azure-rtos/netxduo/addons/pop3/subdir.mk
-include azure-rtos/netxduo/addons/nat/subdir.mk
-include azure-rtos/netxduo/addons/mqtt/subdir.mk
-include azure-rtos/netxduo/addons/mdns/subdir.mk
-include azure-rtos/netxduo/addons/lwm2m/subdir.mk
-include azure-rtos/netxduo/addons/http/subdir.mk
-include azure-rtos/netxduo/addons/ftp/subdir.mk
-include azure-rtos/netxduo/addons/dns/subdir.mk
-include azure-rtos/netxduo/addons/dhcp/subdir.mk
-include azure-rtos/netxduo/addons/cloud/subdir.mk
-include azure-rtos/netxduo/addons/auto_ip/subdir.mk
-include azure-rtos/levelx/common/src/subdir.mk
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := remora_rt1052_AzureRTOS
BUILD_ARTIFACT_EXTENSION := axf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all:
	+@$(MAKE) --no-print-directory main-build && $(MAKE) --no-print-directory post-build

# Main-build Target
main-build: remora_rt1052_AzureRTOS.axf

# Tool invocations
remora_rt1052_AzureRTOS.axf: $(OBJS) $(USER_OBJS) makefile $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU C++ Linker'
	arm-none-eabi-c++ -nostdlib -L"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\binary\threadx\cortex_m7\mcux" -L"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\binary\netxduo\cortex_m7\mcux" -Xlinker -Map="remora_rt1052_AzureRTOS.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -Xlinker --sort-section=alignment -Xlinker --cref -mcpu=cortex-m7 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -T remora_rt1052_AzureRTOS_Debug.ld -o "remora_rt1052_AzureRTOS.axf" $(OBJS) $(USER_OBJS) $(LIBS) -lthreadx -lnetxduo
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) remora_rt1052_AzureRTOS.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "remora_rt1052_AzureRTOS.axf"; # arm-none-eabi-objcopy -v -O binary "remora_rt1052_AzureRTOS.axf" "remora_rt1052_AzureRTOS.bin" ; # checksum -p MIMXRT1052xxxxB -d "remora_rt1052_AzureRTOS.bin";
	-@echo ' '

.PHONY: all clean dependents main-build post-build

-include ../makefile.targets
