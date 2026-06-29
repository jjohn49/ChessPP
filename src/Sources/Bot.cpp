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

// Depths are kept modest because quiescence search resolves captures past the
// horizon, so even a shallow search no longer hangs material in a 1-move tactic.
Move Bot::getEasyMove() {
	return getMinMaxMove(2);
}

Move Bot::getIntermediateMove() {
	return getMinMaxMove(3);
}

Move Bot::getHardMove() {
	return getMinMaxMove(4);
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

// Search score bounds. evaluate() magnitudes are small (tens of points), so these
// sentinels sit far above any real evaluation. MATE is offset by ply so the engine
// prefers faster mates and slower getting-mated.
static constexpr double SEARCH_INF = 1e9;
static constexpr double MATE_SCORE = 1e6;

static inline Piece::Color oppositeColor(Piece::Color c) {
	return (c == Piece::White) ? Piece::Black : Piece::White;
}

// Static evaluation from the perspective of `toMove`.
// evaluate() is white-positive, so negate when it's black to move.
static inline double signedEval(Board & board, Piece::Color toMove) {
	double e = board.evaluate();
	return (toMove == Piece::White) ? e : -e;
}

// Quiescence search: from a "quiet" stand-pat baseline, keep searching only
// capture moves until no captures remain. This resolves recaptures past the
// fixed-depth horizon, so the engine never leaves a piece hanging or misjudges
// a trade just because the capture happened on the last ply.
double Bot::quiesce(Board & board, Piece::Color toMove, double alpha, double beta) {
	double standPat = signedEval(board, toMove);
	if (standPat >= beta) {
		return beta;
	}
	if (standPat > alpha) {
		alpha = standPat;
	}

	vector<Move> moves = board.getAllMovesForColor(toMove);
	for (Move & m : moves) {
		// Only consider captures (getCapturedPiece is set for normal and en passant captures).
		if (m.getCapturedPiece() == nullptr) {
			continue;
		}

		UndoInfo undo = board.makeSearchMove(m);
		if (board.isColorInCheck(toMove)) {
			board.unmakeSearchMove(undo);
			continue; // illegal: leaves own king in check
		}

		double score = -quiesce(board, oppositeColor(toMove), -beta, -alpha);
		board.unmakeSearchMove(undo);

		if (score >= beta) {
			return beta;
		}
		if (score > alpha) {
			alpha = score;
		}
	}

	return alpha;
}

// Color-agnostic negamax with alpha-beta pruning. Returns the score from the
// perspective of `toMove`. Uses make/unmake to avoid deepCopy at every node.
// Illegal moves (leaving own king in check) are skipped; if no legal move
// exists it's checkmate (in check) or stalemate.
double Bot::negamax(Board & board, Piece::Color toMove, int depth, double alpha, double beta, int ply) {
	if (depth == 0) {
		return quiesce(board, toMove, alpha, beta);
	}

	double bestVal = -SEARCH_INF;
	int legalMoves = 0;
	vector<Move> moves = board.getAllMovesForColor(toMove);

	for (Move & m : moves) {
		UndoInfo undo = board.makeSearchMove(m);
		if (board.isColorInCheck(toMove)) {
			board.unmakeSearchMove(undo);
			continue; // illegal: leaves own king in check
		}
		legalMoves++;

		double score = -negamax(board, oppositeColor(toMove), depth - 1, -beta, -alpha, ply + 1);
		board.unmakeSearchMove(undo);

		if (score > bestVal) {
			bestVal = score;
		}
		if (bestVal > alpha) {
			alpha = bestVal;
		}
		if (alpha >= beta) {
			break; // beta cutoff
		}
	}

	if (legalMoves == 0) {
		// No legal moves: checkmate (in check) is worst; stalemate is a draw.
		if (board.isColorInCheck(toMove)) {
			return -(MATE_SCORE - ply); // prefer mates that are further away when losing
		}
		return 0.0;
	}

	return bestVal;
}

// Root-level search with parallel move evaluation. Each legal candidate move is
// scored via negamax on its own deepCopy in a separate thread, then the highest
// scoring move (from the bot's own perspective) is chosen. Ties are broken
// randomly so the bot doesn't play deterministically.
Move Bot::getMinMaxMove(int depth) {
	vector<Move> sortedMoves = getAllMovesSorted();

	if (sortedMoves.empty()) {
		return getAllMoves()[0];
	}

	Piece::Color me = getColor();
	Piece::Color opp = oppositeColor(me);

	// score is nullopt when the candidate move is illegal (leaves own king in check).
	struct MoveResult {
		int index;
		optional<double> score;
	};

	vector<future<MoveResult>> futures;
	futures.reserve(sortedMoves.size());

	for (int i = 0; i < (int)sortedMoves.size(); i++) {
		Move m = sortedMoves[i];
		Board * boardPtr = this->getBoard();

		futures.push_back(std::async(std::launch::async, [boardPtr, m, depth, me, opp, i]() mutable -> MoveResult {
			Board copyBoard = boardPtr->deepCopy();
			UndoInfo undo = copyBoard.makeSearchMove(m);
			if (copyBoard.isColorInCheck(me)) {
				copyBoard.unmakeSearchMove(undo);
				return {i, nullopt}; // illegal
			}
			// Score from the bot's perspective: negate the opponent's negamax value.
			double score = -Bot::negamax(copyBoard, opp, depth - 1, -SEARCH_INF, SEARCH_INF, 1);
			copyBoard.unmakeSearchMove(undo);
			return {i, score};
		}));
	}

	double bestScore = -SEARCH_INF;
	vector<Move> bestMoves;

	for (auto & f : futures) {
		MoveResult result = f.get();
		if (!result.score) continue;

		double score = result.score.value();
		if (score > bestScore) {
			bestScore = score;
			bestMoves.clear();
			bestMoves.push_back(sortedMoves[result.index]);
		} else if (score == bestScore) {
			bestMoves.push_back(sortedMoves[result.index]);
		}
	}

	if (bestMoves.empty()) {
		return sortedMoves[0];
	}

	return bestMoves[rand() % bestMoves.size()];
}
