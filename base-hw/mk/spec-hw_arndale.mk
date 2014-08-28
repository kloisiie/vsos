#
# \brief  Offer build configurations that are specific to base-hw and Pandaboard A2
# \author Martin Stein
# \date   2013-01-09
#

# denote wich specs are also fullfilled by this spec
SPECS += hw platform_arndale

# set address where to link the text segment at
LD_TEXT_ADDR ?= 0x80000000

# include implied specs
include $(call select_from_repositories,mk/spec-hw.mk)
include $(call select_from_repositories,mk/spec-platform_arndale.mk)
