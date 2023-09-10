################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../azure-rtos/levelx/common/src/fx_nand_flash_simulated_driver.c \
../azure-rtos/levelx/common/src/fx_nor_flash_simulator_driver.c \
../azure-rtos/levelx/common/src/lx_nand_flash_256byte_ecc_check.c \
../azure-rtos/levelx/common/src/lx_nand_flash_256byte_ecc_compute.c \
../azure-rtos/levelx/common/src/lx_nand_flash_block_full_update.c \
../azure-rtos/levelx/common/src/lx_nand_flash_block_obsoleted_check.c \
../azure-rtos/levelx/common/src/lx_nand_flash_block_reclaim.c \
../azure-rtos/levelx/common/src/lx_nand_flash_close.c \
../azure-rtos/levelx/common/src/lx_nand_flash_defragment.c \
../azure-rtos/levelx/common/src/lx_nand_flash_driver_block_erase.c \
../azure-rtos/levelx/common/src/lx_nand_flash_driver_block_erased_verify.c \
../azure-rtos/levelx/common/src/lx_nand_flash_driver_block_status_get.c \
../azure-rtos/levelx/common/src/lx_nand_flash_driver_block_status_set.c \
../azure-rtos/levelx/common/src/lx_nand_flash_driver_extra_bytes_get.c \
../azure-rtos/levelx/common/src/lx_nand_flash_driver_extra_bytes_set.c \
../azure-rtos/levelx/common/src/lx_nand_flash_driver_page_erased_verify.c \
../azure-rtos/levelx/common/src/lx_nand_flash_driver_read.c \
../azure-rtos/levelx/common/src/lx_nand_flash_driver_write.c \
../azure-rtos/levelx/common/src/lx_nand_flash_extended_cache_enable.c \
../azure-rtos/levelx/common/src/lx_nand_flash_initialize.c \
../azure-rtos/levelx/common/src/lx_nand_flash_logical_sector_find.c \
../azure-rtos/levelx/common/src/lx_nand_flash_next_block_to_erase_find.c \
../azure-rtos/levelx/common/src/lx_nand_flash_open.c \
../azure-rtos/levelx/common/src/lx_nand_flash_page_ecc_check.c \
../azure-rtos/levelx/common/src/lx_nand_flash_page_ecc_compute.c \
../azure-rtos/levelx/common/src/lx_nand_flash_partial_defragment.c \
../azure-rtos/levelx/common/src/lx_nand_flash_physical_page_allocate.c \
../azure-rtos/levelx/common/src/lx_nand_flash_sector_mapping_cache_invalidate.c \
../azure-rtos/levelx/common/src/lx_nand_flash_sector_read.c \
../azure-rtos/levelx/common/src/lx_nand_flash_sector_release.c \
../azure-rtos/levelx/common/src/lx_nand_flash_sector_write.c \
../azure-rtos/levelx/common/src/lx_nand_flash_simulator.c \
../azure-rtos/levelx/common/src/lx_nand_flash_system_error.c \
../azure-rtos/levelx/common/src/lx_nor_flash_block_reclaim.c \
../azure-rtos/levelx/common/src/lx_nor_flash_close.c \
../azure-rtos/levelx/common/src/lx_nor_flash_defragment.c \
../azure-rtos/levelx/common/src/lx_nor_flash_driver_block_erase.c \
../azure-rtos/levelx/common/src/lx_nor_flash_driver_read.c \
../azure-rtos/levelx/common/src/lx_nor_flash_driver_write.c \
../azure-rtos/levelx/common/src/lx_nor_flash_extended_cache_enable.c \
../azure-rtos/levelx/common/src/lx_nor_flash_initialize.c \
../azure-rtos/levelx/common/src/lx_nor_flash_logical_sector_find.c \
../azure-rtos/levelx/common/src/lx_nor_flash_next_block_to_erase_find.c \
../azure-rtos/levelx/common/src/lx_nor_flash_open.c \
../azure-rtos/levelx/common/src/lx_nor_flash_partial_defragment.c \
../azure-rtos/levelx/common/src/lx_nor_flash_physical_sector_allocate.c \
../azure-rtos/levelx/common/src/lx_nor_flash_sector_mapping_cache_invalidate.c \
../azure-rtos/levelx/common/src/lx_nor_flash_sector_read.c \
../azure-rtos/levelx/common/src/lx_nor_flash_sector_release.c \
../azure-rtos/levelx/common/src/lx_nor_flash_sector_write.c \
../azure-rtos/levelx/common/src/lx_nor_flash_simulator.c \
../azure-rtos/levelx/common/src/lx_nor_flash_system_error.c 

