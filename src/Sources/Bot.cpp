//
// Created by John Johnston on 3/29/24.
//

#include "../Headers/Bot.h"
#include <cstdlib>
#include <ctime>
#include "../Headers/Board.h"
#include <unordered_map>
#include <algorithm>
#include <future>
#include <thread>

Bot::Bot(): Bot{Easy, Piece::NoColor, nullptr} {}

Bot::Bot(BotDifficulty level, Piece::Color color, Board *board): Player{color, board} {
	this->level = level;
}

bool Bot::isBot() {
	return true;
}

bool Bot::move() {
	Move move;
	if (level == Random) {
		move = getRandomMove();
	} else if (level == SuperEasy) {
		move = getSuperEasyMove();
	} else if (level == Easy) {
		move = getEasyMove();
	} else if (level == Intermediate) {
		move = getIntermediateMove();
	} else if (level == Hard) {
		move = getHardMove();
	}

	return this->movePiece(move);
}

Move Bot::getRandomMove() {
	vector<Move> moves = getAllMoves();
	srand(time(0));
	return moves[rand() % moves.size()];
}

Move Bot::getSuperEasyMove() {
	return getAllMoves()[0];
}

Move Bot::getEasyMove() {
	return getMinMaxMove(1);
}

Move Bot::getIntermediateMove() {
	return getNegaMaxMove(2);
}

Move Bot::getHardMove() {
	return getMinMaxMove(5);
}

// Sorts moves by heuristic value (captures, promotions, castling, positional).
// Best moves first for better alpha-beta pruning cutoffs.
vector<Move> Bot::getAllMovesSorted() {
	vector<Move> moves = getAllMoves();

	unordered_map<Piece::Type, int> pointsPerPiece = {
		{Piece::Pawn, 1},
		{Piece::Knight, 3},
		{Piece::Bishop, 3},
		{Piece::Rook, 5},
		{Piece::Queen, 9},
		{Piece::King, 20},
	};

	unordered_map<float, vector<Move>> pointsPerMove;

	for (Move & m : moves) {
		float total = 0;

		if (m.getIsPawnPromotion()) {
			total += 9;
		}

		if (m.getCapturedPiece() != nullptr) {
			total += pointsPerPiece[m.getCapturedPiece()->getType()];
		}

		if (m.getIsKingSideCastle() || m.getIsQueenSideCastle()) {
			total += 2;
		}

		pair<int, int> position = m.getNewPosition();
		total += m.getMovingPiece()->getEvalBoard()[position.first][position.second];

		pointsPerMove[total].push_back(m);
	}

	vector<float> keys{};
	for (auto kv : pointsPerMove) {
		keys.push_back(kv.first);
	}

	sort(keys.begin(), keys.end(), greater<float>());

	vector<Move> ret{};
	if (!keys.empty()) {
		ret = pointsPerMove[keys[0]];
		for (int x = 1; x < (int)keys.size(); x++) {
			ret.insert(ret.end(), pointsPerMove[keys[x]].begin(), pointsPerMove[keys[x]].end());
		}
	}

	return ret;
}

// Maximizer half of alpha-beta search.
// Uses make/unmake to avoid deepCopy at every node.
// Returns nullopt if the move leaves own king in check (illegal).
optional<double> Bot::maxAlphaBeta(Board & board, Move move, int depth, double alpha, double beta) {
	try {
		if (move.getCapturedPiece() != nullptr && move.getCapturedPiece()->getType() == Piece::King) {
			return 1000000000000;
		}

		if (depth == 0) {
			return board.evaluate();
		}

		Piece::Color movingColor = move.getMovingPiece()->getColor();
		UndoInfo undo = board.makeSearchMove(move);

		if (board.isColorInCheck(movingColor)) {
			board.unmakeSearchMove(undo);
			return nullopt;
		}

		double bestVal = -1000000000000;
		Piece::Color oppColor = (movingColor == Piece::White) ? Piece::Black : Piece::White;
		vector<Move> oppMoves = board.getAllMovesForColor(oppColor);

		for (Move & m : oppMoves) {
			try {
				if (m.getCapturedPiece() != nullptr && m.getCapturedPiece()->getType() == Piece::King) {
					board.unmakeSearchMove(undo);
					return 1000000000000;
				}

				optional<double> result = minAlphaBeta(board, m, depth - 1, alpha, beta);
				if (!result) continue;
				double score = result.value();

				if (score > bestVal) {
					bestVal = score;
					if (score > alpha) {
						alpha = score;
					}
				}

				if (score >= beta) {
					board.unmakeSearchMove(undo);
					return score;
				}
			} catch (...) {
				cerr << "Error during loop in maxAlphaBeta" << endl;
				cerr << "Move is : " << m.toString();
				board.unmakeSearchMove(undo);
				rethrow_exception(current_exception());
			}
		}

		board.unmakeSearchMove(undo);
		return bestVal;
	} catch (...) {
		exception_ptr ex = current_exception();
		std::cerr << "Error in maxAlphaBeta for depth: " << depth << ", alpha: " << alpha << ", beta: " << beta << "\n"
		          << "Board: \n" << board.toString() << "\n"
		          << "Move: " << move.toString() << endl;
		rethrow_exception(ex);
	}
}

