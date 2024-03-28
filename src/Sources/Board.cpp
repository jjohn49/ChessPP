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

    rooks = {
            make_shared<Rook>(Rook(0,0,Piece::White)),
            make_shared<Rook>(Rook(0,7,Piece::White)),
            make_shared<Rook>(Rook(7,0,Piece::Black)),
            make_shared<Rook>(Rook(7,7,Piece::Black)),
    };

    board = {
            {rooks[0], nullptr, nullptr, nullptr, kings[0], nullptr, nullptr, rooks[1]},
            {pawns[0], pawns[1], pawns[2], pawns[3], pawns[4], pawns[5], pawns[6], pawns[7]},
            {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
            {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
            {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
            {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
            {pawns[8], pawns[9], pawns[10], pawns[11], pawns[12], pawns[13], pawns[14], pawns[15]},
            {rooks[2], nullptr, nullptr, nullptr, kings[1], nullptr, nullptr, rooks[3]}
    };

    moveHistory = {};
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

    if(move.getIsEnPessant()){
        setPieceAt(move.getOldPosition().first, move.getNewPosition().second, nullptr);
    }else if(move.getIsKingSideCastle()){
        shared_ptr<Piece> rook = getPieceAt(move.getOldPosition().first, 7);
        rook->setHasMoved(true);
        rook->setNewPosition(move.getOldPosition().first, 5);
        setPieceAt(move.getOldPosition().first, 5, rook);
        setPieceAt(move.getOldPosition().first, 7, nullptr);

    }else if(move.getIsQueenSideCastle()){
        shared_ptr<Piece> rook = getPieceAt(move.getOldPosition().first, 0);
        rook->setHasMoved(true);
        rook->setNewPosition(move.getOldPosition().first, 3);
        setPieceAt(move.getOldPosition().first, 3, rook);
        setPieceAt(move.getOldPosition().first, 0, nullptr);
    }else if(move.getIsPawnPromotion()){
        cout << "Pawn Needs to Be promoted: \n"
            << "Type the number for the corrsponding pieces\n"
               <<"(1) promote to Queen\n"
                 <<"(2) promote to Rook\n"
                 <<"(3) promote to Knight\n"
                ;
        int input;
        cin >> input;

        if(input == 1){
            //add a queen
        } else if (input == 2){
            rooks.push_back(make_shared<Rook>(Rook(move.getNewPosition(), move.getMovingPiece()->getColor())));
            setPieceAt(move.getNewPosition(), rooks.back());
        }else if (input == 3){
            // add knight
        }
    }
    moveHistory.push_back(move);

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

Move Board::getLastMove() {
    return moveHistory.back();
}

bool Board::isFirstMove() {
    return moveHistory.empty();
}

shared_ptr<Rook> Board::getRook(int val) {
    return rooks[val];
}

bool Board::isPositionInOppMoves(pair<int, int> position, Piece::Color oppColor) {
    for(Move & m: getAllMovesForColor(oppColor)){
        if(position == m.getNewPosition()){
            return true;
        }
    }

    return false;
}

bool Board::isPositionsInOppMoves(vector<pair<int, int>> positions, Piece::Color oppColor) {
    bool ret = false;

    for(Move & m: getAllMovesForColor(oppColor)){
        for(pair<int,int> & p: positions){
            if(p == m.getNewPosition()){
                return true;
            }
        }
    }
    return ret;
}






