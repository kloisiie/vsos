TARGET   = test-lwip_httpsrv_tracing_nob
LIBS     = lwip libc libc_log
SRC_CC   = main.cc
REQUIRES = foc

INC_DIR += $(REP_DIR)/src/lib/lwip/include
