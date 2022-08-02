CC = gcc
CFLAGS = -Wall -g
LDLIBS = -lm
OBJS = main.o mtfuji.o optimize.o
TARGET = optimizer

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDLIBS)

.PHONY: tmpclean clean
tmpclean:
	rm -f *~
clean: tmpclean
	rm -f $(OBJS) $(TARGET) 
