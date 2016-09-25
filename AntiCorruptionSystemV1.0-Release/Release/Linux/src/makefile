cc = gcc
SRCS = $(shell ls *.c)
OBJS = $(SRCS:%.c=%.o)
HEADERS = ACSHeader.h

AntiCorruptionSystem: $(OBJS) $(HEADERS)
	$(cc) -o ../$@ $^ `pkg-config --cflags --libs gtk+-2.0`

%.o: %.c $(HEADERS)
	$(cc) -c -o $@ $< `pkg-config --cflags gtk+-2.0`
