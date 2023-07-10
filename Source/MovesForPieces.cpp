//
// Created by John Johnston on 7/5/23.
//

#include "../Headers/MovesForPieces.h"
#include <array>



MovesForPieces::MovesForPieces(Board board){
    this->board = board;
}
vector<Move> MovesForPieces::getMovesFor(Piece * piece){
    cout << piece->name << endl;
    if(piece->name == "Pawn"){
       return this->getMovesForPawn(piece);
    }else if(piece->name == "Knight"){
        return this->getMovesForKnight(piece);
    }else if(piece->name == "King"){
        return this->getMovesForKing(piece);
    }else if(piece->name == "Bishop"){
        return this->getMovesForBishop(piece);
    }

    return vector<Move>();
}

vector<Move> MovesForPieces::getMovesForPawn(Piece * pawn){
    vector<Move> regMoves = this->getRegularMovesForPawn(pawn);
    vector<Move> takeMoves = this->getTakeMovesForPawn(pawn);
    //appends take moves to reg moves
    regMoves.insert(regMoves.end(), takeMoves.begin(), takeMoves.end());
    return regMoves;
}

vector<Move> MovesForPieces::getRegularMovesForPawn(Piece * pawn) {
    vector<Move> pawnMoves{};
    int offset = (pawn->color == 'w')? 1 : -1;
    Move newMove{pawn, pawn->x, pawn->y+offset};
    if(isValidMove(newMove)){
        pawnMoves.emplace_back(pawn, pawn->x, pawn->y, pawn->x, pawn->y + offset);
        newMove = Move(pawn, pawn->x, (pawn->y + (2 * offset)));
        if(!pawn->hasMoved  && isValidMove(newMove)){
            pawnMoves.emplace_back(pawn, pawn->x, pawn->y, pawn->x, pawn->y+(2 * offset));
        }
    }

    return pawnMoves;
}

vector<Move> MovesForPieces::getTakeMovesForPawn(Piece * pawn) {
    int offset = (pawn->color == 'w')? 1 : -1;
    vector<Move> pawnMoves{};
    array<int, 2> leftAndRight{-1,1};
    Move newMove;
    for(int &lr :leftAndRight){
        newMove = Move(pawn, (char)(pawn->x+lr), pawn->y + offset);
        if(this->isValidMove(newMove) && board.getPieceAt((char)(pawn->x+lr), pawn->y + offset)){
            pawnMoves.emplace_back(pawn, pawn->x, pawn->y, pawn->x+lr, pawn->y + offset);
        }
    }
    return pawnMoves;
}

vector<Move> MovesForPieces::getMovesForKnight(Piece * knight) {
    vector<Move> moves{};
    char x = knight->x;
    int32_t y = knight->y;
    array<pair<char, int32_t>, 8> positions{make_pair(x+1, y+2),
                                            make_pair(x+1, y-2),
                                            make_pair(x-1, y+2),
                                            make_pair(x-1,y-2),
                                            make_pair(x+2,y+1),
                                            make_pair(x+2,y-1),
                                            make_pair(x-2,y+1),
                                            make_pair(x-2,y-1)};

    for(pair<char, int32_t> &position:positions){
        Move newMove(knight, position.first, position.second);
        if(this->isValidMove(newMove)){
            moves.emplace_back(knight, x,y, position.first, position.second);
        }
    }
    return moves;
}

bool MovesForPieces::isValidMove(Move move) {
    pair<char, int32_t> newPosition{move.newPosition()};
    if(board.isLocationValid(newPosition.first,newPosition.second)){
        //need to change this to add offset
        Piece * p = board.getPieceAt(newPosition.first,newPosition.second);
        Board newBoard = board.makeNewBoardWith(move);
        if(p == nullptr){
            return !isCheck(newBoard,move.getPiece()->color);
        }else if(!move.getPiece()->isSameColor(p)){
            //need to change this to add offset
            return !isCheck(newBoard,move.getPiece()->color);
        }
    }
    return false;
}

vector<Move> MovesForPieces::getMovesForKing(Piece *king) {
    vector<Move> moves{};
    char x{king->x};
    int32_t y{king->y};
    array<pair<char, int32_t>, 8> positions{
            make_pair((char)(x+1), y +1),
            make_pair((char)x, y +1),
            make_pair((char)(x-1), y +1),
            make_pair((char)(x+1), y),
            make_pair((char)(x-1), y),
            make_pair((char)(x+1), y -1),
            make_pair((char)x, y -1),
            make_pair((char)(x-1), y -1),
    };
    Move newMove;
    for(pair<char, int32_t> &position: positions){
        newMove = Move(king, position.first, position.second);
        if(this->isValidMove(newMove)){
            moves.emplace_back(king, x, y, position.first, position.second);
        }
    }

    return moves;
}

bool MovesForPieces::isCheck(Board newBoard, char color) {

    //maybe add multi-threading here
    return false;
}

vector<Move> MovesForPieces::getMovesForBishop(Piece *bishop) {
    //going to be used for both charges in x and y to find the diagnols
    array<int, 2> charged{-1,1};
    vector<Move> moves{};
    char x = bishop->x;
    int32_t y = bishop->y;
    int counter;
    Move newMove;
    for(int &chargeY : charged){

        for(int &chargeX : charged){
            counter = 1;
            newMove = Move(bishop, (char)(x + (chargeX * counter)), y + (chargeY * counter));

            while(this->isValidMove(newMove)){

                moves.emplace_back(bishop, (char)(x + (chargeX * counter)), y + (chargeY * counter));
                //edge case of when bishop meets opponent piece but not at end of the board
                if(board.getPieceAt((char)(x + (chargeX * counter)), y + (chargeY * counter)) && !bishop->isSameColor(board.getPieceAt((char)(x + (chargeX * counter)), y + (chargeY * counter)))){
                    break;
                }
                counter++;
                newMove = Move(bishop, (char)(x + (chargeX * counter)), y + (chargeY * counter));
            }
        }
    }
    return moves;
}

vector<Move> MovesForPieces::getMovesForRook(Piece *rook) {
    vector<Move> moves{};
    array<int, 2> charged{-1,1};
    Move newMove;
    int counter;

    for(int &charge: charged){

        newMove = Move(rook, (char)(rook->x + (counter * charge)), rook->y);
        char x{rook->x};
        int32_t y{rook->y};

        while(this->isValidMove(newMove)){

            moves.emplace_back(newMove);
            //edge case of when bishop meets opponent piece but not at end of the board
            pair<char, int32_t> position{newMove.newPosition()};
            if(board.getPieceAt(position.first, position.second ) && !rook->isSameColor(board.getPieceAt(position.first, position.second ))){
                break;
            }
            counter++;
            newMove = Move(rook, (char)(x + (charge * counter)), y );
        }

        counter = 1;
        newMove = Move(rook, rook->x, y + (counter * charge));


    }


    return moves;
}

vector<Move> MovesForPieces::validConsecutiveMoves(Move move, int charge) {
    vector<Move> moves{};
    Piece * piece{move.getPiece()};

    if(this->isValidMove(move)){

        moves.emplace_back(move);
        //edge case of when bishop meets opponent piece but not at end of the board
        pair<char, int32_t> position{move.newPosition()};
        if(board.getPieceAt(position.first, position.second ) && !piece->isSameColor(board.getPieceAt(position.first, position.second ))){
            return moves;
        }

        move = Move(piece, (char)(position.first + charge), piece->y );
        vector<Move> newMoves{validConsecutiveMoves(move, charge)};
        moves.insert(moves.end(), newMoves.begin(), newMoves.end());
    }


    return moves;
}