// Minimizer half of alpha-beta search.
// Uses make/unmake to avoid deepCopy at every node.
// Returns nullopt if the move leaves own king in check (illegal).
optional<double> Bot::minAlphaBeta(Board & board, Move move, int depth, double alpha, double beta) {
	try {
		if (move.getCapturedPiece() != nullptr && move.getCapturedPiece()->getType() == Piece::King) {
			return -1000000000000;
		}

		if (depth == 0) {
			return board.evaluate();
		}

		Piece::Color movingColor = move.getMovingPiece()->getColor();
		UndoInfo undo = board.makeSearchMove(move);

		if (board.isColorInCheck(movingColor)) {
			board.unmakeSearchMove(undo);
			return nullopt;
		}

		double bestVal = 1000000000000;
		Piece::Color oppColor = (movingColor == Piece::White) ? Piece::Black : Piece::White;
		vector<Move> oppMoves = board.getAllMovesForColor(oppColor);

		for (Move & m : oppMoves) {
			try {
				optional<double> result = maxAlphaBeta(board, m, depth - 1, alpha, beta);
				if (!result) continue;
				double score = result.value();

				if (score < bestVal) {
					bestVal = score;
					if (score < beta) {
						beta = score;
					}
				}

				if (score <= alpha) {
					board.unmakeSearchMove(undo);
					return score;
				}
			} catch (...) {
				cerr << "Error during loop in minAlphaBeta" << endl;
				cerr << "Move is : " << m.toString();
				board.unmakeSearchMove(undo);
				rethrow_exception(current_exception());
			}
		}

		board.unmakeSearchMove(undo);
		return bestVal;
	} catch (...) {
		exception_ptr ex = current_exception();
		std::cerr << "Error in minAlphaBeta for depth: " << depth << ", alpha: " << alpha << ", beta: " << beta << "\n"
		          << "Board: \n" << board.toString() << "\n"
		          << "Move: " << move.toString() << endl;
		rethrow_exception(ex);
	}
}

// Negamax with alpha-beta pruning. Uses make/unmake to avoid deepCopy.
// evaluate() is white-positive, so the result is negated for black.
float Bot::negaMaxAlgo(Board & board, Move & move, int depth, float alpha, float beta) {
	UndoInfo undo = board.makeSearchMove(move);

	if (depth == 0) {
		float eval = board.evaluate();
		float result = (move.getMovingPiece()->getColor() == Piece::Black) ? -eval : eval;
		board.unmakeSearchMove(undo);
		return result;
	}

	float val = -10000000;
	Piece::Color oppColor = (move.getMovingPiece()->getColor() == Piece::White) ? Piece::Black : Piece::White;
	vector<Move> oppMoves = board.getAllMovesForColor(oppColor);

	for (Move & m : oppMoves) {
		if (m.getIsEnPessant()) {
			continue;
		}

		float mEval = -negaMaxAlgo(board, m, depth - 1, -beta, -alpha);
		val = max(mEval, val);
		alpha = max(alpha, val);
		if (alpha >= beta) {
			board.unmakeSearchMove(undo);
			return beta;
		}
	}

	board.unmakeSearchMove(undo);
	return val;
}

