# (AI-assisted) Software N64 backend, out-of-tree build.
ROOT_DIR := .
HAVE_OPENGL := 0
HAVE_PARALLEL := 0
HAVE_PARALLEL_RSP := 0
HAVE_THR_AL := 1
STATIC_LINKING := 0
platform := ps5
include Makefile.common
.PHONY: list flags
list:
	@printf '%s\n' $(SOURCES_CXX) $(SOURCES_C)
flags:
	@printf '%s\n' $(CFLAGS) $(INCFLAGS) $(DYNAFLAGS) -D__LIBRETRO__ -DM64P_PLUGIN_API -DM64P_CORE_PROTOTYPES -DHAVE_POSIX_MEMALIGN -DINLINE=inline -DNDEBUG
