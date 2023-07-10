//
// Created by John Johnston on 7/5/23.
//

#include "../Headers/MovesForPieces.h"
#include <array>



MovesForPieces::MovesForPieces(Board board){
    this->board = board;
}
vector<Move> MovesForPieces::getMovesFor(Piece * piece){
    cout << piece->name << endl;
    if(piece->name == "Pawn"){
       return this->getMovesForPawn(piece);
    }else if(piece->name == "Knight"){
        return this->getMovesForKnight(piece);
    }else if(piece->name == "King"){
        return this->getMovesForKing(piece);
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
    if(isValidMove(newMove)){
        pawnMoves.emplace_back(pawn, pawn->x, pawn->y, pawn->x, pawn->y + offset);
        newMove = Move(pawn, pawn->x, (pawn->y + (2 * offset)));
        if(!pawn->hasMoved  && isValidMove(newMove)){
            pawnMoves.emplace_back(pawn, pawn->x, pawn->y, pawn->x, pawn->y+(2 * offset));
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
        if(this->isValidMove(newMove) && board.getPieceAt((char)(pawn->x+lr), pawn->y + offset)){
            pawnMoves.emplace_back(pawn, pawn->x, pawn->y, pawn->x+lr, pawn->y + offset);
        }
    }
    return pawnMoves;
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
    if(board.isLocationValid(newPosition.first,newPosition.second)){
        //need to change this to add offset
        Piece * p = board.getPieceAt(newPosition.first,newPosition.second);
        Board newBoard = board.makeNewBoardWith(move);
        if(p == nullptr){
            return !isCheck(newBoard,move.getPiece()->color);
        }else if(!move.getPiece()->isSameColor(p)){
            //need to change this to add offset
            return !isCheck(newBoard,move.getPiece()->color);
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

bool MovesForPieces::isCheck(Board newBoard, char color) {

    //maybe add multi-threading here
    return false;
}
