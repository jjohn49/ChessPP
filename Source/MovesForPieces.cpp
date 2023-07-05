//
// Created by John Johnston on 7/5/23.
//

#include "../Headers/MovesForPieces.h"

MovesForPieces::MovesForPieces(Board board){
    this->board = board;
}
vector<Move> MovesForPieces::getMovesFor(Piece piece){
    if(piece.name == "Pawn"){
        this->getMovesForPawn(piece);
    }
}

vector<Move> MovesForPieces::getMovesForPawn(Piece pawn){
    vector<Move> regMoves = this->getRegularMovesForPawn(pawn);
    vector<Move> takeMoves = this->getTakeMovesForPawn(pawn);
    //appends take moves to reg moves
    regMoves.insert(regMoves.end(), takeMoves.begin(), takeMoves.end());
    return regMoves;
}

vector<Move> MovesForPieces::getRegularMovesForPawn(Piece pawn) {
    vector<Move> pawnMoves{};
    //need to change this to add offset
    if(!board.getPieceAt(pawn.y + 1, pawn.x)){
        //need to change this to add offset
        pawnMoves.emplace_back(pawn, pawn.x, pawn.y, pawn.x, pawn.y+1);
        //need to change this to add offset
        if(!pawn.hasMoved && !board.getPieceAt(pawn.y + 2, pawn.x)){
            //need to change this to add offset
            pawnMoves.emplace_back(pawn, pawn.x, pawn.y, pawn.x, pawn.y+2);
        }
    }
    return pawnMoves;
}

vector<Move> MovesForPieces::getTakeMovesForPawn(Piece pawn) {
    //if(board.getPieceAt(pawn.x))
}