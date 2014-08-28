#
# \brief  Build config for Genodes core process
# \author Stefan Kalkowski
# \author Martin Stein
# \date   2012-10-04
#

# declare wich specs must be given to build this target
REQUIRES += platform_arndale

# add include paths
INC_DIR += $(REP_DIR)/src/core/arndale

# add C++ sources
SRC_CC += platform_services.cc \
          platform_support.cc

# add assembly sources
SRC_S += mode_transition.s \
         boot_modules.s \
         crt0.s

# declare source paths
vpath platform_services.cc $(BASE_DIR)/src/core
vpath platform_support.cc  $(REP_DIR)/src/core/arndale
vpath mode_transition.s    $(REP_DIR)/src/core/arm_v7
vpath crt0.s               $(REP_DIR)/src/core/arm

#
# Check if there are other images wich shall be linked to core.
# If not use a dummy boot-modules file wich includes only the symbols.
#
ifeq ($(wildcard $(BUILD_BASE_DIR)/boot_modules.s),)
  vpath boot_modules.s $(REP_DIR)/src/core/arm
else
  INC_DIR += $(BUILD_BASE_DIR)
  vpath boot_modules.s $(BUILD_BASE_DIR)
endif

# include less specific target parts
include $(REP_DIR)/src/core/target.inc

