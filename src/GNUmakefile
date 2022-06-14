# This is the name that our final kernel executable will have.
# Change as needed.
override KERNEL := aqua.elf

# Convenience macro to reliably declare overridable command variables.
define DEFAULT_VAR =
    ifeq ($(origin $1), default)
        override $(1) := $(2)
    endif
    ifeq ($(origin $1), undefined)
        override $(1) := $(2)
    endif
endef

# It is highly recommended to use a custom built cross toolchain to build a kernel.
# We are only using "cxx" as a placeholder here. It may work by using
# the host system's toolchain, but this is not guaranteed.
$(eval $(call DEFAULT_VAR,CCX,cxx))

# Likewise, "ld" here is just a placeholder and your mileage may vary if using the
# host's "ld".
$(eval $(call DEFAULT_VAR,LD,ld))

# User controllable CFLAGS.
CXXFLAGS ?= -O2 -g -Wall -Wextra -pipe

# User controllable linker flags. We set none by default.
LDFLAGS ?=

# Internal C flags that should not be changed by the user.
override INTERNALCFLAGS :=   \
	-I.                  \
        -target x86_64-pc-none-elf \
        -fno-pic                   \
        -mabi=sysv                 \
        -mno-80387                 \
        -mno-mmx                   \
        -mno-3dnow                 \
        -mno-sse                   \
        -mno-sse2                  \
        -mcmodel=kernel            \
        -std=gnu++20             \
	-ffreestanding           \
	-fno-exceptions          \
	-fno-stack-protector     \
	-fno-omit-frame-pointer  \
	-fno-rtti                \
	-mno-red-zone

# Internal linker flags that should not be changed by the user.
override INTERNALLDFLAGS :=    \
	-Tlinker.ld            \
	-nostdlib              \
	-zmax-page-size=0x1000 \
	-static

# Use find to glob all *.cpp files in the directory and extract the object names.
override CFILES := $(shell find ./ -type f -name '*.cpp')
override OBJ := $(CFILES:.cpp=.o)
override HEADER_DEPS := $(CFILES:.cpp=.d)

# Default target.
.PHONY: all
all: $(KERNEL)

stivale2.h:
	curl https://raw.githubusercontent.com/stivale/stivale/master/stivale2.h -o $@

# Link rules for the final kernel executable.
$(KERNEL): $(OBJ)
	ld.lld $(OBJ) $(LDFLAGS) $(INTERNALLDFLAGS) -o $@

# Compilation rules for *.cpp files.
-include $(HEADER_DEPS)
%.o: %.cpp stivale2.h
	clang++ $(CXXFLAGS) $(INTERNALCFLAGS) -c $< -o $@

# Remove object files and the final executable.
.PHONY: clean
clean:
	rm -rf $(KERNEL) $(OBJ) $(HEADER_DEPS)

.PHONY: distclean
distclean: clean
	rm -f stivale2.h
