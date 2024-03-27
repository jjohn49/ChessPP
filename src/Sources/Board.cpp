//
// Created by hugh on 3/21/24.
//

#include "../Headers/Board.h"
#include "../Headers/Pawn.h"

Board::Board() {
    pawns = {
            make_shared<Pawn>(Pawn(1,0,Piece::White)),
            make_shared<Pawn>(Pawn(1,1,Piece::White)),
            make_shared<Pawn>(Pawn(1,2,Piece::White)),
            make_shared<Pawn>(Pawn(1,3,Piece::White)),
            make_shared<Pawn>(Pawn(1,4,Piece::White)),
            make_shared<Pawn>(Pawn(1,5,Piece::White)),
            make_shared<Pawn>(Pawn(1,6,Piece::White)),
            make_shared<Pawn>(Pawn(1,7,Piece::White)),
            make_shared<Pawn>(Pawn(6,0,Piece::Black)),
            make_shared<Pawn>(Pawn(6,1,Piece::Black)),
            make_shared<Pawn>(Pawn(6,2,Piece::Black)),
            make_shared<Pawn>(Pawn(6,3,Piece::Black)),
            make_shared<Pawn>(Pawn(6,4,Piece::Black)),
            make_shared<Pawn>(Pawn(6,5,Piece::Black)),
            make_shared<Pawn>(Pawn(6,6,Piece::Black)),
            make_shared<Pawn>(Pawn(6,7,Piece::Black)),
    };

    kings = {
            make_shared<King>(King(Piece::White)),
            make_shared<King>(King(Piece::Black)),
    };

    board = {
            {nullptr, nullptr, nullptr, kings[0], nullptr, nullptr, nullptr, nullptr},
            {pawns[0], pawns[1], pawns[2], pawns[3], pawns[4], pawns[5], pawns[6], pawns[7]},
            {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
            {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
            {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
            {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
            {pawns[8], pawns[9], pawns[10], pawns[11], pawns[12], pawns[13], pawns[14], pawns[15]},
            {nullptr, nullptr, nullptr, kings[1], nullptr, nullptr, nullptr, nullptr}
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

void Board::movePiece(Move & move) {
    shared_ptr<Piece> pieceMoved = move.getMovingPiece();
    pieceMoved->setNewPosition(move.getNewPosition().first, move.getNewPosition().second);
    pieceMoved->setHasMoved(true);
    setPieceAt(move.getNewPosition(),pieceMoved);
}

void Board::setPieceAt(int row, int col, std::shared_ptr<Piece> pieceDragging) {
    board[row][col] = pieceDragging;
}

void Board::setPieceAt(pair<int, int> position, std::shared_ptr<Piece> pieceDragging) {
    setPieceAt(position.first, position.second, pieceDragging);
}

std::pair<int,int> Board::getColorsKingPosition(Piece::Color color) {
    return (color == Piece::White)? kings[0]->getPosition() : kings[1]->getPosition();
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






