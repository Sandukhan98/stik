
======== SIMPLE COMPILATION ========
=> make
=> ./install/server -n [size of board (max 11)] (-o optional for swap mode) ./install/[first player].so ./install/[second player].so

======== TESTS ========
to launch all tests :
    => make test

to launch test of list_players :
    => make test_list_players

to launch test of elementary board :
    => make test_bitboard

to launch test of game board :
    => make test_board_game

to launch test of options struct :
    => make test_options

to test players :
    => make test_players

======== COMPILE WITH DISPLAY MODE ======
=> make display
open an other terminal and open display folder and launch the following command to start a localhost server:
=> racket server.rkt
you can no more use this terminal you must go back to last open
start the game with the same command :
=> ./install/server -n [size of board (max 11)] (-o optional for swap mode) ./install/[first player].so ./install/[second player].so
open your browser and go to the next link http://localhost:9000/test.html