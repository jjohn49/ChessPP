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

    if(isValidMove(pawn, pawn->x, pawn->y + offset)){
        pawnMoves.emplace_back(pawn, pawn->x, pawn->y, pawn->x, pawn->y + offset);
        if(!pawn->hasMoved  && isValidMove(pawn, pawn->x, (pawn->y + (2 * offset)))){
            pawnMoves.emplace_back(pawn, pawn->x, pawn->y, pawn->x, pawn->y+(2 * offset));
        }
    }

    return pawnMoves;
}

vector<Move> MovesForPieces::getTakeMovesForPawn(Piece * pawn) {
    int offset = (pawn->color == 'w')? 1 : -1;
    vector<Move> pawnMoves{};
    array<int, 2> leftAndRight{-1,1};

    for(int &lr :leftAndRight){
        if(this->isValidMove(pawn, (char)(pawn->x+lr), pawn->y + offset) && board.getPieceAt((char)(pawn->x+lr), pawn->y + offset)){
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
        
        if(this->isValidMove(knight, position.first, position.second)){
            moves.emplace_back(knight, x,y, position.first, position.second);
        }
    }
    return moves;
}

bool MovesForPieces::isValidMove(Piece * piece, char x, int32_t y) {
    if(board.isLocationValid(x,y)){
        //need to change this to add offset
        Piece * p = board.getPieceAt(x,y);
        if(p == nullptr){
            return true;
        }else if(!piece->isSameColor(p)){
            //need to change this to add offset
            return true;
        }
    }
    return false;
}
