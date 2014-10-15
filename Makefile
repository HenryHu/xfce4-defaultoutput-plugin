# Sample Makefile to show how the plugin is built
# The FreeBSD ports can use this directly
SRCS=defaultoutput.c
PLUGINDIR=panel-plugin
CDEPS=`pkg-config --cflags gtk+-2.0 libxfce4panel-1.0 libxfce4ui-1`
LDEPS=`pkg-config --libs gtk+-2.0 libxfce4panel-1.0 libxfce4ui-1`
CC?=cc
CFLAGS+=-fPIC
LIB=libdefaultoutput.so
DESKTOP=defaultoutput.desktop

all:
.for f in $(SRCS)
	$(CC) -Wall -c $(CFLAGS) $(CDEPS) $(PLUGINDIR)/$(f)
.endfor
	$(CC) -Wall $(CFLAGS) $(LDFLAGS) $(LDEPS) *.o -shared -o $(LIB)

clean:
	rm -f *.o $(LIB)

install:
	cp $(LIB) /usr/local/lib/xfce4/panel/plugins/
	cp $(DESKTOP) /usr/local/share/xfce4/panel/plugins/
