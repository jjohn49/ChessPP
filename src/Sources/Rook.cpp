//
// Created by John Johnston on 3/28/24.
//

#include "../Headers/Rook.h"
#include "../Headers/Board.h"

Rook::Rook(int row, int col, Piece::Color color): Piece(row,col,color,Type::Rook){
    this->evalBoard = {
            {0  ,0  ,0  ,0.5,0.5,0  ,0  ,0  },
            {-0.5,0 ,0 ,0 ,0  ,0  ,0   ,-0.5},
            {-0.5,0 ,0 ,0 ,0  ,0  ,0   ,-0.5},
            {-0.5,0 ,0 ,0 ,0  ,0  ,0   ,-0.5},
            {-0.5,0 ,0 ,0 ,0  ,0  ,0   ,-0.5},
            {-0.5,0 ,0 ,0 ,0  ,0  ,0   ,-0.5},
            {.5 ,1  ,1  ,1  ,1  ,1  ,1  ,0.5},
            {0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  },
    };
}

Rook::Rook(std::pair<int, int> position, Piece::Color color): Rook(position.first, position.second,color) {}

Rook::Rook():Piece() {}

vector<Move> Rook::getMoves(Board * board){
    vector<Move> rookMoves{};

    int tempRow = row + 1;
    while(tempRow < 8){
        if(board->getPieceAt(tempRow,col) == nullptr){
            rookMoves.push_back(Move(getPosition(), make_pair(tempRow, col),shared_from_this(), nullptr));
        }else{
            if(board->getPieceAt(tempRow, col)->getColor() != getColor()){
                rookMoves.push_back(Move(getPosition(), make_pair(tempRow,col),shared_from_this(),board->getPieceAt(tempRow,col)));
            }
            break;
        }
        tempRow++;
    }

    tempRow = row - 1;
    while(tempRow >= 0){
        if(board->getPieceAt(tempRow,col) == nullptr){
            rookMoves.push_back(Move(getPosition(), make_pair(tempRow, col),shared_from_this(), nullptr));
        }else{
            if(board->getPieceAt(tempRow, col)->getColor() != getColor()){
                rookMoves.push_back(Move(getPosition(), make_pair(tempRow,col),shared_from_this(),board->getPieceAt(tempRow,col)));
            }
            break;
        }
        tempRow--;
    }

    int tempCol = col + 1;
    while(tempCol < 8){
        if(board->getPieceAt(row,tempCol) == nullptr){
            rookMoves.push_back(Move(getPosition(), make_pair(row, tempCol),shared_from_this(), nullptr));
        }else{
            if(board->getPieceAt(row, tempCol)->getColor() != getColor()){
                rookMoves.push_back(Move(getPosition(), make_pair(row,tempCol),shared_from_this(),board->getPieceAt(row,tempCol)));
            }
            break;
        }
        tempCol++;
    }

    tempCol = col - 1;
    while(tempCol >= 0){
        if(board->getPieceAt(row,tempCol) == nullptr){
            rookMoves.push_back(Move(getPosition(), make_pair(row, tempCol),shared_from_this(), nullptr));
        }else{
            if(board->getPieceAt(row, tempCol)->getColor() != getColor()){
                rookMoves.push_back(Move(getPosition(), make_pair(row,tempCol),shared_from_this(),board->getPieceAt(row,tempCol)));
            }
            break;
        }
        tempCol--;
    }

    return rookMoves;
}

string Rook::getImagePath() {
    return (getColor()==White)? "../assets/PNGs/No shadow/2x/w_rook_2x_ns.png" : "../assets/PNGs/No shadow/2x/b_rook_2x_ns.png";
}

vector<vector<float>> Rook::getEvalBoard() {
    return this->evalBoard;
}