C_DEPS += \
./azure-rtos/levelx/common/src/fx_nand_flash_simulated_driver.d \
./azure-rtos/levelx/common/src/fx_nor_flash_simulator_driver.d \
./azure-rtos/levelx/common/src/lx_nand_flash_256byte_ecc_check.d \
./azure-rtos/levelx/common/src/lx_nand_flash_256byte_ecc_compute.d \
./azure-rtos/levelx/common/src/lx_nand_flash_block_full_update.d \
./azure-rtos/levelx/common/src/lx_nand_flash_block_obsoleted_check.d \
./azure-rtos/levelx/common/src/lx_nand_flash_block_reclaim.d \
./azure-rtos/levelx/common/src/lx_nand_flash_close.d \
./azure-rtos/levelx/common/src/lx_nand_flash_defragment.d \
./azure-rtos/levelx/common/src/lx_nand_flash_driver_block_erase.d \
./azure-rtos/levelx/common/src/lx_nand_flash_driver_block_erased_verify.d \
./azure-rtos/levelx/common/src/lx_nand_flash_driver_block_status_get.d \
./azure-rtos/levelx/common/src/lx_nand_flash_driver_block_status_set.d \
./azure-rtos/levelx/common/src/lx_nand_flash_driver_extra_bytes_get.d \
./azure-rtos/levelx/common/src/lx_nand_flash_driver_extra_bytes_set.d \
./azure-rtos/levelx/common/src/lx_nand_flash_driver_page_erased_verify.d \
./azure-rtos/levelx/common/src/lx_nand_flash_driver_read.d \
./azure-rtos/levelx/common/src/lx_nand_flash_driver_write.d \
./azure-rtos/levelx/common/src/lx_nand_flash_extended_cache_enable.d \
./azure-rtos/levelx/common/src/lx_nand_flash_initialize.d \
./azure-rtos/levelx/common/src/lx_nand_flash_logical_sector_find.d \
./azure-rtos/levelx/common/src/lx_nand_flash_next_block_to_erase_find.d \
./azure-rtos/levelx/common/src/lx_nand_flash_open.d \
./azure-rtos/levelx/common/src/lx_nand_flash_page_ecc_check.d \
./azure-rtos/levelx/common/src/lx_nand_flash_page_ecc_compute.d \
./azure-rtos/levelx/common/src/lx_nand_flash_partial_defragment.d \
./azure-rtos/levelx/common/src/lx_nand_flash_physical_page_allocate.d \
./azure-rtos/levelx/common/src/lx_nand_flash_sector_mapping_cache_invalidate.d \
./azure-rtos/levelx/common/src/lx_nand_flash_sector_read.d \
./azure-rtos/levelx/common/src/lx_nand_flash_sector_release.d \
./azure-rtos/levelx/common/src/lx_nand_flash_sector_write.d \
./azure-rtos/levelx/common/src/lx_nand_flash_simulator.d \
./azure-rtos/levelx/common/src/lx_nand_flash_system_error.d \
./azure-rtos/levelx/common/src/lx_nor_flash_block_reclaim.d \
./azure-rtos/levelx/common/src/lx_nor_flash_close.d \
./azure-rtos/levelx/common/src/lx_nor_flash_defragment.d \
./azure-rtos/levelx/common/src/lx_nor_flash_driver_block_erase.d \
./azure-rtos/levelx/common/src/lx_nor_flash_driver_read.d \
./azure-rtos/levelx/common/src/lx_nor_flash_driver_write.d \
./azure-rtos/levelx/common/src/lx_nor_flash_extended_cache_enable.d \
./azure-rtos/levelx/common/src/lx_nor_flash_initialize.d \
./azure-rtos/levelx/common/src/lx_nor_flash_logical_sector_find.d \
./azure-rtos/levelx/common/src/lx_nor_flash_next_block_to_erase_find.d \
./azure-rtos/levelx/common/src/lx_nor_flash_open.d \
./azure-rtos/levelx/common/src/lx_nor_flash_partial_defragment.d \
./azure-rtos/levelx/common/src/lx_nor_flash_physical_sector_allocate.d \
./azure-rtos/levelx/common/src/lx_nor_flash_sector_mapping_cache_invalidate.d \
./azure-rtos/levelx/common/src/lx_nor_flash_sector_read.d \
./azure-rtos/levelx/common/src/lx_nor_flash_sector_release.d \
./azure-rtos/levelx/common/src/lx_nor_flash_sector_write.d \
./azure-rtos/levelx/common/src/lx_nor_flash_simulator.d \
./azure-rtos/levelx/common/src/lx_nor_flash_system_error.d 

