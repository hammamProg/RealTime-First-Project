#ifndef GLOBALSS_H

#define GLOBALSS_H
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <GLUT/glut.h>
#include <stdbool.h>
#include <signal.h>


//############################################################################# static
void draw_full_supermarket();
void DrawPalestineFlag(int x, int y, int width, int height);
//############################################################################# dynamic
void DrawTextWithDefaultFont(float x, float y, const char* text);
void Draw_Customer(float x, float y, float red, float green, float blue, int id);
void Draw_Cashier(float x, float y,float red, float green, float blue,int id);
//############################################################################# 

#define SHM_KEY_CUSTOMERS_UI 50000
#define SHM_KEY_CUSTOMERS_UI_current 11000

typedef struct {
    float x;
    float y;
    int pid;
    int index;
    char color[10];
} Point;

extern Point *sharedMemory;
extern sem_t semaphore;
extern Point *current_Points;

bool is_sharedMemory_equal_current_Points();

//############################################################################# 
//############################################################################# 

//############################ ui.h
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

void initializeOpenGL(int argc, char** argv);
void Reshape(int width, int height);
void renderScene();
void updateUI();
void handleInput(unsigned char key, int x, int y);
void cleanup();
void drawCustomerBlock(int x,int y,int height,int width);
void changeEditedPoint(int index, float newX, float newY);
//############################
void move_index_x_y_delay(int index, float x, float y, int sleep_amount);

#endif /* GLOBALSS_H */