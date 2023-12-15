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

int acustomer_arrival_rate_min;
int acustomer_arrival_rate_max;
int ashopping_time_min;
int ashopping_time_max;
int ascanning_time_min;
int ascanning_time_max;
int acustomer_impatience_threshold;
int acashier_behavior_degradation_threshold;
int asupermarket_income_threshold;
int anumberOfCashiers;
int anumberOfCustomers;

// FIFO

// Check if FIFO exists
int fifoExists(const char *fifoName)
{
    if (access(fifoName, F_OK) != -1)
    {
        return 1;
    }
    return 0;
}

// Create or clear FIFO
int createOrClearFIFO(const char *fifoName)
{
    struct stat stat_buffer;
    int exists = stat(fifoName, &stat_buffer);

    if (exists == 0)
    {
        // FIFO exists, unlink it to clear
        if (unlink(fifoName) == -1)
        {
            perror("Error unlinking FIFO");
            return -1;
        }
    }

    // Create the FIFO
    int res = mkfifo(fifoName, 0666);
    if (res == -1)
    {
        perror("Error creating FIFO");
        return -1;
    }

    return 0;
}

// Read FIFO
int openFIFORead(const char *fifoName)
{
    int fd = open(fifoName, O_RDONLY);
    if (fd == -1)
    {
        perror("Error opening FIFO for reading");
        return -1;
    }
    return fd;
}

// Write FIFO
int openFIFOWrite(const char *fifoName)
{
    int fd = open(fifoName, O_WRONLY);
    if (fd == -1)
    {
        perror("Error opening FIFO for writing");
        return -1;
    }
    return fd;
}

// Shared Memory

// Create Shared Memory
int createSharedMemory(size_t size, int key)
{
    int shmid = shmget(key, size, IPC_CREAT | 0666);
    if (shmid == -1)
    {
        perror("Error creating shared memory");
        return -1;
    }
    return shmid;
}

// Attach Shred Memory
void *attachSharedMemory(int shmid)
{
    void *ptr = shmat(shmid, NULL, 0);
    if (ptr == (void *)-1)
    {
        perror("Error attaching to shared memory");
        return NULL;
    }
    return ptr;
}

// DeAttach Shred Memory
int detachSharedMemory(const void *ptr)
{
    int res = shmdt(ptr);
    if (res == -1)
    {
        perror("Error detaching from shared memory");
        return -1;
    }
    return 0;
}

// Destroy Shared Memory
int destroySharedMemory(int shmid)
{
    int res = shmctl(shmid, IPC_RMID, NULL);
    if (res == -1)
    {
        perror("Error destroying shared memory");
        return -1;
    }
    return 0;
}

// Semaphore

// Create Semaphore
int createSemaphore(key_t key)
{
    int semid = semget(key, 1, IPC_CREAT | 0666); // Create a semaphore
    if (semid == -1)
    {
        perror("Error creating semaphore");
        return -1;
    }
    return semid;
}

// Initalize Semaphore
int initSemaphore(int semid, int val)
{
    union semun
    {
        int val;
        struct semid_ds *buf;
        unsigned short *array;
    } arg;
    arg.val = val;
    int res = semctl(semid, 0, SETVAL, arg); // Set the semaphore value
    if (res == -1)
    {
        perror("Error initializing semaphore");
        return -1;
    }
    return 0;
}

// perform semaphore operations (wait and signal)
int semaphoreOperation(int semid, int op)
{
    struct sembuf semaphore;
    semaphore.sem_num = 0;
    semaphore.sem_op = op;
    semaphore.sem_flg = 0;
    int res = semop(semid, &semaphore, 1);
    if (res == -1)
    {
        perror("Error performing semaphore operation");
        return -1;
    }
    return 0;
}

// Message Queue

// Create Message Queue
int createMessageQueue(key_t key)
{
    int msqid = msgget(key, IPC_CREAT | 0666); // Create a message queue
    if (msqid == -1)
    {
        perror("Error creating message queue");
        return -1;
    }
    return msqid;
}

// Send to Message Queue
int sendMessage(int msqid, char *msgText)
{
    struct msgbuf
    {
        long mtype;
        char mtext[256];
    } message;
    message.mtype = 1;
    strcpy(message.mtext, msgText);

    int res = msgsnd(msqid, &message, sizeof(message.mtext), 0); // Send message
    if (res == -1)
    {
        perror("Error sending message");
        return -1;
    }
    return 0;
}

// Recive from Message Queue
int receiveMessage(int msqid, char *msgText)
{
    struct msgbuf
    {
        long mtype;
        char mtext[256];
    } message;

    int res = msgrcv(msqid, &message, sizeof(message.mtext), 1, 0); // Receive message
    if (res == -1)
    {
        perror("Error receiving message");
        fflush(stdout);
        return -1;
    }
    strcpy(msgText, message.mtext);

    return 0;
}

// Random
int random_range(int min, int max)
{
    srand(getpid() * time(NULL));
    return min + rand() % (max - min + 1);
}

int random_range_with_randomness(int min, int max, int randomness)
{
    srand(getpid() * time(NULL) + randomness);
    return min + rand() % (max - min + 1);
}