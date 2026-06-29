//
// Created by John Johnston on 3/29/24.
//

#ifndef CHESSPP_BOT_H
#define CHESSPP_BOT_H

#include "Player.h"

enum BotDifficulty {
	Random,
	SuperEasy,
	Easy,
	Intermediate,
	Hard,
	Expert
};

// AI player that selects moves using search algorithms.
// Inherits from Player and overrides move() to use search instead of user input.
class Bot : public Player {
	BotDifficulty level;

public:
	Bot();
	Bot(BotDifficulty level, Piece::Color color, Board * board);
	bool isBot() override;
	bool move() override;

	// Move selection by difficulty
	Move getRandomMove();
	Move getSuperEasyMove();
	Move getEasyMove();
	Move getIntermediateMove();
	Move getHardMove();

	// Root-level search dispatcher (parallel at root, sequential below)
	Move getMinMaxMove(int depth);

	// Move ordering heuristic for better alpha-beta pruning
	vector<Move> getAllMovesSorted();

	// Color-agnostic negamax with alpha-beta. Returns the score from the
	// perspective of `toMove` (positive = good for toMove). Uses make/unmake.
	static double negamax(Board & board, Piece::Color toMove, int depth, double alpha, double beta, int ply);

	// Quiescence search: resolves outstanding captures past the search horizon
	// so the engine never misjudges a hanging piece / recapture. Returns the
	// score from the perspective of `toMove`.
	static double quiesce(Board & board, Piece::Color toMove, double alpha, double beta);
};

#endif //CHESSPP_BOT_H
