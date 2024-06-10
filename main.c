#include "monty.h"

/**
 * main - Entry point of the program
 * @argc: Argument count
 * @argv: Argument vector
 *
 * Return: 0 on success
 */
int main(int argc, char *argv[])
{
		FILE *file;
		char *line = NULL;
		size_t len = 0;
		ssize_t read;
		unsigned int line_number = 0;
		stack_t *stack = NULL;

		if (argc != 2)
			usage_error();

		file = fopen(argv[1], "r");
		if (!file)
			file_error(argv[1]);

		while ((read = custom_getline(&line, &len, file)) != -1)
		{
			char *opcode;

			line_number++;
			opcode = strtok(line, " \t\n");
			if (!opcode || opcode[0] == '#')
				continue;

			execute_instruction(opcode, &stack, line_number);
		}

		free(line);
		fclose(file);
		free_stack(stack);

		return (0);
}
