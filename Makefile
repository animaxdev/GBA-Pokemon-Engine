#
# Template tonc makefile
#
# Yoinked mostly from DKP's template
#

# === SETUP ===========================================================

# --- No implicit rules ---
.SUFFIXES:

# --- Tonc paths ---
# If not defined as environment variable, assumed to be 2 dirs up
export TONCCODE	?= $(CURDIR)/../tonc-code/code/

include $(TONCCODE)/tonc_rules

%.o : %.s
	@echo $(notdir $<)
	$(AS) -I $(dir $<) $(ASFLAGS) -c $< -o $@

# --- Main path ---

export PATH	:=	$(DEVKITARM)/bin:$(PATH)


# === PROJECT DETAILS =================================================
# PROJ		: Base project name
# TITLE		: Title for ROM header (12 characters)
# LIBS		: Libraries to use, formatted as list for linker flags
# BUILD		: Directory for build process temporaries. Should NOT be empty!
# SRCDIRS	: List of source file directories
# DATADIRS	: List of data file directories
# INCDIRS	: List of header file directories
# LIBDIRS	: List of library directories
# General note: use `.' for the current dir, don't leave the lists empty.

export PROJ	?= $(notdir $(CURDIR))
# TITLE should be 12 characters in length, though it can be shorter
TITLE		:= POKEMON_EMPT
# CODE must be 4 characters in length
CODE		:= GAME
GFXLIBS		:=

ifeq ($(strip $(bSpriteSet)), 1)
SPRITELIB	:= GBA
else
SPRITELIB	:= DS
endif

LIBS		:= -ltonc -laudio -ltiles -lsprites$(SPRITELIB) -lbattlescripts -lbattleanimscripts -loverworldscripts -lbattletranscripts

BUILD		:= build
SRCDIRS		:= source source/Allocation source/Tasks source/Tasks/ScriptRunners source/Tasks/Memory source/Callbacks source/Text source/Core source/Core/Data source/Core/Pokemon source/Audio source/Audio/GameBoySounds source/Audio/M4A source/Audio/M4A/Controllers source/Entities source/Scenes source/Scenes/Battles source/Scenes/Menus source/Scenes/Misc source/Scenes/Overworld source/Input source/Input/Battles source/Input/Menus source/Input/Misc source/Input/Overworld source/Data source/Data/TitleScreen source/Data/Tilesets
DATADIRS	:= 
INCDIRS		:= include
LIBDIRS		:= $(TONCCODE)/tonclib $(CURDIR)

#---------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# INCLUDES is a list of directories containing extra header files
# AUDIO is a list of directories containing sound and music files
#---------------------------------------------------------------------------------
TARGET   :=  $(shell basename $(CURDIR))
BUILD    :=  build
SOURCES  :=  gfx source data
INCLUDES :=  include build
AUDIO    :=  audio

CFILES    :=  $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES  :=  $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES    :=  $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
BINFILES  :=  $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.bin)))

