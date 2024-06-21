#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>
#include <dirent.h>

#define LSH_RL_BUFSIZE 1024
#define LH_FLAG 0
void run_shell(void);

int main() {
    run_shell();
    return EXIT_SUCCESS;
}


void hndls(int ptf){
    

    DIR *dir;
    struct dirent *entry;
    dir = opendir(".");
    if(dir == NULL){
        perror("dir error");
        exit(EXIT_FAILURE);
        }
    while((entry = readdir(dir))){

        if(ptf == LH_FLAG){
            printf("%s\n", entry->d_name);
              
        } 
        else{
        if(!(entry->d_name[0] == '.')){
            printf("%s\n", entry->d_name);
            }       
        }
        
      
        }
    closedir(dir);
}



void run_shell(void) {
    char hn[LSH_RL_BUFSIZE + 1];
    char un[LSH_RL_BUFSIZE + 1];

    if (gethostname(hn, sizeof(hn)) == -1) {
        perror("error in hn: exit code -1");
        exit(EXIT_FAILURE);
    }

    if (getlogin_r(un, sizeof(un)) == -1) {
        perror("error with username");
        exit(EXIT_FAILURE);
    }

    printf("%s@%s$ ", un, hn);

    char input[LSH_RL_BUFSIZE];
    while (1) {
        fgets(input, LSH_RL_BUFSIZE, stdin);
    

        /**
         *  @base
         **/
        

        if(strncmp(input, "ls", 2) == 0){
            hndls(0);

        }
        if(strncmp(input, "man", 3) == 0){
            printf("Shell Commands \n");
            printf("type \"exit\" to exit out of shell\n\n");
        }
        if (strncmp(input, "exit", 4) == 0) {
            break;
        }




        printf("%s@%s$ ", un, hn);
    }
}





