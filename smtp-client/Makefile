CC      = gcc
TARGET = client
SRC  = client.c

.PHONY: all clean

all: $(TARGET) 

$(TARGET): $(SRC)
	@$(CC) $(SRC) -o $(TARGET) -lssl -lcrypto

clean:
	@rm -f $(TARGET)