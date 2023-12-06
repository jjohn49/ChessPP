//
// Created by John Johnston on 12/4/23.
//

#include "../Header/Chess.h"

Chess::Chess() {

}

void Chess::printBoard() {
    for( auto & row: this->board){
        std::string rowStr = "";
        for(std::shared_ptr<Piece> & square: row){
            if(square != nullptr){
                rowStr += square->toString() + "\t";
            }else{
                rowStr+= "|\t\t\t|";
            }
        }
        std::cout<<rowStr<<"\n";
    }
}