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
 * sub - Subtracts the top element of the stack from the second top element.
 * @stack: Double pointer to the top of the stack.
 * @line_number: Line number where the instruction appears.
 *
 * Description: The result is stored in the second top element of the stack,
 * and the top element is removed. If the stack contains less than two elements,
 * an error is printed, and the program exits.
 */
void sub(stack_t **stack, unsigned int line_number)
{
    stack_t *temp;

    if (*stack == NULL || (*stack)->next == NULL)
    {
        fprintf(stderr, "L%u: can't sub, stack too short\n", line_number);
        exit(EXIT_FAILURE);
    }

    temp = *stack;
    (*stack)->next->n -= (*stack)->n;
    *stack = (*stack)->next;
    (*stack)->prev = NULL;
    free(temp);
}
/**
 * div_op - Divides the second top element of the stack by the top element.
 * @stack: Double pointer to the top of the stack.
 * @line_number: Line number where the instruction appears.
 *
 * Description: The result is stored in the second top element of the stack,
 * and the top element is removed. If the stack contains less than two elements,
 * or if the top element is 0, appropriate error messages are printed, and
 * the program exits with EXIT_FAILURE.
 */
void div_op(stack_t **stack, unsigned int line_number)
{
    stack_t *temp;

    if (*stack == NULL || (*stack)->next == NULL)
    {
        fprintf(stderr, "L%u: can't div, stack too short\n", line_number);
        exit(EXIT_FAILURE);
    }

    if ((*stack)->n == 0)
    {
        fprintf(stderr, "L%u: division by zero\n", line_number);
        exit(EXIT_FAILURE);
    }

    temp = *stack;
    (*stack)->next->n /= (*stack)->n;
    *stack = (*stack)->next;
    (*stack)->prev = NULL;
    free(temp);
}
/**
 * mul - Multiplies the second top element of the stack with the top element.
 * @stack: Double pointer to the top of the stack.
 * @line_number: Line number where the instruction appears.
 *
 * Description: If the stack contains less than two elements, prints an error
 * message and exits with status EXIT_FAILURE. The result is stored in the
 * second top element of the stack, and the top element is removed. At the end:
 * - The top element of the stack contains the result.
 * - The stack is one element shorter.
 */
void mul(stack_t **stack, unsigned int line_number)
{
    stack_t *temp;

    if (*stack == NULL || (*stack)->next == NULL)
    {
        fprintf(stderr, "L%u: can't mul, stack too short\n", line_number);
        exit(EXIT_FAILURE);
    }

    temp = *stack;
    (*stack)->next->n *= (*stack)->n;
    *stack = (*stack)->next;
    (*stack)->prev = NULL;
    free(temp);
}
/**
 * mod - Computes the remainder of the division of the second top element
 *       of the stack by the top element of the stack.
 * @stack: Double pointer to the top of the stack.
 * @line_number: Line number where the instruction appears.
 *
 * Description: If the stack contains less than two elements, prints an error
 * message and exits with status EXIT_FAILURE. If the top element of the
 * stack is 0, prints an error message and exits with status EXIT_FAILURE.
 * The result is stored in the second top element of the stack, and the top
 * element is removed. At the end:
 * - The top element of the stack contains the result.
 * - The stack is one element shorter.
 */
void mod(stack_t **stack, unsigned int line_number)
{
    stack_t *temp;

    if (*stack == NULL || (*stack)->next == NULL)
    {
        fprintf(stderr, "L%u: can't mod, stack too short\n", line_number);
        exit(EXIT_FAILURE);
    }

    if ((*stack)->n == 0)
    {
        fprintf(stderr, "L%u: division by zero\n", line_number);
        exit(EXIT_FAILURE);
    }

    temp = *stack;
    (*stack)->next->n %= (*stack)->n;
    *stack = (*stack)->next;
    (*stack)->prev = NULL;
    free(temp);
}
/**
 * pchar - Prints the character at the top of the stack.
 * @stack: Double pointer to the top of the stack.
 * @line_number: Line number where the instruction appears.
 *
 * Description: If the stack is empty, prints an error message and exits with
 * status EXIT_FAILURE. If the integer stored at the top of the stack is out
 * of the ASCII range, prints an error message and exits with status
 * EXIT_FAILURE. Otherwise, prints the character represented by the integer
 * value at the top of the stack followed by a newline character. Updates the
 * stack accordingly.
 */
void pchar(stack_t **stack, unsigned int line_number)
{
    if (*stack == NULL)
    {
        fprintf(stderr, "L%u: can't pchar, stack empty\n", line_number);
        exit(EXIT_FAILURE);
    }

    if ((*stack)->n < 0 || (*stack)->n > 127)
    {
        fprintf(stderr, "L%u: can't pchar, value out of range\n", line_number);
        exit(EXIT_FAILURE);
    }

    putchar((*stack)->n);
    putchar('\n');

}
/**
 * pstr - Prints the string starting from the top of the stack.
 * @stack: Double pointer to the top of the stack.
 * @line_number: Line number where the instruction appears.
 *
 * Description: Treats the integer stored in each element of the stack as the
 * ASCII value of the character to be printed. The string stops when either:
 * the stack is over, the value of the element is 0, or the value of the element
 * is not in the ASCII table. If the stack is empty, prints only a new line.
 */
void pstr(stack_t **stack, unsigned int line_number)
{
    stack_t *current = *stack;

    (void)line_number;

    while (current != NULL)
    {
        if (current->n <= 0 || current->n > 127)
            break;
        putchar(current->n);
        current = current->next;
    }

    putchar('\n');
}
/**
 * rotl - Rotates the stack to the top.
 * @stack: Double pointer to the top of the stack.
 * @line_number: Line number where the instruction appears.
 *
 * Description: The top element of the stack becomes the last one,
 * and the second top element of the stack becomes the first one.
 * rotl never fails.
 */
void rotl(stack_t **stack, unsigned int line_number)
{
    stack_t *current = *stack;
    stack_t *new_top;

    (void)line_number;

    if (*stack == NULL || (*stack)->next == NULL)
        return;

    new_top = (*stack)->next;
    new_top->prev = NULL;

    while (current->next != NULL)
        current = current->next;

    current->next = *stack;
    (*stack)->prev = current;
    (*stack)->next = NULL;

    *stack = new_top;
}
/**
 * rotr - Rotates the stack to the bottom.
 * @stack: Double pointer to the top of the stack.
 * @line_number: Line number where the instruction appears.
 *
 * Description: The last element of the stack becomes the top element of the stack.
 * rotr never fails.
 */
void rotr(stack_t **stack, unsigned int line_number)
{
    stack_t *current = *stack;
    stack_t *new_top;

    (void)line_number;

    if (*stack == NULL || (*stack)->next == NULL)
        return;

    /* Traverse to the last element */
    while (current->next != NULL)
        current = current->next;

    /* Set new top */
    new_top = current;
    current->prev->next = NULL;
    new_top->prev = NULL;
    new_top->next = *stack;
    (*stack)->prev = new_top;

    /* Update stack top */
    *stack = new_top;
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
        {"sub", sub},
				{"div", div_op},
        {"mul", mul},
        {"mod", mod},
        {"pchar", pchar},
        {"pstr", pstr},
        {"rotl", rotl},
        {"rotr", rotr},
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
