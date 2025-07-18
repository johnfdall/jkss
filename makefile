CC = gcc
CFLAGS = -Wall -Wextra -O2
SRCDIR = src
BUILDDIR = build

# Source files
NETWORK_SRCS = $(SRCDIR)/network/network.c
SERVER_SRCS = $(SRCDIR)/server/server.c $(SRCDIR)/server/game_state.c
CLIENT_SRCS = $(SRCDIR)/client/client.c

# Object files
NETWORK_OBJS = $(NETWORK_SRCS:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)
SERVER_OBJS = $(SERVER_SRCS:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)
CLIENT_OBJS = $(CLIENT_SRCS:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)

# Targets
SERVER_TARGET = $(BUILDDIR)/server.out
CLIENT_TARGET = $(BUILDDIR)/client.out

.PHONY: all clean server client

all: server client

server: $(SERVER_TARGET)
client: $(CLIENT_TARGET)

$(SERVER_TARGET): $(NETWORK_OBJS) $(SERVER_OBJS) | $(BUILDDIR)
	$(CC) $(CFLAGS) -o $@ $^

$(CLIENT_TARGET): $(NETWORK_OBJS) $(CLIENT_OBJS) | $(BUILDDIR)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR):
	mkdir -p $(BUILDDIR)/network $(BUILDDIR)/server $(BUILDDIR)/client

clean:
	rm -rf $(BUILDDIR)

install-deps:
	@echo "No additional dependencies required"
