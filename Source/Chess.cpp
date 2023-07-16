//
// Created by hugh on 7/12/23.
//

#include "../Headers/Chess.h"

Chess::Chess(){
    board = Board();
    movesForPieces = MovesForPieces(&board);
}

//for debugging purposes
Chess::Chess(Board &board) {
    this->board = board;
    movesForPieces = MovesForPieces(&board);
}

vector<Move> Chess::getAllMovesForColor(Board board, char color) {
    vector<Move> moves{};
    vector<Move> m;
    Piece * piece = new Piece();
    MovesForPieces mfp{&board};
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

bool Chess::isColorInCheck(Board * board, char color) {
    vector<Move> opponentMoves{(color == 'w')? this->getAllMovesForColor('b') : this->getAllMovesForColor('w')};
    Piece king{*board->getKingForColor(color)};


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
    Piece king{*board.getKingForColor(color)};


    for(Move &m: opponentMoves){
        cout << m.toString() << endl;
        if(m.contains(king.x, king.y)){
            return true;
        }
    }

    return false;
}

void Chess::movePiece(Move move) {
    Piece * pieceMoved{move.getPiece()};

    Board newBoard{board.makeNewBoardWith(move)};

    this->board.movePiece(move);


}

void Chess::colorMoveAPiece(char color) {
    this->board.printBoard();
    cout << "Valid Moves" << endl;
    int counter = 0;
    vector<Move> legalMoves{this->getAllLegalMovesFor(color)};
    for(Move &move: legalMoves){
        cout << "(" << counter++ << ") " << move.toString() << endl;
    }

    std::string input;
    cout << "Please pick a number to choose your move";
    cin>>input;

    Move moveChose{legalMoves.at(std::stoi(input))};
    this->movePiece(moveChose);
    if(moveChose.isMoveCastling()){
        this->movePiece(legalMoves.at(std::stoi(input) + 1));
    }

}

vector<Move> Chess::getAllLegalMovesFor(char color){
    vector<Move> legalMoves{};
    for(Move &move: this->getAllMovesForColor(color)){
        board.movePiece(move);
        if(!this->isColorInCheck(color)){
            legalMoves.emplace_back(move);
        }
    }
    this->checkForCastling(&legalMoves, color);

    return legalMoves;
}

void Chess::checkForCastling(vector<Move> *moves, char color) {
    Piece * king = this->board.getKingForColor(color);

    if(king->hasMoved){
        return;
    }

    Board newBoard{this->board};

    if(color == 'w'){
        if(board.getPieceAt('a', 1)!= nullptr && !board.getPieceAt('a', 1)->hasMoved && board.getPieceAt('b',1) == nullptr && board.getPieceAt('c', 1) == nullptr && board.getPieceAt('d',1)==
                                                                                                                                                                     nullptr){
            //check if anything is in the way of it the king ever goes into check in the process
            vector<Move> opponentsMove{this->getAllMovesForColor((color == 'w')? 'b':'w')};

            bool possible{true};
            for(Move &move: opponentsMove){
                if(move.contains((char)(king->x -1), king->y) || move.contains((char)(king->x -2), king->y) || move.contains(king->x, king->y)){
                    possible = false;
                    break;
                }
            }

            if(possible){
                moves->emplace_back(king, 'e', 1, 'c', 1, false, true);
                moves->emplace_back(this->board.getPieceAt('a',1), 'a', 1, 'd', 1, false, true);
            }
        }
/*
        if(board.getPieceAt('h', 1)!= nullptr && !board.getPieceAt('a', 1)->hasMoved){
            //check if anything is in the way of it the king ever goes into check in the process
        }
    }else{
        if(board.getPieceAt('a', 8)!= nullptr && !board.getPieceAt('a', 1)->hasMoved){
            //check if anything is in the way of it the king ever goes into check in the process
        }

        if(board.getPieceAt('h', 8)!= nullptr && !board.getPieceAt('a', 1)->hasMoved){
            //check if anything is in the way of it the king ever goes into check in the process
        }*/
    }

}

/*Chess::~Chess() {
    delete this->board;
}*/




