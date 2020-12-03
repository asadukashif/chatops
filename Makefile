CC=gcc
FLAGS=-Wall -g -pthread

# Folder Paths
HEADER=./header
OBJ=./obj
BIN=./bin

# All Object Files.
OBJS=$(U_OBJ) $(SH_OBJ) $(CL_OBJ)
# All Header Files.
HEADERS=$(U_HEADER) $(SH_HEADER) $(CL_HEADER)

# Relative Paths of Objects. 
U_OBJ=$(OBJ)/utils.o
SH_OBJ=$(OBJ)/server_helper.o
CL_OBJ=$(OBJ)/colors.o

# Relative Paths for Header files. 
U_HEADER=$(HEADER)/utils.h
SH_HEADER=$(HEADER)/server_helper.h
CL_HEADER=$(HEADER)/colors.h

# List of all binaries.
BINS=$(BIN)/server $(BIN)/client

# ************* Rules ************* #
all: $(BINS)

$(BIN)/server: server.c $(OBJS)
	$(CC) $(FLAGS) $^ -o $@

$(BIN)/client: client.c $(U_OBJ) $(CL_OBJ)
	$(CC) $(FLAGS) $^ -o $@

$(OBJ)/%.o: %.c $(HEADER)/%.h
	$(CC) $(FLAGS) -c $< -o $@


clean:
	rm $(BINS) $(OBJS)