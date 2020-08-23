usbtan-cli: main.c
	gcc -DENABLE_CLI -o usbtan-cli main.c -I/usr/include/gwenhywfar5/ -I/usr/include/libchipcard5/  `pkg-config --cflags --libs glib-2.0` -lgwenhywfar