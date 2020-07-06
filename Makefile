TARGET = bin/validator

CROSS_COMPILE=/mnt/hgfs/Share/tool/gcc-linaro-5.3.1-2016.05-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-
# CROSS_COMPILE=/opt/P18-L2/opt/EmbedSky/gcc-linaro-5.3.1-2016.05-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-
CC=$(CROSS_COMPILE)g++
AR=$(CROSS_COMPILE)ar 

BUILD_DIR = build
SRC_PATH = src
BIN_PATH = bin

C_INCLUDES = \
src/TcpSocket \
sdk/inc

EXCLUDE_C=$(SRC_PATH)/test_api.c
DIRS = $(shell find "$(SRC_PATH)" -maxdepth 1 -type d) 		#  All direction paths space separeted maxdepth = 1
SRCS_C= $(foreach dir, $(DIRS), $(wildcard $(dir)/*.cpp))	#  All srcs files situated in DIRS with space separated list

SRC= $(filter-out $(EXCLUDE_C),${SRCS_C})					# All src files needed for compiling excluding

OBJS1 = $(addprefix build/, $(SRC:src/%=%))	
OBJS := $(OBJS1:.cpp=.o)									# All object files

CFLAGS = -fPIC $(addprefix -I,$(C_INCLUDES)) -fpermissive -Wl,-rpath=/opt/lib
LDFLAGS=-L. -lpos  -lpthread -lrt -L sdk/lib
LDFLAGS +=-lpng -liconv -lfreetype -lz 


LIBAPI = libpos

.PHONY: all 

all: echomessages $(TARGET) 

$(BUILD_DIR)/%.o : $(SRC_PATH)/%.cpp
	@echo compiling $<
	mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) -o $@ -c $<	
	
$(TARGET) : $(OBJS) 
	@echo "Linking { $^ } => $@"
	mkdir -p $(shell dirname $@) 
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) 

echomessages: 
	@echo "********************************************************************************************"
	@echo "SRC_PATH = $(SRC_PATH)"
	@echo DIRS = $(DIRS)
	@echo SRC = $(SRC)
	@echo OBJS = $(OBJS)
	@echo "********************************************************************************************"
	@echo ============================================================================================

clean:
	rm -f *.o $(OBJS) $(TARGET)
	rm -rf tmp
