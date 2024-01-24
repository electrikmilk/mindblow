//
// Created by Brandon Jordan on 1/22/24.
//

#include "lexer.h"

void lex() {
    idx = -1;
    advance();

    while (idx != -1) {
        switch (current_char) {
            case OP_RIGHT:
            case OP_LEFT:
            case OP_OUT:
            case OP_IN:
            case OP_ADD:
            case OP_SUB: {
                size_t operand = 1;
                while (peek(1) == current_char) {
                    ++operand;
                    advance();
                }

                op operation = {
                        .kind = current_char,
                        .operand = operand,
                };
                append_operation(&operation);
            }
                break;
            case OP_JUMP_ZERO: {
                size_t addr = operations.size;
                op operation = {
                        .kind = current_char,
                        .operand = 0,
                };
                append_operation(&operation);
                append_addr(&addr);
            }
                break;
            case OP_JUMP_NONZERO: {
                if (addresses.size == 0) {
                    lex_error("ended loop with no starting loop");
                }

                size_t addr = addresses.items[--addresses.size];
                op operation = {
                        .kind = current_char,
                        .operand = addr + 1,
                };
                append_operation(&operation);

                operations.items[addr].operand = operations.size;
            }
                break;
            default: {
            }
        }

        advance();
    }
}