// Root-level alpha-beta search with parallel move evaluation.
// Searches the first move serially to narrow the alpha/beta window,
// then dispatches remaining moves in parallel via std::async.
Move Bot::getMinMaxMove(int depth) {
	vector<Move> sortedMoves = getAllMovesSorted();

	if (sortedMoves.empty()) {
		return getAllMoves()[0];
	}

	bool isWhite = (getColor() == Piece::White);
	double bestScore = isWhite ? -100000000000000.0 : 100000000000000.0;
	vector<Move> bestMoves;
	double alpha = -100000000000000.0;
	double beta = 100000000000000.0;

	// Search the first move serially to establish a good alpha/beta window
	{
		Move & firstMove = sortedMoves[0];
		Board copyBoard = this->getBoard()->deepCopy();
		optional<double> val = minAlphaBeta(copyBoard, firstMove, depth, alpha, beta);
		if (val) {
			bestScore = val.value();
			bestMoves.push_back(firstMove);
			if (isWhite) {
				alpha = max(alpha, bestScore);
			} else {
				beta = min(beta, bestScore);
			}
		}
	}

	if (sortedMoves.size() <= 1) {
		if (bestMoves.empty()) return sortedMoves[0];
		return bestMoves[rand() % bestMoves.size()];
	}

	// Search remaining moves in parallel (each thread gets its own deepCopy)
	struct MoveResult {
		int index;
		optional<double> score;
	};

	double sharedAlpha = alpha;
	double sharedBeta = beta;

	vector<future<MoveResult>> futures;
	futures.reserve(sortedMoves.size() - 1);

	for (int i = 1; i < (int)sortedMoves.size(); i++) {
		Move m = sortedMoves[i];
		Board * boardPtr = this->getBoard();
		double localAlpha = sharedAlpha;
		double localBeta = sharedBeta;

		futures.push_back(std::async(std::launch::async, [boardPtr, m, depth, localAlpha, localBeta, i]() mutable -> MoveResult {
			Board copyBoard = boardPtr->deepCopy();
			optional<double> val = minAlphaBeta(copyBoard, m, depth, localAlpha, localBeta);
			return {i, val};
		}));
	}

	for (auto & f : futures) {
		MoveResult result = f.get();
		if (!result.score) continue;

		double score = result.score.value();

		if (isWhite) {
			if (score > bestScore) {
				bestScore = score;
				bestMoves.clear();
				bestMoves.push_back(sortedMoves[result.index]);
			} else if (score == bestScore) {
				bestMoves.push_back(sortedMoves[result.index]);
			}
		} else {
			if (score < bestScore) {
				bestScore = score;
				bestMoves.clear();
				bestMoves.push_back(sortedMoves[result.index]);
			} else if (score == bestScore) {
				bestMoves.push_back(sortedMoves[result.index]);
			}
		}
	}

	if (bestMoves.empty()) {
		return sortedMoves[0];
	}

	return bestMoves[rand() % bestMoves.size()];
}

// Root-level negamax search with parallel move evaluation.
// Searches the first move serially, then dispatches remaining in parallel.
Move Bot::getNegaMaxMove(int depth) {
	vector<Move> sortedMoves = getAllMovesSorted();

	if (sortedMoves.empty()) {
		return getAllMoves()[0];
	}

	float bestScore = -10000000;
	vector<Move> bestMoves;

	// Search first move serially to establish a baseline
	{
		Move & firstMove = sortedMoves[0];
		Board copyBoard = getBoard()->deepCopy();
		float val = -negaMaxAlgo(copyBoard, firstMove, depth, -1000000, 1000000);
		bestScore = val;
		bestMoves.push_back(firstMove);
	}

	if (sortedMoves.size() <= 1) {
		return bestMoves[0];
	}

	// Search remaining moves in parallel (each thread gets its own deepCopy)
	struct MoveResult {
		int index;
		float score;
	};

	vector<future<MoveResult>> futures;
	futures.reserve(sortedMoves.size() - 1);

	for (int i = 1; i < (int)sortedMoves.size(); i++) {
		Move m = sortedMoves[i];
		Board * boardPtr = getBoard();

		futures.push_back(std::async(std::launch::async, [boardPtr, m, depth, i]() mutable -> MoveResult {
			Board copyBoard = boardPtr->deepCopy();
			float val = -negaMaxAlgo(copyBoard, m, depth, -1000000, 1000000);
			return {i, val};
		}));
	}

	for (auto & f : futures) {
		MoveResult result = f.get();

		if (result.score > bestScore) {
			bestScore = result.score;
			bestMoves.clear();
			bestMoves.push_back(sortedMoves[result.index]);
		} else if (result.score == bestScore) {
			bestMoves.push_back(sortedMoves[result.index]);
		}
	}

	if (bestMoves.empty()) {
		return sortedMoves[0];
	}

	return bestMoves[rand() % bestMoves.size()];
}
