/*
 * main.c
 *
 *  Created on: Oct 18, 2019
 *      Author: artem
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#define limit 4097

int main() {
    char s[limit];
    pid_t check = 0;

    while(fgets(s, limit, stdin) != NULL) {
        if(s[0] == '\n')
            continue;
        int len = 0;
        int i;
        for(i = 0; s[i]; i++)
            if(s[i] == ' '
            && (s[i+1] != ' ' || s[i+1] != '\n' || s[i+1] != '\0'))
                len++;
        len++;
        char **args = (char**)calloc(len+1, sizeof(char*));
        if(args == NULL) {
            fprintf(stderr, "Calloc failed!\n");
            return 0;
        }
        char *sup;
        char *sup2 = s;
        int j;
        for(j = 0, sup = strchr(s, ' '); sup; j++, sup2 = sup, sup = strchr(sup, ' ')) {
            args[j] = sup2;
            *sup = '\0';
            sup++;
            while(*sup == ' ')
                sup++;
            //printf("args[%d] = %s ", j, args[j]);
        }
        if(*sup2 != '\n' && *sup2 != '\0') {
            args[j] = sup2;
            int sup2len = strlen(sup2);
            if(sup2len > 0 && args[j][sup2len - 1] == '\n') // В конце пробелов быть не может (в противном случае не зашли бы в цикл)
                args[j][--sup2len] = '\0';
            j++;
        }
        //printf("args[%d] = %s ", j, args[j]);
        args[j] = NULL;
        check = fork();
        if(check == -1)
            perror("fork");
        else
            if(check) {
                int l;
                if(wait(&l) == -1)
                    perror("wait");
            }
            else {
                execvp(args[0], args);
                perror("execvp");
                free(args);
                return 0;
            }
        free(args);
    }
    return 0;
}
