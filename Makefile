export CFLAGS = -Wall -Winline -g -O2 -fPIC

ifeq "$(shell uname)" "Linux"
	export ARFLAGS = rvU
endif


top_srcdir := .
3rd_srcdir := $(top_srcdir)/3rd_party

prefix := $(top_srcdir)/stage
bin := $(prefix)/bin
lib := $(prefix)/lib
inc := $(prefix)/include


libraries := libbfio libcaes libcdata libcdatetime libcerror libcfile \
	libclocale libcnotify libcpath libcsplit libcthreads \
	libfcache libfdata libfguid libfvalue libhmac libodraw libsmdev \
	libsmraw libuna

depend_libdirs := $(addprefix $(top_srcdir)/,$(libraries))


3rd_libraries := zlib bzip2
3rd_depend_libdirs := $(addprefix $(3rd_srcdir)/,$(3rd_libraries))


libewf_dir := $(top_srcdir)/libewf
ewftools_dir := $(top_srcdir)/ewftools


.PHONY: all clean install uninstall $(3rd_depend_libdirs) $(depend_libdirs) $(ewftools_dir)

all: TARGET = all
all: $(ewftools_dir)
	
clean: TARGET = clean
clean: $(ewftools_dir)

install: all
	mkdir -p $(bin) $(lib) $(inc) $(inc)/libewf
	cp -f $(top_srcdir)/include/libewf/*.h $(inc)/libewf
	cp -f $(top_srcdir)/include/libewf*.h $(inc)
	install $(libewf_dir)/*.so $(lib)
	install $(ewftools_dir)/ewfacquire $(bin)

uninstall: 
	$(RM) -r $(inc)/libewf
	$(RM) $(inc)/libewf*.h 
	$(RM) $(lib)/libewf*
	$(RM) $(bin)/ewfacquire

$(ewftools_dir): $(libewf_dir)
	$(MAKE) --directory=$@ $(TARGET)

$(libewf_dir): $(3rd_depend_libdirs) $(depend_libdirs)
	$(MAKE) --directory=$@ $(TARGET)

$(3rd_depend_libdirs) $(depend_libdirs):
	$(MAKE) --directory=$@ $(TARGET)


