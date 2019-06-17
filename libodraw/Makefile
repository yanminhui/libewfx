top_srcdir := ..

libraries := libcerror libbfio libcfile libcdata libcnotify libuna libclocale \
	libcpath

depend_libdirs := $(addprefix $(top_srcdir)/,$(libraries))

CPPFLAGS += -DHAVE_CONFIG_H=1 \
	    $(addprefix -I ,$(depend_libdirs)) \
	    -I $(top_srcdir)/common \
	    -I $(top_srcdir)/include

library := libodraw.a
sources := $(wildcard *.c)
objects := $(subst .c,.o,$(sources))

lbracket := (
rbracket := )

library_objs := $(addprefix $(lbracket),$(objects))
library_objs := $(addsuffix $(rbracket),$(library_objs))
library_objs := $(addprefix $(library),$(library_objs))

.PHONY: all clean

all: $(library)
	
$(library): $(library_objs)

clean:
	$(RM) $^ $(library) $(objects)

