//
// Created by John Johnston on 7/5/23.
//

#include "../Headers/MovesForPieces.h"
#include <array>


MovesForPieces::MovesForPieces() {
    board = nullptr;
}


MovesForPieces::MovesForPieces(Board * board){
    this->board = board;
}
void MovesForPieces::getMovesFor(vector<Move> & moves, Piece & piece){

    std::string name{piece.name};
    if(name == "Pawn"){
       this->getMovesForPawn(moves, piece);
    }else if(name == "Knight"){
        this->getMovesForKnight(moves, piece);
    }else if(name == "King"){
        this->getMovesForKing(moves, piece);
    }else if(name == "Bishop"){
        this->getMovesForBishop(moves, piece);
    }else if(name == "Rook"){
       this->getMovesForRook(moves, piece);
    }else if(name =="Queen"){
        this->getMovesForQueen(moves, piece);
    }

}

void MovesForPieces::getMovesForPawn(vector<Move> & moves, Piece & pawn){

    this->getRegularMovesForPawn(moves, pawn);
    this->getTakeMovesForPawn(moves, pawn);
}

void MovesForPieces::getRegularMovesForPawn(vector<Move> & pawnMoves, Piece & pawn) {
    int offset = (pawn.color == 'w')? 1 : -1;
    Move newMove{pawn, pawn.x, pawn.y+offset};

    if(board->getPieceAt(pawn.x, pawn.y + offset) == nullptr){
        pawnMoves.emplace_back(newMove);
        newMove = Move(pawn, pawn.x, (pawn.y + (2 * offset)));
        if(!pawn.hasMoved  && board->getPieceAt(pawn.x, pawn.y + (2* offset)) == nullptr){
            pawnMoves.emplace_back(newMove);
        }
    }
}

void MovesForPieces::getTakeMovesForPawn(vector<Move> & pawnMoves, Piece & pawn) {
    int offset = (pawn.color == 'w')? 1 : -1;

    array<int, 2> leftAndRight{-1,1};
    Move newMove;
    for(int &lr :leftAndRight){
        newMove = Move(pawn, (char)(pawn.x+lr), pawn.y + offset);
        if(this->isValidMove(pawnMoves, newMove)){
            //pawnMoves.emplace_back(pawn, pawn->x, pawn->y, pawn->x+lr, pawn->y + offset, false, false, true);
        }
    }
    if(board->getAllPreviousMoves().size() > 0){
        this->checkForEnPessant(pawnMoves, pawn);
    }

}

void MovesForPieces::checkForEnPessant(vector<Move> &move, Piece &pawn) {
    Move lastMove{board->getAllPreviousMoves().back()};
    Piece lastMovePiece{lastMove.getPiece()};
    if((pawn.color=='w' && pawn.y == 5)){


        if(lastMovePiece.getName() == "Pawn" && lastMove.oldPosition() == make_pair((char)(pawn.x + 1), pawn.y+2) && lastMove.newPosition() ==
                                                                                                                               make_pair((char)(pawn.x + 1), pawn.y)){
            move.emplace_back(pawn, pawn.x, pawn.y, (char)(pawn.x + 1), pawn.y + 1, true);
        }


        if(lastMovePiece.getName() == "Pawn" && lastMove.oldPosition() == make_pair((char)(pawn.x - 1), pawn.y+2) && lastMove.newPosition() ==
                                                                                                                       make_pair((char)(pawn.x - 1), pawn.y)){
            move.emplace_back(pawn, pawn.x, pawn.y, (char)(pawn.x - 1), pawn.y + 1, true);
        }
    }else if((pawn.color == 'b' && pawn.y == 4)){

        if(lastMovePiece.getName() == "Pawn" && lastMove.oldPosition() == make_pair((char)(pawn.x + 1), pawn.y-2) && lastMove.newPosition() ==
                                                                                                                       make_pair((char)(pawn.x + 1), pawn.y)){
            move.emplace_back(pawn, pawn.x, pawn.y, (char)(pawn.x + 1), pawn.y - 1, true);
        }


        if(lastMovePiece.getName() == "Pawn" && lastMove.oldPosition() == make_pair((char)(pawn.x - 1), pawn.y-2) && lastMove.newPosition() ==
                                                                                                                       make_pair((char)(pawn.x - 1), pawn.y)){
            move.emplace_back(pawn, pawn.x, pawn.y, (char)(pawn.x - 1), pawn.y - 1, true);
        }
    }
}

void MovesForPieces::getMovesForKnight(vector<Move> & moves, Piece & knight) {
    char x = knight.x;
    int32_t y = knight.y;
    array<pair<char, int32_t>, 8> positions{make_pair(x+1, y+2),
                                            make_pair(x+1, y-2),
                                            make_pair(x-1, y+2),
                                            make_pair(x-1,y-2),
                                            make_pair(x+2,y+1),
                                            make_pair(x+2,y-1),
                                            make_pair(x-2,y+1),
                                            make_pair(x-2,y-1)};

    for(pair<char, int32_t> &position:positions){
        Move newMove(knight, position.first, position.second);
        if(this->isValidMove(moves, newMove)){
            //moves.emplace_back(knight, x,y, position.first, position.second);
        }
    }
}

