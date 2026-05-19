/*
    -This program should let the user add tasks, list them, mark them as done, 
    and delete them. Each task should be saved in one file.
    -I can use a struct to hold the task name and if its done or not.
    -Add task function should create a struct instance with the name and 
    defaulted to not done. This function should also add it to the file
    -List function should list the name and if its done or not
    -Mark done function should just mark it done or make it un done
    -Delete function should delete the struct and free all allocated memory. 
    it should also delete the contents from the file
        -For each task i will have the name and the done/not done as 2 lines in 
        the file plus an empty line after those two to mark the end of that 
        task in the file
        -when deleting from the file i can search for the appropiate task name 
        and then delete the next 3 lines

    The program should start by introducing the program and explain the commands
*/
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_BUFF_SIZE 101 // 100 chars max, 1 extra for \0

// struct to store task data
typedef struct{
    char task_name[MAX_BUFF_SIZE];
    bool is_done;
} Task;

// dynamic (global) array of Task pointers 
// initially allocate memory for 10 Task pointers with an init function to be 
// called in main
Task** global_array;

// global meta data
int global_array_size = 0;
int global_array_cap = 0;
//int global_position = 0;

void fputs_error_check(int status);
void free_tasks(void);

// initialize the array of pointers
void init_Task_array(void){
    // init global array of pointers to hold 10 tasks to start
    global_array = malloc(10 * sizeof(Task));
    if(global_array == NULL){
        fprintf(stderr, "Error: Memory Allocation Failed!\n");
        exit(1);
    }

    // initial size is 10 
    global_array_size = 10;
}

// write a task to a file
void file_write_task(char* filename, char* task_name, bool is_true){

    // open file for appending as to not overwrite previous file data
    FILE* fptr = fopen(filename, "a");
    if(fptr == NULL){
        fprintf(stderr, "Error: Unable to open file...\n");
        free_tasks();
        free(global_array);
        exit(1);
    }
 
    int fputs_status;

    // write the task name, the boolean, and an empty line
    fputs_status = fputs(task_name, fptr);
    fputs_error_check(fputs_status);
    fputs_status = fputs("\n", fptr);
    fputs_error_check(fputs_status);
    if(is_true == 1){
        fputs_status = fputs("COMPLETE\n\n", fptr);
        fputs_error_check(fputs_status);
    } else {
        fputs_status = fputs("INCOMPLETE\n\n", fptr);
        fputs_error_check(fputs_status);
    }

    printf("File written to successfully!\n");
    fclose(fptr);
}

// function to add task to the global array and to the file
void add_task(char* task_name, bool is_done, char* filename){
    /*
        Add task function needs to allocate memory for the struct, 
        populate its members, add the struct (pointer) to the global array, 
        add the data to the file.
            -When adding the struct pointer, need to reallocate
    */

    // allocate memory for a task
    Task* task = malloc(sizeof(Task));
    if(task == NULL){
        fprintf(stderr, "Error: Memory Allocation Failed!\n");
        free_tasks();
        free(global_array);
        exit(1);
    } 
    

    // populate members
    strcpy(task->task_name, task_name);
    task->is_done = is_done;

    // add task ptr to dynamic global array (check for reallocation first)
    if(global_array_cap == global_array_size){ // realloc check
        int new_size = global_array_size * 2; // double prev size
        Task** temp = realloc(global_array, new_size * sizeof(Task*));
        if(temp == NULL){
            fprintf(stderr, "Error: Memory Allocation Failed!\n");
            free(task);
            free_tasks();
            free(global_array);
            exit(1);
        }
    
        // reassign the global array
        global_array = temp;

        global_array_size = new_size;
        global_array[global_array_cap] = task;
        ++global_array_cap;
    } else{
        global_array[global_array_cap] = task;
        ++global_array_cap;
    }

    // save to file
    file_write_task(filename, task_name, is_done);
}

int main(int argc, char* argv[]){
    
    // cmd line args check
    if(argc != 2){
        fprintf(stderr, "Usage: ./exe filename.txt\n");
        exit(1);  
    }

    printf("Hello! Welcome to your command line task manager program!\n\n");
    printf("Enter one of the following commands "  
          "(all tasks will be saved to a file until they are deleted)\n");
    printf("Also, use the mark as done command when a task is marked as done "
            "to mark it as undone (all tasks are marked undone by default)\n");
    printf("(The name of the task is limited to 100 characters)\n\n");
    printf("Commands: add task - \"a\", list all tasks - \"l\", list task by name" 
            " - \"n\", mark task as done/undone - \"m\", delete task by name - \"d\"," 
            " delete all tasks - \"x\", quit program- \"q\"\n\n");

    int c;
    int running = 1;
    char buffer[MAX_BUFF_SIZE];
    char* filename = argv[1];

    // inits 
    init_Task_array();

    while(running){
        printf("Enter a command: ");

        // get input from user
        c = getchar();

        // getchar() error checks
        if(c == EOF){
            fprintf(stderr, "Error: EOF\n");
            free_tasks();
            free(global_array);
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
                    free_tasks();
                    free(global_array);
                    exit(1);
                } else {
                    // clear the newline from the input buffer
                    buffer[strcspn(buffer, "\n")] = 0;
                }
                printf("Adding task...\n");
                add_task(buffer, false, filename);
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

void free_tasks(void){
    for(int i = 0; i < global_array_cap; ++i){
        free(global_array[i]);
    }
}

void fputs_error_check(int status){
    if(status == EOF){
        fprintf(stderr, "fputs error...\n");
        free_tasks();
        free(global_array);
        exit(1);
    }
}






