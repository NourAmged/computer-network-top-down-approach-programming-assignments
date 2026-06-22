CC      = gcc
TARGET_1 = server
TARGET_2 = client
SRC_1    = server.c
SRC_2    = client.c

.PHONY: all clean

all: $(TARGET_1) $(TARGET_2)

$(TARGET_1): $(SRC_1)
	@$(CC) $(SRC_1) -o $(TARGET_1)

$(TARGET_2): $(SRC_2)
	@$(CC) $(SRC_2) -o $(TARGET_2)

clean:
	@rm -f $(TARGET_1) $(TARGET_2)