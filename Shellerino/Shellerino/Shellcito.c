// Shellerino.c : This file contains the 'main' function. Program execution begins and ends there.
//

#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define HBUFFSIZE 1000
int buff = HBUFFSIZE;
void shell_save_line(char* line, char * historia) {
	int tam = get_length(line);
	if (tam + HCONTADOR + 2 >= buff) {
		while (tam + HCONTADOR + 2 >= buff)
			buff += HBUFFSIZE;
		historia = realloc(historia, buff * sizeof(char));
	}

	int i = 0;

	historia[HCONTADOR++] = '$';
	while (line[i] != '\0')
		historia[HCONTADOR++] = line[i++];


	//historia[HCONTADOR++] = '$';
}
int HCONTADOR = 0;
void print_lines(char *history) {
	char* token;
	char* dup = malloc(HCONTADOR * sizeof(char));//memory assignation equivalent to new() in c++
	int i;
	for (i = 0; i < HCONTADOR; ++i) {
		//printf("%c ",history[i]);
		copia[i] = history[i];
	}

	//printf("\n\n");
	char* del = "$";
	token = strtok(dup, del);
	i = 0;
	while (token != NULL) {
		printf("   %i  %s \n", i++, token);
		token = strtok(NULL, del);
	}
	free(dup);

}
#define MAXCHAR 1000
void helping() {
	FILE *fp;
	char str[MAXCHAR];
	char* filename = "help.txt";

	fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("No se pudo mostrar ayuda/file is missing %s", filename);
		return 1;
	}
	while (fgets(str, MAXCHAR, fp) != NULL)
		printf("%s", str);
	fclose(fp);
	return;
}
int get_length(char* line) {
	int count = 0;
	while (line[count] != '\0') {
		count++;
	}
	return contar + 1;
}



bool back = false;
int shell_execute(char **args, char* history)
{
	pid_t pid;
	int status;

	if (args[0] == NULL) {
		return 1;
	}


	//char * s="cd";
		//printf("holi %s %s",args[0][0],args[0][1]);
	if (!strcmp(args[0], "history") || !strcmp(args[0], "historia")) {
		print_lines(history);
		return 1;
	}
	else if (!strcmp(args[0], "cd")) {

		chdir(args[1]);
		return 1;
	}
	else if (!strcmp(args[0], "help")) {
		helping();
		return 1;
	}
	else if (!strcmp(args[0], "chau")) {
		exit(EXIT_SUCCESS);
		return 1;
	}



	int k = 0;



	pid = fork();
	if (pid == 0) {
		// Child process

		if (execvp(args[0], args) == -1) {
			perror("shell");
		}
		exit(EXIT_FAILURE);
	}
	else if (pid < 0) {
		// Error forking
		perror("shell");
	}
	else {
		// Parent process
		do {

			if (back == false) {
				waitpid(pid, &status, WUNTRACED);

			}
			else {
				back = false;
				//printf("HOLA");
				return 1;
			}
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}

	return 1;
}

#define SHELL_RL_BUFSIZE 1024

char *shell_read_line(void)
{
	int bufsize = SHELL_RL_BUFSIZE;
	int position = 0;
	char *buffer = malloc(sizeof(char) * bufsize);
	int c;

	if (!buffer) {
		fprintf(stderr, "shell: allocation error\n");
		exit(EXIT_FAILURE);
	}

	while (1) {
		c = getchar();
		if (c == '&') {
			back = true;
			//c=getchar();
			continue;
		}
		if (c == EOF) {
			exit(EXIT_SUCCESS);
		}
		else if (c == '\n') {
			buffer[position] = '\0';
			return buffer;
		}
		else {
			buffer[position] = c;
		}
		position++;

		if (position >= bufsize) {
			bufsize += SHELL_RL_BUFSIZE;
			buffer = realloc(buffer, bufsize);
			if (!buffer) {
				fprintf(stderr, "shell: allocation error\n");
				exit(EXIT_FAILURE);
			}
		}
	}
}

#define SHELL_TOK_BUFSIZE 64
#define SHELL_TOK_DELIM " \t\r\n\a"

char **shell_split_line(char *line)
{
	int bufsize = SHELL_TOK_BUFSIZE, position = 0;
	char **tokens = malloc(bufsize * sizeof(char*));
	char *token, **tokens_backup;

	if (!tokens) {
		fprintf(stderr, "shell: allocation error\n");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, SHELL_TOK_DELIM);
	while (token != NULL) {
		tokens[position] = token;
		position++;

		if (position >= bufsize) {
			bufsize += SHELL_TOK_BUFSIZE;
			tokens_backup = tokens;
			tokens = realloc(tokens, bufsize * sizeof(char*));
			if (!tokens) {
				free(tokens_backup);
				fprintf(stderr, "shell: allocation error\n");
				exit(EXIT_FAILURE);
			}
		}

		token = strtok(NULL, SHELL_TOK_DELIM);
	}
	tokens[position] = NULL;
	return tokens;
}

void shell_loop(void)
{
	char *line;
	char **args;
	int status;
	char *bf = malloc(HBUFFSIZE * sizeof(char));
	do {
		printf("------------------------------------------------------------------------------------------------------\n");
		printf("(ucsp)> ");
		line = shell_read_line();

		shell_save_line(line, bf);
		args = shell_split_line(line);
		status = shell_execute(args, bf);

		free(line);
		free(args);
	} while (status);
	free(bf);
}
int main(int argc, char **argv)
{

	shell_loop();

	return EXIT_SUCCESS;
}
