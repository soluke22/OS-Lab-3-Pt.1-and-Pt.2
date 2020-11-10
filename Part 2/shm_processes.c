#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>

void  StudentProcess(int []);
void  DadProcess(int []);
int turn=0; 

int  main(int  argc, char *argv[])
{
     int    ShmID;
     int    *ShmPTR;
     pid_t  pid;
     int    status;
  
     srandom(getpid());

     if (argc != 3) {
         printf("Use: %s 0 0\n", argv[0]);
        
     }

     ShmID = shmget(IPC_PRIVATE, 4*sizeof(int), IPC_CREAT | 0666);
     if (ShmID < 0) {
          printf("*** shmget error (server) ***\n");
          exit(1);
     }
     printf("Server has received a shared memory of four integers...\n");

     ShmPTR = (int *) shmat(ShmID, NULL, 0);
     if (*ShmPTR == -1) {
          printf("*** shmat error (server) ***\n");
          exit(1);
     }
     printf("Server has attached the shared memory...\n");

     ShmPTR[0] = atoi(argv[1]);
     ShmPTR[1] = atoi(argv[2]);
  
     printf("Server has filled %d %d %d %d in shared memory...\n",
            ShmPTR[0], ShmPTR[1], ShmPTR[2], ShmPTR[3]);

     printf("Server is about to fork a child process...\n");
     pid = fork();
     if (pid < 0) {
          printf("*** fork error (server) ***\n");
          exit(1);
     }
     else if (pid == 0) {
          StudentProcess(ShmPTR);
          exit(0);
     }
    else{
        DadProcess(ShmPTR);
      
    }
    wait(&status);
    printf("Server has detected the completion of its child...\n");
    shmdt((void *) ShmPTR);
    printf("Server has detached its shared memory...\n");
    shmctl(ShmID, IPC_RMID, NULL);
    printf("Server has removed its shared memory...\n");
    printf("Server exits...\n");
    exit(0);
       
}


void  DadProcess(int  SharedMem[])
{
    int i = 0;
    int BankAccount = SharedMem[0];
    int balance; 
    
   
    for(i=0; i<25;i++)
    {
      sleep(random() %5); 
      
      
        while(SharedMem[1]!=0)
        { 
            }
      BankAccount = SharedMem[0];
      
       if(BankAccount<=100)
       {
        balance=random() % 100; 
        
          if(balance % 2 == 0)
          {
            BankAccount = BankAccount + balance; 
            printf("Dear old Dad: Deposits $%d / Balance = $%d\n", balance, BankAccount);
          }
          else
          {
            printf("Dear old Dad: Doesn't have any money to give\n");
          }
       }
      else
      {
        printf("Dear old Dad: Thinks Student has enough Cash ($%d)\n", BankAccount);
      }
      
      SharedMem[0]=BankAccount; 
      SharedMem[1]=1;
    }

}

void  StudentProcess(int  SharedMem[])
{
    int i = 0;
    int BankAccount = SharedMem[0];
    int balanceNeed; 
    
    for(i=0; i<25;i++)
    {
      sleep(random() %5); 
      

      
        while(SharedMem[1]!=1)
        {
        }
      BankAccount = SharedMem[0];
      balanceNeed=random()%50;
      printf("Poor Student needs $%d\n", balanceNeed);
       
      if(balanceNeed<=BankAccount)
      {
        BankAccount=BankAccount-balanceNeed;
        printf("Poor Student: Withdraws $%d / Bank Account Balance = $%d\n", balanceNeed, BankAccount);
      }
      else
      {
        printf("Poor Student: Not Enough Cash ($%d)\n", BankAccount );
      }
      SharedMem[0]=BankAccount;
      
      SharedMem[1]=0;
    }
}