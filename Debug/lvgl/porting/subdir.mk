################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lvgl/porting/lv_port_disp.c \
../lvgl/porting/lv_port_fs.c \
../lvgl/porting/lv_port_indev.c 

OBJS += \
./lvgl/porting/lv_port_disp.o \
./lvgl/porting/lv_port_fs.o \
./lvgl/porting/lv_port_indev.o 

C_DEPS += \
./lvgl/porting/lv_port_disp.d \
./lvgl/porting/lv_port_fs.d \
./lvgl/porting/lv_port_indev.d 


# Each subdirectory must supply rules for building sources it contributes
lvgl/porting/%.o lvgl/porting/%.su lvgl/porting/%.cyclo: ../lvgl/porting/%.c lvgl/porting/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H750xx -c -I../Core/Inc -I../FATFS/Target -I../FATFS/App -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Utilities/JPEG -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I"E:/STM32_Project/CubeIDE/h750_fatfs/lvgl" -I"E:/STM32_Project/CubeIDE/h750_fatfs/lvgl/porting" -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lvgl-2f-porting

clean-lvgl-2f-porting:
	-$(RM) ./lvgl/porting/lv_port_disp.cyclo ./lvgl/porting/lv_port_disp.d ./lvgl/porting/lv_port_disp.o ./lvgl/porting/lv_port_disp.su ./lvgl/porting/lv_port_fs.cyclo ./lvgl/porting/lv_port_fs.d ./lvgl/porting/lv_port_fs.o ./lvgl/porting/lv_port_fs.su ./lvgl/porting/lv_port_indev.cyclo ./lvgl/porting/lv_port_indev.d ./lvgl/porting/lv_port_indev.o ./lvgl/porting/lv_port_indev.su

.PHONY: clean-lvgl-2f-porting

