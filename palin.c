#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>


typedef struct Data{
    char palindromeList[50][256];
} SharedData;


int main(int argc, char *argv[]){

    int id;
    int key = 7531;


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



    int processIndex = atoi(argv[1]);
    //char palindromeString[256];
    //strcpy(palindromeString, argv[2]);
    int palindromeIndex = atoi(argv[2]); 


    char myString[256];
    strncpy(myString, shmPtr->palindromeList[palindromeIndex],256);







    printf("Index: %d StringIndex#: %d String: %s", processIndex, palindromeIndex, shmPtr->palindromeList[palindromeIndex]);
    sleep(1);
    return 0;
}