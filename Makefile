
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
ifndef KERNEL_SRC
    # If KERNEL_SRC is not set, define it For Yocto SDK
    KERNEL_SRC := ${SDKTARGETSYSROOT}/usr/src/kernel
    # If KERNEL_SRC is not set, define it For Local
# KERNEL_SRC := /lib/modules/$(shell uname -r)/build
endif
#Set Include Directories
INCLUDE := -I$(PWD)/Include
#Define Build Directory
BUILD_DIR := $(PWD)/build
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
obj-m := Source/${OBJ}
##################################################
################# Main Methouds ##################
##################################################
.PHONY: all
all:
	@make -C $(KERNEL_SRC) M=$(PWD) modules
	@echo "=============== Building Finished ==============="
.PHONY: clean
clean:
	@rm -f *.o *~ core .depend .*.cmd *.ko *.mod.c
	@rm -f Module.markers Module.symvers modules.order
	@rm -rf .tmp_versions Modules.symvers
	@echo "=============== Cleaning Finished ==============="
.PHONY: install
modules_install:
	@$(MAKE) -C $(KERNEL_SRC) M=$(PWD) modules_install
#*************************************************
#*************************************************
