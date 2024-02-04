//
// Created by Brandon Jordan on 1/23/24.
//

#include "lexer.h"
#include "interpret.h"

void interpret() {
    stack.items = malloc(sizeof(unsigned char));
    while (p < operations.size) {
        op operation = operations.items[p];
        switch (operation.kind) {
            case OP_RIGHT:
                head += operation.operand;
                grow_stack();

                break;
            case OP_LEFT:
                if (head == 0) {
                    interpret_error("cannot move anymore left, already at bottom of stack!");
                }

                head -= operation.operand;
                break;
            case OP_IN: {
                char input[1];
                scanf("%c", input);
                stack.items[head] = input[0];
            }
                break;
            case OP_OUT:
                for (size_t i = 0; i < operation.operand; i++) {
                    printf("%c", stack.items[head]);
                }

                break;
            case OP_JUMP_ZERO:
                if (stack.items[head] == 0) {
                    p = operation.operand;
                    continue;
                }

                break;
            case OP_JUMP_NONZERO:
                if (stack.items[head] != 0) {
                    p = operation.operand;
                    continue;
                }

                break;
            case OP_ADD:
                stack.items[head] += operation.operand;

                break;
            case OP_SUB:
                stack.items[head] -= operation.operand;

                break;
        }
        ++p;
    }
}