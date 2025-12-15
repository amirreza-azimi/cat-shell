#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define MAX 1024
#define MAX_HISTORY 50

char cwd[MAX];
char *history[MAX_HISTORY];
int history_counts = 0;

void run_command(char **argv) {
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    char cmdline[MAX] = "";

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    for (int i = 0; argv[i]; i++) {
        strcat(cmdline, argv[i]);
        strcat(cmdline, " ");
    }

    char full_command[MAX];
    snprintf(full_command, MAX, "cmd /c %s", cmdline);

    if (!CreateProcessA(
        NULL,
        full_command,
        NULL,
        NULL,
        FALSE,
        0,
        NULL,
        NULL,
        &si,
        &pi
    )) {
        printf("ERROR: Could not run command\n");
        return;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}



typedef struct {
    const char *name;
    void (*func)();
} CustomCommand;

void cmd_hello() {
    printf("Hey Dude!\n");
}

void cmd_clean() {
    char *argv[] = {"cls", NULL};
    run_command(argv);
}

void cmd_path() {
    printf("Your cat is located in: %s\n", cwd);
}

void cmd_foods() {
    printf("Here is all of your cat foods: ");
    char *argv[] = {"dir", NULL};
    run_command(argv);
}

void cmd_help() {
    printf("\nCat is here to help you!\n\nExclusive Commands:\n\n    1.hey -> you can just say hello to computer.\n    2.clean -> this is just cls.\n    3.whereismycat -> this is pwd but with more fun.\n    4.catfood -> showing folder and files like dir.\n\nYou can see a full list of Exclusive Commands in:\n\n    https://www.catweb.com\n\nYou can see a full list of not exclusive commands (cmd commands) in:\n\n    https://learn.microsoft.com/en-us/windows-server/administration/windows-commands/windows-commands\n\n");
}

void cmd_version() {
    printf("\nCat is in version 1.1.0.\n\n");
}

void cmd_history() {
    printf("\nCat Meows:\n");
    for(int i = 0; i < history_counts; i++) {
        printf("  %d: %s\n", i+1, history[i]);
    }
    printf("\n");
}

CustomCommand commands[] = {
    {"hey", cmd_hello},
    {"clean", cmd_clean},
    {"whereismycat", cmd_path},
    {"catfoods", cmd_foods},
    {"help", cmd_help},
    {"mycatversion", cmd_version},
    {"catmeows", cmd_history},
    {NULL, NULL},
};

int parse_input(char *input, char **argv) {
    int argc = 0;

    char *token = strtok(input, " ");
    while (token != NULL && argc < 31) {
        argv[argc++] = token;
        token = strtok(NULL, " ");
    }

    argv[argc] = NULL;
    return argc;
}

int main() {
    char input[MAX];
    // char cwd[MAX];
    
    printf("\033[38;5;208m"); // orange
    
    printf("  ______     ___   .___________.        _______. __    __   _______  __       __ \n");
    printf(" /      |   /   \\  |           |       /       ||  |  |  | |   ____||  |     |  |\n");
    printf("|  ,----'  /  ^  \\ `---|  |----`      |   (----`|  |__|  | |  |__   |  |     |  |\n");
    printf("|  |      /  /_\\  \\    |  |            \\   \\    |   __   | |   __|  |  |     |  |\n");
    printf("|  `----./  _____  \\   |  |        .----)   |   |  |  |  | |  |____ |  `----.|  `----.\n");
    printf(" \\______/__/     \\__\\  |__|        |_______/    |__|  |__| |_______||_______||_______|\n");
    printf("                    +-------------------------------------------+\n");
    printf("                    |                 cat shell                 |\n");
    printf("                    +-------------------------------------------+\n");
    
    printf("\033[0m"); // reset

    while(1){

        GetCurrentDirectoryA(MAX, cwd);
        printf("caaaaaaaaaat> ");
        
        fgets(input, MAX, stdin);
        input[strcspn(input, "\n")] = '\0';
        char *argv[64];
        int argc = parse_input(input, argv);

        if (argc == 0) continue;
        
        if (strcmp(argv[0], "cd") == 0) {
            if (argc < 2) {
                printf("cd: missing path\n");
            } else {
                SetCurrentDirectoryA(argv[1]);
            }
            continue;
        }


        if (strlen(input) == 0) continue;

        if(history_counts < MAX_HISTORY){
            history[history_counts] = strdup(input);
            history_counts++;
        }
        else{
            free(history[0]);
            for(int j = 1; j < MAX_HISTORY; j++){
                history[j-1] = history[j];
            }
            history[MAX_HISTORY-1] = strdup(input);
        }

        if(strcmp(input, "0") == 0){
            printf("have a nice day or night or ... i don't know just bye.\n");
            Sleep(2000);
            break;
        }
        else{
        int i = 0;
        int found = 0;
        while(commands[i].name != NULL) {
            if(strcmp(argv[0], commands[i].name) == 0) {
                commands[i].func();
                found = 1;
                break;
            }
        i++;
        }

        if(!found) {
            run_command(argv);
        }
        }
    }

    for (int i = 0; i < history_counts; i++) {
        free(history[i]);
    }

    return 0;
}


// have a nice day or night or ... i don't know just bye.
