//
// Created by John Johnston on 3/21/24.
//

#include "../Headers/Pawn.h"
#include "../Headers/Board.h"

Pawn::Pawn(int row, int col, Piece::Color color): Piece(row, col, color, Piece::Type::Pawn) {
    this->evalBoard = {
            {0,0,0,0,0,0,0,0},
            {0.5,1,1,-2,-2,1,1,0.5},
            {0.5,-0.5,-1,0,0,-1,-0.5,0.5},
            {0,0,0,2,2,0,0,0},
            {0.5,0.5,1,2.5,2.5,1,0.5,0.5},
            {1,1,2,2,3,2,2,1,1},
            {5,5,5,5,5,5,5,5},
            {0,0,0,0,0,0,0,0}
    };
};

vector<Move> Pawn::getMoves(Board *board) {
    int colorDirection = (this->getColor() == White)? 1: -1;
    vector<Move> pawnMoves{};

    //Gets the non-take moves, including the double hop at the start
    if(board->getPieceAt(this->row + (1*colorDirection), this->col) == nullptr){
        Move temp = Move(this->row,this->col, this->row + (1 * colorDirection), this->col, shared_from_this());

        //Only Promoted pawns can go on these rows
        if(temp.getNewPosition().first == 0 || temp.getNewPosition().first == 7){
            temp.setIsPawnPromotion(true);
        }
        pawnMoves.push_back(temp);

        if(!getHasMoved() && board->getPieceAt(this->row + (2*colorDirection), this->col) == nullptr){
            pawnMoves.push_back(Move(this->row,this->col, this->row + (2 * colorDirection), this->col, shared_from_this()));
        }
    }

    //Gets the take moves
    if(col > 0 && (0 < row && row < 7)){
        if(board->getPieceAt(row + (1*colorDirection), col - 1) != nullptr && board->getPieceAt(row + (1*colorDirection), col - 1)->getColor() != getColor()){
            Move temp = Move(row,col,row + (1*colorDirection),col-1,shared_from_this(), board->getPieceAt(row + (1*colorDirection), col - 1));
            if(temp.getNewPosition().first == 0 || temp.getNewPosition().first == 7){
                temp.setIsPawnPromotion(true);
            }
            pawnMoves.push_back(temp);
        }
    }
    if(col < 7&& (0 < row && row < 7)){
        if(board->getPieceAt(row + (1*colorDirection), col + 1)!= nullptr && board->getPieceAt(row + (1*colorDirection), col + 1)->getColor() != getColor()){
            Move temp = Move(row,col, row + (1*colorDirection), col + 1, shared_from_this(), board->getPieceAt(row + (1*colorDirection), col + 1));
            if(temp.getNewPosition().first == 0 || temp.getNewPosition().first == 7){
                temp.setIsPawnPromotion(true);
            }
            pawnMoves.push_back(temp);
        }
    }

    if(!board->isFirstMove() && getEnPessantMove(board)){
        pawnMoves.push_back(getEnPessantMove(board).value());
    }

    return pawnMoves;
}

string Pawn::getImagePath() {
    return (this->getColor() == White)? "../assets/PNGs/No shadow/2x/w_pawn_2x_ns.png" : "../assets/PNGs/No shadow/2x/b_pawn_2x_ns.png";
}

optional<Move> Pawn::getEnPessantMove(Board *board) {

    int colorDirection = (getColor()==Piece::Color::White)? 1:-1;

    Move lastMove = board->getLastMove();

    if(lastMove.getMovingPiece()->getType() == Type::Pawn && abs(lastMove.getOldPosition().first - lastMove.getNewPosition().first) == 2 && lastMove.getMovingPiece()->getColor() != getColor()){
        //something
        if(lastMove.getNewPosition().first == getPosition().first){
            int colDiff = getPosition().second - lastMove.getNewPosition().second;
            return Move(getPosition(), make_pair(row + colorDirection, col - colDiff), shared_from_this(), lastMove.getMovingPiece(),
                        true);
        }
    }

    return nullopt;
}

vector<vector<float>> Pawn::getEvalBoard() {
    return this->evalBoard;
}
