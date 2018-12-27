ICBODY       := NY8A051D
CLIENT_NAME  := "MuShenLIn"
CB_FILE      := MuShenLIn.cb
CB_FILE_SP   := MuShenLIn.cb
HOST_TOOL	 := NYIDE 3.10
USER_C       := config.c F628.c main.c 
USER_ASM     := 
OUT_NAME	 := MuShenLIn
NYC_NY8_PATH := f:\Nyquest\NYC_NY8

#  - small: disable bank ram
#  - small_8b: disable bank ram (NY8B series)
#  - bank2: 2 bank ram
#  - bank2_8b: 2 bank ram (NY8B series)
#  - bank4: 4 bank ram
MODEL	:= small

ENABLE_INIT_BSS := 1

##################################################

CC := sdcc
AS := nyas
LD := nylinker
AR := nyar

LDFLAGS		:= -p $(ICBODY) --client $(CLIENT_NAME) -g -M --riffbin --elf --disasm --host "$(HOST_TOOL)" --cinit --no-programchecksum --codechecksum --opt-banksel 
CFLAGS 		:= -mny8 -p$(ICBODY) -D__CPU_$(ICBODY) --debug -I"$(NYC_NY8_PATH)/include" -I"../MuShenLIn" -I"Dependency Files" -I"f:\Nyquest\NYIDE\\include\NY8 C"  -MD --stack-size 16  --backup-stk 2
ASFLAGS 	:= -p $(ICBODY) -g -l -I "$(NYC_NY8_PATH)/inc"

DEPS		:= $(patsubst %.c,%.d, $(USER_C))
ASM_SRC		:= $(USER_ASM) $(patsubst %.c,%.s, $(USER_C))
USER_OBJ	:= $(patsubst %.s,%.o, $(ASM_SRC))
USER_OBJ	:= $(patsubst %.asm,%.o, $(USER_OBJ))
USER_OBJ	:= $(sort $(USER_OBJ))

IC_LIB		:= $(NYC_NY8_PATH)/lib/$(ICBODY).a
BIN_FILE	:= $(strip $(OUT_NAME)).bin

ifneq (,$(findstring small,$(MODEL)))
	COMMON_LIB	:= $(NYC_NY8_PATH)/lib/libsdcc_small.a
	CFG_FILE	:= $(NYC_NY8_PATH)/cfg/small/$(ICBODY).cfg
else
	COMMON_LIB	:= $(NYC_NY8_PATH)/lib/libsdcc_bankram.a
	CFG_FILE	:= $(NYC_NY8_PATH)/cfg/bankram/$(ICBODY).cfg
	ASFLAGS		+= -D ENABLE_RAM_BANK=1
endif

ifeq ($(ENABLE_INIT_BSS),1)
	LIB_STARTUP		:= $(NYC_NY8_PATH)/lib/startup/$(strip $(MODEL)).a
else
	LIB_STARTUP		:= $(NYC_NY8_PATH)/lib/startup/no-bss/$(strip $(MODEL)).a
endif

.SUFFIXES:
.SECONDARY:
.PHONY: all clean

all: $(BIN_FILE)

$(BIN_FILE): $(USER_OBJ) $(CB_FILE)
	$(LD) $(USER_OBJ) "$(IC_LIB)" "$(COMMON_LIB)" "$(LIB_STARTUP)" "$(CB_FILE_SP)" -s "$(CFG_FILE)" -o "$(OUT_NAME)" $(LDFLAGS)
#	nyreadelf -wL $(strip $(OUT_NAME)).elf > $(strip $(OUT_NAME))_dbg_line.txt

%.s: %.c
	$(CC) -S $< -o $@ $(CFLAGS)

%.o: %.s
	$(AS) $< $(ASFLAGS)

%.o: %.asm
	$(AS) $< $(ASFLAGS)
	
-include $(DEPS)

rebuild: clean all

clean:
	-@del "$(OUT_NAME).bin" "$(OUT_NAME).fpga" "$(OUT_NAME).elf" "$(OUT_NAME).link.lst" "$(OUT_NAME).map"
	-@del $(patsubst %.c, %.adb, $(USER_C))
	-@del $(patsubst %.c, %.p, $(USER_C))
	-@del $(patsubst %.c, %.d, $(USER_C))
	-@del $(patsubst %.c, %.s, $(USER_C))
	-@del $(addsuffix .o, $(basename $(ASM_SRC)))
	-@del $(addsuffix .lst, $(basename $(ASM_SRC)))

