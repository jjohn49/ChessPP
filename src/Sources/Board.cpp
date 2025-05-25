//
// Created by hugh on 3/21/24.
//

#include "../Headers/Board.h"
#include "../Headers/Pawn.h"

#include <unordered_map>

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

    bishops = {
            make_shared<Bishop>(Bishop(0,2,Piece::White)),
            make_shared<Bishop>(Bishop(0,5,Piece::White)),
            make_shared<Bishop>(Bishop(7,2,Piece::Black)),
            make_shared<Bishop>(Bishop(7,5,Piece::Black)),
    };

    knights = {
            make_shared<Knight>(Knight(0,1,Piece::White)),
            make_shared<Knight>(Knight(0,6,Piece::White)),
            make_shared<Knight>(Knight(7,1,Piece::Black)),
            make_shared<Knight>(Knight(7,6,Piece::Black)),
    };

    queens = {
            make_shared<Queen>(Queen(Piece::White)),
            make_shared<Queen>(Queen(Piece::Black))
    };

    board = {
            {rooks[0], knights[0], bishops[0], queens[0], kings[0], bishops[1], knights[1], rooks[1]},
            {pawns[0], pawns[1], pawns[2], pawns[3], pawns[4], pawns[5], pawns[6], pawns[7]},
            {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
            {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
            {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
            {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
            {pawns[8], pawns[9], pawns[10], pawns[11], pawns[12], pawns[13], pawns[14], pawns[15]},
            {rooks[2], knights[2], bishops[2], queens[1], kings[1], bishops[3], knights[3], rooks[3]}
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

string Board::toString() {

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
    std::pair<int,int> kingsPosition = getColorsKingPosition(color);

    vector<Move> colorMoves{};
    for(auto & row: board){
        for(shared_ptr<Piece> & p: row){
            if(p != nullptr && p->getColor() == color){
                auto pMoves = p->getMoves(this);
                for(auto & m: pMoves){
                    if(m.getNewPosition() == kingsPosition){
                        return true;
                    }
                }
            }
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

bool Board::isPositionInBounds(pair<int, int> position) {
    return position.first >= 0 && position.first <= 7 && position.second >= 0 && position.second <=7;
}

shared_ptr<Piece> Board::addPiece(Piece::Type type, Piece::Color color, pair<int,int> position) {
    if(type==Piece::Queen){
        shared_ptr<Queen> queen = make_shared<Queen>(position, color);
        queens.push_back(queen);
        return queen;
    }else if(type==Piece::Knight){
        shared_ptr<Knight> knight = make_shared<Knight>(position,color);
        knights.push_back(knight);
        return knight;
    }else{
        shared_ptr<Rook> rook = make_shared<Rook>(position,color);
        rooks.push_back(rook);
        return rook;
    }
}

float Board::evaluate() {

    float totalEvaluation = 0;

    unordered_map<Piece::Type, int> pointsPerPiece = {
            {Piece::Pawn,1},
            {Piece::Knight,3},
            {Piece::Bishop,3},
            {Piece::Rook,5},
            {Piece::Queen,9},
            {Piece::King,20},
    };


    for(int x = 0; x < 8; x++){
        for(int y= 0; y < 8; y++){
            shared_ptr<Piece> cur = getPieceAt(x,y);
            if(cur != nullptr){
                if(cur->getColor()==Piece::Black){
                    totalEvaluation -= ((cur->getEvalBoard()[invertRow(x)][y] *0.1) + pointsPerPiece[cur->getType()]);
                }else{
                    totalEvaluation += ((cur->getEvalBoard()[x][y] *0.1) + pointsPerPiece[cur->getType()]);
                }
            }
        }
    }

    //cout << totalEvaluation << endl;

    return totalEvaluation;
}

int Board::invertRow(int row) {
    return 7 - row;
}

Board Board::deepCopy() {
    Board copyBoard = Board();

    copyBoard.board = {
            {nullptr, nullptr,nullptr, nullptr,nullptr, nullptr,nullptr, nullptr},
            {nullptr, nullptr,nullptr, nullptr,nullptr, nullptr,nullptr, nullptr},
            {nullptr, nullptr,nullptr, nullptr,nullptr, nullptr,nullptr, nullptr},
            {nullptr, nullptr,nullptr, nullptr,nullptr, nullptr,nullptr, nullptr},
            {nullptr, nullptr,nullptr, nullptr,nullptr, nullptr,nullptr, nullptr},
            {nullptr, nullptr,nullptr, nullptr,nullptr, nullptr,nullptr, nullptr},
            {nullptr, nullptr,nullptr, nullptr,nullptr, nullptr,nullptr, nullptr},
            {nullptr, nullptr,nullptr, nullptr,nullptr, nullptr,nullptr, nullptr}
    };

    copyBoard.pawns = {};
    copyBoard.rooks = {};
    copyBoard.knights = {};
    copyBoard.bishops = {};
    copyBoard.kings = {};
    copyBoard.queens = {};

    for(auto p: pawns){
        auto pPos = p->getPosition();
        shared_ptr<Pawn> copyPawn = nullptr;
        if(this->getPieceAt(pPos.first, pPos.second) == p) {
            copyPawn = std::make_shared<Pawn>(*p);
            copyBoard.pawns.push_back(copyPawn);
            copyBoard.setPieceAt(pPos, copyPawn);
        }
    }

    for(auto r: rooks){
        auto rPos = r->getPosition();
        shared_ptr<Rook> copyRook = nullptr;
        if(this->getPieceAt(rPos.first, rPos.second) == r) {
            copyRook = std::make_shared<Rook>(*r);
            copyBoard.rooks.push_back(copyRook);
            copyBoard.setPieceAt(rPos, copyRook);
        }
    }

    for(auto k: knights){
        auto kPos = k->getPosition();
        shared_ptr<Knight> copyKnight = nullptr;
        if(this->getPieceAt(kPos.first, kPos.second) == k) {
            copyKnight = std::make_shared<Knight>(*k);
            copyBoard.knights.push_back(copyKnight);
            copyBoard.setPieceAt(kPos, copyKnight);
        }
    }

    for(auto b: bishops){
        auto bPos = b->getPosition();
        if(this->getPieceAt(bPos.first,bPos.second) == b){
            shared_ptr<Bishop> copyBishop = make_shared<Bishop>(*b);
            copyBoard.bishops.push_back(copyBishop);
            copyBoard.setPieceAt(bPos.first,bPos.second,copyBishop);
        }
    }

    for(auto q: queens){
        auto qPos = q->getPosition();
        if(this->getPieceAt(qPos.first,qPos.second) == q){
            shared_ptr<Queen> copyQueen = make_shared<Queen>(*q);
            copyBoard.queens.push_back(copyQueen);
            copyBoard.setPieceAt(qPos.first,qPos.second,copyQueen);
        }
    }

    for(auto k: kings){
        auto kPos = k->getPosition();
        shared_ptr<King> copyKing = nullptr;
        if(this->getPieceAt(kPos.first, kPos.second) == k) {
            copyKing = std::make_shared<King>(*k);
            copyBoard.kings.push_back(copyKing);
            copyBoard.setPieceAt(kPos, copyKing);
        }
    }

    return copyBoard;
}







