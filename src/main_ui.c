#include "ui_global.h"
#include "glopal.h"

Point *sharedMemory;
sem_t semaphore;
Point *current_Points;


int main(int argc, char** argv) {  
    readInputDataFromFile();
    // !==================================== Semaphore for the sharedMemory ====================================
    sem_t *semaphore = sem_open(SEM_KEY_ITEMS, O_CREAT, 0666, 1);
    // !================================ current_Points ============================================
    int shmid_current = createSharedMemory(sizeof(Point)*anumberOfCustomers, SHM_KEY_CUSTOMERS_UI_current); // Create shared memory segment
    if (shmid_current == -1)
    {
        printf("Error creating Customers Points shared memory\n");
        return 1;
    }

    current_Points = (Point *)attachSharedMemory(shmid_current); // Attach shared memory

    if (current_Points == NULL)
    {
        printf("Error attaching Customers Points shared memory\n");
        return 2;
    }

   
    for(int i=0;i<anumberOfCustomers;i++){
        current_Points[i].x=-180;
        current_Points[i].y=-50;
        current_Points[i].index=i;
        strcpy(current_Points[i].color,"orange");
    }

    // !==================================== Shared Memory for customers ====================================
    int shmid = createSharedMemory(sizeof(Point)*anumberOfCustomers, SHM_KEY_CUSTOMERS_UI); // Create shared memory segment

    if (shmid == -1)
    {
        printf("Error creating Customers Points shared memory\n");
        return 1;
    }

    sharedMemory = (Point *)attachSharedMemory(shmid); // Attach shared memory

    if (sharedMemory == NULL)
    {
        printf("Error attaching Customers Points shared memory\n");
        return 2;
    }
    

    // For initialize the values of the shared memory
    for(int i=0;i<anumberOfCustomers;i++){
        sharedMemory[i].x= -180;
        sharedMemory[i].y= -50;
        sharedMemory[i].index=i;
    }
    initializeOpenGL(argc, argv);

    return 0;
}
