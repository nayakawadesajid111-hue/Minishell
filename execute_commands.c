/***********************************************************************
 * File Name   : command.c
 * Project     : Mini Shell
 *
 * Description :
 * This file contains functions responsible for:
 *  - Loading external commands from a file
 *  - Identifying command types (built-in/external)
 *  - Executing built-in shell commands
 *  - Executing external commands
 *  - Handling piped commands
 *
 ***********************************************************************/

#include <stdio.h>
#include <string.h>
#include <wait.h>
#include <unistd.h>
#include "main.h"

/*---------------------------------------------------------------------
 * Array containing supported built-in shell commands.
 *--------------------------------------------------------------------*/
char *builtin_cmds[] =
	{
		"echo", "printf", "read", "cd", "pwd", "pushd", "popd", "dirs",
		"let", "eval", "set", "unset", "export", "declare", "typeset",
		"readonly", "getopts", "source", "exit", "exec", "shopt",
		"caller", "true", "type", "hash", "bind", "help", "clear",
		"jobs", "fg", "bg", NULL};

/* External variables shared across modules */
extern char *external_cmds[156];
extern int status;
extern list *head;

extern volatile int count;
extern char process_status[50];

/*****************************************************************************
 * Function    : extract_external_commands
 *
 * Description :
 *      Reads all external commands from "external_cmd.txt" and stores
 *      them dynamically in the external_cmds array.
 *
 * Input       :
 *      external_commands - Array to store command names
 *
 * Return      :
 *      None
 *****************************************************************************/
void extract_external_commands(char **external_commands)
{
	int i = 0, fd;

	fd = open("external_cmd.txt", O_RDONLY, 0777);

	if (fd == -1)
	{
		perror("open");
	}

	for (i = 0; i < 152; i++)
	{
		char ch;
		char buffer[35];
		int j = 0;

		while (read(fd, &ch, 1) > 0)
		{
			if (ch == '\n')
				break;

			buffer[j++] = ch;
		}

		buffer[j] = '\0';

		int len = strlen(buffer);

		external_cmds[i] = malloc(len + 1);
		strcpy(external_cmds[i], buffer);
	}

	external_cmds[i] = NULL;

	close(fd);
}

/*****************************************************************************
 * Function    : get_command
 *
 * Description :
 *      Extracts the command name from the user input.
 *
 * Example:
 *      Input : "ls -l"
 *      Output: "ls"
 *
 * Input       :
 *      input_string - User entered command line
 *
 * Return      :
 *      Pointer to command string
 *****************************************************************************/
char *get_command(char *input_string)
{
	static char buffer[25];
	int i = 0;

	while (input_string[i] != ' ' && input_string[i] != '\0')
	{
		buffer[i] = input_string[i];
		i++;
	}

	buffer[i] = '\0';

	return buffer;
}

/*****************************************************************************
 * Function    : check_command_type
 *
 * Description :
 *      Determines whether the command is:
 *          BUILTIN
 *          EXTERNAL
 *          NO_COMMAND
 *
 * Input       :
 *      command - Command string
 *
 * Return      :
 *      BUILTIN / EXTERNAL / NO_COMMAND
 *****************************************************************************/
int check_command_type(char *command)
{
	int i = 0;

	/* Search built-in command list */
	while (builtin_cmds[i] != NULL)
	{
		if (strcmp(command, builtin_cmds[i]) == 0)
		{
			return BUILTIN;
		}
		i++;
	}

	i = 0;

	/* Search external command list */
	while (external_cmds[i] != NULL)
	{
		if (strcmp(command, external_cmds[i]) == 0)
		{
			return EXTERNAL;
		}
		i++;
	}

	return NO_COMMAND;
}

/*****************************************************************************
 * Function    : execute_internal_commands
 *
 * Description :
 *      Executes built-in shell commands such as:
 *          exit
 *          cd
 *          pwd
 *          clear
 *          echo
 *          jobs
 *          fg
 *          bg
 *
 * Input       :
 *      input_string - Complete command entered by user
 *
 * Return      :
 *      None
 *****************************************************************************/
