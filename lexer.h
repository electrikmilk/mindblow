//
// Created by Brandon Jordan on 1/22/24.
//

#ifndef BF_LEXER_H
#define BF_LEXER_H

#include <stdio.h>
#include <stdlib.h>

typedef enum {
    OP_RIGHT = '>',
    OP_LEFT = '<',
    OP_ADD = '+',
    OP_SUB = '-',
    OP_IN = ',',
    OP_OUT = '.',
    OP_JUMP_ZERO = '[',
    OP_JUMP_NONZERO = ']',
} op_type;

typedef struct {
    op_type kind;
    size_t operand;
} op;

typedef struct {
    op *items;
    size_t size;
} ops;

ops operations;

typedef struct {
    size_t *items;
    size_t size;
} address_stack;

address_stack addresses;

char *buffer;
size_t file_length;

static ssize_t idx;
unsigned char current_char;

size_t line_idx = 1;
size_t line_char_idx = 1;

char *filename;

void print_operations() {
    printf("==== :: OPERATIONS [%ld] :: ====\n\n", operations.size);
    for (size_t i = 0; i < operations.size; i++) {
        op operation = operations.items[i];
        // dim, italic, bold esc seqs
        printf("\033[2m\033[3m%02ld\033[0m \033[2m\033[1m%c\033[0m \t ", i, operation.kind);
        switch (operation.kind) {
            case OP_RIGHT:
                // italic esc seq
                printf("\033[3mRIGHT\033[0m");
                break;
            case OP_LEFT:
                printf("\033[3mLEFT\033[0m ");
                break;
            case OP_IN:
                // blue esc seq
                printf("\033[34mINPUT\033[0m");
                break;
            case OP_OUT:
                // orange esc seq
                printf("\033[33mOUT\033[0m  ");
                break;
            case OP_JUMP_ZERO:
                // underline esc seq
                printf("\033[4mJUMP\033[0m ");
                break;
            case OP_JUMP_NONZERO:
                printf("\033[4mBACK\033[0m ");
                break;
            case OP_ADD:
                // green esc seq
                printf("\033[32mADD\033[0m  ");
                break;
            case OP_SUB:
                // red esc seq
                printf("\033[31mSUB\033[0m  ");
                break;
        }
        // reset esc seqs
        printf("\033[0m    ");

        switch (operation.kind) {
            case OP_JUMP_ZERO:
            case OP_JUMP_NONZERO: {
                // dark gray esc seq
                printf("\033[100m");
            }
                break;
            default:
                // invert esc seq
                printf("\033[7m");
        }

        printf(" %02ld \033[0m\n", operation.operand);
    }
}

void lex_error(char *message) {
    // esc seq for red
    printf("\033[31m");

    printf("lexer error: %s (%s:%ld:%ld)\n\n", message, filename, line_idx, line_char_idx);
    print_operations();

    // reset esc seqs
    printf("\033[0m");
    exit(1);
}

void append_operation(op *operation) {
    if ((operations.items = realloc(operations.items, sizeof(*operations.items) + sizeof(op))) == NULL) {
        char message[37];
        sprintf(message, "failed to allocate for operation '%c'", operation->kind);
        lex_error(message);
    }
    operations.items[operations.size] = *operation;
    ++operations.size;
}

void append_addr(const size_t *addr) {
    if ((addresses.items = realloc(addresses.items, sizeof(*addresses.items) + sizeof(size_t))) == NULL) {
        char message[80];
        sprintf(message, "failed to allocate address for operation '%ld'", *addr);
        lex_error(message);
    }
    addresses.items[addresses.size] = *addr;
    ++addresses.size;
}

void advance() {
    size_t next = idx + 1;
    if (next >= file_length) {
        idx = -1;
        return;
    }
    if (current_char == '\n') {
        ++line_idx;
        line_char_idx = 1;
    } else {
        ++line_char_idx;
    }

    ++idx;
    current_char = buffer[idx];
}

unsigned char peek(int inc) {
    size_t next = idx + inc;
    if (next >= file_length) {
        return 0;
    }

    return buffer[next];
}

#endif //BF_LEXER_H
