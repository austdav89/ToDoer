
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#include "todoer.h"
#include "data.h"

const char *homeDir;
char *fileName = "/.todoer";

int loadData(Todo **headptr, Todo **tailptr){
    FILE *fptr;
    char fullPath[MAX_PATH_SIZE];
    char *buff = NULL;
    size_t lineMax = LINE_MAX;
    int lineSize;
    Todo *head = *headptr;
    Todo *tail = *tailptr;
    Todo *trav = head = NULL;

    if ((homeDir = getenv("HOME")) == NULL) {
        homeDir = getpwuid(getuid())->pw_dir;
    }

    strcpy(fullPath, homeDir);
    strcat(fullPath, fileName);
    
    if ((fptr = fopen(fullPath, "r"))){
        head = trav = tail = createTodo();
        while ((lineSize = getline(&buff, &lineMax, fptr)) >= 0){
            if (trav == NULL){
                tail->next = trav = createTodo();
            }
            if (lineSize > 1){
                buff[lineSize-1] = '\0';
                trav->name = malloc(sizeof(char) * lineSize);
                if (trav->name != NULL){
                    strcpy(trav->name, buff);
                }
                else
                    exit(1);
            }
            lineSize = getline(&buff, &lineMax, fptr);
            trav->done = (buff[0] == '0') ? 0 : 1;
            tail = trav;
            trav = trav->next;
        }
        fclose(fptr);
        free(buff);
        *headptr = head;
        *tailptr = tail;

        return 1;
    } else {
        return 0;
      }
}

int saveData(Todo *data, int num){
    FILE *fptr;
    char fullPath[MAX_PATH_SIZE];

    if ((homeDir = getenv("HOME")) == NULL) {
        homeDir = getpwuid(getuid())->pw_dir;
    }
    strcpy(fullPath, homeDir);
    strcat(fullPath, fileName);
    //printf("Saving Data to %s...\n", fullPath);
    
    if (num){
        fptr = fopen(fullPath, "w"); 
        if (fptr != NULL){
            for (int i = 0; i < num; i++){
                fwrite(data->name, strlen(data->name), 1, fptr);
                fwrite("\n", 1, 1, fptr);
                fwrite(data->done ? "1":"0", 1, 1, fptr);
                fwrite("\0\n", 2 , 1, fptr);
                data = data->next;
            }
            fclose(fptr);
        }
    }else {
        remove(fullPath);
    }
    return true;
}
