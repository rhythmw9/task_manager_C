/*
    This program should let the user add tasks, list them, mark them as done, and delete them. Each task should be saved in one file.
    I can use a struct to hold the task name and if its done or not.
    Add task function should create a struct instance with the name and defaulted to not done. This function should also add it to the file
    List function should list the name and if its done or not
    Mark done function should just mark it done or make it un done
    Delete function should delete the struct and free all allocated memory. it should also delete the contents from the file
        For each task i will have the name and the done/not done as 2 lines in the file plus an empty line after those two to mark the end of that task in the file
        when deleting from the file i can search for the appropiate task name and then delete the next 3 lines

    The program should start by introducing the program and explain the commands
*/
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_BUFF_SIZE 101 // 100 chars max, 1 extra for \0

typedef struct{
    char* task_name;
    bool is_done;
} Task;

// dynamic (global) array of Task pointers - initially allocate memory for 10 Task pointers with an init function to be called in main
Task** global_array;
int global_array_size = 0;
int global_array_cap = 0;
int global_position = 0;

void init_Task_array(void){
    global_array = malloc(10 * sizeof(Task*));
    if(global_array == NULL){
        fprintf(stderr, "Error: Memory Allocation Failed!\n");
        exit(1);
    }
    global_array_size = 10;
}

void add_task(char* task_name, bool is_done){
    /*
        Add task function needs to allocate memory for the struct, populate its members, add the struct (pointer) to the global array, add the data to the file.
            -When adding the struct pointer, need to reallocate
    */

    // allocate memory for a task
    Task* task = malloc(sizeof(Task));
    if(task == NULL){
        fprintf(stderr, "Error: Memory Allocation Failed!\n");
        exit(1);
    }

    // allocate memory for task name
    char* temp_task_name = malloc(sizeof(char) * sizeof(task_name));
    if(temp_task_name == NULL){
        fprintf(stderr, "Error: Memory Allocation Failed!\n");
        free(task);
        exit(1);
    }

    // populate members
    strcpy(task->task_name, temp_task_name);
    task->is_done = is_done;

    // add task ptr to dynamic global array (check for reallocation first)
    if(global_array_cap >= global_array_size){ // realloc check
        int new_size = global_array_size * 2;
        Task** temp = realloc(global_array, new_size * sizeof(Task*));
        if(temp == NULL){
            fprintf(stderr, "Error: Memory Allocation Failed!\n");
            exit(1);
            free(temp_task_name);
            free(task);
        }
        global_array_size = new_size;
        global_array[global_position] = task;
        ++global_array_cap;
        ++global_position;
    } else{
        global_array[global_position] = task;
        ++global_array_cap;
        ++global_position;
    }

    // still need to save to file
}

int main(void){
    printf("Hello! Welcome to your command line task manager program!\n\n");
    printf("Enter one of the following commands (all tasks will be saved to a file until they are deleted)\n");
    printf("Also, use the mark as done command when a task is marked as done to mark it as undone (all tasks are marked undone by default)\n");
    printf("(The name of the task is limited to 100 characters)\n\n");
    printf("Commands: add task - \"a\", list all tasks - \"l\", list task by name - \"n\", mark task as done/undone - \"m\", delete task by name - \"d\", delete all tasks - \"x\", quit program- \"q\"\n\n");

    int c;
    int running = 1;
    char buffer[MAX_BUFF_SIZE];

    // init globals
    init_Task_array();

    while(running){
        printf("Enter a command: ");

        // get input from user
        c = getchar();

        // getchar() error checks
        if(c == EOF){
            fprintf(stderr, "Error: EOF\n");
            exit(1);
        } else if(c == '\n'){
            fprintf(stderr, "Error: Empty Input\n");
            // no exit here, re run loop
        }

        while(getchar() != '\n'); // clear the input buffer

        switch(c){
            case 'a': // add task
                printf("Enter task name: ");
                if(fgets(buffer, MAX_BUFF_SIZE, stdin) == NULL){
                    fprintf(stderr, "Error: Failed to Read the Input Stream\n");
                    exit(1);
                } else {
                    buffer[strspn(buffer, "\n")] = '\0'; // clear the newline from the input buffer
                }
                printf("Adding task...\n");
                add_task(buffer, true);
                printf("Task added...\n");
                break;
            case 'l': // list all tasks
                printf("Listing all tasks\n");
                break;
            case 'n': // list task by name
                printf("Listing task by name\n");
                break;
            case 'm': // mark task as done/undone
                printf("Marking task\n");
                break;
            case 'd': // delete task by name
                printf("Deleting task by name\n");
                break;
            case 'x': // delete all tasks
                printf("Deleting all tasks\n");
                break;
            case 'q': // quit
                printf("Quitting program...\n");
                running = 0;
                break;
            default:
                fprintf(stderr, "Error: Invalid Command\n");
        }
    }
    return EXIT_SUCCESS;
}
