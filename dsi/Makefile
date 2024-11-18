# SPDX-License-Identifier: GPL-2.0-or-later
# Standard preamble
ifeq ($(strip $(DEVKITARM)),)
$(error "DEVKITARM is not set, invalid developer environment.")
endif
include $(DEVKITARM)/ds_rules

# Configurable parameters
TARGET 			:= 	$(shell basename $(CURDIR)/..)
BUILD 			:= 	build
SOURCES 		:= 	src
INCLUDES 		:=
ARCH			:=	-mthumb -mthumb-interwork -march=armv5te -mtune=arm946e-s

CFLAGS			:=	-Wall							\
                    -Werror 						\
                    -O2								\
                    -fomit-frame-pointer			\
                    -ffast-math						\
                    $(ARCH)
CFLAGS 			+= 	$(INCLUDE)						\
					-DARM9
CXXFLAGS		:=	$(CFLAGS)						\
					-fno-rtti						\
					-fno-exceptions
ASFLAGS			:=	$(ARCH)
LDFLAGS			=	-specs=ds_arm9.specs			\
					$(ARCH)							\
					-Wl,-Map,$(notdir $*.map)
LIBS			:=	-ldswifi9 -lnds9
LIBDIRS			:=	$(LIBNDS)

# Metadata
GAME_TITLE		:=	DSCar Controller
GAME_SUBTITLE1	:=	DSCar Team 2024
GAME_SUBTITLE2	:=	GitHub amyipdev/dscar
GAME_ICON		:= 	$(CURDIR)/../logo.bmp

# Standard suffix
ifneq ($(BUILD),$(notdir $(CURDIR)))
export OUTPUT	:=	$(CURDIR)/$(TARGET)
export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir))
export DEPSDIR	:=	$(CURDIR)/$(BUILD)
CFILES			:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES			:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
BINFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.bin)))

ifeq ($(strip $(CPPFILES)),)
export LD		:=	$(CC)
else
export LD		:=	$(CXX)
endif

export OFILES	:=	$(BINFILES:.bin=.o) 								\
					$(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)
export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) 		\
					$(foreach dir,$(LIBDIRS),-I$(dir)/include) 			\
					-I$(CURDIR)/$(BUILD)
export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib)

.PHONY: $(BUILD) clean
$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile
clean:
	@echo clean ...
	@rm -fr $(BUILD) $(TARGET).elf $(TARGET).nds $(TARGET).ds.gba

else
DEPENDS			:=	$(OFILES:.o=.d)
$(OUTPUT).nds: $(OUTPUT).elf
$(OUTPUT).elf: $(OFILES)
%.bin.o: %.bin
	@echo $(notdir $<)
	$(bin2o)
-include $(DEPENDS)
endif

bear:
	$(MAKE) -C . clean
	bear -- make
