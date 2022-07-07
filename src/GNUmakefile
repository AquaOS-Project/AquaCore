override KERNEL := aqua.elf

define DEFAULT_VAR =
    ifeq ($(origin $1), default)
        override $(1) := $(2)
    endif
    ifeq ($(origin $1), undefined)
        override $(1) := $(2)
    endif
endef

$(eval $(call DEFAULT_VAR,CCX,cxx))

$(eval $(call DEFAULT_VAR,LD,ld))

CXXFLAGS ?= -O2 -g -Wall -Wextra -pipe

LDFLAGS ?=

override INTERNALCFLAGS :=   \
	-I.                      \
    -target x86_64-pc-none-elf \
    -fno-pic                 \
	-fno-rtti	           	 \
    -mabi=sysv               \
    -mno-80387               \
    -mno-mmx                 \
    -mno-3dnow               \
    -mno-sse                 \
    -mno-sse2                \
    -mcmodel=kernel          \
    -std=gnu++20             \
	-ffreestanding           \
	-fno-exceptions          \
	-fno-stack-protector     \
	-fno-omit-frame-pointer  \
	-fno-rtti                \
	-mno-red-zone

override INTERNALLDFLAGS :=    \
	-Tlinker.ld            \
	-nostdlib              \
	-zmax-page-size=0x1000 \
	-static


override INTERNALASFLAGS := \
        -I.			   \
	-fno-pic                   \
	-mabi=sysv                 \
	-mno-80387                 \
	-mno-mmx                   \
	-mno-3dnow                 \
	-mno-sse                   \
	-mno-sse2                  \
	-mcmodel=kernel	           \
	-mgeneral-regs-only        \
	-masm=intel

override CFILES := $(shell find ./ -type f -name '*.cpp')
override ASFILES := $(shell find ./ -type f -name '*.S')
override OBJ := $(CFILES:.cpp=.o)
override OBJ += $(ASFILES:.S=_S.o)
override HEADER_DEPS := $(CFILES:.cpp=.d)

.PHONY: all
all: $(KERNEL)

$(KERNEL): $(OBJ)
	ld.lld $(OBJ) $(LDFLAGS) $(INTERNALLDFLAGS) -o $@

-include $(HEADER_DEPS)
%.o: %.cpp
	clang++ $(CXXFLAGS) $(INTERNALCFLAGS) -c $< -o $@

%_S.o: %.S
	$(CC) $(INTERNALASFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(KERNEL) $(OBJ) $(HEADER_DEPS)

.PHONY: distclean
distclean: clean
