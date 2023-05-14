// The MIT License (MIT)
// 
// Copyright (c) 2016 Trevor Bakker 
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
#define _GNU_SOURCE

#include <stdio.h>

#include <unistd.h>

#include <sys/wait.h>

#include <stdlib.h>

#include <errno.h>

#include <string.h>

#include <signal.h>

#define WHITESPACE " \t\n" // We want to split our command line up into tokens
// so we need to define what delimits our tokens.
// In this case  white space
// will separate the tokens on our command line

#define MAX_COMMAND_SIZE 255 // The maximum command-line size

#define MAX_NUM_ARGUMENTS 11 // Mav shell only supports 11 arguments 
// [one main command and 10 arguments to that command]

#define MAX_HISTORY 15 // Max number of commands to store in history

int main() {

    char * command_string = (char * ) malloc(MAX_COMMAND_SIZE);
    char * hist[MAX_HISTORY];
    int hist_count = 0;
    int pid_history[MAX_HISTORY];

    for (int i = 0; i < MAX_HISTORY; i++) {
        hist[i] = (char * ) malloc(MAX_COMMAND_SIZE);
    }
    for (int i = 0; i < MAX_HISTORY; i++) {
        hist[i] = NULL;
        pid_history[i] = 0;
    }

    while (1) {
        // Print out the msh prompt
        printf("msh> ");

        // Read the command from the commandline.  The
        // maximum command that will be read is MAX_COMMAND_SIZE
        // This while command will wait here until the user
        // inputs something since fgets returns NULL when there
        // is no input
        while (!fgets(command_string, MAX_COMMAND_SIZE, stdin));

        //added this if statement to check for empty input ex: hitting enter or tab key or spacebar
        if (command_string[0] == '\n' || command_string[0] == '\t' || command_string[0] == ' ') {
            continue;
        }

        // printf("Command entered: %s, length: %lu,", command_string, strlen(command_string));

        /* Parse input */
        char * token[MAX_NUM_ARGUMENTS];

        for (int i = 0; i < MAX_NUM_ARGUMENTS; i++) {
            token[i] = NULL;
        }

        int token_count = 0;

        // Pointer to point to the token
        // parsed by strsep
        char * argument_ptr = NULL;

        char * working_string = strdup(command_string);

        // we are going to move the working_string pointer so
        // keep track of its original value so we can deallocate
        // the correct amount at the end
        char * head_ptr = working_string;

        // Tokenize the input strings with whitespace used as the delimiter
        while (((argument_ptr = strsep( & working_string, WHITESPACE)) != NULL) &&
            (token_count < MAX_NUM_ARGUMENTS)) {
            token[token_count] = strndup(argument_ptr, MAX_COMMAND_SIZE);
            if (strlen(token[token_count]) == 0) {
                token[token_count] = NULL;
            }
            token_count++;
        }

        //checking for !n command 
        if (token[0][0] == '!' && token[0][1] != '\0') {
            int idx = atoi( & token[0][1]);
            if (idx < 0 || idx > MAX_HISTORY) {
                printf("Invalid history number");
                continue;
            } else if (hist[idx] == NULL) {
                printf("No command in history");
                continue;
            } else {
                strcpy(command_string, hist[idx]);
                continue;
            }

        }
        hist[hist_count++] = strdup(command_string); //adding command to history

        if (hist_count == MAX_HISTORY) {
            hist_count = 0;
        }

        //checking for history command only 
        if (strcmp(token[0], "history") == 0 && token[1] == NULL) {
            for (int i = 0; i < MAX_HISTORY; i++) {
                if (hist[i] != NULL) {
                    printf("%d: %s", i, hist[i]);
                }
            }

            continue;
        }
        // checking for history -p command
        else if (strcmp(token[0], "history") == 0 && strcmp(token[1], "-p") == 0) {
            for (int i = 0; i < MAX_HISTORY; i++) {
                if (hist[i] != NULL) {
                    printf("%d : %-7d: %s", i, pid_history[i], hist[i]);
                }
            }

            continue;

        }
        // checking for quit or exit command
        if ((strcmp(token[0], "exit") == 0 || strcmp(token[0], "quit") == 0) && token[0] != NULL) {
            free(head_ptr);
            free(command_string);
            for (int i = 0; i < MAX_HISTORY; i++) {
                free(hist[i]);
            }
            exit(0);
        }

        //cheching for cd command and its variations of cd and cd .. and cd . 
        if (strcmp(token[0], "cd") == 0) {
            if (token[1] != NULL) {
                int ret = chdir(token[1]);

                if (ret == -1) {
                    printf("%s: %s", token[1], "No such file or directory.\n");
                }
            } else {
                chdir(getenv("HOME"));

            }
            continue;
        }

        if (strcmp(token[0], "history") == 0) {
            for (int i = 0; i < MAX_HISTORY; i++) {
                if (hist[i] != NULL) {
                    printf("%d: %s", i, hist[i]);
                }
            }
            continue;
        }

        pid_t pid = fork(); // forking starts here
        // all the child processes will execute the following code 
        if (pid == 0) {
            int ret = execvp(token[0], token);
            hist[hist_count++] = strdup(command_string); //adding to history 4
            if (ret == -1) {
                printf("%s: %s", token[0], "Command not found.\n");
            }
            exit(1); // Exiting the child process if unknown command
        } else {
            int status;
            wait( & status);

            pid_history[hist_count] = pid;
        }

        // Cleanup allocated memory
        for (int i = 0; i < MAX_NUM_ARGUMENTS; i++) {
            if (token[i] != NULL) {
                free(token[i]);
            }
        }

        free(head_ptr);

    }

    free(command_string);

    return 0;
}