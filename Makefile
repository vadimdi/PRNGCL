all: amd intel nvidia

CC = g++
CFLAGS = -Wall -W -O2

SRCS = PRNGCL.cpp \
	hgpucl/hgpucl.cpp \
	hgpucl/hgpucl_buffer.cpp \
	hgpucl/hgpucl_context.cpp \
	hgpucl/hgpucl_devices.cpp \
	hgpucl/hgpucl_error.cpp \
	hgpucl/hgpucl_kernel.cpp \
	hgpucl/hgpucl_platforms.cpp \
	hgpucl/hgpucl_program.cpp \
	src/hgpu_convert.cpp \
	src/hgpu_enum.cpp \
	src/hgpu_error.cpp \
	src/hgpu_io.cpp \
	src/hgpu_md5.cpp \
	src/hgpu_parameters.cpp \
	src/hgpu_string.cpp \
	src/hgpu_timer.cpp \
	randomcl/hgpu_prng.cpp \
	randomcl/hgpu_prng_test.cpp \
	randomcl/prngcl_xor128.cpp \
	randomcl/prngcl_xor7.cpp \
	randomcl/prngcl_ranmar.cpp \
	randomcl/prngcl_ranlux.cpp \
	randomcl/prngcl_ranecu.cpp \
	randomcl/prngcl_pm.cpp \
	randomcl/prngcl_constant.cpp \
	randomcl/prngcl_mrg32k3a.cpp
	
HDRS = PRNGCL.h \
	hgpucl/hgpucl.h \
	hgpucl/hgpucl_buffer.h \
	hgpucl/hgpucl_constants.h \
	hgpucl/hgpucl_context.h \
	hgpucl/hgpucl_devices.h \
	hgpucl/hgpucl_error.h \
	hgpucl/hgpucl_kernel.h \
	hgpucl/hgpucl_platforms.h \
	hgpucl/hgpucl_program.h \
	hgpucl/hgpucl_structs.h \
	include/hgpu.h \
	include/hgpu_constants.h \
	include/hgpu_convert.h \
	include/hgpu_enum.h \
	include/hgpu_error.h \
	include/hgpu_io.h \
	include/hgpu_md5.h \
	include/hgpu_parameters.h \
	include/hgpu_platform.h \
	include/hgpu_string.h \
	include/hgpu_strucs.h \
	include/hgpu_timer.h \
	include/hgpu_types.h \
	randomcl/hgpu_prng.h \
	randomcl/hgpu_prng_test.h

is_64=$(shell s=`uname -m`; if (echo $$s | grep x86_64 > /dev/null); then echo 1; fi)

ifndef AMDAPPSDKROOT
AMDAPPSDKROOT=/opt/AMDAPP/
endif
		
# amd
SDK_INC_AMD=$(AMDAPPSDKROOT)/include
ifeq ($(is_64), 1)
SDK_LIB_AMD=$(AMDAPPSDKROOT)/lib/x86_64/
else
SDK_LIB_AMD=$(AMDAPPSDKROOT)/lib/x86/
endif

# intel
SDK_INC_INTEL=/usr/include/
ifeq ($(is_64), 1)
SDK_LIB_INTEL=/usr/lib64/
endif

# nvidia
SDK_INC_NVIDIA=/usr/local/cuda/include/
SDK_LIB_NVIDIA=/usr/lib/

OBJS_AMD=$(patsubst %.c,%.amd.o,$(SRCS))
OBJS_INTEL=$(patsubst %.c,%.intel.o,$(SRCS))
OBJS_NVIDIA=$(patsubst %.c,%.nvidia.o,$(SRCS))

TARGET=$(patsubst %.cpp,%,$(wildcard *.cpp))

amd:	$(join $(TARGET),.amd)
intel:	$(join $(TARGET),.intel)
nvidia:	$(join $(TARGET),.nvidia)

%.amd:	$(SRCS) $(HDRS)
	$(CC) $(CFLAGS) $(SRCS) -I$(SDK_INC_AMD) -L$(SDK_LIB_AMD) -Wl,-rpath,$(SDK_LIB_AMD)  -o $@ -lOpenCL

%.intel:$(SRCS) $(HDRS)
	$(CC) $(CFLAGS) $(SRCS) -I$(SDK_INC_INTEL) -L$(SDK_LIB_INTEL) -Wl,-rpath,$(PATH_TO_INTEL_LIB) -o $@ -lOpenCL

%.nvidia:$(SRCS) $(HDRS)
	$(CC) $(CFLAGS) $(SRCS) -I$(SDK_INC_NVIDIA) -L$(SDK_LIB_NVIDIA) -o $@ -lOpenCL

clobber:
	rm -rf $(TARGET).amd $(TARGET).intel $(TARGET).nvidia $(OBJS_AMD) $(OBJS_INTEL) $(OBJS_NVIDIA)

clean:
	rm -f $(TARGET).amd $(TARGET).intel $(TARGET).nvidia
