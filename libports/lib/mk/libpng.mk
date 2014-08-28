LIBPNG     = libpng-1.4.1
LIBPNG_DIR = $(REP_DIR)/contrib/$(LIBPNG)
LIBS      += libc libm zlib

# find 'config.h'
INC_DIR += $(REP_DIR)/src/lib/libpng

CC_DEF    += -DHAVE_CONFIG_H -DPNG_CONFIGURE_LIBPNG

SRC_C = png.c pngset.c pngget.c pngrutil.c pngtrans.c pngwutil.c \
        pngread.c pngrio.c pngwio.c pngwrite.c pngrtran.c pngwtran.c \
        pngmem.c pngerror.c pngpread.c 

vpath %.c $(LIBPNG_DIR)

SHARED_LIB = yes
