include ../make.conf 

XOBJS=index
FLGS=-O3 -I./cgilib/  ./cgilib/cgilib.c 
WSUBDIRS=demo dev disk env fs net ps sys usr 
all:
	@echo begin to complie www
	@for obj in $(XOBJS); do \
	echo "$(CC) $(CFLAGS) $(FLGS) $$obj$(CEXT) -o $$obj$(XCGIEXT)"; \
	$(CC) $(CFLAGS) $(FLGS) $$obj$(CEXT) -o $$obj$(XCGIEXT); \
	chmod 755 $$obj$(XCGIEXT);\
	done;
	@echo begin to complie subdir 
	@for dir in $(WSUBDIRS); do \
		make -C $$dir all;\
	done;

clean:
	@echo begin to clean www
	@for obj in $(XOBJS); do \
	rm $$obj$(XCGIEXT);\
	done;
	@echo begin to clean subdir 
	@for dir in $(WSUBDIRS); do \
		make -C $$dir clean;\
	done;
