//
// Created by John Johnston on 3/28/24.
//

#include "../Headers/Knight.h"
#include "../Headers/Board.h"

Knight::Knight():Piece(){}
Knight::Knight(int row, int col, Color color): Piece(row,col,color,Type::Knight){
    this->evalBoard = {
            {-5 ,-4 ,-3 ,-3 ,-3 ,-3 ,-4 ,-5 },
            {-4 ,-2 ,0  ,.5 ,.5 ,0  ,-2 ,-4 },
            {-3 ,0.5,1  ,1.5,1.5,1  ,0.5,-3 },
            {-3 ,0  ,1.5,2  ,2  ,1.5,0  ,-3 },
            {-3 ,.5 ,1.5,2  ,2  ,1.5,.5 ,-3 },
            {-3 ,0  ,1  ,1.5,1.5,1  ,0  ,-3 },
            {-4 ,-2 ,0  ,0  ,0  ,0  ,-2 ,-4 },
            {-5 ,-4 ,-3 ,-3 ,-3 ,-3 ,-4 ,-5 },
    };
}


Knight::Knight(pair<int,int> position, Color color): Knight(position.first,position.second,color){}

vector<Move> Knight::getMoves(Board * board) {
    vector<Move> knightMoves{};
    vector<pair<int,int>> positions = {
            //up right
            make_pair(row+1,col+2),
            make_pair(row+2,col+1),

            //up left
            make_pair(row+1,col-2),
            make_pair(row+2,col-1),

            //down right
            make_pair(row-1,col+2),
            make_pair(row-2,col+1),

            //down left
            make_pair(row-1,col-2),
            make_pair(row-2,col-1),
    };

    for(pair<int,int> & p : positions){
        if(board->isPositionInBounds(p)){
            if(board->getPieceAt(p) == nullptr || board->getPieceAt(p)->getColor() != getColor()){
                knightMoves.push_back(Move(getPosition(), p, shared_from_this(), board->getPieceAt(p)));
            }
        }
    }

    return knightMoves;
}
string Knight::getImagePath() {
    return (getColor()==White)? "../assets/PNGs/No shadow/2x/w_knight_2x_ns.png":"../assets/PNGs/No shadow/2x/b_knight_2x_ns.png";
}

vector<vector<float>> Knight::getEvalBoard() {
    return this->evalBoard;
}