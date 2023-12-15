#include "ui_global.h"
#include "glopal.h"
#include "queue.h"
sem_t semaphore;


int main(int argc, char *argv[])
{
    readInputDataFromFile();
    /*==================================== Shared Memory for customers ====================================*/
    int shmid_ui = createSharedMemory(sizeof(Point)*anumberOfCustomers, SHM_KEY_CUSTOMERS_UI); // Create shared memory segment

    if (shmid_ui == -1)
    {
        printf("Error creating Customers Points shared memory\n");
        return 1;
    }

    sharedMemory = (Point *)attachSharedMemory(shmid_ui); // Attach shared memory

    if (sharedMemory == NULL)
    {
        printf("Error attaching Customers Points shared memory\n");
        return 2;
    }



    int cashier_number = atoi(argv[1]);

    /*---------------------------------------Shared Memory -----------------------------------------*/
    int shmid = createSharedMemory(sizeof(CashierData), SHM_KEY_BASE + cashier_number); // Create shared memory segment

    if (shmid == -1)
    {
        printf("Error creating shared memory\n");
        return EXIT_FAILURE;
    }

    CashierData *cashierData = (CashierData *)attachSharedMemory(shmid); // Attach shared memory

    if (cashierData == NULL)
    {
        printf("Error attaching shared memory\n");
        return EXIT_FAILURE;
    }

    char *order = (char *)malloc(sizeof(char) * 257);
    int cashier_behavior = random_range(CASHIER_STARTING_BEHAVIOR_MIN, CASHIER_STARTING_BEHAVIOR_MAX);
    int scanning_time = random_range(ascanning_time_min, ascanning_time_max);
    int income = 0;
    int time = 0;
    int WaitUntil = 0;
    int timeToFinishTheCustomer = 0;
    int timeToFinishTheQueue = 0;
    int numberOfRemainedOrders = 0;
    int served_customer_number = -1;
    int enterded_customer_number = -1;
    int msqid = atoi(argv[2]);
    int msqState = 0;
    struct msqid_ds buf;
    // GQueue* myQueue = g_queue_new(); // Create a linked list as a queue
    struct Queue* myQueue = createQueue(MAX_SIZE);
    cashierData->timeToFinishTheQueue = 0;
    cashierData->number_of_served_customers = 0;
    cashierData->done_customer = -1;
    

    bool isOneEnterdOrExit = false;
    while (1)
    {
        msqState = msgctl(msqid, IPC_STAT, &buf);
        if (msqState == -1) {
            perror("msgctl");
            exit(EXIT_FAILURE);
        } else if (buf.msg_qnum > 0) {
            receiveMessage(msqid, order);
            enqueue(myQueue, atoi(strtok(order, " "))); //price
            timeToFinishTheCustomer = atoi(strtok(NULL, " ")) * scanning_time;

            timeToFinishTheQueue += timeToFinishTheCustomer;
            enqueue(myQueue, timeToFinishTheCustomer);
            enterded_customer_number = atoi(strtok(NULL, " "));
            enqueue(myQueue, enterded_customer_number);
            
            numberOfRemainedOrders++;
            cashierData->number_of_served_customers++;
            fflush(stdout);

            isOneEnterdOrExit = true;
        }
        if (WaitUntil <= time) {
            if (WaitUntil == time) { // a customer done
                cashierData->done_customer = served_customer_number; // in first customer -> served_customer_number = -1
                cashierData->cashier_income = income; // in first customer -> income = 0

                if (cashierData->done_customer != -1) {
                    printf("customer%d finish shoping by cashier%d, timeToFinishTheQueue = %d, income = %d\n"
                        , cashierData->done_customer, cashier_number, timeToFinishTheQueue, cashierData->cashier_income); // testing (!)
                    fflush(stdout);
                }
            }
            if (numberOfRemainedOrders > 0) { // starting serve a new customer
                income += dequeue(myQueue); // Dequeue an element
                WaitUntil = numberOfRemainedOrders == 1 ? dequeue(myQueue) + time : dequeue(myQueue) + WaitUntil;
                served_customer_number = dequeue(myQueue);
                cashierData->client_being_served = served_customer_number;
                
                numberOfRemainedOrders--;
                cashierData->number_of_served_customers++; //! current customer number
                isOneEnterdOrExit = true;
            }
        }

        if (isOneEnterdOrExit) {
            printf("number of customer with cashier%d = %d\n--------------------------\n", cashier_number, numberOfRemainedOrders+1); // testing (!)
            fflush(stdout);
            isOneEnterdOrExit = false;
        }
        time++;
        usleep(TIME_UNIT_us);
        if (timeToFinishTheQueue > 0) timeToFinishTheQueue--;
        cashierData->timeToFinishTheQueue = timeToFinishTheQueue;

        if (time % 20 == 0 && served_customer_number >= 0) { // Cashiers’ positive behavior drops with time
            cashier_behavior -= random_range(0, acashier_behavior_degradation_threshold);
            cashierData->cashier_behavior = cashier_behavior;
        }

        if (cashier_behavior <= 0)
        {
            // signal to main.c 
            if (kill(getppid(), SIGUSR2) == -1) {
                perror("Signal SIGUSR2 sending failed");
                exit(EXIT_FAILURE);
            }
            cashierData->cashier_availability = false;
            break;
        }
        if (cashierData->cashier_income >= asupermarket_income_threshold)
        {
            // signal to main.c 
            if (kill(getppid(), SIGINFO) == -1) {
                perror("Signal SIGINFO sending failed");
                exit(EXIT_FAILURE);
            }
        }
        
    }

    // Free the memory used by the queue
    freeQueue(myQueue);
    detachSharedMemory(shmid_ui);
    detachSharedMemory(cashierData); // Detach shared memory

    free(order);

    destroySharedMemory(shmid);
    return 0;
}