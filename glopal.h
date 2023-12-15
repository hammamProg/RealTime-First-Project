#ifndef GLOBALS_H
#define GLOBALS_H
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
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <limits.h>
#include <semaphore.h>
#include <pthread.h>
#include <signal.h>


#include "Read_Input_File.h"

#define SHM_KEY_ITEMS 0x9999
#define SEM_KEY_ITEMS 333


#define SHM_KEY_BASE 1000
#define MSQ_KEY_BASE 3000
#define TIME_UNIT_us 100000

#define CASHIER_STARTING_BEHAVIOR_MIN 10
#define CASHIER_STARTING_BEHAVIOR_MAX 30

#define END_SIMULATION_CUSTOMER_IMPATIONCE_THRESHOLD 2
#define END_SIMULATION_CASHIER_DROP_THRESHOLD 3


extern int acustomer_arrival_rate_min;
extern int acustomer_arrival_rate_max;
extern int ashopping_time_min;
extern int ashopping_time_max;
extern int ascanning_time_min;
extern int ascanning_time_max;
extern int acustomer_impatience_threshold;
extern int acashier_behavior_degradation_threshold;
extern int asupermarket_income_threshold;
extern int anumberOfCashiers;
extern int anumberOfCustomers;

int fifoExists(const char *fifoName);
int createOrClearFIFO(const char *fifoName);
int openFIFORead(const char *fifoName);
int openFIFOWrite(const char *fifoName);
int createSharedMemory(size_t size, int key);
void *attachSharedMemory(int shmid);
int detachSharedMemory(const void *ptr);
int destroySharedMemory(int shmid);
int createSemaphore(key_t key);
int initSemaphore(int semid, int val);
int semaphoreOperation(int semid, int op);
int createMessageQueue(key_t key);
int sendMessage(int msqid, char *msgText);
int receiveMessage(int msqid, char *msgText);
int random_range(int min, int max);
int random_range_with_randomness(int min, int max, int randomness);

#endif /* GLOBALS_H */