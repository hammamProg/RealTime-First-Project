#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include "Read_Input_File.h"
#include "Cashier.h"
#include <signal.h>
#include "glopal.h"

volatile int impatient_customers_count = 0;  // Counter to be incremented in the signal handler
volatile int cashiers_behaivs_0_count = 0;  
volatile bool one_cashier_income_threshold = false;  
volatile int num_success_customers = 0;  

// Signal handler function
void signalHandler(int signum) {
    if (signum == SIGUSR1) {
        impatient_customers_count++;
        printf("\n\n\n++ Impatient Customer!\n");
    }
}

void signal2Handler(int signum) {
    if (signum == SIGUSR2) {
        cashiers_behaivs_0_count++;
        printf("\n\n\n++ Cashiers drop behaivor!\n");
    }
}

void signal3Handler(int signum) {
    if (signum == SIGINFO) {
        one_cashier_income_threshold=true;
        printf("\n\n\n++ One cashier income threshold!\n");
    }
}
void signal4Handler(int signum) {
    if (signum == SIGALRM) {
        num_success_customers++;
        printf("\n\n\n++ num_success_customers!\n");
    }
}

int main()
{
    // Register the signal handler
    if (signal(SIGUSR1, signalHandler) == SIG_ERR) {
        perror("Signal registration failed");
        exit(EXIT_FAILURE);
    }
    if (signal(SIGUSR2, signal2Handler) == SIG_ERR) {
        perror("Signal registration failed");
        exit(EXIT_FAILURE);
    }
    if (signal(SIGINFO, signal3Handler) == SIG_ERR) {
        perror("Signal registration failed");
        exit(EXIT_FAILURE);
    }
    if (signal(SIGALRM, signal4Handler) == SIG_ERR) {
        perror("Signal registration failed");
        exit(EXIT_FAILURE);
    }

    // ?================== scan the items from the file into our store_items_shared_memory
    readItemsFromFile(MAX_NUM_ITEMS);
    readInputDataFromFile();
    
    printf("****************************************************************\n");
    printf("Number Of Cashiers = %d\n", anumberOfCashiers);
    printf("Number Of Customers = %d\n", anumberOfCustomers);
    printf("Supermarket Income Threshold = %d\n", asupermarket_income_threshold);
    printf("****************************************************************\n");
    fflush(stdout);

    pid_t ui_pid,cashier_pids[anumberOfCashiers], customer_pids[anumberOfCustomers];

    char str_i[5];
    char str_msqid[20];
    char str_msqids[20 * anumberOfCashiers];
    int msqids[anumberOfCashiers];
    
    sprintf(str_msqids, "");


    //! ============================ run (ui process) ========================================================
    ui_pid = fork();
    if (ui_pid == -1)
    {
        perror("fork ui");
        exit(EXIT_FAILURE);
    }
    // process for customer
    else if (ui_pid == 0)
    {
        execl("./main_ui", "main_ui", NULL);
        perror("execl");
        exit(EXIT_FAILURE);
    }

    // Cashier code
    //! ============================ run (Cashier processes) ========================================================
    for (int i = 0; i < anumberOfCashiers; i++)
    {

        sprintf(str_i, "%d", i); // for cashier number
        int msqid = createMessageQueue(MSQ_KEY_BASE + i);

        msqids[i] = msqid;
        sprintf(str_msqid, "%d", msqid);
        strcat(str_msqids, str_msqid);
        if (i < anumberOfCashiers - 1)
            strcat(str_msqids, " ");

        cashier_pids[i] = fork();
        if (cashier_pids[i] == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        // process for cashier
        else if (cashier_pids[i] == 0)
        {
            execl("./Cashier", "Cashier", str_i, str_msqid, NULL);
            perror("execl");
            exit(EXIT_FAILURE);
        }
    }

    //! ============================ run (Customer processes) ========================================================
    for (int i = 0; i < anumberOfCustomers; i++)
    {
        int time_to_arraive = random_range(acustomer_arrival_rate_min, acustomer_arrival_rate_max);
        sprintf(str_i, "%d", i); // for customer number

        printf("customer%d to arrive after %d seconds\n", i, time_to_arraive); // testing
        sleep(time_to_arraive);//! wait for customer to arrive
        customer_pids[i] = fork();
        if (customer_pids[i] == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (customer_pids[i] == 0)
        {
            execl("./Customer", "Customer", str_i, str_msqids, NULL);
            perror("execl");
            exit(EXIT_FAILURE);
        }
    }

    //! ============================ run (Main Process) ========================================================
    //TODO   END simulation if:     :[X] 1- all customers are served ]
    //TODO                          :[X] 2- #(cashiers behaivors) > (threshold)
    //TODO                          :[X] 3- #(customers became impation) > (threshold)
    //TODO                          :[X] 4- 1 cashier income > (threshold)

    while (1)
    {
        // END simulation condition:
        if(impatient_customers_count > END_SIMULATION_CUSTOMER_IMPATIONCE_THRESHOLD){
            printf("End simulation condition: impatient_customers_count\n");
            break;
        }
        if(cashiers_behaivs_0_count > END_SIMULATION_CASHIER_DROP_THRESHOLD){
            printf("End simulation condition: cashiers_behaivs_0_count\n");
            break;
        }
        if(one_cashier_income_threshold){
            printf("End simulation condition: one_cashier_income_threshold\n");
            break;
        }
        if(num_success_customers == anumberOfCustomers){
            printf("End simulation condition: all customer finished successfully\n");
            break;
        }
    }
    

    for (int i = 0; i < anumberOfCustomers; i++)
        kill(customer_pids[i], 9);

    for (int i = 0; i < anumberOfCashiers; i++)
        kill(cashier_pids[i], 9);

    for (int i = 0; i < anumberOfCashiers; i++) {
        if (msgctl(msqids[i], IPC_RMID, (struct msqid_ds *)0) == -1) {
            perror("msgctl failed");
    }
    }    
    printf("****************************************************************\n Waiting for ui to finish\n\n");
    waitpid(ui_pid, NULL, 0);
    // kill(ui_pid, 9);

    return 0;
}