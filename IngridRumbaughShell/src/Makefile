# Defining the compiler 
CC = gcc 

# compiler flags:

CFLAGS = -g -I.

TARGET = shell_script_v2

all: $(TARGET) 

$(TARGET): $(TARGET).c $(TARGET).h
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c
    
CLEAN:
	$(RM) $(TARGET)

INSTALL: $(TARGET)
	cp $(TARGET) ../install/bin

