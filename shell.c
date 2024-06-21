#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>
#include <dirent.h>

#define LSH_RL_BUFSIZE 1024
#define LSH_TOK_BUFSIZE 64
#define LH_FLAG 0

void run_shell(void);
void hndls(int ptf);
void fmproc(char **tokens);

int main() {
    run_shell();
    return EXIT_SUCCESS;
}

void hndls(int ptf) {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(".");
    if (dir == NULL) {
        perror("dir error");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
        if (ptf == LH_FLAG) {
            printf("%s\n", entry->d_name);
        } else {
            if (!(entry->d_name[0] == '.')) {
                printf("%s\n", entry->d_name);
            }
        }
    }

    closedir(dir);
}

void run_shell(void) {
    char hn[LSH_RL_BUFSIZE + 1];
    char un[LSH_RL_BUFSIZE + 1];
    char input[LSH_RL_BUFSIZE];
    char *tokens[LSH_TOK_BUFSIZE];
    char *token;
    int token_count;

    if (gethostname(hn, sizeof(hn)) == -1) {
        perror("error in hn");
        exit(EXIT_FAILURE);
    }

    if (getlogin_r(un, sizeof(un)) == -1) {
        perror("error with username");
        exit(EXIT_FAILURE);
    }

    while (1) {
        token_count = 0;

        printf("%s@%s$ ", un, hn);
        fgets(input, LSH_RL_BUFSIZE, stdin);

        token = strtok(input, " \t\r\n\a");
        while (token != NULL && token_count < LSH_TOK_BUFSIZE - 1) {
            tokens[token_count] = token;
            token_count++;
            token = strtok(NULL, " \t\r\n\a");
        }
        tokens[token_count] = NULL; 

        if (tokens[0] == NULL) {
            perror("empty command");
        } else if (strcmp(tokens[0], "cd") == 0) {
            if (token_count < 2) {
                fprintf(stderr, "cd: missing directory\n");
            } else if (chdir(tokens[1]) != 0) {
                perror("cd");
            }
        } else if (strncmp(tokens[0], "ls", 2) == 0) {
            hndls(0);
        } else if (strncmp(tokens[0], "man", 3) == 0) {
            printf("Shell Commands\n");
            printf("Type \"exit\" to exit out of shell\n\n");
        } else if (strncmp(tokens[0], "exit", 4) == 0) {
            break;
        } else {
            fmproc(tokens);
        }
    }
}

void fmproc(char **tokens) {
    pid_t pid, wtpid;
    int status;

    pid = fork();
    if (pid == 0) {
        if (execvp(tokens[0], tokens) == -1) {
            perror("execvp error");
            exit(EXIT_FAILURE);
        }
    } else if (pid < 0) {gc
        perror("fork error");
    } else {
        do {
            wtpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
}
