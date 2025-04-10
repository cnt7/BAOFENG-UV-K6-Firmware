AS = arm-none-eabi-gcc
CC = arm-none-eabi-gcc
LD = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

GIT_HASH := $(shell git rev-parse --short HEAD)

ASFLAGS = -mcpu=cortex-m0 -mthumb -c

CFLAGS = -mcpu=cortex-m0 -mthumb -c -Os -ffunction-sections -fdata-sections -Wall -save-temps=obj
CFLAGS += -DGIT_HASH=\"$(GIT_HASH)\"

LDFLAGS = -mcpu=cortex-m0 -Os -Wl,--gc-sections,--relax -mthumb --specs=nosys.specs --specs=nano.specs -Wl,-Map,linkmap.map -Wl,-T,kd32f328xb.ld

TARGET = bf_uvk6

ifeq ($(OS),Windows_NT)
TOP := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))
else
TOP := $(shell pwd)
endif

INC =
INC += -I $(TOP)/src
INC += -I $(TOP)/src/Libraries/CMSIS/Include
INC += -I $(TOP)/src/Libraries/CMSIS/Device
INC += -I $(TOP)/src/Libraries/StdPeriph_Driver/inc
INC += -I $(TOP)/src/Voice
INC += -I $(TOP)/src/Interface
INC += -I $(TOP)/src/Gui
INC += -I $(TOP)/src/Driver
INC += -I $(TOP)/src/CPS
INC += -I $(TOP)/src/Core
INC += -I $(TOP)/src/Common
INC += -I $(TOP)/src/BSP
INC += -I $(TOP)/src/App

OBJS =
# Startup files
OBJS += src/Libraries/CMSIS/Device/support/gcc/startup_kd32f328xb.o
OBJS += src/Libraries/CMSIS/Device/system_kd32f328.o
# OBJS += src/syscalls.o

# StdPeriph_Driver
OBJS += src/Libraries/StdPeriph_Driver/src/KD32f328_adc.o
OBJS += src/Libraries/StdPeriph_Driver/src/KD32f328_crc.o
OBJS += src/Libraries/StdPeriph_Driver/src/KD32f328_dma.o
OBJS += src/Libraries/StdPeriph_Driver/src/KD32f328_exti.o
OBJS += src/Libraries/StdPeriph_Driver/src/KD32f328_flash.o
OBJS += src/Libraries/StdPeriph_Driver/src/KD32f328_gpio.o
OBJS += src/Libraries/StdPeriph_Driver/src/KD32f328_iwdg.o
OBJS += src/Libraries/StdPeriph_Driver/src/KD32f328_misc.o
OBJS += src/Libraries/StdPeriph_Driver/src/KD32f328_pwr.o
OBJS += src/Libraries/StdPeriph_Driver/src/KD32f328_rcc.o
OBJS += src/Libraries/StdPeriph_Driver/src/KD32f328_spi.o
OBJS += src/Libraries/StdPeriph_Driver/src/KD32f328_tim.o
OBJS += src/Libraries/StdPeriph_Driver/src/KD32f328_usart.o
OBJS += src/Libraries/StdPeriph_Driver/src/KD32f328_wwdg.o

# Common
OBJS += src/Common/Delay.o
OBJS += src/Common/Globe.o
OBJS += src/Common/BitMap.o

# Core
OBJS += src/Core/Functions.o
OBJS += src/Core/Radio.o
OBJS += src/Core/RadioTask.o

# CPS
OBJS += src/CPS/ProgromFlash.o

# Driver
OBJS += src/Driver/crc.o
OBJS += src/Driver/DevFD6818.o
OBJS += src/Driver/DevFD6818Data.o
OBJS += src/Driver/FlashFont.o
OBJS += src/Driver/key_ptt.o
OBJS += src/Driver/keyboard.o
OBJS += src/Driver/NorFlash.o
OBJS += src/Driver/RadioDataReset.o
OBJS += src/Driver/RadioDataStorage.o
OBJS += src/Driver/Rda5807.o
OBJS += src/Driver/Sc5260.o
OBJS += src/Driver/Systick.o

# Gui
OBJS += src/Gui/DisplayBattery.o
OBJS += src/Gui/DisplayCheckPassword.o
OBJS += src/Gui/DisplayDtmf.o
OBJS += src/Gui/DisplayFm.o
OBJS += src/Gui/DisplayInputbox.o
OBJS += src/Gui/DisplayMain.o
OBJS += src/Gui/DisplayMenu.o
OBJS += src/Gui/DisplayPowerOn.o
OBJS += src/Gui/DisplayScanQT.o
OBJS += src/Gui/DisplaySearch.o
OBJS += src/Gui/DisplayWeather.o
OBJS += src/Gui/LcdFillDot.o

# Interface
OBJS += src/Interface/i2c.o

# Voice
OBJS += src/Voice/Beep.o
OBJS += src/Voice/VoiceBroadcast.o

# BSP
OBJS += src/BSP/Board.o
OBJS += src/BSP/BoardFun.o
OBJS += src/BSP/kd32f328_it.o

# App
OBJS += src/App/AppAlarm.o
OBJS += src/App/AppDtmf.o
OBJS += src/App/AppFm.o
OBJS += src/App/AppMain.o
OBJS += src/App/AppMenu.o
OBJS += src/App/AppMoni.o
OBJS += src/App/AppScan.o
OBJS += src/App/AppScanQT.o
OBJS += src/App/AppSearch.o
OBJS += src/App/AppStopWatch.o
OBJS += src/App/AppTask.o
OBJS += src/App/AppWeather.o
OBJS += src/App/Battery.o
OBJS += src/App/DualStandby.o
OBJS += src/App/main.o



all: $(TARGET)
	$(OBJCOPY) -O binary $< $<.bin
	$(OBJCOPY) -O ihex $< $<.hex
	$(SIZE) -B -d $<

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) $^ -o $@ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

%.o: %.S
	$(AS) $(ASFLAGS) $< -o $@

.FORCE:

-include $(DEPS)

rebuild: 
	rm -f $(TARGET).bin $(TARGET)
	@make all

clean:
	rm -f $(TARGET).bin $(TARGET).hex $(TARGET) $(OBJS) $(DEPS)
	@find . -type f -name '*.i' -delete
	@find . -name '*.s' -not -path './src/Libraries/CMSIS/Device/support/*' -delete
	rm -rf linkmap.map