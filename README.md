VSOS
####

VSOS is branch of Genode by OS5, IS, CAS.


                      =================================
                      Genode Operating System Framework
                      =================================


This is the source tree of the reference implementation of the Genode OS
architecture. For a general overview about the architecture, please refer to
the project's official website:

:Official project website for the Genode OS Framework:

  [http://genode.org/documentation/general-overview]

The current implementation can be compiled for 8 different kernels: Linux,
L4ka::Pistachio, L4/Fiasco, OKL4, NOVA, Fiasco.OC, Codezero, and a custom
kernel for running Genode directly on ARM-based hardware. Whereas the Linux
version serves us as development vehicle and enables us to rapidly develop the
generic parts of the system, the actual target platforms of the framework are
microkernels. There is no "perfect" microkernel - and neither should there be
one. If a microkernel pretended to be fit for all use cases, it wouldn't be
"micro". Hence, all microkernels differ in terms of their respective features,
complexity, and supported hardware architectures.

Genode allows the use of each of the kernels listed above with a rich set of
device drivers, protocol stacks, libraries, and applications in a uniform way.
For developers, the framework provides an easy way to target multiple different
kernels instead of tying the development to a particular kernel technology. For
kernel developers, Genode contributes advanced workloads, stress-testing their
kernel, and enabling a variety of application use cases that would not be
possible otherwise. For users and system integrators, it enables the choice of
the kernel that fits best with the requirements at hand for the particular
usage scenario.


Directory overview
##################

The source tree is composed of the following subdirectories:

:'doc':

  This directory contains general documentation. Please consider the following
  document for a quick guide to get started with the framework:

  ! doc/getting_started.txt

  If you are curious about the ready-to-use components that come with the
  framework, please review the components overview:

  ! doc/components.txt

:'repos':

  This directory contains the so-called source-code repositories of Genode.
  Please refer to the README file in the 'repos' directory to learn more
  about the roles of the individual repositories.

:'tool':

  Source-code management tools and scripts. Please refer to the README file
  contained in the directory.


Contact
#######

The best way to get in touch with Genode developers and users is the project's
mailing list. Please feel welcome to join in!

:Genode Mailing Lists:

  [http://genode.org/community/mailing-lists]

