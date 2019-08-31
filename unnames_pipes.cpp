#include <stdio.h>
#include <unistd.h>
#include <string.h>
#define READ 0
#define WRITE 1
#define COMMAND_COUNT 10

char commands[COMMAND_COUNT][COMMAND_COUNT];
char bin_commands[COMMAND_COUNT][COMMAND_COUNT];

int main (int argc, char** argv) {
	int iCommand = 0;
	int i = 1;
	bool isArgument = false;
	for (; i < argc; ++i) {
		printf("%s\n", argv[i]);
		if (argv[i][0] == '|') {
			++iCommand;
			isArgument = false;
			continue;
		}
		if (isArgument)
			strncat(commands[iCommand], " ", 1);
		else
			strncat(bin_commands[iCommand], argv[i], strlen(argv[i]));
		
		strncat(commands[iCommand], argv[i], strlen(argv[i]));
		isArgument = true;
		
	}
	const int realCommandCount = iCommand + 1;
	
	int fd [2];
	pipe (fd); /* Create an unnamed pipe */
	for(int k = 0; k < 2; ++k) {
		if (fork () != 0) /* Parent, writer */
		{
			close (fd[READ]); /* Close unused end */
			dup2 (fd[WRITE], 1); /* Duplicate used end to stdout */
			close (fd[WRITE]); /* Close original used end */
			execlp (bin_commands[k], commands[k], NULL); /* Execute writer program */
			perror ("connect"); /* Should never execute */
		}
		else /* Child, reader */
		{
			close (fd[WRITE]); /* Close unused end */
			dup2 (fd[READ], 0); /* Duplicate used end to stdin */
			close (fd[READ]); /* Close original used end */
			execlp (bin_commands[k+1], commands[k+1], NULL); /* Execute reader program */
			perror ("connect"); /* Should never execute */
		}
	}
	return 0;
}


