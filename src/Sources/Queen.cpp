//
// Created by John Johnston on 3/28/24.
//

#include "../Headers/Queen.h"
#include "../Headers/Board.h"



Queen::Queen():Piece() {}
Queen::Queen(Piece::Color color): Piece((color==White)? 0: 7, 3, color, Type::Queen) {
    this->evalBoard = {
            {-2 ,-1 ,-1 ,-0.5,-0.5,-1 ,-1 ,-2 },
            {-1 ,0  ,0.5,0  ,0  ,0  ,0 ,-1 },
            {-1 ,0.5,0.5,0.5,0.5,0.5,0 ,-1 },
            {0  ,0   ,0.5,0.5,0.5,0.5,0 ,-0.5},
            {-0.5 ,0,0.5 ,0.5 ,0.5  ,0.5,0,-0.5 },
            {-1 ,0  ,0.5 ,.5  ,.5  ,0.5,0 ,-1 },
            {-1 ,0 ,0  ,0 ,0 ,0  ,0 ,-1 },
            {-2 ,-1 ,-1 ,-0.5 ,-0.5 ,-1 ,-1 ,-2 },
    };
}
Queen::Queen(int row, int col, Piece::Color color): Piece(row,col,color,Type::Queen) {
    this->evalBoard = {
            {-2 ,-1 ,-1 ,-0.5,-0.5,-1 ,-1 ,-2 },
            {-1 ,0  ,0.5,0  ,0  ,0  ,0 ,-1 },
            {-1 ,0.5,0.5,0.5,0.5,0.5,0 ,-1 },
            {0  ,0   ,0.5,0.5,0.5,0.5,0 ,-0.5},
            {-0.5 ,0,0.5 ,0.5 ,0.5  ,0.5,0,-0.5 },
            {-1 ,0  ,0.5 ,.5  ,.5  ,0.5,0 ,-1 },
            {-1 ,0 ,0  ,0 ,0 ,0  ,0 ,-1 },
            {-2 ,-1 ,-1 ,-0.5 ,-0.5 ,-1 ,-1 ,-2 },
    };
}
Queen::Queen(pair<int, int> position, Piece::Color color): Queen(position.first, position.second, color) {}

vector<Move> Queen::getMoves(Board *board) {

    vector<Move> queenMoves{};

    int tempRow, tempCol;

    //Rook Moves Copied From Rook.cpp

    //top
    tempRow = row + 1;
    while(tempRow < 8){
        if(board->getPieceAt(tempRow,col) == nullptr){
            queenMoves.push_back(Move(getPosition(), make_pair(tempRow, col),shared_from_this(), nullptr));
        }else{
            if(board->getPieceAt(tempRow, col)->getColor() != getColor()){
                queenMoves.push_back(Move(getPosition(), make_pair(tempRow,col),shared_from_this(),board->getPieceAt(tempRow,col)));
            }
            break;
        }
        tempRow++;
    }

    //bottom
    tempRow = row - 1;
    while(tempRow >= 0){
        if(board->getPieceAt(tempRow,col) == nullptr){
            queenMoves.push_back(Move(getPosition(), make_pair(tempRow, col),shared_from_this(), nullptr));
        }else{
            if(board->getPieceAt(tempRow, col)->getColor() != getColor()){
                queenMoves.push_back(Move(getPosition(), make_pair(tempRow,col),shared_from_this(),board->getPieceAt(tempRow,col)));
            }
            break;
        }
        tempRow--;
    }

    //right
    tempCol = col + 1;
    while(tempCol < 8){
        if(board->getPieceAt(row,tempCol) == nullptr){
            queenMoves.push_back(Move(getPosition(), make_pair(row, tempCol),shared_from_this(), nullptr));
        }else{
            if(board->getPieceAt(row, tempCol)->getColor() != getColor()){
                queenMoves.push_back(Move(getPosition(), make_pair(row,tempCol),shared_from_this(),board->getPieceAt(row,tempCol)));
            }
            break;
        }
        tempCol++;
    }

    //left
    tempCol = col - 1;
    while(tempCol >= 0){
        if(board->getPieceAt(row,tempCol) == nullptr){
            queenMoves.push_back(Move(getPosition(), make_pair(row, tempCol),shared_from_this(), nullptr));
        }else{
            if(board->getPieceAt(row, tempCol)->getColor() != getColor()){
                queenMoves.push_back(Move(getPosition(), make_pair(row,tempCol),shared_from_this(),board->getPieceAt(row,tempCol)));
            }
            break;
        }
        tempCol--;
    }

    //Bishop Moves Copied From Bishop.cpp

    //bottom right
    tempRow = row - 1;
    tempCol = col + 1;
    while(tempRow >= 0 && tempCol<=7){
        if(board->getPieceAt(tempRow,tempCol) == nullptr){
            queenMoves.push_back(Move(getPosition(), make_pair(tempRow, tempCol),shared_from_this(), nullptr));
        }else{
            if(board->getPieceAt(tempRow, tempCol)->getColor() != getColor()){
                queenMoves.push_back(Move(getPosition(), make_pair(tempRow,tempCol),shared_from_this(),board->getPieceAt(tempRow,tempCol)));
            }
            break;
        }
        tempRow--;
        tempCol++;
    }

    //bottom left
    tempRow = row - 1;
    tempCol = col - 1;
    while(tempRow >= 0 && tempCol>= 0){
        if(board->getPieceAt(tempRow,tempCol) == nullptr){
            queenMoves.push_back(Move(getPosition(), make_pair(tempRow, tempCol),shared_from_this(), nullptr));
        }else{
            if(board->getPieceAt(tempRow, tempCol)->getColor() != getColor()){
                queenMoves.push_back(Move(getPosition(), make_pair(tempRow,tempCol),shared_from_this(),board->getPieceAt(tempRow,tempCol)));
            }
            break;
        }
        tempRow--;
        tempCol--;
    }

    //top right
    tempRow = row + 1;
    tempCol = col + 1;
    while(tempRow <= 7 && tempCol<=7){
        if(board->getPieceAt(tempRow,tempCol) == nullptr){
            queenMoves.push_back(Move(getPosition(), make_pair(tempRow, tempCol),shared_from_this(), nullptr));
        }else{
            if(board->getPieceAt(tempRow, tempCol)->getColor() != getColor()){
                queenMoves.push_back(Move(getPosition(), make_pair(tempRow,tempCol),shared_from_this(),board->getPieceAt(tempRow,tempCol)));
            }
            break;
        }
        tempRow++;
        tempCol++;
    }

    //top left
    tempRow = row + 1;
    tempCol = col - 1;
    while(tempRow <=7 && tempCol>= 0){
        if(board->getPieceAt(tempRow,tempCol) == nullptr){
            queenMoves.push_back(Move(getPosition(), make_pair(tempRow, tempCol),shared_from_this(), nullptr));
        }else{
            if(board->getPieceAt(tempRow, tempCol)->getColor() != getColor()){
                queenMoves.push_back(Move(getPosition(), make_pair(tempRow,tempCol),shared_from_this(),board->getPieceAt(tempRow,tempCol)));
            }
            break;
        }
        tempRow++;
        tempCol--;
    }


    return queenMoves;
}

string  Queen::getImagePath() {
    return (getColor()==White)? "../assets/PNGs/No shadow/2x/w_queen_2x_ns.png":"../assets/PNGs/No shadow/2x/b_queen_2x_ns.png";
}

vector<vector<float>> Queen::getEvalBoard() {
    return this->evalBoard;
}
