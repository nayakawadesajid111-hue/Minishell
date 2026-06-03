#ifndef MAIN_H
#define MAIN_H

/* Standard Library Headers */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio_ext.h>

/* System Headers */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <error.h>
#include <errno.h>

/* Command Types */
#define BUILTIN     1   // Built-in shell command
#define EXTERNAL    2   // External/Linux command
#define NO_COMMAND  3   // Invalid or empty command

/* Return Status Codes */
#define SUCCESS             0
#define FAILURE            -1
#define DATA_NOT_FOUND     -2
#define POSITION_NOT_FOUND -3
#define LIST_EMPTY         -4


#define SIGINT_FL        0xAA
#define SIGTSTP_FL       0xAB  
#define SIGTSTP_STOPPED  0xA1

typedef int data_t;

/* Terminal Color Macros */
#define COLOR_RED      "\x1b[31m"
#define COLOR_GREEN    "\x1b[32m"
#define COLOR_YELLOW   "\x1b[33m"
#define COLOR_BLUE     "\x1b[34m"
#define COLOR_MAGENTA  "\x1b[35m"
#define COLOR_CYAN     "\x1b[36m"
#define COLOR_WHITE    "\x1b[37m"
#define COLOR_RESET    "\x1b[0m"
#define COLOR_BOLD     "\x1b[1m"

/* Background Colors */
#define BG_RED         "\x1b[41m"
#define BG_GREEN       "\x1b[42m"
#define BG_YELLOW      "\x1b[43m"
#define BG_BLUE        "\x1b[44m"

/*--------------------------------------------------------------------
 * Structure: node
 * Purpose   : Stores information about background processes.
 *             Used in linked list implementation for process tracking.
 *-------------------------------------------------------------------*/
typedef struct node
{
    data_t pid;          // Process ID
    char cmd[50];        // Command name
    char state[50];     // Process status (Running/Stopped/Completed)
    data_t count;
    struct node *link;   // Pointer to next node
} list;

/*--------------------------------------------------------------------
 * Linked List Functions
 *-------------------------------------------------------------------*/

/*
 * Insert a new process node at the beginning of the list.
 * Returns SUCCESS or FAILURE.
 */
int insert_at_first(list **head, data_t data, char *cmd, char *status,data_t count);

/*
 * Delete the first node from the process list.
 * Returns SUCCESS or FAILURE.
 */
int delete_first(list **head);

/*
 * Display all nodes in the process list.
 */
void print_list(list *head);

void reverse(list **head);

/*--------------------------------------------------------------------
 * Signal Handling Functions
 *-------------------------------------------------------------------*/

/*
 * Generic signal handler.
 */
void handler(int signum);

/*
 * Display status of a given process using PID.
 */
void print_status(int target_pid);

/*
 * Signal handler for shell-specific signals.
 */
void signal_handler(int sig_num);

/*--------------------------------------------------------------------
 * Input Processing Functions
 *-------------------------------------------------------------------*/

/*
 * Reads user input and updates shell prompt.
 */
void scan_input(char *prompt, char *input_string);

/*
 * Extracts the command from the input string.
 */
char *get_command(char *input_string);

/*
 * Modifies prompt/input string as required.
 */
void copy_change(char *prompt, char *input_string);

/*--------------------------------------------------------------------
 * Command Execution Functions
 *-------------------------------------------------------------------*/

/*
 * Determines whether the command is built-in,
 * external, or invalid.
 */
int check_command_type(char *command);

/*
 * Implements shell echo functionality.
 */
void echo(char *input_string, int status);

/*
 * Executes built-in shell commands.
 */
void execute_internal_commands(char *input_string);

/*
 * Loads external commands from a predefined source.
 */
void extract_external_commands(char **external_commands);

/*
 * Executes external commands using fork() and exec().
 */
void execute_external_commands(char *input_string);

/*
 * Executes commands connected using pipes.
 * Example: ls | grep txt | wc
 */
void execute_external_commands_with_pipe(char **args);


void print_status(int target_pid);



#endif