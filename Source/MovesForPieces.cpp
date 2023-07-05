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
    int offset = (pawn.color == 'w')? 1 : -1;
    //need to change this to add offset
    if(board.isLocationValid(pawn.x,pawn.y + offset) && !board.getPieceAt(pawn.x , pawn.y + offset) ){
        //need to change this to add offset
        pawnMoves.emplace_back(pawn, pawn.x, pawn.y, pawn.x, pawn.y + offset);
        //need to change this to add offset
        if(!pawn.hasMoved && board.isLocationValid(pawn.x, pawn.y + (2 * offset)) && !board.getPieceAt(pawn.x, pawn.y + (2 * offset)) ){
            //need to change this to add offset
            pawnMoves.emplace_back(pawn, pawn.x, pawn.y, pawn.x, pawn.y+(2 * offset));
        }
    }
    return pawnMoves;
}

vector<Move> MovesForPieces::getTakeMovesForPawn(Piece pawn) {
    int offset = (pawn.color == 'w')? 1 : -1;
    vector<Move> pawnMoves{};
    array<int, 2> leftAndRight{-1,1};
    //need to change this to add offset
    for(int &lr :leftAndRight){
        if(board.isLocationValid((char)(pawn.x+lr),pawn.y + offset) && board.getPieceAt((char)(pawn.x+lr),pawn.y + offset)){
            //need to change this to add offset
            Piece p = board.getPieceAt((char)(pawn.x+lr),pawn.y + offset).value();
            if(!pawn.isSameColor(p)){
                //need to change this to add offset
                pawnMoves.emplace_back(pawn, pawn.x, pawn.y, pawn.x+lr, pawn.y + offset);
            }
        }
    }
    return pawnMoves;

}