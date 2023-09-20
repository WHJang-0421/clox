#include "common.h"
#include "chunk.h"
#include "debug.h"

int main(int argc, const char * argv[]) {
    Chunk chunk;
    initChunk(&chunk);

    for (int i = 0; i < 1000; i++) {
        writeConstant(&chunk, i * 1.53541, i / 10 + 1);
    }

    writeChunk(&chunk, OP_RETURN, 123);

    disassembleChunk(&chunk, "test chunk");

    freeChunk(&chunk);

    return 0;
}