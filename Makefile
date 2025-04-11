CC = gcc
CFLAGS = -O3 -flto -march=native -Iinclude
LDFLAGS = 
BIN_DIR = bin

SRC_DIR = src

HTTP_SRCS = $(SRC_DIR)/httpserver.c $(SRC_DIR)/base64.c $(SRC_DIR)/cassie.pb-c.c
HTTP_OBJS = $(HTTP_SRCS:.c=.o)
HTTP_OUT = $(BIN_DIR)/httpserver
HTTP_LIBS = -lmicrohttpd -lprotobuf-c

GAME_SRCS = $(SRC_DIR)/gameserver.c $(SRC_DIR)/gshandler.c $(SRC_DIR)/netpacket.c $(SRC_DIR)/cassie.pb-c.c
GAME_OBJS = $(GAME_SRCS:.c=.o)
GAME_OUT = $(BIN_DIR)/gameserver
GAME_LIBS = -lprotobuf-c -lpthread

all: $(HTTP_OUT) $(GAME_OUT)

$(HTTP_OUT): $(HTTP_SRCS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(HTTP_LIBS)

$(GAME_OUT): $(GAME_SRCS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(GAME_LIBS)

clean:
	rm -f $(BIN_DIR)/*

.PHONY: all clean
