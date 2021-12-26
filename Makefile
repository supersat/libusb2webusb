
OBJS = libusb-1.0.so.0
all: $(OBJS)

libusb-1.0.so.0: libusb.c server.c
	$(CC) $(CFLAGS) -o $@ -fPIC -shared -ldl -lwebsockets

clean:
	rm $(OBJS)