CFLAGS  = -Wall -Wextra -std=c99
LDFLAGS = -lpthread -framework OpenGL -framework GLUT -DGL_SILENCE_DEPRECATION

BIN_DIR = bin
SRC_DIR = .


all: main_ui Customer Cashier main
	
main_ui: $(SRC_DIR)/main_ui.o
	gcc $(CFLAGS) -o $(SRC_DIR)/main_ui $(SRC_DIR)/main_ui.c $(SRC_DIR)/ui_global.c $(SRC_DIR)/glopal.c $(SRC_DIR)/Read_Input_File.c $(LDFLAGS) -lpthread

Customer: $(SRC_DIR)/Customer.o
	gcc $(CFLAGS) -o $(SRC_DIR)/Customer $(SRC_DIR)/Customer.c $(SRC_DIR)/ui_global.c $(SRC_DIR)/glopal.c $(SRC_DIR)/Read_Input_File.c $(LDFLAGS) -lpthread

Cashier: $(SRC_DIR)/Cashier.o
	gcc $(CFLAGS) -o $(SRC_DIR)/Cashier $(SRC_DIR)/Cashier.c $(SRC_DIR)/ui_global.c $(SRC_DIR)/glopal.c $(SRC_DIR)/Read_Input_File.c $(SRC_DIR)/queue.c $(LDFLAGS) -lpthread
	
main: $(SRC_DIR)/main.o 
	gcc -o $(SRC_DIR)/main $(SRC_DIR)/main.c $(SRC_DIR)/glopal.c $(SRC_DIR)/Read_Input_File.c -lpthread
	./$(SRC_DIR)/main

clean: 
	rm -f *.o $(BIN_DIR)/supermarket_simulation

remove_all_shm:
	sudo ipcs -m | awk '/^m/ {print $2}' | xargs -n1 ipcrm -m