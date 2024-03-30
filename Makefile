
##################################################
# *  FILE DESCRIPTION
#########################
# *  Author:  Khaled El-Sayed (@t0ti20)
# *  Created:  29.03.2024
# *  File:  Makefile
# *  Module:  Global
# *  Description:  Make File For Device Driver Using Yocto SDK
##################################################
##################################################
################ Configuration ###################
##################################################
#Kernel Souerce Code
KERNEL_SRC := /opt/poky/4.0.17/sysroots/cortexa72-poky-linux/usr/src/kernel
#KERNEL_SRC := /lib/modules/$(shell uname -r)/build
#Set Include Directories
INCLUDE := -I$(PWD)/Include
#Define Build Directory
BUILD_DIR := $(PWD)/Build
##################################################
################### Variables ####################
##################################################
#Get Source Files
SRC := $(shell ls $(PWD)/Source/*.c | grep -v '\.mod\.c$$' | xargs -n1 basename)
#Select Object Files
OBJ = $(SRC:.c=.o)
#Select Compiler Flags
EXTRA_CFLAGS += ${INCLUDE}
#Exporting Current Modules
obj-m := ./Source/${OBJ}
##################################################
################# Main Methouds ##################
##################################################
.PHONY: all
all:
	@make -C $(KERNEL_SRC) M=$(PWD) modules
	@mv -f $(PWD)/Source/*.mod $(PWD)/Source/*.o $(PWD)/Source/.*.cmd $(PWD)/Source/*.ko $(PWD)/Source/*.mod.c ${BUILD_DIR} > /dev/null 2>&1
	@echo "=============== Building Finished ==============="
.PHONY: clean
clean:
	@make -C $(KERNEL_SRC) M=$(PWD) clean > /dev/null 2>&1
	@echo "=============== Cleaning Finished ==============="
#*************************************************
#*************************************************
