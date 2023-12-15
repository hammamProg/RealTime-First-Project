#include "ui_global.h"
#include "glopal.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/fcntl.h>
#include <time.h>
#include <GLUT/glut.h>



// ====================================================================================================
// ===================  draw_funcs.c

float layer_1[]={
    -150.0, 00.0,
    -150.0, 150.0,
    150.0, 150.0,
    150.0, 0.0,
};

void Draw_Layer_1() {
    
    glBegin(GL_QUADS);
    glColor3f(0.32, 0.37, 0.88);	
    glVertex2f(layer_1[0], layer_1[1]);
    glVertex2f(layer_1[2], layer_1[3]);
    glVertex2f(layer_1[4], layer_1[5]);
    glVertex2f(layer_1[6], layer_1[7]);
    glEnd();
}

void Draw_Door() {
    glBegin(GL_QUADS);
    glColor3f(0.37, 0.74, 1);	
    glVertex2f(-30, 0);
    glVertex2f(-30, 70);
    glVertex2f(-1, 70);
    glVertex2f(-1, 0);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.37, 0.74, 1);	
    glVertex2f(30, 0);
    glVertex2f(30, 70);
    glVertex2f(1, 70);
    glVertex2f(1, 0);
    glEnd();
}


void Draw_rectangle(int x,int y, int hight, int width, float red, float green, float blue){
    glBegin(GL_QUADS);
    glColor3f(red, green, blue);	
    glVertex2f(x, y);
    glVertex2f(x, y+width);
    glVertex2f(x+hight, y+width);
    glVertex2f(x+hight, y);
    glEnd();
}

void Draw_square(int x,int y, int hight, float red, float green, float blue){
    glBegin(GL_QUADS);
    glColor3f(red, green, blue);	
    glVertex2f(x, y);
    glVertex2f(x, y+hight);
    glVertex2f(x+hight, y+hight);
    glVertex2f(x+hight, y);
    glEnd();
}
void Draw_full_fence(){
    // left_above_center
    for(int y=-10; y<165; y+=15){
        Draw_square(-170,y,10,0.64, 0.64, 0.47);
    }
    // left -> ( under Exit)
    for(int y=-170; y<-80; y+=15){
        Draw_square(-170,y,10,0.64, 0.64, 0.47);
    }

    // Bottom
    for(int x=-170; x<165; x+=15){
        Draw_square(x,-170,10,0.64, 0.64, 0.47);
    }
    // Right
    for(int y=-110; y<165; y+=15){
        Draw_square(160,y,10,0.64, 0.64, 0.47);
    }
    // Top
    for(int x=-170; x<165; x+=15){
        Draw_square(x,160,10,0.64, 0.64, 0.47);
    }
}


void Draw_Sign(int x, int y, float red, float green, float blue) {
    // Green -> White -> Green
    float dimentions[]={
        x,    y,
        x,    y+30,
        x+55, y+30,
        x+55, y,
    };
    int padding = 2;
    int inner_padding = padding+2;

    glBegin(GL_QUADS);
    glColor3f(red, green, blue);	
    glVertex2f(dimentions[0], dimentions[1]);
    glVertex2f(dimentions[2], dimentions[3]);
    glVertex2f(dimentions[4], dimentions[5]);
    glVertex2f(dimentions[6], dimentions[7]);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(1, 1, 1);	
    glVertex2f(dimentions[0]+padding, dimentions[1]+padding);
    glVertex2f(dimentions[2]+padding, dimentions[3]-padding);
    glVertex2f(dimentions[4]-padding, dimentions[5]-padding);
    glVertex2f(dimentions[6]-padding, dimentions[7]+padding);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(red, green, blue);	
    glVertex2f(dimentions[0]+inner_padding, dimentions[1]+inner_padding);
    glVertex2f(dimentions[2]+inner_padding, dimentions[3]-inner_padding);
    glVertex2f(dimentions[4]-inner_padding, dimentions[5]-inner_padding);
    glVertex2f(dimentions[6]-inner_padding, dimentions[7]+inner_padding);
    glEnd();
}

void Draw_divider_between_enter_exit(){
    glBegin(GL_QUADS);
    glColor3f(1, 1, 1);	
    glVertex2f(-175, -76);
    glVertex2f(-175, -74);
    glVertex2f(-100, -74);
    glVertex2f(-100, -76);
    glEnd();

}

