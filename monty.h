#ifndef MONTY_H
#define MONTY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#define BUFFER_SIZE 1024

/**
 * struct stack_s - doubly linked list representation of a stack (or queue)
 * @n: integer
 * @prev: points to the previous element of the stack (or queue)
 * @next: points to the next element of the stack (or queue)
 *
 * Description: doubly linked list node structure
 * for stack, queues, LIFO, FIFO
 */
typedef struct stack_s
{
    int n;
    struct stack_s *prev;
    struct stack_s *next;
} stack_t;

/**
 * struct instruction_s - opcode and its function
 * @opcode: the opcode
 * @f: function to handle the opcode
 *
 * Description: opcode and its function
 * for stack, queues, LIFO, FIFO
 */
typedef struct instruction_s
{
    char *opcode;
    void (*f)(stack_t **stack, unsigned int line_number);
} instruction_t;

#define BUFFER_SIZE 1024

/**
 * custom_getline - Reads an entire line from a stream
 * @lineptr: Pointer to the buffer where the read line will be stored
 * @n: Pointer to the size of the buffer
 * @stream: The input stream to read from
 *
 * Return: The number of characters read, including the newline character
 */
ssize_t custom_getline(char **lineptr, size_t *n, FILE *stream)
{
                char *buffer;
                int c;
                size_t i = 0;

                if (*lineptr == NULL)
                {
        *lineptr = malloc(BUFFER_SIZE);
        if (*lineptr == NULL)
                        return (-1);
        *n = BUFFER_SIZE;
                }

                buffer = *lineptr;

                while ((c = fgetc(stream)) != EOF)
                {
        if (i >= *n - 1)
        {
                        char *new_buffer = realloc(buffer, *n + BUFFER_SIZE);
                        if (new_buffer == NULL)
                return (-1);
                        buffer = new_buffer;
                        *lineptr = buffer;
                        *n += BUFFER_SIZE;
        }
        if (c == '\n')
                        break;
        buffer[i++] = c;
                }

                if (i == 0 && c == EOF)
        return (-1);

                buffer[i] = '\0';
                return (i);
}

/**
 * usage_error - Prints usage error and exits
 */
void usage_error(void)
{
                fprintf(stderr, "USAGE: monty file\n");
                exit(EXIT_FAILURE);
}

/**
 * file_error - Prints file error and exits
 * @filename: The name of the file that caused the error
 */
void file_error(char *filename)
{
                fprintf(stderr, "Error: Can't open file %s\n", filename);
                exit(EXIT_FAILURE);
}

/**
 * unknown_instruction_error - Prints unknown instruction error and exits
 * @opcode: The opcode that caused the error
 * @line_number: The line number where the error occurred
 */
void unknown_instruction_error(char *opcode, unsigned int line_number)
{
                fprintf(stderr, "L%u: unknown instruction %s\n", line_number, opcode);
                exit(EXIT_FAILURE);
}

/**
 * malloc_error - Prints malloc error and exits
 */
void malloc_error(void)
{
                fprintf(stderr, "Error: malloc failed\n");
                exit(EXIT_FAILURE);
}

/**
 * free_stack - Frees a stack
 * @stack: The stack to be freed
 */
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

/**
 * is_integer - Checks if a string represents an integer
 * @str: The string to be checked
 *
 * Return: 1 if the string is an integer, 0 otherwise
 */
int is_integer(char *str)
{
                if (str == NULL || *str == '\0')
        return (0);

                if (*str == '-' || *str == '+')
        str++;

                while (*str)
                {
        if (!isdigit(*str))
                        return (0);
        str++;
                }
                return (1);
}

/**
 * push - Pushes an element to the stack
 * @stack: The stack
 * @line_number: The line number
 */
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
                new_node = malloc(sizeof(stack_t)); /* Moved declaration to beginning */
                if (!new_node)
        malloc_error();

                new_node->n = value;
                new_node->prev = NULL;
                new_node->next = *stack;

                if (*stack)
        (*stack)->prev = new_node;

                *stack = new_node;
}

