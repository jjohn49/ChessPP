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

	// Root-level search dispatchers (parallel at root, sequential below)
	Move getMinMaxMove(int depth);
	Move getNegaMaxMove(int depth);

	// Move ordering heuristic for better alpha-beta pruning
	vector<Move> getAllMovesSorted();

	// Alpha-beta search (maximizer/minimizer pair, uses make/unmake)
	static optional<double> maxAlphaBeta(Board & board, Move move, int depth, double alpha, double beta);
	static optional<double> minAlphaBeta(Board & board, Move move, int depth, double alpha, double beta);

	// Negamax search (single function, uses make/unmake)
	static float negaMaxAlgo(Board & board, Move & move, int depth, float alpha, float beta);
};

#endif //CHESSPP_BOT_H
