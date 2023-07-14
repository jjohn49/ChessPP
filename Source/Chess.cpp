//
// Created by hugh on 7/12/23.
//

#include "../Headers/Chess.h"

Chess::Chess(){
    this->board = Board();
    movesForPieces = MovesForPieces(board);
}

//for debugging purposes
Chess::Chess(Board &board) {
    this->board = board;
    movesForPieces = MovesForPieces(board);
}

vector<Move> Chess::getAllMovesForColor(Board &board, char color) {
    vector<Move> moves{};
    vector<Move> m;
    Piece * piece = new Piece();
    MovesForPieces mfp{board};
    for(int row = 1; row <=8; row ++){
        for(int col = 'a'; col <= 'h'; col++){
            piece = board.getPieceAt((char)col,row);
            if(piece != nullptr && piece->color == color){
                vector<Move> m{mfp.getMovesFor(piece)};
                moves.insert(moves.end(), m.begin(), m.end());
            }

        }
    }
    delete piece;

    return moves;
}

vector<Move> Chess::getAllMovesForColor(char color) {
    vector<Move> moves{};
    vector<Move> m;
    Piece * piece;
    for(int row = 1; row <=8; row ++){
        for(int col = 'a'; col <= 'h'; col++){
            piece = board.getPieceAt((char)col,row);
            if(piece != nullptr && piece->color == color){
                vector<Move> m{this->movesForPieces.getMovesFor(piece)};
                moves.insert(moves.end(), m.begin(), m.end());
            }

        }
    }

    return moves;
}

bool Chess::isColorInCheck(Board &board, char color) {
    vector<Move> opponentMoves{(color == 'w')? this->getAllMovesForColor('b') : this->getAllMovesForColor('w')};
    Piece king{board.getKingForColor(color)};


    for(Move &m: opponentMoves){
        //cout << m.toString() << endl;
        if(m.contains(king.x, king.y)){
            return true;
        }
    }

    return false;
}

bool Chess::isColorInCheck(char color) {
    vector<Move> opponentMoves{(color == 'w')? this->getAllMovesForColor('b') : this->getAllMovesForColor('w')};
    Piece king{board.getKingForColor(color)};


    for(Move &m: opponentMoves){
        cout << m.toString() << endl;
        if(m.contains(king.x, king.y)){
            return true;
        }
    }

    return false;
}

bool Chess::tryToMovePiece(Move move) {
    Piece * pieceMoved{move.getPiece()};
    vector<Move> allLegalMoves{this->getAllMovesForColor(pieceMoved->color)};

    bool isInLegalMoves{false};
    for(Move &legalMove: allLegalMoves){
        if(move == legalMove ){}
        isInLegalMoves = true;
    }

    if(isInLegalMoves){
        Board newBoard{board.makeNewBoardWith(move)};
        if(!this->isColorInCheck(newBoard,pieceMoved->color)){
            this->board.movePieceLogic(move);
            return true;
        }
    }

    return false;
}