# build audio file list, include full path
export AUDIOFILES := $(foreach dir,$(notdir $(wildcard $(AUDIO)/*.*)),$(CURDIR)/$(AUDIO)/$(dir))

# --- switches ---

bMB		:= 0	# Multiboot build
bTEMPS	:= 0	# Save gcc temporaries (.i and .s files)
bDEBUG	:= 0	# Generate debug info (bDEBUG2? Not a full DEBUG flag. Yet)

# === BUILD FLAGS =====================================================
# This is probably where you can stop editing
# NOTE: I've noticed that -fgcse and -ftree-loop-optimize sometimes muck 
#	up things (gcse seems fond of building masks inside a loop instead of 
#	outside them for example). Removing them sometimes helps

# --- Architecture ---

ARCH    := -mthumb-interwork -mthumb
RARCH   := -mthumb-interwork -mthumb
IARCH   := -mthumb-interwork -marm -mlong-calls

# --- Main flags ---

ifeq ($(strip $(bDEBUG)), 2)

CFLAGS		:= -mcpu=arm7tdmi -mtune=arm7tdmi -O3
CFLAGS		+= -Wall
CFLAGS		+= $(INCLUDE)
CFLAGS		+=

CXXFLAGS	:= $(CFLAGS) -fno-exceptions
CXXFLAGS	+= -fno-rtti

ASFLAGS		:= $(ARCH) $(INCLUDE)
LDFLAGS 	:= $(ARCH) -Wl,-Map,$(PROJ).map

else ifeq ($(strip $(bDEBUG)), 1)

CFLAGS		:= -mcpu=arm7tdmi -mtune=arm7tdmi -O3
CFLAGS		+= -Wall
CFLAGS		+= $(INCLUDE)
CFLAGS		+=

CXXFLAGS	:= $(CFLAGS) -fno-exceptions
CXXFLAGS	+= -fno-rtti

ASFLAGS		:= $(ARCH) $(INCLUDE)
LDFLAGS 	:= $(ARCH) -Wl,-Map,$(PROJ).map

else

CFLAGS		:= -mcpu=arm7tdmi -mtune=arm7tdmi -O3
CFLAGS		+= -Wall
CFLAGS		+= $(INCLUDE)
CFLAGS		+= -ffast-math -fno-strict-aliasing

CXXFLAGS	:= $(CFLAGS) -fno-exceptions
CXXFLAGS	+= -fno-rtti

ASFLAGS		:= $(ARCH) $(INCLUDE)
LDFLAGS 	:= $(ARCH) -Wl,-Map,$(PROJ).map
endif


# --- switched additions ----------------------------------------------

# --- Multiboot ? ---
ifeq ($(strip $(bMB)), 1)
	TARGET	:= $(PROJ).mb
else
	
	
ifeq ($(strip $(bDEBUG)), 2)
	TARGET	:= $(PROJ)_debug_VBA
else ifeq ($(strip $(bDEBUG)), 1)
	TARGET	:= $(PROJ)_debug_Eclipse
else
	TARGET	:= $(PROJ)
endif

endif

# --- Save temporary files ? ---
ifeq ($(strip $(bTEMPS)), 1)
	CFLAGS		+= -save-temps
	CXXFLAGS	+= -save-temps
endif

# --- Debug info ? ---

ifeq ($(strip $(bDEBUG)), 2)
	CFLAGS		+= -DNDEBUG
	CXXFLAGS	+= -DNDEBUG
#	ASFLAGS		+= -DNDEBUG
else ifeq ($(strip $(bDEBUG)), 1)
	CFLAGS		+= -DDEBUG -g
	CXXFLAGS	+= -DDEBUG -g
	ASFLAGS		+=	-g
#	ASFLAGS		+= -DDEBUG
	LDFLAGS		+= -g
else
	CFLAGS		+= -DNDEBUG
	CXXFLAGS	+= -DNDEBUG
#	ASFLAGS		+= -DNDEBUG
endif



# === BUILD PROC ======================================================

ifneq ($(BUILD),$(notdir $(CURDIR)))

# Still in main dir: 
# * Define/export some extra variables
# * Invoke this file again from the build dir
# PONDER: what happens if BUILD == "" ?

export OUTPUT	:=	$(CURDIR)/$(TARGET)
export VPATH	:=									\
	$(foreach dir, $(SRCDIRS) , $(CURDIR)/$(dir))	\
	$(foreach dir, $(DATADIRS), $(CURDIR)/$(dir))

export DEPSDIR	:=	$(CURDIR)/$(BUILD)

# --- List source and data files ---

CFILES		:=	$(foreach dir, $(SRCDIRS) , $(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=	$(foreach dir, $(SRCDIRS) , $(notdir $(wildcard $(dir)/*.cpp)))
SFILES		:=	$(foreach dir, $(SRCDIRS) , $(notdir $(wildcard $(dir)/*.s)))
BINFILES	:=	$(foreach dir, $(DATADIRS), $(notdir $(wildcard $(dir)/*.*)))

# --- Set linker depending on C++ file existence ---
ifeq ($(strip $(CPPFILES)),)
	export LD	:= $(CC)
else
	export LD	:= $(CXX)
endif

# --- Define object file list ---
export OFILES	:=	$(addsuffix .o, $(BINFILES))					\
					$(CFILES:.c=.o) $(CPPFILES:.cpp=.o)				\
					$(SFILES:.s=.o)

# --- Create include and library search paths ---
export INCLUDE	:=	$(foreach dir,$(INCDIRS),-I$(CURDIR)/$(dir))	\
					$(foreach dir,$(LIBDIRS),-I$(dir)/include)		\
					-I$(CURDIR)/$(BUILD)
 
export LIBPATHS	:=	-L$(CURDIR) $(foreach dir,$(LIBDIRS),-L$(dir)/lib)

# --- More targets ----------------------------------------------------

.PHONY: $(BUILD) clean

# --- Create BUILD if necessary, and run this makefile from there ---

$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@make --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile
	arm-none-eabi-nm -Sn $(OUTPUT).elf > $(BUILD)/$(TARGET).map
	gbafix $(TARGET).gba -p -c$(CODE) -m00
all	: $(BUILD)

clean:
	@echo clean ...
	@rm -fr $(BUILD) $(TARGET).elf $(TARGET).gba


else		# If we're here, we should be in the BUILD dir

DEPENDS	:=	$(OFILES:.o=.d)

# --- Main targets ----

$(OUTPUT).gba	:	$(OUTPUT).elf
$(OUTPUT).elf	:   $(OFILES)

-include $(DEPENDS)


endif		# End BUILD switch

#-------------------------------------------------------------
# rule for converting the output into an object file
#-------------------------------------------------------------
%.bin.o:	%.bin
#-------------------------------------------------------------
	@$(bin2o)

#-------------------------------------------------------------
# rule for generating soundbank file from audio files
#-------------------------------------------------------------
soundbank.bin:	$(AUDIOFILES)
#-------------------------------------------------------------
	@mmutil $^ -osoundbank.bin -hsoundbank.h

# EOF
