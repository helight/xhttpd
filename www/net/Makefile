include ../../make.conf
XOBJS=
OBJS=netdev mac
FLGS=-O3 -I../cgilib/  ../cgilib/cgilib.c
all:
	@for obj in $(OBJS); do \
	echo "$(CC) $(CFLAGS) $(FLGS) $$obj$(CEXT) -o $$obj$(CGIEXT)"; \
	$(CC) $(CFLAGS) $(FLGS) $$obj$(CEXT) -o $$obj$(CGIEXT); \
	chmod 755 $$obj$(CGIEXT);\
	done;
	@for obj in $(XOBJS); do \
	echo "$(CC) $(CFLAGS) $(FLGS) $$obj$(CEXT) -o $$obj$(XCGIEXT)"; \
	$(CC) $(CFLAGS) $(FLGS) $$obj$(CEXT) -o $$obj$(XCGIEXT); \
	chmod 755 $$obj$(XCGIEXT);\
	done;

clean:
	@for obj in $(OBJS); do \
	echo "rm -rf $$obj$(CGIEXT)"; \
	rm -rf $$obj$(CGIEXT); \
	done;
	@for obj in $(XOBJS); do \
	echo "rm -rf $$obj$(XCGIEXT)"; \
	rm -rf $$obj$(XCGIEXT); \
	done;
	@echo "rm -rf *.o"
	@rm -rf *.o

