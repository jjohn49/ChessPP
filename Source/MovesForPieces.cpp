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
vector<Move> MovesForPieces::getMovesFor(Piece * piece){

    std::string name{piece->name};
    if(name == "Pawn"){
       return this->getMovesForPawn(piece);
    }else if(name == "Knight"){
        return this->getMovesForKnight(piece);
    }else if(name == "King"){
        return this->getMovesForKing(piece);
    }else if(name == "Bishop"){
        return this->getMovesForBishop(piece);
    }else if(name == "Rook"){
       return this->getMovesForRook(piece);
    }else if(name =="Queen"){
        return this->getMovesForQueen(piece);
    }

    return vector<Move>();
}

vector<Move> MovesForPieces::getMovesForPawn(Piece * pawn){
    vector<Move> regMoves = this->getRegularMovesForPawn(pawn);
    vector<Move> takeMoves = this->getTakeMovesForPawn(pawn);
    //appends take moves to reg moves
    regMoves.insert(regMoves.end(), takeMoves.begin(), takeMoves.end());
    return regMoves;
}

vector<Move> MovesForPieces::getRegularMovesForPawn(Piece * pawn) {
    vector<Move> pawnMoves{};
    int offset = (pawn->color == 'w')? 1 : -1;
    Move newMove{pawn, pawn->x, pawn->y+offset};

    if(board->getPieceAt(pawn->x, pawn->y + offset) == nullptr){
        pawnMoves.emplace_back(newMove);
        newMove = Move(pawn, pawn->x, (pawn->y + (2 * offset)));
        if(!pawn->hasMoved  && board->getPieceAt(pawn->x, pawn->y + (2* offset)) == nullptr){
            pawnMoves.emplace_back(newMove);
        }
    }

    return pawnMoves;
}

vector<Move> MovesForPieces::getTakeMovesForPawn(Piece * pawn) {
    int offset = (pawn->color == 'w')? 1 : -1;
    vector<Move> pawnMoves{};
    array<int, 2> leftAndRight{-1,1};
    Move newMove;
    for(int &lr :leftAndRight){
        newMove = Move(pawn, (char)(pawn->x+lr), pawn->y + offset);
        if(this->isValidMove(newMove) && board->getPieceAt((char)(pawn->x+lr), pawn->y + offset)){
            pawnMoves.emplace_back(pawn, pawn->x, pawn->y, pawn->x+lr, pawn->y + offset);
        }
    }
    if(board->getAllPreviousMoves().size() > 0){
        this->checkForEnPessant(&pawnMoves, pawn);
    }

    return pawnMoves;
}

void MovesForPieces::checkForEnPessant(vector<Move> *move, Piece *pawn) {
    Move lastMove{board->getAllPreviousMoves().back()};
    Piece lastMovePiece{*lastMove.getPiece()};
    if((pawn->color=='w' && pawn->y == 5)){


        if(lastMovePiece.getName() == "Pawn" && lastMove.oldPosition() == make_pair((char)(pawn->x + 1), pawn->y+2) && lastMove.newPosition() ==
                                                                                                                               make_pair((char)(pawn->x + 1), pawn->y)){
            move->emplace_back(pawn, pawn->x, pawn->y, (char)(pawn->x + 1), pawn->y + 1, true);
        }


        if(lastMovePiece.getName() == "Pawn" && lastMove.oldPosition() == make_pair((char)(pawn->x - 1), pawn->y+2) && lastMove.newPosition() ==
                                                                                                                       make_pair((char)(pawn->x - 1), pawn->y)){
            move->emplace_back(pawn, pawn->x, pawn->y, (char)(pawn->x - 1), pawn->y + 1, true);
        }
    }else if((pawn->color == 'b' && pawn->y == 4)){

        if(lastMovePiece.getName() == "Pawn" && lastMove.oldPosition() == make_pair((char)(pawn->x + 1), pawn->y-2) && lastMove.newPosition() ==
                                                                                                                       make_pair((char)(pawn->x + 1), pawn->y)){
            move->emplace_back(pawn, pawn->x, pawn->y, (char)(pawn->x + 1), pawn->y - 1, true);
        }


        if(lastMovePiece.getName() == "Pawn" && lastMove.oldPosition() == make_pair((char)(pawn->x - 1), pawn->y-2) && lastMove.newPosition() ==
                                                                                                                       make_pair((char)(pawn->x - 1), pawn->y)){
            move->emplace_back(pawn, pawn->x, pawn->y, (char)(pawn->x - 1), pawn->y - 1, true);
        }
    }
}

vector<Move> MovesForPieces::getMovesForKnight(Piece * knight) {
    vector<Move> moves{};
    char x = knight->x;
    int32_t y = knight->y;
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
        if(this->isValidMove(newMove)){
            moves.emplace_back(knight, x,y, position.first, position.second);
        }
    }
    return moves;
}

bool MovesForPieces::isValidMove(Move move) {
    pair<char, int32_t> newPosition{move.newPosition()};
    if(board->isLocationValid(newPosition.first,newPosition.second)){
        //need to change this to add offset
        Piece * p = board->getPieceAt(newPosition.first,newPosition.second);
        if(p == nullptr || (!move.getPiece()->isSameColor(p))){
            return true;
        }
    }
    return false;
}


vector<Move> MovesForPieces::getMovesForKing(Piece *king) {
    vector<Move> moves{};
    char x{king->x};
    int32_t y{king->y};
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
        if(this->isValidMove(newMove)){
            moves.emplace_back(king, x, y, position.first, position.second);
        }
    }

    return moves;
}

vector<Move> MovesForPieces::getMovesForBishop(Piece *bishop) {
    //going to be used for both charges in x and y to find the diagnols
    vector<Move> m{};
    this->bishopLogic(bishop, &m);
    return m;
}

void MovesForPieces::bishopLogic(Piece * bishop, vector<Move> * moves){
    array<int, 2> charge{-1,1};

    for(int &chargeX : charge){
        for(int &chargeY : charge){
            Move firstMove{bishop, (char)(bishop->x + chargeX), bishop->y+chargeY};
            this->getConsecutiveMoves(moves, firstMove, true, true, chargeX, chargeY);
        }
    }

}

vector<Move> MovesForPieces::getMovesForRook(Piece *rook) {
    vector<Move> m{};
    this->rookLogic(rook, &m);

    return m;
}

void MovesForPieces::rookLogic(Piece *rook, vector<Move> * moves) {
    array<int, 2> charged{-1,1};
    Move newMove;

    for(int &charge: charged){

        newMove = Move(rook, (char)(rook->x + charge), rook->y);
        getConsecutiveMoves(moves, newMove, false, true, charge, charge);
        newMove = Move(rook, rook->x, (rook->y + charge));
        getConsecutiveMoves(moves, newMove, true, false, charge, charge);
    }
}

void MovesForPieces::getConsecutiveMoves(vector<Move> * moves, Move newMove, bool vertical, bool horizontal, int chargeX, int chargeY){
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
    }else if(!newMove.getPiece()->isSameColor(board->getPieceAt(newPosition.first, newPosition.second))){
        moves->emplace_back(newMove);
    }
}

Move MovesForPieces::addToMovesandGetNextMove(vector<Move> * moves, Move newMove, bool horizontal, bool vertical, int chargeX, int chargeY){
    moves->emplace_back(newMove);
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

vector<Move> MovesForPieces::getMovesForQueen(Piece *queen) {
    vector<Move> m{};
    this->rookLogic(queen, &m);
    this->bishopLogic(queen, &m);
    //cout << m.at(0).toString() << endl;
    return m;
}









