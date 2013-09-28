CC = g++
CFLAGS = -I$(AMDAPPSDKROOT)/include -g
LDFLAGS = -L$(AMDAPPSDKROOT)/lib/x86_64 -lOpenCL

# project name
TARGET = prngcl

SRCS =  PRNGCL.cpp \
	clinterface/clinterface.cpp \
	clinterface/hgpucl.cpp \
	clinterface/hgpucl_buffer.cpp \
	clinterface/hgpucl_context.cpp \
	clinterface/hgpucl_devices.cpp \
	clinterface/hgpucl_error.cpp \
	clinterface/hgpucl_kernel.cpp \
	clinterface/hgpucl_platforms.cpp \
	clinterface/hgpucl_program.cpp \
	src/hgpu_convert.cpp \
	src/hgpu_enum.cpp \
	src/hgpu_error.cpp \
	src/hgpu_io.cpp \
	src/hgpu_md5.cpp \
	src/hgpu_parameters.cpp \
	src/hgpu_string.cpp \
	src/hgpu_timer.cpp \
	random/hgpu_prng.cpp \
	random/hgpu_prng_test.cpp \
	random/prngcl_xor128.cpp \
	random/prngcl_xor7.cpp \
	random/prngcl_ranmar.cpp \
	random/prngcl_ranlux.cpp \
	random/prngcl_ranecu.cpp \
	random/prngcl_pm.cpp \
	random/prngcl_constant.cpp \
	random/prngcl_mrg32k3a.cpp
	
HDRS =  PRNGCL.h \
	clinterface/clinterface.h \
	clinterface/hgpucl.h \
	clinterface/hgpucl_buffer.h \
	clinterface/hgpucl_constants.h \
	clinterface/hgpucl_context.h \
	clinterface/hgpucl_devices.h \
	clinterface/hgpucl_error.h \
	clinterface/hgpucl_kernel.h \
	clinterface/hgpucl_platforms.h \
	clinterface/hgpucl_program.h \
	include/hgpu_convert.h \
	include/hgpu_enum.h \
	include/hgpu_error.h \
	include/hgpu_io.h \
	include/hgpu_md5.h \
	include/hgpu_parameters.h \
	include/hgpu_string.h \
	include/hgpu_timer.h \
	random/hgpu_prng.cpp \
	random/hgpu_prng_test.h

OBJS = $(SRCS:.cpp=.o)

$(TARGET): $(SRCS) $(HDRS)

ifndef AMDAPPSDKROOT
	@echo >&2
	@echo "AMD APP SDK not installed" >&2
	@exit 1
else
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)
endif

clobber:
	rm -rf $(TARGET) $(OBJS)

clean:
	rm -f $(TARGET)