void execute_internal_commands(char *input_string)
{
	/* Exit shell */
	if (strcmp(input_string, "exit") == 0)
	{
		exit(0);
	}

	/* Change directory */
	else if (strncmp(input_string, "cd ", 3) == 0)
	{
		chdir(input_string + 3);
	}

	/* Print current working directory */
	else if (strcmp(input_string, "pwd") == 0)
	{
		char path[102];

		getcwd(path, sizeof(path));
		printf("%s\n", path);
	}

	/* Clear terminal */
	else if (strcmp(input_string, "clear") == 0)
	{
		printf("\033c");
	}

	/* Handle echo command */
	else if (strncmp(input_string, "echo", 4) == 0)
	{
		if (strcmp(input_string, "echo") == 0)
		{
			printf("\n");
		}

		/* Display current shell PID */
		else if ((strlen(input_string) > 4) &&
				 strcmp(input_string + 5, "$$") == 0)
		{
			printf("%d\n", getpid());
		}

		/* Display shell path */
		else if ((strlen(input_string) > 4) &&
				 strcmp(input_string + 5, "$SHELL") == 0)
		{
			printf("%s\n", getenv("SHELL"));
		}

		/* Display exit status of previous command */
		else if ((strlen(input_string) > 4) &&
				 strcmp(input_string + 5, "$?") == 0)
		{
			if (WIFEXITED(status))
			{
				printf("%d\n", WEXITSTATUS(status));
			}
		}
	}

	/* Display stopped jobs */
	else if (strcmp(input_string, "jobs") == 0)
	{
		print_list(head);
	}

	/* Bring latest background job to foreground */
	else if (strcmp(input_string, "fg") == 0)
	{

		if (head != NULL)
		{

			printf("%s\n", head->cmd);

			if (kill(head->pid, SIGCONT) == -1)
			{
				perror("Kill failure : ");
				return;
			}

			int st;

			waitpid(head->pid, &st, WUNTRACED);

			if (WIFEXITED(st) || WIFSIGNALED(st))
			{
				delete_first(&head);
				if (count-- < 1)
				{
					count = 1;
				}
			}
			else if (WIFSTOPPED(st))
			{
				strcpy(head->state, "Stopped");
			}
		}

		else
		{
			printf("-bash: fg: current: no such job\n");
		}
	}

	/* Resume latest stopped job in background */
	else if (strcmp(input_string, "bg") == 0)
	{

		if (head != NULL)
		{

			printf("[%d]   %s   &\n", head->count, head->cmd);

			if (kill(head->pid, SIGCONT) == -1)
			{
				perror("kill failure : ");
				return;
			}
			strcpy(head->state, "Running");

			if (signal(SIGCHLD, handler) == SIG_ERR)
			{
				perror("Signal : ");
				return;
			}
		}
		else
		{
			printf("-bash: bg: current: no such job\n");
		}
	}
}

/*****************************************************************************
 * Function    : execute_external_commands
 *
 * Description :
 *      Executes external Linux commands.
 *      Supports:
 *          - Single command execution
 *          - Pipeline execution
 *
 * Input       :
 *      input_string - User entered command
 *
 * Return      :
 *      None
 *****************************************************************************/
void execute_external_commands(char *input_string)
{
	/* No pipe present */
	if (strchr(input_string, '|') == NULL)
	{
		char *args[64];
		int i = 0;

		/* Tokenize command arguments */
		char *token = strtok(input_string, " ");

		while (token != NULL && i < 63)
		{
			args[i++] = token;
			token = strtok(NULL, " ");
		}

		args[i] = NULL;

		/* Execute command */
		execvp(args[0], args);

		perror("execvp failed");
		exit(EXIT_FAILURE);
	}

	/* Pipe command */
	else
	{
		char *args[64];
		int i = 0;

		char *token = strtok(input_string, " ");

		while (token != NULL && i < 63)
		{
			args[i++] = token;
			token = strtok(NULL, " ");
		}

		args[i] = NULL;

		execute_external_commands_with_pipe(args);
	}
}

/*****************************************************************************
 * Function    : execute_external_commands_with_pipe
 *
 * Description :
 *      Executes multiple commands connected through pipes.
 *
 * Example:
 *      ls -l | grep txt | wc -l
 *
 * Input       :
 *      args - Tokenized command arguments
 *
 * Return      :
 *      None
 *****************************************************************************/
void execute_external_commands_with_pipe(char **args)
{
	int cmd_start[64];
	int cmd_count = 0;

	/* First command starts at index 0 */
	cmd_start[cmd_count++] = 0;

	/* Identify command boundaries */
	for (int i = 1; args[i] != NULL; i++)
	{
		if (strcmp(args[i], "|") == 0)
		{
			args[i] = NULL;
			cmd_start[cmd_count++] = i + 1;
		}
	}

	int n_pipes = cmd_count - 1;

	/* Create required pipes */
	int fd[n_pipes][2];

	for (int i = 0; i < n_pipes; i++)
	{
		if (pipe(fd[i]) == -1)
		{
			perror("pipe");
			return;
		}
	}

	/* Create child process for each command */
	for (int i = 0; i < cmd_count; i++)
	{
		int pid = fork();

		if (pid == -1)
		{
			perror("fork");
			return;
		}

		if (pid == 0)
		{
			/* Connect input from previous pipe */
			if (i > 0)
				dup2(fd[i - 1][0], STDIN_FILENO);

			/* Connect output to next pipe */
			if (i < n_pipes)
				dup2(fd[i][1], STDOUT_FILENO);

			/* Close unused pipe descriptors */
			for (int k = 0; k < n_pipes; k++)
			{
				close(fd[k][0]);
				close(fd[k][1]);
			}

			execvp(args[cmd_start[i]],
				   args + cmd_start[i]);

			perror("execvp");
			exit(EXIT_FAILURE);
		}
	}

	/* Parent closes all pipe ends */
	for (int i = 0; i < n_pipes; i++)
	{
		close(fd[i][0]);
		close(fd[i][1]);
	}

	/* Wait for all child processes */
	for (int i = 0; i < cmd_count; i++)
	{
		wait(NULL);
	}
}