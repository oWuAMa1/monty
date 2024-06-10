#include "monty.h"

ssize_t custom_getline(char **lineptr, size_t *n, FILE *stream)
{
    char *buffer;
    int c;
    size_t i = 0;

    if (*lineptr == NULL)
    {
        *lineptr = malloc(BUFFER_SIZE);
        if (*lineptr == NULL)
            return -1;
        *n = BUFFER_SIZE;
    }

    buffer = *lineptr;

    while ((c = fgetc(stream)) != EOF)
    {
        if (i >= *n - 1)
        {
            char *new_buffer = realloc(buffer, *n + BUFFER_SIZE);
            if (new_buffer == NULL)
                return -1;
            buffer = new_buffer;
            *lineptr = buffer;
            *n += BUFFER_SIZE;
        }
        if (c == '\n')
            break;
        buffer[i++] = c;
    }

    if (i == 0 && c == EOF)
        return -1;

    buffer[i] = '\0';
    return i;
}

void usage_error(void)
{
    fprintf(stderr, "USAGE: monty file\n");
    exit(EXIT_FAILURE);
}

void file_error(char *filename)
{
    fprintf(stderr, "Error: Can't open file %s\n", filename);
    exit(EXIT_FAILURE);
}

void unknown_instruction_error(char *opcode, unsigned int line_number)
{
    fprintf(stderr, "L%u: unknown instruction %s\n", line_number, opcode);
    exit(EXIT_FAILURE);
}

void malloc_error(void)
{
    fprintf(stderr, "Error: malloc failed\n");
    exit(EXIT_FAILURE);
}

void free_stack(stack_t *stack)
{
    stack_t *temp;

    while (stack != NULL)
    {
        temp = stack;
        stack = stack->next;
        free(temp);
    }
}

int is_integer(char *str)
{
    if (str == NULL || *str == '\0')
        return 0;

    if (*str == '-' || *str == '+')
        str++;

    while (*str)
    {
        if (!isdigit(*str))
            return 0;
        str++;
    }
    return 1;
}

void push(stack_t **stack, unsigned int line_number)
{
    char *arg;
    int value;
    stack_t *new_node;

    arg = strtok(NULL, " \t\n");
    if (!arg || !is_integer(arg))
    {
        fprintf(stderr, "L%u: usage: push integer\n", line_number);
        exit(EXIT_FAILURE);
    }

    value = atoi(arg);
    new_node = malloc(sizeof(stack_t)); /* Moved declaration to beginning*/
    if (!new_node)
        malloc_error();

    new_node->n = value;
    new_node->prev = NULL;
    new_node->next = *stack;

    if (*stack)
        (*stack)->prev = new_node;

    *stack = new_node;
}

void pall(stack_t **stack, unsigned int line_number)
{
    stack_t *current;

    (void)line_number;

    current = *stack; /* Moved declaration to beginning*/

    while (current)
    {
        printf("%d\n", current->n);
        current = current->next;
    }
}

void execute_instruction(char *opcode, stack_t **stack, unsigned int line_number)
{
    instruction_t instructions[] = {
        {"push", push},
        {"pall", pall},
        {NULL, NULL}
    };

    int i;
    for (i = 0; instructions[i].opcode != NULL; i++)
    {
        if (strcmp(opcode, instructions[i].opcode) == 0)
        {
            instructions[i].f(stack, line_number);
            return;
        }
    }

    unknown_instruction_error(opcode, line_number);
}

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

    return 0;
}
