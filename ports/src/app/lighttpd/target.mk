TARGET = lighttpd

include $(REP_DIR)/src/app/lighttpd/target.inc

LIBS += libc libm libc_log libc_fs libc_lwip_nic_dhcp
