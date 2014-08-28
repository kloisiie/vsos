#
# FAT File System Module using a Block session as disk I/O backend
#

INC_DIR += $(REP_DIR)/src/lib/ffat/contrib

SRC_C  = ff.c ccsbcs.c
SRC_CC = diskio_block.cc

LIBS = base

vpath % $(REP_DIR)/src/lib/ffat/
vpath % $(REP_DIR)/contrib/ff007e/src
vpath % $(REP_DIR)/contrib/ff007e/src/option