bool MovesForPieces::isValidMove(vector<Move> & moves, Move move) {
    pair<char, int32_t> newPosition{move.newPosition()};
    if(board->isLocationValid(newPosition.first,newPosition.second)){
        //need to change this to add offset
        Piece piece{move.getPiece()};
        Piece * p = board->getPieceAt(newPosition.first,newPosition.second);
        if(p == nullptr){
            moves.emplace_back(move);
            return true;
        }else if((!move.getPiece().isSameColor(p))){
            moves.emplace_back(piece, piece.x, piece.y, newPosition.first, newPosition.second, false, false, true);
            //board->getCapturedPieces().emplace_back(p);
            return true;
        }
    }
    return false;
}


void MovesForPieces::getMovesForKing(vector<Move> & moves, Piece & king) {
    char x{king.x};
    int32_t y{king.y};
    array<pair<char, int32_t>, 8> positions{
            make_pair((char)(x+1), y +1),
            make_pair((char)x, y +1),
            make_pair((char)(x-1), y +1),
            make_pair((char)(x+1), y),
            make_pair((char)(x-1), y),
            make_pair((char)(x+1), y -1),
            make_pair((char)x, y -1),
            make_pair((char)(x-1), y -1),
    };
    Move newMove;
    for(pair<char, int32_t> &position: positions){
        newMove = Move(king, position.first, position.second);
        if(this->isValidMove(moves, newMove)){
            //moves.emplace_back(king, x, y, position.first, position.second);
        }
    }
}

void MovesForPieces::getMovesForBishop(vector<Move> & moves, Piece &bishop) {
    //going to be used for both charges in x and y to find the diagnols

    this->bishopLogic(bishop, moves);

}

void MovesForPieces::bishopLogic(Piece & bishop, vector<Move> & moves){
    array<int, 2> charge{-1,1};

    for(int &chargeX : charge){
        for(int &chargeY : charge){
            Move firstMove{bishop, (char)(bishop.x + chargeX), bishop.y+chargeY};
            this->getConsecutiveMoves(moves, firstMove, true, true, chargeX, chargeY);
        }
    }

}

void MovesForPieces::getMovesForRook(vector<Move> & moves, Piece &rook) {
    this->rookLogic(rook, moves);

}

void MovesForPieces::rookLogic(Piece &rook, vector<Move> & moves) {
    array<int, 2> charged{-1,1};
    Move newMove;

    for(int &charge: charged){

        newMove = Move(rook, (char)(rook.x + charge), rook.y);
        getConsecutiveMoves(moves, newMove, false, true, charge, charge);
        newMove = Move(rook, rook.x, (rook.y + charge));
        getConsecutiveMoves(moves, newMove, true, false, charge, charge);
    }
}

void MovesForPieces::getConsecutiveMoves(vector<Move> & moves, Move newMove, bool vertical, bool horizontal, int chargeX, int chargeY){
    //cout << newMove.toString() << endl;
    if(!vertical && !horizontal){
        cout << "GET CONSECUTIVE MOVES METHOD IS DOING NOTHING. BOTH VERTICAL AND HORIZONTAL ARE FALSE" <<  endl;
        return;
    }
    pair<char, int> newPosition{newMove.newPosition()};

    if(!board->isLocationValid(newPosition.first, newPosition.second)){
        //cout << "Failed 1" << endl;
        return;
    }

    if(board->getPieceAt(newPosition.first, newPosition.second) == nullptr ){
        Move nextMove{this->addToMovesandGetNextMove(moves, newMove, horizontal, vertical, chargeX, chargeY)};
        this->getConsecutiveMoves(moves, nextMove, vertical, horizontal, chargeX, chargeY);
    }else if(!newMove.getPiece().isSameColor(board->getPieceAt(newPosition.first, newPosition.second))){
        this->isValidMove(moves, newMove);
    }
}

Move MovesForPieces::addToMovesandGetNextMove(vector<Move> & moves, Move newMove, bool horizontal, bool vertical, int chargeX, int chargeY){
    this->isValidMove(moves, newMove);
    pair<char, int> newPosition{newMove.newPosition()};
    char newX{newPosition.first};
    int newY{newPosition.second};

    if(horizontal){
        newX = (char)(newX + chargeX);
    }

    if(vertical){
        newY = newY + chargeY;
    }
    Move nextMove{newMove.getPiece(), newX, newY};
    return nextMove;
}

void MovesForPieces::getMovesForQueen(vector<Move> & moves, Piece & queen) {
    this->rookLogic(queen, moves);
    this->bishopLogic(queen, moves);

}











