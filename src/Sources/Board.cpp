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

    kings = {
            King(Piece::White),
            King(Piece::Black),
    };

    board = {
            {nullptr, nullptr, nullptr, make_shared<King>(kings[0]), nullptr, nullptr, nullptr, nullptr},
            {make_shared<Pawn>(pawns[0]), make_shared<Pawn>(pawns[1]), make_shared<Pawn>(pawns[2]), make_shared<Pawn>(pawns[3]), make_shared<Pawn>(pawns[4]), make_shared<Pawn>(pawns[5]), make_shared<Pawn>(pawns[6]), make_shared<Pawn>(pawns[7])},
            {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
            {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
            {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
            {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
            {make_shared<Pawn>(pawns[8]), make_shared<Pawn>(pawns[9]), make_shared<Pawn>(pawns[10]), make_shared<Pawn>(pawns[11]), make_shared<Pawn>(pawns[12]), make_shared<Pawn>(pawns[13]), make_shared<Pawn>(pawns[14]), make_shared<Pawn>(pawns[15])},
            {nullptr, nullptr, nullptr, make_shared<King>(kings[1]), nullptr, nullptr, nullptr, nullptr}
    };
}

shared_ptr<Piece> Board::getPieceAt(int row, int col) {
    return board[row][col];
}

shared_ptr<Piece> Board::getPieceAt(pair<int, int> position) {
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

void Board::movePiece(Move move) {
    int oldRow = move.getOldPosition().first;
    int oldCol = move.getOldPosition().second;

    int newRow = move.getNewPosition().first;
    int newCol = move.getNewPosition().second;

    board[newRow][newCol] = board[oldRow][oldCol];
    board[oldRow][oldCol].reset();

    board[newRow][newCol]->setNewPosition(newRow, newCol);

}

void Board::setPieceAt(int row, int col, std::shared_ptr<Piece> pieceDragging) {
    board[row][col] = pieceDragging;
}

void Board::setPieceAt(pair<int, int> position, std::shared_ptr<Piece> pieceDragging) {
    setPieceAt(position.first, position.second, pieceDragging);
}

std::pair<int,int> Board::getColorsKingPosition(Piece::Color color) {
    return (color == Piece::White)? kings[0].getPosition() : kings[1].getPosition();
}

std::vector<Move> Board::getAllMovesForColor(Piece::Color color) {
    vector<Move> colorMoves{};
    for(auto & row: board){
        for(shared_ptr<Piece> & p: row){
            if(p != nullptr && p->getColor() == color){
                auto pMoves = p->getMoves(this);
                colorMoves.insert(colorMoves.end(),pMoves.begin(),pMoves.end());
            }
        }
    }
    return colorMoves;
}

bool Board::isColorInCheck(Piece::Color color) {
    vector<Move> oppMoves = (color == Piece::White)? getAllMovesForColor(Piece::Black): getAllMovesForColor(Piece::White);
    std::pair<int,int> kingsPosition = getColorsKingPosition(color);

    for(Move & move : oppMoves){
        if(kingsPosition == move.getNewPosition()){
            return true;
        }
    }
    
    return false;
}