OBJS += \
./azure-rtos/levelx/common/src/fx_nand_flash_simulated_driver.o \
./azure-rtos/levelx/common/src/fx_nor_flash_simulator_driver.o \
./azure-rtos/levelx/common/src/lx_nand_flash_256byte_ecc_check.o \
./azure-rtos/levelx/common/src/lx_nand_flash_256byte_ecc_compute.o \
./azure-rtos/levelx/common/src/lx_nand_flash_block_full_update.o \
./azure-rtos/levelx/common/src/lx_nand_flash_block_obsoleted_check.o \
./azure-rtos/levelx/common/src/lx_nand_flash_block_reclaim.o \
./azure-rtos/levelx/common/src/lx_nand_flash_close.o \
./azure-rtos/levelx/common/src/lx_nand_flash_defragment.o \
./azure-rtos/levelx/common/src/lx_nand_flash_driver_block_erase.o \
./azure-rtos/levelx/common/src/lx_nand_flash_driver_block_erased_verify.o \
./azure-rtos/levelx/common/src/lx_nand_flash_driver_block_status_get.o \
./azure-rtos/levelx/common/src/lx_nand_flash_driver_block_status_set.o \
./azure-rtos/levelx/common/src/lx_nand_flash_driver_extra_bytes_get.o \
./azure-rtos/levelx/common/src/lx_nand_flash_driver_extra_bytes_set.o \
./azure-rtos/levelx/common/src/lx_nand_flash_driver_page_erased_verify.o \
./azure-rtos/levelx/common/src/lx_nand_flash_driver_read.o \
./azure-rtos/levelx/common/src/lx_nand_flash_driver_write.o \
./azure-rtos/levelx/common/src/lx_nand_flash_extended_cache_enable.o \
./azure-rtos/levelx/common/src/lx_nand_flash_initialize.o \
./azure-rtos/levelx/common/src/lx_nand_flash_logical_sector_find.o \
./azure-rtos/levelx/common/src/lx_nand_flash_next_block_to_erase_find.o \
./azure-rtos/levelx/common/src/lx_nand_flash_open.o \
./azure-rtos/levelx/common/src/lx_nand_flash_page_ecc_check.o \
./azure-rtos/levelx/common/src/lx_nand_flash_page_ecc_compute.o \
./azure-rtos/levelx/common/src/lx_nand_flash_partial_defragment.o \
./azure-rtos/levelx/common/src/lx_nand_flash_physical_page_allocate.o \
./azure-rtos/levelx/common/src/lx_nand_flash_sector_mapping_cache_invalidate.o \
./azure-rtos/levelx/common/src/lx_nand_flash_sector_read.o \
./azure-rtos/levelx/common/src/lx_nand_flash_sector_release.o \
./azure-rtos/levelx/common/src/lx_nand_flash_sector_write.o \
./azure-rtos/levelx/common/src/lx_nand_flash_simulator.o \
./azure-rtos/levelx/common/src/lx_nand_flash_system_error.o \
./azure-rtos/levelx/common/src/lx_nor_flash_block_reclaim.o \
./azure-rtos/levelx/common/src/lx_nor_flash_close.o \
./azure-rtos/levelx/common/src/lx_nor_flash_defragment.o \
./azure-rtos/levelx/common/src/lx_nor_flash_driver_block_erase.o \
./azure-rtos/levelx/common/src/lx_nor_flash_driver_read.o \
./azure-rtos/levelx/common/src/lx_nor_flash_driver_write.o \
./azure-rtos/levelx/common/src/lx_nor_flash_extended_cache_enable.o \
./azure-rtos/levelx/common/src/lx_nor_flash_initialize.o \
./azure-rtos/levelx/common/src/lx_nor_flash_logical_sector_find.o \
./azure-rtos/levelx/common/src/lx_nor_flash_next_block_to_erase_find.o \
./azure-rtos/levelx/common/src/lx_nor_flash_open.o \
./azure-rtos/levelx/common/src/lx_nor_flash_partial_defragment.o \
./azure-rtos/levelx/common/src/lx_nor_flash_physical_sector_allocate.o \
./azure-rtos/levelx/common/src/lx_nor_flash_sector_mapping_cache_invalidate.o \
./azure-rtos/levelx/common/src/lx_nor_flash_sector_read.o \
./azure-rtos/levelx/common/src/lx_nor_flash_sector_release.o \
./azure-rtos/levelx/common/src/lx_nor_flash_sector_write.o \
./azure-rtos/levelx/common/src/lx_nor_flash_simulator.o \
./azure-rtos/levelx/common/src/lx_nor_flash_system_error.o 


