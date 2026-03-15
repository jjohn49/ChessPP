#include <metal_stdlib>
using namespace metal;

// Each board is 64 bytes (row-major). Each byte:
//   0 = empty
//   bits 0-2 = piece type (1=Pawn, 2=Bishop, 3=Knight, 4=Rook, 5=Queen, 6=King)
//   bit 3 = color (1=white, 0=black)

kernel void evaluateBoards(
    device const uint8_t* boards       [[buffer(0)]],   // N * 64 bytes
    constant float* pieceValues        [[buffer(1)]],    // 7 floats (index 0 unused)
    constant float* pieceSquareTables  [[buffer(2)]],    // 7 * 64 = 448 floats (type * 64 + square)
    device float* results              [[buffer(3)]],    // N floats
    uint id                            [[thread_position_in_grid]]
) {
    float eval = 0.0f;
    uint offset = id * 64;

    for (uint sq = 0; sq < 64; sq++) {
        uint8_t piece = boards[offset + sq];
        if (piece == 0) continue;

        uint type = piece & 0x07;       // bits 0-2: piece type
        bool isWhite = (piece >> 3) & 1; // bit 3: color

        float materialVal = pieceValues[type];
        float positionalVal = pieceSquareTables[type * 64 + sq] * 0.1f;
        float totalVal = materialVal + positionalVal;

        eval += isWhite ? totalVal : -totalVal;
    }

    results[id] = eval;
}
