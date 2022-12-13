# This is an example Makefile to show how to build the library

COMPILER_ROOT = 
CMSIS_ROOT = 

# Compilation tools
CC := $(COMPILER_ROOT)/ARMCompiler6.18/bin/armclang
ARMAR :=  $(COMPILER_ROOT)/ARMCompiler6.18/bin/armar

# Compilation flags (here for Cortex-M55)
CFLAGS := -mcpu=cortex-m55 --target=arm-arm-none-eabi \
 -Wsign-compare \
 -Wdouble-promotion \
 -Ofast -ffast-math \
 -DNDEBUG \
 -Wall -Wextra  -Werror \
 -fshort-enums -fshort-wchar \
 -mfloat-abi=hard


# Path to CMSIS_5
CMSIS_5 := $(CMSIS_ROOT)/CMSIS_5

# Path to CMSIS_DSP
CMSIS_DSP := $(CMSIS_ROOT)/CMSIS-DSP

# Path to CMSIS Core includes for Cortex-M
# For low end Cortex-A, use Core_A
# For high end Cortex-A (aarch64), don't use CMSIS 
# Core Includes (Refer to the CMSIS-DSP README to 
# know how to build in that case)
CMSIS_CORE_INCLUDES := $(CMSIS_5)/CMSIS/Core/Include 

# Sources
SRCS := $(CMSIS_DSP)/Source/BasicMathFunctions/BasicMathFunctions.c \
 $(CMSIS_DSP)/Source/CommonTables/CommonTables.c \
 $(CMSIS_DSP)/Source/InterpolationFunctions/InterpolationFunctions.c \
 $(CMSIS_DSP)/Source/BayesFunctions/BayesFunctions.c \
 $(CMSIS_DSP)/Source/MatrixFunctions/MatrixFunctions.c \
 $(CMSIS_DSP)/Source/ComplexMathFunctions/ComplexMathFunctions.c \
 $(CMSIS_DSP)/Source/QuaternionMathFunctions/QuaternionMathFunctions.c \
 $(CMSIS_DSP)/Source/ControllerFunctions/ControllerFunctions.c \
 $(CMSIS_DSP)/Source/SVMFunctions/SVMFunctions.c \
 $(CMSIS_DSP)/Source/DistanceFunctions/DistanceFunctions.c \
 $(CMSIS_DSP)/Source/StatisticsFunctions/StatisticsFunctions.c \
 $(CMSIS_DSP)/Source/FastMathFunctions/FastMathFunctions.c \
 $(CMSIS_DSP)/Source/SupportFunctions/SupportFunctions.c \
 $(CMSIS_DSP)/Source/FilteringFunctions/FilteringFunctions.c \
 $(CMSIS_DSP)/Source/TransformFunctions/TransformFunctions.c \
 $(CMSIS_DSP)/Source/WindowFunctions/WindowFunctions.c

# Includes
DSP_INCLUDES := $(CMSIS_DSP)/Include \
  $(CMSIS_DSP)/PrivateInclude 

# If Neon and Cortex-A
#DSP_INCLUDES += $(CMSIS_DSP)/ComputeLibrary/Include 
#SRCS += $(CMSIS_DSP)/ComputeLibrary/Source/arm_cl_tables.c 

# Compilation flags for include folders
INC_FLAGS := $(addprefix -I,$(DSP_INCLUDES))
INC_FLAGS += $(addprefix -I,$(CMSIS_CORE_INCLUDES))
CFLAGS += $(INC_FLAGS)

# Output folder for build products
BUILDDIR := ./builddir

OBJECTS := $(SRCS:%=$(BUILDDIR)/%.o)

# Build rules
$(BUILDDIR)/libCMSISDSP.a: $(OBJECTS)
	$(ARMAR) -rc $@ $(OBJECTS)
	

$(BUILDDIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@
