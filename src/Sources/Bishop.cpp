//
// Created by John Johnston on 3/28/24.
//

#include "../Headers/Bishop.h"
#include "../Headers/Board.h"

Bishop::Bishop(int row, int col, Piece::Color color): Piece(row,col,color,Type::Bishop) {}

Bishop::Bishop(pair<int, int> position, Piece::Color color): Bishop(position.first, position.second, color) {}

Bishop::Bishop():Piece() {}

vector<Move> Bishop::getMoves(Board *board) {
    vector<Move> bishopMoves{};
    int tempRow, tempCol;

    //bottom right
    tempRow = row - 1;
    tempCol = col + 1;
    while(tempRow >= 0 && tempCol<=7){
        if(board->getPieceAt(tempRow,tempCol) == nullptr){
            bishopMoves.push_back(Move(getPosition(), make_pair(tempRow, tempCol),shared_from_this(), nullptr));
        }else{
            if(board->getPieceAt(tempRow, tempCol)->getColor() != getColor()){
                bishopMoves.push_back(Move(getPosition(), make_pair(tempRow,tempCol),shared_from_this(),board->getPieceAt(tempRow,tempCol)));
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
            bishopMoves.push_back(Move(getPosition(), make_pair(tempRow, tempCol),shared_from_this(), nullptr));
        }else{
            if(board->getPieceAt(tempRow, tempCol)->getColor() != getColor()){
                bishopMoves.push_back(Move(getPosition(), make_pair(tempRow,tempCol),shared_from_this(),board->getPieceAt(tempRow,tempCol)));
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
            bishopMoves.push_back(Move(getPosition(), make_pair(tempRow, tempCol),shared_from_this(), nullptr));
        }else{
            if(board->getPieceAt(tempRow, tempCol)->getColor() != getColor()){
                bishopMoves.push_back(Move(getPosition(), make_pair(tempRow,tempCol),shared_from_this(),board->getPieceAt(tempRow,tempCol)));
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
            bishopMoves.push_back(Move(getPosition(), make_pair(tempRow, tempCol),shared_from_this(), nullptr));
        }else{
            if(board->getPieceAt(tempRow, tempCol)->getColor() != getColor()){
                bishopMoves.push_back(Move(getPosition(), make_pair(tempRow,tempCol),shared_from_this(),board->getPieceAt(tempRow,tempCol)));
            }
            break;
        }
        tempRow++;
        tempCol--;
    }

    return bishopMoves;
}

string Bishop::getImagePath() {
    return (getColor() == White)? "../assets/PNGs/No shadow/2x/w_bishop_2x_ns.png":"../assets/PNGs/No shadow/2x/b_bishop_2x_ns.png";
}
