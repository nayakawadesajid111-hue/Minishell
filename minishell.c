/***********************************************************************
 * Name            : Sajid
 * Project         : Mini Shell
 * Date            : 14-05-2026
 *
 * Description:
 * --------------
 * This is the entry point of the Mini Shell project.
 * The program clears the terminal screen, initializes
 * the shell prompt, and starts accepting user commands
 * through the scan_input() function.
 *
 ***********************************************************************/

/* User-defined header file containing function prototypes,
   macros, structure definitions, and required libraries */
#include "main.h"

/* Default shell prompt displayed to the user */
char prompt_str[] = "minishell@Project-25033f_027";

/* Buffer to store user input command */
unsigned char input_str[35];

/*--------------------------------------------------------------------
 * Function : main
 * Input    : None
 * Output   : Returns 0 on successful execution
 *
 * Description:
 *  - Clears the terminal screen.
 *  - Starts the shell by calling scan_input().
 *  - scan_input() continuously accepts and processes
 *    user commands until the shell exits.
 *-------------------------------------------------------------------*/
int main()
{
    /* Clear terminal screen */
    system("clear");

    /* Start reading and processing user commands */
    scan_input(prompt_str, input_str);

    return 0;
}