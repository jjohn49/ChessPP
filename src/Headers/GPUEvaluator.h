///
// GPUEvaluator.h - Abstract interface for batch board evaluation
//

#ifndef CHESSPP_GPUEVALUATOR_H
#define CHESSPP_GPUEVALUATOR_H

#include <vector>
#include <cstdint>
#include <memory>

// Compact board representation for GPU evaluation.
// Each byte encodes a square: 0 = empty, otherwise bits 0-2 = piece type, bit 3 = color (1=white, 0=black)
// Piece types: 1=Pawn, 2=Bishop, 3=Knight, 4=Rook, 5=Queen, 6=King (matches Piece::Type enum)
struct CompactBoard {
    uint8_t squares[64]; // row-major: squares[row * 8 + col]
};

class GPUEvaluator {
public:
    virtual ~GPUEvaluator() = default;

    // Evaluate a batch of board positions, returning one score per board.
    // Positive scores favor white, negative favor black.
    virtual std::vector<float> evaluateBatch(const std::vector<CompactBoard>& boards) = 0;

    // Returns true if this evaluator is GPU-accelerated
    virtual bool isGPUAccelerated() const = 0;

    // Factory: create the best available evaluator for this platform
    static std::unique_ptr<GPUEvaluator> create();
};

#endif //CHESSPP_GPUEVALUATOR_H
