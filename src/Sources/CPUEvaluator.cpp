//
// CPUEvaluator.cpp - CPU fallback for batch board evaluation
//

#include "../Headers/GPUEvaluator.h"
#include <cstring>

class CPUEvaluator : public GPUEvaluator {
    // Indexed by Piece::Type: NoType=0, Pawn=1, Bishop=2, Knight=3, Rook=4, Queen=5, King=6
    static constexpr float pieceValues[7] = {0.0f, 1.0f, 3.0f, 3.0f, 5.0f, 9.0f, 20000.0f};

public:
    bool isGPUAccelerated() const override { return false; }

    std::vector<float> evaluateBatch(const std::vector<CompactBoard>& boards) override {
        std::vector<float> results(boards.size());

        for (size_t i = 0; i < boards.size(); i++) {
            float eval = 0.0f;
            for (int sq = 0; sq < 64; sq++) {
                uint8_t piece = boards[i].squares[sq];
                if (piece == 0) continue;

                uint8_t type = piece & 0x07;
                bool isWhite = (piece >> 3) & 1;
                float val = pieceValues[type];
                eval += isWhite ? val : -val;
            }
            results[i] = eval;
        }

        return results;
    }
};

#ifndef USE_METAL
// Factory implementation for non-Metal platforms
std::unique_ptr<GPUEvaluator> GPUEvaluator::create() {
    return std::make_unique<CPUEvaluator>();
}
#endif
