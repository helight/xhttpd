OBJECT=xhttpd
APPOBJ=www
all: $(OBJECT) $(APPOBJ)
	(cp httpd/$(OBJECT) ./)
	@echo finish the compile. please run $(OBJECT)

$(OBJECT): 
	@echo start to compile xhttpd
	@make -C httpd all
	@make -C www all

$(APPOBJ):
	@echo start to compile www 
	@make -C www all

clean:
	@echo clean the $(OBJECT)
	@if [ -e $(OBJECT) ]; \
	then \
		rm $(OBJECT); \
	fi
	@make -C httpd clean
	@make -C www clean
	@echo clean finished
