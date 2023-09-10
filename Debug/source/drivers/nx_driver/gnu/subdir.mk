################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
../source/drivers/nx_driver/gnu/nx_driver_imxrt10xx_low_level.S 

OBJS += \
./source/drivers/nx_driver/gnu/nx_driver_imxrt10xx_low_level.o 


# Each subdirectory must supply rules for building sources it contributes
source/drivers/nx_driver/gnu/%.o: ../source/drivers/nx_driver/gnu/%.S source/drivers/nx_driver/gnu/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU Assembler'
	arm-none-eabi-gcc -c -x assembler-with-cpp -D__NEWLIB__ -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\board" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\source" -g3 -mcpu=cortex-m7 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -D__NEWLIB__ -specs=nano.specs -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source-2f-drivers-2f-nx_driver-2f-gnu

clean-source-2f-drivers-2f-nx_driver-2f-gnu:
	-$(RM) ./source/drivers/nx_driver/gnu/nx_driver_imxrt10xx_low_level.o

.PHONY: clean-source-2f-drivers-2f-nx_driver-2f-gnu

