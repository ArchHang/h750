################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lvgl/src/extra/libs/png/lodepng.c \
../lvgl/src/extra/libs/png/lv_png.c 

OBJS += \
./lvgl/src/extra/libs/png/lodepng.o \
./lvgl/src/extra/libs/png/lv_png.o 

C_DEPS += \
./lvgl/src/extra/libs/png/lodepng.d \
./lvgl/src/extra/libs/png/lv_png.d 


# Each subdirectory must supply rules for building sources it contributes
lvgl/src/extra/libs/png/%.o lvgl/src/extra/libs/png/%.su lvgl/src/extra/libs/png/%.cyclo: ../lvgl/src/extra/libs/png/%.c lvgl/src/extra/libs/png/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H750xx -c -I../Core/Inc -I../FATFS/Target -I../FATFS/App -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Utilities/JPEG -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I"E:/STM32_Project/CubeIDE/h750_fatfs/lvgl" -I"E:/STM32_Project/CubeIDE/h750_fatfs/lvgl/porting" -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lvgl-2f-src-2f-extra-2f-libs-2f-png

clean-lvgl-2f-src-2f-extra-2f-libs-2f-png:
	-$(RM) ./lvgl/src/extra/libs/png/lodepng.cyclo ./lvgl/src/extra/libs/png/lodepng.d ./lvgl/src/extra/libs/png/lodepng.o ./lvgl/src/extra/libs/png/lodepng.su ./lvgl/src/extra/libs/png/lv_png.cyclo ./lvgl/src/extra/libs/png/lv_png.d ./lvgl/src/extra/libs/png/lv_png.o ./lvgl/src/extra/libs/png/lv_png.su

.PHONY: clean-lvgl-2f-src-2f-extra-2f-libs-2f-png

