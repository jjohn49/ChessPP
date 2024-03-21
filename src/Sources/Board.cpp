//
// Created by hugh on 3/21/24.
//

#include "../Headers/Board.h"
#include "../Headers/Pawn.h"

Board::Board() {
    pawns = {
            Pawn(1,0,Piece::White),Pawn(1,1,Piece::White),Pawn(1,2,Piece::White),Pawn(1,3,Piece::White),Pawn(1,4,Piece::White),Pawn(1,5,Piece::White),Pawn(1,6,Piece::White),Pawn(1,7,Piece::White),
            Pawn(6,0,Piece::Black),Pawn(6,1,Piece::Black),Pawn(6,2,Piece::Black),Pawn(6,3,Piece::Black),Pawn(6,4,Piece::Black),Pawn(6,5,Piece::Black),Pawn(6,6,Piece::Black),Pawn(6,7,Piece::Black),
    };

    board = {
            {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
            {make_shared<Pawn>(pawns[0]), make_shared<Pawn>(pawns[1]), make_shared<Pawn>(pawns[2]), make_shared<Pawn>(pawns[3]), make_shared<Pawn>(pawns[4]), make_shared<Pawn>(pawns[5]), make_shared<Pawn>(pawns[6]), make_shared<Pawn>(pawns[7])},
            {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
            {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
            {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
            {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
            {make_shared<Pawn>(pawns[8]), make_shared<Pawn>(pawns[9]), make_shared<Pawn>(pawns[10]), make_shared<Pawn>(pawns[11]), make_shared<Pawn>(pawns[12]), make_shared<Pawn>(pawns[13]), make_shared<Pawn>(pawns[14]), make_shared<Pawn>(pawns[15])},
            {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}
    };
}

Piece *Board::getPieceAt(int row, int col) {
    return board[row][col].get();
}

Piece *Board::getPieceAt(pair<int, int> position){
    return getPieceAt(position.first, position.second);
}

void Board::print() {

    for(int r = 0; r < 8; r++){
        for(int c = 0; c < 8; c++){
            if(this->getPieceAt(r,c)){
                cout << "x";
            }else{
                cout<<"_";
            }
        }
        cout << endl;
    }

}


