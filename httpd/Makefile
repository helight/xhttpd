include ../make.conf

OBJECT = xhttpd
OBJS = xhttpd.o funs.o init.o set_socket.o \
        request.o response.o do_cgi.o do_log.o config.o
SRCS = $(OBJS:.=.c)

HEADS = xhttpd.h funs.h init.h debug.h set_socket.h\
        http.h request.h response.h do_cgi.h config.h

all: $(OBJECT)

$(OBJECT): $(OBJS) 
	$(CC) $(CFLAGS)  $^ -o $@
$(OBJS): $(HEADS)

clean:
	rm -rf *.o; rm -rf $(OBJECT)

	

