CC = gcc
RELEASE ?= 0
CFLAGS = -Iinclude

BIN_DIR = bin
SRC_DIR = src

ifeq ($(RELEASE),0)
	CFLAGS += -g
	OUT_DIR = $(BIN_DIR)/debug
else
	CFLAGS += -O3 -flto -march=native -Wl,-s
	LDFLAGS += -s
	OUT_DIR = $(BIN_DIR)/O3
endif

OBJ_DIR = $(OUT_DIR)/build

HTTP_DIR = $(SRC_DIR)/httpserver
GAME_DIR = $(SRC_DIR)/gameserver
SHARED_DIR = $(SRC_DIR)/shared

IKCP_SRC = $(GAME_DIR)/ikcp.c
IKCP_OBJ = $(OBJ_DIR)/gameserver/ikcp.o
IKCP_CFLAGS = $(CFLAGS)

HTTP_SRCS = \
	$(HTTP_DIR)/main.c \
	$(HTTP_DIR)/util.c \
	$(HTTP_DIR)/authhandler.c \
	$(HTTP_DIR)/dispatchhandler.c \
	$(HTTP_DIR)/router.c \
	$(SHARED_DIR)/cassie.pb-c.c

HTTP_OBJS = $(patsubst $(SRC_DIR)/%, $(OBJ_DIR)/%, $(HTTP_SRCS:.c=.o))
HTTP_OUT = $(OUT_DIR)/httpserver
HTTP_LIBS = -lmicrohttpd -lprotobuf-c

GAME_HANDLER_DIR = $(wildcard $(GAME_DIR)/handler/*)
GAME_HANDLER_SRCS = $(wildcard $(GAME_HANDLER_DIR)/*.c)

GAME_SRCS = \
	$(GAME_DIR)/handler.c \
	$(GAME_DIR)/ikcp.c \
	$(GAME_DIR)/main.c \
	$(GAME_DIR)/packet.c \
	$(GAME_DIR)/session.c \
	$(GAME_DIR)/udpserver.c \
	$(GAME_DIR)/util.c \
	$(SHARED_DIR)/cassie.pb-c.c \
	$(GAME_HANDLER_SRCS)

GAME_OBJS = $(patsubst $(SRC_DIR)/%, $(OBJ_DIR)/%, $(GAME_SRCS:.c=.o))
GAME_OUT = $(OUT_DIR)/gameserver
GAME_LIBS = -lprotobuf-c

all: $(GAME_OUT) $(HTTP_OUT)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(IKCP_OBJ): $(IKCP_SRC)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(IKCP_CFLAGS) -c $< -o $@

$(GAME_OUT): $(GAME_OBJS) $(IKCP_OBJ)
	@mkdir -p $(OUT_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(GAME_LIBS)

$(HTTP_OUT): $(HTTP_OBJS)
	@mkdir -p $(OUT_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(HTTP_LIBS)

clean:
	rm -rf $(BIN_DIR)/*

.PHONY: all clean
