//
// Created by hugh on 7/12/23.
//

#include "../Headers/Chess.h"

Chess::Chess(){
    this->board = Board();
    movesForPieces = MovesForPieces(board);
}

vector<Move> Chess::getAllMovesForColor(char color) {
    vector<Move> moves{};
    vector<Move> m;
    Piece * piece = new Piece();
    for(int row = 1; row <=8; row ++){
        for(int col = 'a'; col <= 'h'; col++){
            piece = board.getPieceAt((char)col,row);
            if(piece != nullptr && piece->color == color){
                vector<Move> m{this->movesForPieces.getMovesFor(piece)};
                moves.insert(moves.end(), m.begin(), m.end());
            }

        }
    }
    delete piece;

    return moves;
}
