top_srcdir := ..
3rd_srcdir := $(top_srcdir)/3rd_party


libraries := libcerror libcnotify libuna libfvalue libcdata libfdata libfcache \
	libbfio libcfile libcthreads libhmac libfguid libclocale libcpath \
	libcsplit

depend_libdirs := $(addprefix $(top_srcdir)/,$(libraries))

CPPFLAGS += -DHAVE_CONFIG_H=1 \
	    $(addprefix -I ,$(depend_libdirs)) \
	    -I $(top_srcdir)/common \
	    -I $(top_srcdir)/include

LDFLAGS += $(foreach var,$(libraries),$(top_srcdir)/$(var)/$(var).a)


3rd_libraries := zlib bzip2

3rd_depend_libdir := $(addprefix $(3rd_srcdir)/,$(3rd_libraries))

CPPFLAGS += $(addprefix -I ,$(3rd_depend_libdir))

LDFLAGS += $(foreach var,$(3rd_libraries),$(3rd_srcdir)/$(var)/$(var).a) 


library := libewf.a
dylibrary := $(subst .a,.so,$(library))
sources := $(wildcard *.c)
objects := $(subst .c,.o,$(sources))

lbracket := (
rbracket := )

library_objs := $(addprefix $(lbracket),$(objects))
library_objs := $(addsuffix $(rbracket),$(library_objs))
library_objs := $(addprefix $(library),$(library_objs))


.PHONY: all clean

all: $(dylibrary) $(library)

clean:
	$(RM) $^ $(dylibrary) $(library) $(objects)

$(dylibrary): LDFLAGS += -shared
$(dylibrary): $(objects)
	$(LINK.c) $^ $(OUTPUT_OPTION)

$(library): $(library_objs)

