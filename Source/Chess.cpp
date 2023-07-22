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
    vector<Move> * moves = new vector<Move>();
    vector<Move> m;
    Piece * piece = new Piece();
    MovesForPieces mfp{&board};
    for(int row = 1; row <=8; row ++){
        for(int col = 'a'; col <= 'h'; col++){
            piece = board.getPieceAt((char)col,row);
            if(piece != nullptr && piece->color == color){
                mfp.getMovesFor(moves, piece);

            }

        }
    }
    delete piece;

    return *moves;
}

vector<Move> Chess::getAllMovesForColor(char color) {
    vector<Move> * moves = new vector<Move>();
    vector<Move> m;
    Piece * piece;
    for(int row = 1; row <=8; row ++){
        for(int col = 'a'; col <= 'h'; col++){
            piece = board.getPieceAt((char)col,row);
            if(piece != nullptr && piece->color == color){
                this->movesForPieces.getMovesFor(moves, piece);

            }

        }
    }

    return *moves;
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
    Piece * king{this->board.getKingForColor(color)};


    for(Move &m: opponentMoves){
        //cout << m.toString() << endl;
        if(m.contains(king->x, king->y)){
            return true;
        }
    }

    return false;
}

void Chess::movePiece(Move move) {
    Piece * pieceMoved{move.getPiece()};

    //Board newBoard{board.makeNewBoardWith(move)};
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
        board.movePiece(move, true);
        if(!this->isColorInCheck(color)){
            legalMoves.emplace_back(move);
        }
        board.revertMove(move);
    }
    this->checkForCastling(&legalMoves, color);

    return legalMoves;
}

void Chess::checkForCastling(vector<Move> *moves, char color) {
    Piece * king = this->board.getKingForColor(color);

    if(color == 'w'){
        if(Piece * rook{this->board.getPieceAt('a',1)}){
            this->castlingLogic(moves, king, rook);
        }

        if(Piece * rook{this->board.getPieceAt('h',1)}){
            this->castlingLogic(moves, king, rook);
        }

    }else{
        if(Piece * rook{this->board.getPieceAt('a',8)}){
            this->castlingLogic(moves, king, rook);
        }

        if(Piece * rook{this->board.getPieceAt('h',8)}){
            this->castlingLogic(moves, king, rook);
        }
    }
}

void Chess::castlingLogic(vector<Move> * moves, Piece * king, Piece * rook){

    if(!king->hasMoved || !rook->hasMoved){
        bool isQueenSide{king->x > rook->x};
        //check if anything is in the way of it the king ever goes into check in the process
        vector<Move> opponentsMove{this->getAllMovesForColor((king->color == 'w')? 'b':'w')};

        bool possible{true};
        for(Move &move: opponentsMove){
            int counter = king->x;
            while((isQueenSide && counter >= 'c') || (!isQueenSide && counter <= 'g')){
                if(move.contains((char)counter, king->y) || ((char)counter != 'e' && this->board.getPieceAt((char)counter, king->y) != nullptr)){
                    possible = false;
                    break;
                }

                if((char)counter == 'c' || (char)counter == 'g'){
                    break;
                }

                if(isQueenSide){
                    counter--;
                }else{
                    counter++;
                }
            }
        }

        if(possible && isQueenSide){
            moves->emplace_back(king, king->x, king->y, 'c', king->y, false, true);
            moves->emplace_back(rook, rook->x, rook->y, 'd', rook->y, false, true);
        }else if(possible){
            moves->emplace_back(king, king->x, king->y, 'g', king->y, false, true);
            moves->emplace_back(rook, rook->x, rook->y, 'f', king->y, false, true);
        }
    }
};

/*Chess::~Chess() {
    delete this->board;
}*/
