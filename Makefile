CFLAGS  = -Wall -Wextra -std=c99
LDFLAGS = -lpthread -framework OpenGL -framework GLUT -DGL_SILENCE_DEPRECATION
BIN_DIR = bin


all: main_ui Customer Cashier main
	
main_ui: main_ui.o
	gcc $(CFLAGS) -o main_ui main_ui.c ui_global.c glopal.c Read_Input_File.c $(LDFLAGS) -lpthread

Customer: Customer.o
	gcc $(CFLAGS) -o Customer Customer.c ui_global.c glopal.c Read_Input_File.c $(LDFLAGS) -lpthread

Cashier: Cashier.o
	gcc $(CFLAGS) -o Cashier Cashier.c ui_global.c glopal.c Read_Input_File.c queue.c $(LDFLAGS) -lpthread
	
main: main.o 
	gcc -o main main.c glopal.c Read_Input_File.c -lpthread
	./main

clean: 
	rm -f *.o $(BIN_DIR)/supermarket_simulation

remove_all_shm:
	sudo ipcs -m | awk '/^m/ {print $2}' | xargs -n1 ipcrm -m