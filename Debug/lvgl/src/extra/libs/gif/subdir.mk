################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lvgl/src/extra/libs/gif/gifdec.c \
../lvgl/src/extra/libs/gif/lv_gif.c 

OBJS += \
./lvgl/src/extra/libs/gif/gifdec.o \
./lvgl/src/extra/libs/gif/lv_gif.o 

C_DEPS += \
./lvgl/src/extra/libs/gif/gifdec.d \
./lvgl/src/extra/libs/gif/lv_gif.d 


# Each subdirectory must supply rules for building sources it contributes
lvgl/src/extra/libs/gif/%.o lvgl/src/extra/libs/gif/%.su lvgl/src/extra/libs/gif/%.cyclo: ../lvgl/src/extra/libs/gif/%.c lvgl/src/extra/libs/gif/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H750xx -c -I../Core/Inc -I../FATFS/Target -I../FATFS/App -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Utilities/JPEG -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I"E:/STM32_Project/CubeIDE/h750_fatfs/lvgl" -I"E:/STM32_Project/CubeIDE/h750_fatfs/lvgl/porting" -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lvgl-2f-src-2f-extra-2f-libs-2f-gif

clean-lvgl-2f-src-2f-extra-2f-libs-2f-gif:
	-$(RM) ./lvgl/src/extra/libs/gif/gifdec.cyclo ./lvgl/src/extra/libs/gif/gifdec.d ./lvgl/src/extra/libs/gif/gifdec.o ./lvgl/src/extra/libs/gif/gifdec.su ./lvgl/src/extra/libs/gif/lv_gif.cyclo ./lvgl/src/extra/libs/gif/lv_gif.d ./lvgl/src/extra/libs/gif/lv_gif.o ./lvgl/src/extra/libs/gif/lv_gif.su

.PHONY: clean-lvgl-2f-src-2f-extra-2f-libs-2f-gif

