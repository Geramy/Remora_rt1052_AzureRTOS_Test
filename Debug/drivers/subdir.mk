################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/fsl_cache.c \
../drivers/fsl_clock.c \
../drivers/fsl_common.c \
../drivers/fsl_common_arm.c \
../drivers/fsl_dmamux.c \
../drivers/fsl_edma.c \
../drivers/fsl_enet.c \
../drivers/fsl_flexspi.c \
../drivers/fsl_gpio.c \
../drivers/fsl_gpt.c \
../drivers/fsl_lpuart.c \
../drivers/fsl_lpuart_cmsis.c \
../drivers/fsl_lpuart_edma.c \
../drivers/fsl_pit.c \
../drivers/fsl_pwm.c \
../drivers/fsl_qtmr.c \
../drivers/fsl_tempmon.c 

C_DEPS += \
./drivers/fsl_cache.d \
./drivers/fsl_clock.d \
./drivers/fsl_common.d \
./drivers/fsl_common_arm.d \
./drivers/fsl_dmamux.d \
./drivers/fsl_edma.d \
./drivers/fsl_enet.d \
./drivers/fsl_flexspi.d \
./drivers/fsl_gpio.d \
./drivers/fsl_gpt.d \
./drivers/fsl_lpuart.d \
./drivers/fsl_lpuart_cmsis.d \
./drivers/fsl_lpuart_edma.d \
./drivers/fsl_pit.d \
./drivers/fsl_pwm.d \
./drivers/fsl_qtmr.d \
./drivers/fsl_tempmon.d 

OBJS += \
./drivers/fsl_cache.o \
./drivers/fsl_clock.o \
./drivers/fsl_common.o \
./drivers/fsl_common_arm.o \
./drivers/fsl_dmamux.o \
./drivers/fsl_edma.o \
./drivers/fsl_enet.o \
./drivers/fsl_flexspi.o \
./drivers/fsl_gpio.o \
./drivers/fsl_gpt.o \
./drivers/fsl_lpuart.o \
./drivers/fsl_lpuart_cmsis.o \
./drivers/fsl_lpuart_edma.o \
./drivers/fsl_pit.o \
./drivers/fsl_pwm.o \
./drivers/fsl_qtmr.o \
./drivers/fsl_tempmon.o 


# Each subdirectory must supply rules for building sources it contributes
drivers/%.o: ../drivers/%.c drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__NEWLIB__ -DCPU_MIMXRT1052CVL5B -DCPU_MIMXRT1052CVL5B_cm7 -DSDK_OS_BAREMETAL -DXIP_EXTERNAL_FLASH=1 -DXIP_BOOT_HEADER_ENABLE=1 -DSERIAL_PORT_TYPE_UART=1 -DSDK_DEBUGCONSOLE=1 -DSDK_DEBUGCONSOLE_UART -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\board" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\source" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\xip" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\drivers" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\CMSIS_driver" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\CMSIS" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\device" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\component\serial_manager" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\component\uart" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\utilities" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\threadx\common\inc" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\threadx\ports\cortex_m7\gnu\inc" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\threadx\common_modules\inc" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\threadx\common_modules\module_manager\inc" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\threadx\ports_module\cortex_m7\gnu\inc" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\netxduo\addons\auto_ip" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\netxduo\addons\cloud" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\netxduo\addons\BSD" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\netxduo\addons\dhcp" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\netxduo\addons\dns" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\netxduo\addons\ftp" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\netxduo\addons\http" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\netxduo\addons\lwm2m" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\netxduo\addons\mdns" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\netxduo\addons\mqtt" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\netxduo\addons\nat" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\netxduo\addons\pop3" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\netxduo\addons\ppp" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\netxduo\addons\pppoe" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\netxduo\addons\ptp" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\netxduo\addons\smtp" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\netxduo\addons\snmp" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\netxduo\addons\sntp" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\netxduo\addons\telnet" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\netxduo\addons\tftp" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\netxduo\addons\web" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\netxduo\addons\websocket" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\netxduo\common\inc" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\netxduo\crypto_libraries\inc" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\netxduo\nx_secure\inc" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\netxduo\nx_secure\ports" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\netxduo\ports\cortex_m7\gnu\inc" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\filex\common\inc" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\filex\ports\cortex_m7\gnu\inc" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\levelx\common\inc" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\component\phyksz8081" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\component\lists" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\component\silicon_id" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m7 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -D__NEWLIB__ -fstack-usage -specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-drivers

clean-drivers:
	-$(RM) ./drivers/fsl_cache.d ./drivers/fsl_cache.o ./drivers/fsl_clock.d ./drivers/fsl_clock.o ./drivers/fsl_common.d ./drivers/fsl_common.o ./drivers/fsl_common_arm.d ./drivers/fsl_common_arm.o ./drivers/fsl_dmamux.d ./drivers/fsl_dmamux.o ./drivers/fsl_edma.d ./drivers/fsl_edma.o ./drivers/fsl_enet.d ./drivers/fsl_enet.o ./drivers/fsl_flexspi.d ./drivers/fsl_flexspi.o ./drivers/fsl_gpio.d ./drivers/fsl_gpio.o ./drivers/fsl_gpt.d ./drivers/fsl_gpt.o ./drivers/fsl_lpuart.d ./drivers/fsl_lpuart.o ./drivers/fsl_lpuart_cmsis.d ./drivers/fsl_lpuart_cmsis.o ./drivers/fsl_lpuart_edma.d ./drivers/fsl_lpuart_edma.o ./drivers/fsl_pit.d ./drivers/fsl_pit.o ./drivers/fsl_pwm.d ./drivers/fsl_pwm.o ./drivers/fsl_qtmr.d ./drivers/fsl_qtmr.o ./drivers/fsl_tempmon.d ./drivers/fsl_tempmon.o

.PHONY: clean-drivers

