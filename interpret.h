//
// Created by Brandon Jordan on 1/23/24.
//

#ifndef BF_INTERPRET_H
#define BF_INTERPRET_H

unsigned long head = 0;
unsigned long p = 0;

typedef struct {
    char *items;
    size_t size;
} memory_stack;

memory_stack stack;

void print_stack() {
    printf("==== :: STACK [%ld] :: ====\n\n", stack.size);
    for (size_t i = 0; i < stack.size; i++) {
        printf("%02ld | %c\n", i, stack.items[i]);
    }
}

void interpret_error(char *message) {
    // esc seq for red
    printf("\033[31m");

    op current_operation = operations.items[p];

    printf("[Interpreter] Error: %s (%s)\n\n", message, filename);
    printf("\tInstruction: %ld\n", p);
    printf("\tTape head: %ld\n", head);
    printf("\tOperation: %c (%ld)\n\n", current_operation.kind, current_operation.operand);

    // reset esc seqs
    printf("\033[0m");

    print_operations();
    print_stack();

    exit(1);
}

void grow_stack() {
    if (head <= stack.size) {
        return;
    }
    if ((stack.items = realloc(stack.items, stack.size * sizeof(size_t) + sizeof(size_t))) == NULL) {
        interpret_error("failed to allocate stack when moving right! D:");
    }
    stack.size++;
}

#endif //BF_INTERPRET_H
