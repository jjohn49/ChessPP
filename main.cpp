#include <iostream>
#include "Headers/Piece.h"

int main() {

    Piece p{"piece", 'a', 1};
    std::cout << p.toString() << std::endl;
    return 0;
}
