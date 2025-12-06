#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define MAX 1024
#define MAX_HISTORY 50

char cwd[MAX];
char *history[MAX_HISTORY];
int history_counts = 0;

typedef struct {
    const char *name;
    void (*func)();
} CustomCommand;

void cmd_hello() {
    printf("Hey Dude!\n");
}

void cmd_clean() {
    system("cls");
}

void cmd_path() {
    printf("Your cat is located in: %s\n", cwd);
}

void cmd_food() {
    printf("Here is all of your cat food: ");
    system("dir");
}

void cmd_help() {
    printf("\nCat is here to help you!\n\nExclusive Commands:\n\n    1.hello -> you can just say hello to computer.\n    2.clean -> this is just cls.\n    3.whereismycat -> this is pwd but with more fun.\n    4.catfood -> showing folder and files like dir.\n\nYou can see a full list of not exclusive commands (cmd commands) in:\n\n    https://learn.microsoft.com/en-us/windows-server/administration/windows-commands/windows-commands\n\n");
}

void cmd_version() {
    printf("\nCat is in version 1.0.0.\n\n");
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
    {"catfood", cmd_food},
    {"help", cmd_help},
    {"mycatversion", cmd_version},
    {"catmeows", cmd_history},
    {NULL, NULL},
};



void run_command(char *cmd) {
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    char command[MAX];
    strcpy(command, cmd);

    char full_command[MAX];
    snprintf(full_command, MAX, "cmd /c %s", command);

    if(!CreateProcessA(
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
        printf("ERROR: Could not run '%s'\n", cmd);
        return;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
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
            if(strcmp(input, commands[i].name) == 0) {
                commands[i].func();
                found = 1;
                break;
            }
        i++;
        }

        if(!found) {
            run_command(input);
        }
    }


        if(strncmp(input, "cd ", 3) == 0){
            char *path = input + 3;
            if(SetCurrentDirectoryA(path)){

            } else {
                printf("ERROR: Could not change directory to '%s'\n", path);
            }
        }
    }

    return 0;
}


// have a nice day or night or ... i don't know just bye.