# Each subdirectory must supply rules for building sources it contributes
azure-rtos/levelx/common/src/%.o: ../azure-rtos/levelx/common/src/%.c azure-rtos/levelx/common/src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__NEWLIB__ -DCPU_MIMXRT1052CVL5B -DCPU_MIMXRT1052CVL5B_cm7 -DSDK_OS_BAREMETAL -DXIP_EXTERNAL_FLASH=1 -DXIP_BOOT_HEADER_ENABLE=1 -DSERIAL_PORT_TYPE_UART=1 -DSDK_DEBUGCONSOLE=1 -DSDK_DEBUGCONSOLE_UART -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\board" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\source" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\xip" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\drivers" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\CMSIS_driver" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\CMSIS" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\device" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\component\serial_manager" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\component\uart" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\utilities" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\threadx\common\inc" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\threadx\ports\cortex_m7\gnu\inc" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\threadx\common_modules\inc" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\threadx\common_modules\module_manager\inc" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\threadx\ports_module\cortex_m7\gnu\inc" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\netxduo\addons\auto_ip" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\netxduo\addons\cloud" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\netxduo\addons\BSD" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\netxduo\addons\dhcp" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\netxduo\addons\dns" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\netxduo\addons\ftp" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\netxduo\addons\http" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\netxduo\addons\lwm2m" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\netxduo\addons\mdns" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\netxduo\addons\mqtt" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\netxduo\addons\nat" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\netxduo\addons\pop3" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\netxduo\addons\ppp" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\netxduo\addons\pppoe" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\netxduo\addons\ptp" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\netxduo\addons\smtp" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\netxduo\addons\snmp" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\netxduo\addons\sntp" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\netxduo\addons\telnet" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\netxduo\addons\tftp" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\netxduo\addons\web" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\netxduo\addons\websocket" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\netxduo\common\inc" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\netxduo\crypto_libraries\inc" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\netxduo\nx_secure\inc" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\netxduo\nx_secure\ports" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\netxduo\ports\cortex_m7\gnu\inc" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\filex\common\inc" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\filex\ports\cortex_m7\gnu\inc" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\azure-rtos\levelx\common\inc" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\component\phyksz8081" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\component\lists" -I"C:\Users\Geramy\Documents\MCUXpressoIDE_11.7.1_9221\workspace\remora_rt1052_AzureRTOS\component\silicon_id" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m7 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -D__NEWLIB__ -fstack-usage -specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-azure-2d-rtos-2f-levelx-2f-common-2f-src

