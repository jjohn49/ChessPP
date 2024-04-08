# ChessPP
Chess created using C++ and the SDL2 Game Library!

<img width="806" alt="Screenshot 2024-03-28 at 4 19 13 PM" src="https://github.com/jjohn49/ChessPP/assets/98774069/8eeff2b9-ed20-45f3-b1da-608c91eb3252">

# Installation

1. Download File from Release of Clone Repo
2. Make sur you have cmake installed
   - Can be found here: https://cmake.org/download/
3. In the Root Directory run `cmake .`
4. Enjoy!

# Bots and Other Tips

* In order to run bots, simply uncomment the other initializer in main.cpp.
* To change the difficulty from Random to Easy, change the Bot Difficulty Enum from Random to Easy in the Chess Constructor.
* To change the time duration, manually edit the timeLeft float variable, located in the constructor opf Player in Player.cpp.

## ChessPP follows all the rules of Chess
Examples:

### 1. Knights Jumping Pieces

 
<img width="1197" alt="Screenshot 2024-04-08 at 7 02 20 PM" src="https://github.com/jjohn49/ChessPP/assets/98774069/fe510834-8878-47b9-9c98-e5cb79532f7a">


### 2.En Pessant

<img width="1196" alt="Screenshot 2024-04-08 at 7 02 52 PM" src="https://github.com/jjohn49/ChessPP/assets/98774069/aeb8894a-ec19-4a16-b4be-a7e3664caaa4">
<img width="1198" alt="Screenshot 2024-04-08 at 7 03 15 PM" src="https://github.com/jjohn49/ChessPP/assets/98774069/e36cbbd0-a19b-4c62-a6ca-12ca4eb067ab">

### 3. Pawn Promotion

<img width="1199" alt="Screenshot 2024-04-08 at 7 04 13 PM" src="https://github.com/jjohn49/ChessPP/assets/98774069/7cf99901-066c-45ed-81a7-f8f9484e1cb4">
<img width="1202" alt="Screenshot 2024-04-08 at 7 04 26 PM" src="https://github.com/jjohn49/ChessPP/assets/98774069/2cd3a9c8-05a8-4451-8773-1bfe66e6d687">

### 4. Castling
<img width="1197" alt="Screenshot 2024-04-08 at 7 05 18 PM" src="https://github.com/jjohn49/ChessPP/assets/98774069/d2bf9aac-0508-4a1b-8e5e-f0c61df2d282">

<img width="1200" alt="Screenshot 2024-04-08 at 7 05 31 PM" src="https://github.com/jjohn49/ChessPP/assets/98774069/37fc6a99-83ae-487d-a170-63282d3fc7e2">


## Notes for V1.2:

* 1.~~The graphics can be a little bit finnacky, but it still works.~~ Major Graphic imporovements with new Release 1.2.
* 2.~~In order to promote a pawn, you need to check the console and input the piece you would like.~~ V1.2 Brings a simple UI so you no longer have too use the terminal!
* 3.~~The Game does not tell you when you are in check / check mate, however, it will not let you place a piece if you are checkmated or in check after said move.~~ Game will automatically end the game when a player is checkmated or runs out of time and displays who is the winner
* ~~If you play a move that gets you out of check, the game will respond normally.~~ No Longer necessary