void Draw_road(){
    glBegin(GL_QUADS);
    glColor3f(0.18, 0.22, 0.22);
    int x=-175;
    int y=-70;	

    glVertex2f(x, y);
    glVertex2f(x, y+25);
    glVertex2f(0, y+25);
    glVertex2f(0, y);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.18, 0.22, 0.22);
    x=-175;
    y=-105;	
    glVertex2f(x, y);
    glVertex2f(x, y+25);
    glVertex2f(0, y+25);
    glVertex2f(0, y);
    glEnd();
}



void draw_full_supermarket(){
    // Supermarket
    Draw_Layer_1();
    Draw_Door();

    // fence
    Draw_full_fence();

    // enter & exit
    Draw_Sign(-175,-40,0.08, 0.57, 0.2);  // Enter Sign
    Draw_Sign(120,-120,0.98, 0.04, 0.05);// Exit  Sign
    Draw_divider_between_enter_exit();

    // road
    // Draw_road();
}


///////////////////////////////////////////////////////////////
//////////////////////////// Dynamic Objects
///////////////////////////////////////////////////////////////
int CASHIER_WIDTH = 10;
int CASHIER_HEIGHT = 10;

int CUSTOMER_WIDTH = 10;
int CUSTOMER_HEIGHT = 10;


void Draw_Cashier(float x, float y,float red, float green, float blue, int id) {
    glColor3f(red,green,blue);  // Blue color for the cashier
    glBegin(GL_QUADS);
    glVertex2f(x - CASHIER_WIDTH / 2, y - CASHIER_HEIGHT / 2);
    glVertex2f(x + CASHIER_WIDTH / 2, y - CASHIER_HEIGHT / 2);
    glVertex2f(x + CASHIER_WIDTH / 2, y + CASHIER_HEIGHT / 2);
    glVertex2f(x - CASHIER_WIDTH / 2, y + CASHIER_HEIGHT / 2);
    glEnd();

    char str[50] = "Cashier";
    // Concatenate the string with the formatted integer
    sprintf(str + strlen(str), "%d", id);

    DrawTextWithDefaultFont(x-12, y+6,str);

}

void Draw_Customer(float x, float y, float red, float green, float blue, int id) {
    glColor3f(red,green,blue);  // Red color for the customer
    glBegin(GL_QUADS);
    glVertex2f(x - CUSTOMER_WIDTH / 2, y - CUSTOMER_HEIGHT / 2);
    glVertex2f(x + CUSTOMER_WIDTH / 2, y - CUSTOMER_HEIGHT / 2);
    glVertex2f(x + CUSTOMER_WIDTH / 2, y + CUSTOMER_HEIGHT / 2);
    glVertex2f(x - CUSTOMER_WIDTH / 2, y + CUSTOMER_HEIGHT / 2);
    glEnd();

    char str[50] = "customer ";
    // Concatenate the string with the formatted integer
    sprintf(str + strlen(str), "%d", id);


    DrawTextWithDefaultFont(x-12, y+6, str);


    
}

///////////////////////////////////////////////////////////////
//////////////////////////// Flag & Text Draw :)
///////////////////////////////////////////////////////////////


