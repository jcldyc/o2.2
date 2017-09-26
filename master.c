#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for fork 
#include <sys/types.h> // for pid_t
#include <sys/wait.h> // for wait 
#include <string.h>
#include <sys/shm.h>

typedef struct Data{
    char palindromeList[50][256];
} SharedData;


void childProcessing(int childIndex, int palindromeIndex);

int main()
{
    //Spawn a child to run the program.
    int processesRunning = 0;
    pid_t pids[19];
    int n = 19;
    int palindromeCount = 0;

    int id;
    int key = 7531;

    //create shared memory array
    SharedData shm;
    SharedData *shmPtr;
    shmPtr = &shm;

    //reserve space for struct and attach to pointer
    if ((id = shmget(key,sizeof(shm), IPC_CREAT | 0666)) < 0)
    {
        perror("SHMGET");
        exit(1);
    }

    if((shmPtr = shmat(id, NULL, 0)) == (SharedData *) -1)
    {
        perror("SHMAT");
        exit(1);
    }

    //attach file
    FILE* file = fopen("stringList.txt", "r");


    //read file into array
    char line[256];
    int i = 0;
    while (fgets(line, sizeof(line), file) && i<50) { 
        sprintf(shmPtr->palindromeList[i], "%s",line);
        //printf("%s", shmPtr->palindromeList[i]);
        i++;
    }
    fclose(file);


    // Spawn 19 children
    for (i = 0; i < n; i++) {
      if ((pids[i] = fork()) < 0) {
        perror("fork");
        abort();
      } else if (pids[i] == 0) {
        printf("forking process number: %d\n",i+1);
        childProcessing(i, palindromeCount);
        printf("error spawning child\n");
        exit(0);
      }
      palindromeCount++;
    }
   

    //Wait for children to exit.
    int status;
    pid_t pid;
    while (n > 0) {
      pid = wait(&status);
      printf("Child with PID %ld exited with status 0x%x.\n", (long)pid, status);
      n--;
    }


    //after all the processes have ran
    printf("All child processes have ran and exited\n");
    shmdt(shmPtr);
        return 0;

}

void childProcessing(int childIndex, int palindromeIndex){

    //int to string
    char index[10];
    snprintf(index, 10, "%d", childIndex);

    char palinIndex[10];
    snprintf(palinIndex,10,"%d", palindromeIndex);

    //set passed arguments. ./palin childIndex palindromeIndex
    char *args[]={"./palin", index, palinIndex};

    printf("executing palin %d for pid %ld\n", childIndex + 1, (long)getpid());
    execvp("./palin", args);
    printf("Error executing palin %s\n", index+1);
    exit(127); //error out
}









