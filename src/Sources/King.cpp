//
// Created by John Johnston on 3/26/24.
//

#include "../Headers/King.h"
#include "../Headers/Board.h"

King::King(): Piece() {}
King::King(Color color): Piece((color==White)? 0: 7, 3, color, Piece::Type::King) {}

vector<Move> King::getMoves(Board *board) {
    vector<Move> moves{};

    //TODO: Make All the Iterations of a King Move
    int vals[] = {-1,0,1};

    for(int x: vals){
        for(int y: vals){
            int newRow = row + x;
            int newCol = col + y;
            if((x==0 && y==0) || (newRow <0) || (newRow>7) || (newCol <0) || (newCol>7)){
                continue;
            }

            if(board->getPieceAt(newRow, newCol) == nullptr || board->getPieceAt(newRow, newCol)->getColor() != getColor())
            {
                moves.push_back(Move(getPosition(), make_pair(newRow, newCol), shared_from_this(), board->getPieceAt(newRow,newCol)));
            }
        }
    }

    auto castleMoves = getCastlingMoves(board);

    moves.insert(moves.end(),castleMoves.begin(), castleMoves.end());

    return moves;
}

vector<Move> King::getCastlingMoves(Board *board) {
    vector<Move> castleMoves{};

    int colorOffset = (getColor()==White)? 0:2;
    Color oppColor = (getColor()==White)? Black:White;

    if(!getHasMoved()){
        //King Side Rook
        if(!board->getRook(colorOffset)->getHasMoved()){

            vector<pair<int,int>> squares = {
                    make_pair(row,col),
                    make_pair(row,col -1),
                    make_pair(row,col - 2)
            };

            if(board->getPieceAt(squares[0])== nullptr && board->getPieceAt(squares[1])== nullptr && board->getPieceAt(squares[2])== nullptr && !board->isPositionsInOppMoves(squares,oppColor)){
                castleMoves.push_back(Move(getPosition(), squares[2], shared_from_this(), nullptr, false, false, true));
            }
        }

        //Queen Side Rook
        if(!board->getRook(colorOffset + 1)->getHasMoved()){

        }
    }


    return castleMoves;
}

string King::getImagePath() {
    return (getColor() == White)? "../assets/PNGs/No shadow/2x/w_king_2x_ns.png" : "../assets/PNGs/No shadow/2x/b_king_2x_ns.png";
}