void DrawPalestineFlag(int x, int y, int width, int height) {
    // Draw black stripe
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(x, y + height / 3.0);
    glVertex2f(x + width, y + height / 3.0);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();

    // Draw white stripe
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(x, y - height / 3.0);
    glVertex2f(x + width, y - height / 3.0);
    glVertex2f(x + width, y + height / 3.0);
    glVertex2f(x, y + height / 3.0);
    glEnd();

    // Draw green stripe
    glColor3f(0.0f, 0.6f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(x, y - height);
    glVertex2f(x + width, y - height);
    glVertex2f(x + width, y - height / 3.0);
    glVertex2f(x, y - height / 3.0);
    glEnd();

    // Draw red triangle
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(x, y - height);
    glVertex2f(x + width / 3.0, y);
    glVertex2f(x, y + height );
    glEnd();
}

void DrawText(float x, float y, const char* text, void* font) {
    glRasterPos2f(x, y);

    for (int i = 0; text[i] != '\0'; i++) {
        glutBitmapCharacter(font, text[i]);
    }
}
void DrawTextWithDefaultFont(float x, float y, const char* text) {
    DrawText(x, y, text, GLUT_BITMAP_HELVETICA_12);
}


// ====================================================================================================
// ===================  ui.c


#define SPEED 1

Point *sharedMemory;
Point *current_Points;

bool is_sharedMemory_equal_current_Points(){
    // to make sure that the sharedMemory is equal to current_Points
    for(int i=0;i<anumberOfCustomers;i++){
        if(sharedMemory[i].x != current_Points[i].x || sharedMemory[i].y != current_Points[i].y){
            return false;
        }
    }
    return true;
}

void animate() {
    sem_t *semaphore = sem_open(SEM_KEY_ITEMS, O_CREAT, 0666, 1);

    bool   animationNotComplete = true;  // Assume animation is complete
    Point var;

    wait(semaphore);
    while(animationNotComplete){
        for (int i = 0; i < anumberOfCustomers; ++i) {
            // printf("sharedMemory[%d]=> (%d,%d)\n",sharedMemory[i].x,sharedMemory[i].y);
            var.index=sharedMemory[i].index;
            var.pid=sharedMemory[i].pid;
            var.x=sharedMemory[i].x;
            var.y=sharedMemory[i].y;

            // printf("var%d (%d,%d)\tcurrent(%f,%f)\n",i,var.x,var.y,current_Points[i].x,current_Points[i].y);
            
            // Check if X-axis movement is needed
            if (var.x != current_Points[i].x) {
                if (var.x > current_Points[i].x) {
                    current_Points[i].x += SPEED;
                } else {
                    current_Points[i].x -= SPEED;
                }
            }

            // Check if Y-axis movement is needed
            if (var.y != current_Points[i].y) {
                if (var.y > current_Points[i].y) {
                    current_Points[i].y += SPEED;
                } else {
                    current_Points[i].y -= SPEED;
                }
            }

            // Check if animation for this point is complete
            if (var.x != current_Points[i].x || var.y != current_Points[i].y) {
                animationNotComplete = false;
            }
        }
    }
    sem_post(semaphore);

    if (!animationNotComplete) {
        glutPostRedisplay();
    }
}


// Function to draw all points as customers
void drawAllCustomers() {
    for (int i = 0; i < anumberOfCustomers; ++i) {
        // Compare strings using strcmp
        int result = strcmp(current_Points[i].color, "orange");
        // Check the result
        if (result == 0) {
            Draw_Customer(current_Points[i].x, current_Points[i].y, 1, 0.65, 0.2, i);  // Red color for the customer
        } else {
            Draw_Customer(current_Points[i].x, current_Points[i].y, 1,0,0, i);  // Red color for the customer
        }
    }
}

void initializeOpenGL(int argc, char** argv) {
    // Initialize OpenGL context and window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Supermarket Cashier Simulation");

    // Set up viewport, projection, etc.
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
    glMatrixMode(GL_MODELVIEW);

    // Register callback functions
    glutDisplayFunc(renderScene);
    glutIdleFunc(animate);
    glutKeyboardFunc(handleInput);
    glutReshapeFunc(Reshape);

    // Additional setup if needed
    glClearColor(1,1,1, 1.0);  // Set background color to white
    glutMainLoop();
}
void handleInput(unsigned char key, int x, int y) {
    // Handle user input
    // For example, 'q' to quit the application
    if (key == 'q') {
        cleanup();
        exit(0);
    }
}

void Reshape(int width, int height) {
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-175, 175, -175, 175);
    glMatrixMode(GL_MODELVIEW);
}



void renderScene() {
    // OpenGL rendering code
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw (Market)
    draw_full_supermarket();
    DrawPalestineFlag(-30,110,60,15);
    // Draw (Markt Title)
    glColor3f(1.0f, 1.0f, 1.0f); // Text color
    DrawTextWithDefaultFont(-140, 140, "SuperMarket");
    DrawTextWithDefaultFont(-15, 80, "Welcome :)");
    DrawTextWithDefaultFont(-155, -26, "ENTER");
    DrawTextWithDefaultFont(140, -106, "EXIT");

    // Draw (Cashier)
    for(int i=1; i<= anumberOfCashiers; i++){
        Draw_Cashier(145, i*(-20),0,0,1,i);
    }
    
    // Draw (Customers)
    drawAllCustomers();

    
    glutSwapBuffers();
    glFlush();
}

void updateUI() {
    // Update UI elements based on simulation state
    // Update cashier and customer positions, item scanning progress, etc.
    glutPostRedisplay();
    
}

void cleanup() {
    // Cleanup and release resources
    // Free allocated memory, close OpenGL context, etc.
}
// ================================================


void move_index_x_y_delay(int index, float x, float y, int sleep_amount) {

    // printf("(take semaphore edit)\n");
    sharedMemory[index].x = x;
    sharedMemory[index].y = y;
    // printf("(take semaphore done)\n");

    // printf("new customer%d (%f,%f)\n",index,sharedMemory[index].x,sharedMemory[index].y);
    sleep(sleep_amount);
}

// ====================================================================================================
// ===================  sharedMemory.c
