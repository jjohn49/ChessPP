//
// MetalEvaluator.mm - Metal GPU batch board evaluation (macOS / Apple Silicon)
//

#ifdef USE_METAL

#import <Foundation/Foundation.h>
#import <Metal/Metal.h>
#include "../Headers/GPUEvaluator.h"
#include <iostream>
#include <vector>

class MetalEvaluator : public GPUEvaluator {
    id<MTLDevice> device;
    id<MTLCommandQueue> commandQueue;
    id<MTLComputePipelineState> pipelineState;
    id<MTLBuffer> pieceValuesBuffer;
    id<MTLBuffer> pieceSquareTablesBuffer;

    bool initialized = false;

public:
    MetalEvaluator() {
        @autoreleasepool {
            device = MTLCreateSystemDefaultDevice();
            if (!device) {
                std::cerr << "MetalEvaluator: No Metal device found" << std::endl;
                return;
            }

            commandQueue = [device newCommandQueue];
            if (!commandQueue) {
                std::cerr << "MetalEvaluator: Failed to create command queue" << std::endl;
                return;
            }

            // Load the Metal shader library
            NSError* error = nil;

            // Try loading from the shader source file
            NSString* shaderPath = [[NSBundle mainBundle] pathForResource:@"evaluate" ofType:@"metal"];
            NSString* shaderSource = nil;

            if (shaderPath) {
                shaderSource = [NSString stringWithContentsOfFile:shaderPath encoding:NSUTF8StringEncoding error:&error];
            }

            // Fallback: try loading from current directory or known paths
            if (!shaderSource) {
                NSArray* searchPaths = @[
                    @"src/Shaders/evaluate.metal",
                    @"../src/Shaders/evaluate.metal",
                    @"evaluate.metal"
                ];

                for (NSString* path in searchPaths) {
                    shaderSource = [NSString stringWithContentsOfFile:path encoding:NSUTF8StringEncoding error:&error];
                    if (shaderSource) break;
                }
            }

            if (!shaderSource) {
                // Embed the shader as a fallback
                shaderSource = @R"(
                    #include <metal_stdlib>
                    using namespace metal;

                    kernel void evaluateBoards(
                        device const uint8_t* boards       [[buffer(0)]],
                        constant float* pieceValues        [[buffer(1)]],
                        constant float* pieceSquareTables  [[buffer(2)]],
                        device float* results              [[buffer(3)]],
                        uint id                            [[thread_position_in_grid]]
                    ) {
                        float eval = 0.0f;
                        uint offset = id * 64;
                        for (uint sq = 0; sq < 64; sq++) {
                            uint8_t piece = boards[offset + sq];
                            if (piece == 0) continue;
                            uint type = piece & 0x07;
                            bool isWhite = (piece >> 3) & 1;
                            float materialVal = pieceValues[type];
                            float positionalVal = pieceSquareTables[type * 64 + sq] * 0.1f;
                            float totalVal = materialVal + positionalVal;
                            eval += isWhite ? totalVal : -totalVal;
                        }
                        results[id] = eval;
                    }
                )";
            }

            id<MTLLibrary> library = [device newLibraryWithSource:shaderSource options:nil error:&error];
            if (!library) {
                std::cerr << "MetalEvaluator: Failed to compile shader: "
                          << [[error localizedDescription] UTF8String] << std::endl;
                return;
            }

            id<MTLFunction> kernelFunction = [library newFunctionWithName:@"evaluateBoards"];
            if (!kernelFunction) {
                std::cerr << "MetalEvaluator: Failed to find evaluateBoards kernel" << std::endl;
                return;
            }

            pipelineState = [device newComputePipelineStateWithFunction:kernelFunction error:&error];
            if (!pipelineState) {
                std::cerr << "MetalEvaluator: Failed to create pipeline state: "
                          << [[error localizedDescription] UTF8String] << std::endl;
                return;
            }

            // Upload piece values (indexed by Piece::Type enum)
            // NoType=0, Pawn=1, Bishop=2, Knight=3, Rook=4, Queen=5, King=6
            float pieceValues[7] = {0.0f, 1.0f, 3.0f, 3.0f, 5.0f, 9.0f, 20000.0f};
            pieceValuesBuffer = [device newBufferWithBytes:pieceValues
                                                   length:sizeof(pieceValues)
                                                  options:MTLResourceStorageModeShared];

            // Upload piece-square tables (7 types * 64 squares = 448 floats)
            // For now, initialize to zeros — the positional values come from each piece's getEvalBoard()
            // In a full integration, these would be populated from the piece eval boards
            float pieceSquareTables[7 * 64];
            memset(pieceSquareTables, 0, sizeof(pieceSquareTables));
            pieceSquareTablesBuffer = [device newBufferWithBytes:pieceSquareTables
                                                         length:sizeof(pieceSquareTables)
                                                        options:MTLResourceStorageModeShared];

            initialized = true;
            std::cout << "MetalEvaluator: Initialized on " << [[device name] UTF8String] << std::endl;
        }
    }

    ~MetalEvaluator() override = default;

    bool isGPUAccelerated() const override {
        return initialized;
    }

    std::vector<float> evaluateBatch(const std::vector<CompactBoard>& boards) override {
        size_t count = boards.size();
        std::vector<float> results(count, 0.0f);

        if (!initialized || count == 0) {
            return results;
        }

        @autoreleasepool {
            // Create input buffer (Apple Silicon unified memory = zero-copy with StorageModeShared)
            size_t boardsSize = count * 64;
            id<MTLBuffer> boardsBuffer = [device newBufferWithBytes:boards.data()
                                                             length:boardsSize
                                                            options:MTLResourceStorageModeShared];

            // Create output buffer
            id<MTLBuffer> resultsBuffer = [device newBufferWithLength:count * sizeof(float)
                                                              options:MTLResourceStorageModeShared];

            // Create command buffer and encoder
            id<MTLCommandBuffer> commandBuffer = [commandQueue commandBuffer];
            id<MTLComputeCommandEncoder> encoder = [commandBuffer computeCommandEncoder];

            [encoder setComputePipelineState:pipelineState];
            [encoder setBuffer:boardsBuffer offset:0 atIndex:0];
            [encoder setBuffer:pieceValuesBuffer offset:0 atIndex:1];
            [encoder setBuffer:pieceSquareTablesBuffer offset:0 atIndex:2];
            [encoder setBuffer:resultsBuffer offset:0 atIndex:3];

            // Dispatch threads
            NSUInteger threadGroupSize = std::min((NSUInteger)pipelineState.maxTotalThreadsPerThreadgroup, (NSUInteger)256);
            MTLSize gridSize = MTLSizeMake(count, 1, 1);
            MTLSize groupSize = MTLSizeMake(threadGroupSize, 1, 1);

            [encoder dispatchThreads:gridSize threadsPerThreadgroup:groupSize];
            [encoder endEncoding];

            // Submit and wait
            [commandBuffer commit];
            [commandBuffer waitUntilCompleted];

            // Read results back
            float* resultPtr = (float*)[resultsBuffer contents];
            memcpy(results.data(), resultPtr, count * sizeof(float));
        }

        return results;
    }
};

// Factory implementation for Metal platform
std::unique_ptr<GPUEvaluator> GPUEvaluator::create() {
    auto evaluator = std::make_unique<MetalEvaluator>();
    if (evaluator->isGPUAccelerated()) {
        return evaluator;
    }
    return nullptr; // Caller should fall back to CPU evaluator
}

#endif // USE_METAL