clean-azure-2d-rtos-2f-levelx-2f-common-2f-src:
	-$(RM) ./azure-rtos/levelx/common/src/fx_nand_flash_simulated_driver.d ./azure-rtos/levelx/common/src/fx_nand_flash_simulated_driver.o ./azure-rtos/levelx/common/src/fx_nor_flash_simulator_driver.d ./azure-rtos/levelx/common/src/fx_nor_flash_simulator_driver.o ./azure-rtos/levelx/common/src/lx_nand_flash_256byte_ecc_check.d ./azure-rtos/levelx/common/src/lx_nand_flash_256byte_ecc_check.o ./azure-rtos/levelx/common/src/lx_nand_flash_256byte_ecc_compute.d ./azure-rtos/levelx/common/src/lx_nand_flash_256byte_ecc_compute.o ./azure-rtos/levelx/common/src/lx_nand_flash_block_full_update.d ./azure-rtos/levelx/common/src/lx_nand_flash_block_full_update.o ./azure-rtos/levelx/common/src/lx_nand_flash_block_obsoleted_check.d ./azure-rtos/levelx/common/src/lx_nand_flash_block_obsoleted_check.o ./azure-rtos/levelx/common/src/lx_nand_flash_block_reclaim.d ./azure-rtos/levelx/common/src/lx_nand_flash_block_reclaim.o ./azure-rtos/levelx/common/src/lx_nand_flash_close.d ./azure-rtos/levelx/common/src/lx_nand_flash_close.o ./azure-rtos/levelx/common/src/lx_nand_flash_defragment.d ./azure-rtos/levelx/common/src/lx_nand_flash_defragment.o ./azure-rtos/levelx/common/src/lx_nand_flash_driver_block_erase.d ./azure-rtos/levelx/common/src/lx_nand_flash_driver_block_erase.o ./azure-rtos/levelx/common/src/lx_nand_flash_driver_block_erased_verify.d ./azure-rtos/levelx/common/src/lx_nand_flash_driver_block_erased_verify.o ./azure-rtos/levelx/common/src/lx_nand_flash_driver_block_status_get.d ./azure-rtos/levelx/common/src/lx_nand_flash_driver_block_status_get.o ./azure-rtos/levelx/common/src/lx_nand_flash_driver_block_status_set.d ./azure-rtos/levelx/common/src/lx_nand_flash_driver_block_status_set.o ./azure-rtos/levelx/common/src/lx_nand_flash_driver_extra_bytes_get.d ./azure-rtos/levelx/common/src/lx_nand_flash_driver_extra_bytes_get.o ./azure-rtos/levelx/common/src/lx_nand_flash_driver_extra_bytes_set.d ./azure-rtos/levelx/common/src/lx_nand_flash_driver_extra_bytes_set.o ./azure-rtos/levelx/common/src/lx_nand_flash_driver_page_erased_verify.d ./azure-rtos/levelx/common/src/lx_nand_flash_driver_page_erased_verify.o ./azure-rtos/levelx/common/src/lx_nand_flash_driver_read.d ./azure-rtos/levelx/common/src/lx_nand_flash_driver_read.o ./azure-rtos/levelx/common/src/lx_nand_flash_driver_write.d ./azure-rtos/levelx/common/src/lx_nand_flash_driver_write.o ./azure-rtos/levelx/common/src/lx_nand_flash_extended_cache_enable.d ./azure-rtos/levelx/common/src/lx_nand_flash_extended_cache_enable.o ./azure-rtos/levelx/common/src/lx_nand_flash_initialize.d ./azure-rtos/levelx/common/src/lx_nand_flash_initialize.o ./azure-rtos/levelx/common/src/lx_nand_flash_logical_sector_find.d ./azure-rtos/levelx/common/src/lx_nand_flash_logical_sector_find.o ./azure-rtos/levelx/common/src/lx_nand_flash_next_block_to_erase_find.d ./azure-rtos/levelx/common/src/lx_nand_flash_next_block_to_erase_find.o ./azure-rtos/levelx/common/src/lx_nand_flash_open.d ./azure-rtos/levelx/common/src/lx_nand_flash_open.o ./azure-rtos/levelx/common/src/lx_nand_flash_page_ecc_check.d ./azure-rtos/levelx/common/src/lx_nand_flash_page_ecc_check.o ./azure-rtos/levelx/common/src/lx_nand_flash_page_ecc_compute.d ./azure-rtos/levelx/common/src/lx_nand_flash_page_ecc_compute.o ./azure-rtos/levelx/common/src/lx_nand_flash_partial_defragment.d ./azure-rtos/levelx/common/src/lx_nand_flash_partial_defragment.o ./azure-rtos/levelx/common/src/lx_nand_flash_physical_page_allocate.d ./azure-rtos/levelx/common/src/lx_nand_flash_physical_page_allocate.o ./azure-rtos/levelx/common/src/lx_nand_flash_sector_mapping_cache_invalidate.d ./azure-rtos/levelx/common/src/lx_nand_flash_sector_mapping_cache_invalidate.o ./azure-rtos/levelx/common/src/lx_nand_flash_sector_read.d ./azure-rtos/levelx/common/src/lx_nand_flash_sector_read.o ./azure-rtos/levelx/common/src/lx_nand_flash_sector_release.d ./azure-rtos/levelx/common/src/lx_nand_flash_sector_release.o ./azure-rtos/levelx/common/src/lx_nand_flash_sector_write.d ./azure-rtos/levelx/common/src/lx_nand_flash_sector_write.o ./azure-rtos/levelx/common/src/lx_nand_flash_simulator.d ./azure-rtos/levelx/common/src/lx_nand_flash_simulator.o ./azure-rtos/levelx/common/src/lx_nand_flash_system_error.d ./azure-rtos/levelx/common/src/lx_nand_flash_system_error.o ./azure-rtos/levelx/common/src/lx_nor_flash_block_reclaim.d ./azure-rtos/levelx/common/src/lx_nor_flash_block_reclaim.o ./azure-rtos/levelx/common/src/lx_nor_flash_close.d ./azure-rtos/levelx/common/src/lx_nor_flash_close.o ./azure-rtos/levelx/common/src/lx_nor_flash_defragment.d ./azure-rtos/levelx/common/src/lx_nor_flash_defragment.o ./azure-rtos/levelx/common/src/lx_nor_flash_driver_block_erase.d ./azure-rtos/levelx/common/src/lx_nor_flash_driver_block_erase.o ./azure-rtos/levelx/common/src/lx_nor_flash_driver_read.d ./azure-rtos/levelx/common/src/lx_nor_flash_driver_read.o ./azure-rtos/levelx/common/src/lx_nor_flash_driver_write.d ./azure-rtos/levelx/common/src/lx_nor_flash_driver_write.o ./azure-rtos/levelx/common/src/lx_nor_flash_extended_cache_enable.d ./azure-rtos/levelx/common/src/lx_nor_flash_extended_cache_enable.o ./azure-rtos/levelx/common/src/lx_nor_flash_initialize.d ./azure-rtos/levelx/common/src/lx_nor_flash_initialize.o ./azure-rtos/levelx/common/src/lx_nor_flash_logical_sector_find.d ./azure-rtos/levelx/common/src/lx_nor_flash_logical_sector_find.o ./azure-rtos/levelx/common/src/lx_nor_flash_next_block_to_erase_find.d ./azure-rtos/levelx/common/src/lx_nor_flash_next_block_to_erase_find.o ./azure-rtos/levelx/common/src/lx_nor_flash_open.d ./azure-rtos/levelx/common/src/lx_nor_flash_open.o ./azure-rtos/levelx/common/src/lx_nor_flash_partial_defragment.d ./azure-rtos/levelx/common/src/lx_nor_flash_partial_defragment.o ./azure-rtos/levelx/common/src/lx_nor_flash_physical_sector_allocate.d ./azure-rtos/levelx/common/src/lx_nor_flash_physical_sector_allocate.o ./azure-rtos/levelx/common/src/lx_nor_flash_sector_mapping_cache_invalidate.d
	-$(RM) ./azure-rtos/levelx/common/src/lx_nor_flash_sector_mapping_cache_invalidate.o ./azure-rtos/levelx/common/src/lx_nor_flash_sector_read.d ./azure-rtos/levelx/common/src/lx_nor_flash_sector_read.o ./azure-rtos/levelx/common/src/lx_nor_flash_sector_release.d ./azure-rtos/levelx/common/src/lx_nor_flash_sector_release.o ./azure-rtos/levelx/common/src/lx_nor_flash_sector_write.d ./azure-rtos/levelx/common/src/lx_nor_flash_sector_write.o ./azure-rtos/levelx/common/src/lx_nor_flash_simulator.d ./azure-rtos/levelx/common/src/lx_nor_flash_simulator.o ./azure-rtos/levelx/common/src/lx_nor_flash_system_error.d ./azure-rtos/levelx/common/src/lx_nor_flash_system_error.o

.PHONY: clean-azure-2d-rtos-2f-levelx-2f-common-2f-src

