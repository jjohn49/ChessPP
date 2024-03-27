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
    
    return moves;
}

string King::getImagePath() {
    return (getColor() == White)? "../assets/PNGs/No shadow/2x/w_king_2x_ns.png" : "../assets/PNGs/No shadow/2x/b_king_2x_ns.png";
}

