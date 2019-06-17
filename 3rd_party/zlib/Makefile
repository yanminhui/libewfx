library := zlib.a
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