/**
 * pall - Prints all values on the stack
 * @stack: The stack
 * @line_number: The line number
 */
void pall(stack_t **stack, unsigned int line_number)
{
                stack_t *current;

                (void)line_number;

                current = *stack; /* Moved declaration to beginning */

                while (current)
                {
        printf("%d\n", current->n);
        current = current->next;
                }
}

/**
 * pint - Prints the value at the top of the stack.
 * @stack: Double pointer to the top of the stack.
 * @line_number: Line number where the instruction appears.
 *
 * Description: Prints the value at the top of the stack, followed by a new line.
 * If the stack is empty, prints an error message and exits with EXIT_FAILURE.
 */
void pint(stack_t **stack, unsigned int line_number)
{
    if (*stack == NULL)
    {
        fprintf(stderr, "L%u: can't pint, stack empty\n", line_number);
        exit(EXIT_FAILURE);
    }

    printf("%d\n", (*stack)->n);
}
/**
 * pop - Removes the top element of the stack.
 * @stack: Double pointer to the top of the stack.
 * @line_number: Line number where the instruction appears.
 *
 * Description: Removes the top element of the stack. If the stack is empty,
 * prints an error message and exits with EXIT_FAILURE.
 */
void pop(stack_t **stack, unsigned int line_number)
{
    stack_t *temp;

    if (*stack == NULL)
    {
        fprintf(stderr, "L%u: can't pop an empty stack\n", line_number);
        exit(EXIT_FAILURE);
    }

    temp = *stack;
    *stack = (*stack)->next;
    if (*stack != NULL)
        (*stack)->prev = NULL;
    free(temp);
}
/**
 * swap - Swaps the top two elements of the stack.
 * @stack: Double pointer to the top of the stack.
 * @line_number: Line number where the instruction appears.
 *
 * Description: Swaps the top two elements of the stack. If the stack contains
 * less than two elements, prints an error message and exits with EXIT_FAILURE.
 */
void swap(stack_t **stack, unsigned int line_number)
{
    stack_t *first, *second;

    if (*stack == NULL || (*stack)->next == NULL)
    {
        fprintf(stderr, "L%u: can't swap, stack too short\n", line_number);
        exit(EXIT_FAILURE);
    }

    first = *stack;
    second = (*stack)->next;

    first->next = second->next;
    if (second->next != NULL)
        second->next->prev = first;

    second->prev = first->prev;
    second->next = first;
    first->prev = second;
    *stack = second;
}
/**
 * add - Adds the top two elements of the stack.
 * @stack: Double pointer to the top of the stack.
 * @line_number: Line number where the instruction appears.
 *
 * Description: Adds the top two elements of the stack. If the stack contains
 * less than two elements, prints an error message and exits with EXIT_FAILURE.
 * The result is stored in the second top element of the stack, and the top
 * element is removed, so that at the end the stack is one element shorter.
 */
void add(stack_t **stack, unsigned int line_number)
{
    stack_t *temp;

    if (*stack == NULL || (*stack)->next == NULL)
    {
        fprintf(stderr, "L%u: can't add, stack too short\n", line_number);
        exit(EXIT_FAILURE);
    }

    temp = *stack;
    (*stack)->next->n += (*stack)->n;
    *stack = (*stack)->next;
    (*stack)->prev = NULL;
    free(temp);
}
/**
 * nop - Doesn’t do anything.
 * @stack: Double pointer to the top of the stack.
 * @line_number: Line number where the instruction appears.
 *
 * Description: This function does nothing.
 */
void nop(stack_t **stack, unsigned int line_number)
{
    (void)stack;
    (void)line_number;
}

/**
 * execute_instruction - Executes an instruction
 * @opcode: The opcode
 * @stack: The stack
 * @line_number: The line number
 */
void execute_instruction(char *opcode, stack_t **stack, unsigned int line_number)
{
    instruction_t instructions[] = {
        {"push", push},
        {"pall", pall},
        {"pint", pint},
        {"pop", pop},
        {"swap", swap},
        {"add", add},
        {"nop", nop},
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
#endif /* MONTY_H */
