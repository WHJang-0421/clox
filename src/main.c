#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"

int main(int argc, const char * argv[]) {
    initVM();
    Chunk chunk;
    initChunk(&chunk);

    // problem 1
    #define ADD_CONSTANT(i, line) \
        do { \
            int constant = addConstant(&chunk, i);\
            writeChunk(&chunk, OP_CONSTANT, line);\
            writeChunk(&chunk, constant, line);\
        } while (false)
    // line 1
    ADD_CONSTANT(1, 1);
    ADD_CONSTANT(2, 1);
    writeChunk(&chunk, OP_MULTIPLY, 1);
    ADD_CONSTANT(3, 1);
    writeChunk(&chunk, OP_ADD, 1);
    writeChunk(&chunk, OP_RETURN, 1);
    interpret(&chunk);
    disassembleChunk(&chunk, "test chunk");
    freeVM();
    freeChunk(&chunk);
    // line 2
    initVM();
    initChunk(&chunk);
    ADD_CONSTANT(2, 2);
    ADD_CONSTANT(3, 2);
    writeChunk(&chunk, OP_MULTIPLY, 2);
    ADD_CONSTANT(1, 2);
    writeChunk(&chunk, OP_ADD, 2);
    writeChunk(&chunk, OP_RETURN, 2);
    interpret(&chunk);
    disassembleChunk(&chunk, "test chunk");
    freeVM();
    freeChunk(&chunk);
    // line 3
    initVM();
    initChunk(&chunk);
    ADD_CONSTANT(3, 3);
    ADD_CONSTANT(2, 3);
    writeChunk(&chunk, OP_SUBTRACT, 3);
    ADD_CONSTANT(1, 3);
    writeChunk(&chunk, OP_SUBTRACT, 3);
    writeChunk(&chunk, OP_RETURN, 3);
    interpret(&chunk);
    disassembleChunk(&chunk, "test chunk");
    freeVM();
    freeChunk(&chunk);
    // line 4
    initVM();
    initChunk(&chunk);
    ADD_CONSTANT(2, 4);
    ADD_CONSTANT(3, 4);
    writeChunk(&chunk, OP_MULTIPLY, 4);
    ADD_CONSTANT(1, 4);
    writeChunk(&chunk, OP_ADD, 4);
    ADD_CONSTANT(4, 4);
    ADD_CONSTANT(5, 4);
    writeChunk(&chunk, OP_NEGATE, 4);
    writeChunk(&chunk, OP_DIVIDE, 4);
    writeChunk(&chunk, OP_SUBTRACT, 4);
    writeChunk(&chunk, OP_RETURN, 4);
    interpret(&chunk);
    disassembleChunk(&chunk, "test chunk");
    freeVM();
    freeChunk(&chunk);

    // for performance measurement
    for (int i = 0; i < 1000000; i++) {
    initVM();
    initChunk(&chunk);
    ADD_CONSTANT(10, 5);
    #undef ADD_CONSTANT
    for (int i = 0; i < 250; i++) {
        writeChunk(&chunk, OP_NEGATE, 5);
    }
    writeChunk(&chunk, OP_RETURN, 5);
    freeVM();
    freeChunk(&chunk);
    }

    return 0;
}