top_srcdir := ..
3rd_srcdir := $(top_srcdir)/3rd_party

libraries := libfvalue libbfio libsmdev libsmraw libcdata libcerror \
       	libodraw libcfile libclocale libcnotify libcthreads libhmac \
	libcdatetime libuna libcsplit libcpath libfguid libewf \
	libcsplit libfdata libfcache

depend_libdir := $(addprefix $(top_srcdir)/,$(libraries))

CPPFLAGS += -DHAVE_CONFIG_H=1 \
	    $(addprefix -I ,$(depend_libdir)) \
	    -I $(top_srcdir)/common \
	    -I $(top_srcdir)/include

LDFLAGS += $(addprefix -L ,$(depend_libdir)) \

LDLIBS += -lewf -pthread
LDLIBS += $(foreach var,$(libraries),$(top_srcdir)/$(var)/$(var).a)


3rd_libraries := zlib bzip2

3rd_depend_libdir := $(addprefix $(3rd_srcdir)/,$(3rd_libraries))

CPPFLAGS += $(addprefix -I ,$(3rd_depend_libdir))

LDFLAGS += $(addprefix -L ,$(3rd_depend_libdir)) \

LDLIBS += $(foreach var,$(3rd_libraries),$(3rd_srcdir)/$(var)/$(var).a) 


ewfacquire_sources := \
	byte_size_string.c \
	digest_hash.c \
	device_handle.c \
	ewfacquire.c \
	ewfinput.c \
	ewftools_getopt.c \
	ewftools_output.c \
	ewftools_signal.c \
	ewftools_system_string.c \
	guid.c \
	imaging_handle.c \
	log_handle.c \
	platform.c \
	process_status.c \
	storage_media_buffer.c \
	storage_media_buffer_queue.c
ewfacquire_objects := $(subst .c,.o,$(ewfacquire_sources))

binaries := ewfacquire
sources := $(wildcard *.c)
objects := $(subst .c,.o,$(sources))


.PHONY: all clean

all: $(binaries)

clean:
	$(RM) $(binaries) $(objects)

ewfacquire: $(ewfacquire_objects)

