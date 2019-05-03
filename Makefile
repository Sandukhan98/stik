SRC=src
TEST=src/tests
PLAYERS=src/players
TESTPLAYERS=src/players/tests
INSTALL=install

CC=gcc
CFLAGS=-W -Wall -std=c99
CPPFLAGS=-I ${SRC} -I ${PLAYERS}

.PHONY: all build install test clean display

all: build install

#build ---------------------------------------------------------------
build: server you_lose.so you_lose_again.so random_player.so damn.so you_lose_againx2.so

list_players.o: ${SRC}/list_players.h ${SRC}/list_players.c
	${CC} ${CFLAGS} ${CPPFLAGS} ${SRC}/list_players.c -c

server: ${SRC}/server.c  list_players.o option.o bitboard.o board_game.o list_moves.o
	${CC} ${CFLAGS} -o server ${SRC}/server.c list_moves.o list_players.o option.o bitboard.o board_game.o -ldl

list_moves.o: ${SRC}/list_moves.h ${SRC}/list_moves.c
	${CC} ${CFLAGS} ${CPPFLAGS} -c ${SRC}/list_moves.c

bitboard.o: ${SRC}/bitboard.h ${SRC}/bitboard.c
	${CC} ${CFLAGS} ${CPPFLAGS} -c ${SRC}/bitboard.c

board_game.o: ${SRC}/board_game.h ${SRC}/board_game.c
	${CC} ${CFLAGS} ${CPPFLAGS} -c ${SRC}/board_game.c

option.o:${SRC}/option.h ${SRC}/option.c
	${CC} ${CFLAGS} ${CPPFLAGS} -c ${SRC}/option.c

eval_move.o: ${SRC}/eval_move.c ${SRC}/eval_move.h
	${CC} ${CFLAGS} ${CPPFLAGS} -c ${SRC}/eval_move.c


#players -------------------------------------------------------------
same_move_player.o: ${PLAYERS}/same_move_player.c
	${CC} ${CFLAGS} ${CPPFLAGS} ${PLAYERS}/same_move_player.c ${SRC}/board_game.c ${SRC}/bitboard.c -c

same_move_player.so: ${PLAYERS}/same_move_player.c
	${CC} ${CFLAGS} ${CPPFLAGS} -o same_move_player.so ${PLAYERS}/same_move_player.c ${SRC}/board_game.c ${SRC}/bitboard.c -fPIC -shared

linear_player.o: ${PLAYERS}/linear_player.c
	${CC} ${CFLAGS} ${CPPFLAGS} ${PLAYERS}/linear_player.c ${SRC}/board_game.c ${SRC}/bitboard.c -c

linear_player.so: ${PLAYERS}/linear_player.c
	${CC} ${CFLAGS} ${CPPFLAGS} -o linear_player.so ${PLAYERS}/linear_player.c ${SRC}/board_game.c ${SRC}/bitboard.c -fPIC -shared

random_player.o: ${PLAYERS}/random_player.c
	${CC} ${CFLAGS} ${CPPFLAGS} ${PLAYERS}/random_player.c ${SRC}/board_game.c ${SRC}/bitboard.c -c

random_player.so: ${PLAYERS}/random_player.c
	${CC} ${CFLAGS} ${CPPFLAGS} -o random_player.so ${PLAYERS}/random_player.c ${SRC}/board_game.c ${SRC}/bitboard.c -fPIC -shared

manual_player.so: ${PLAYERS}/manual_player.c ${SRC}/board_game.c ${SRC}/bitboard.c ${SRC}/eval_move.c ${SRC}/eval_move.h
	${CC} ${CFLAGS} ${CPPFLAGS} -o manual_player.so ${PLAYERS}/manual_player.c ${SRC}/board_game.c ${SRC}/bitboard.c ${SRC}/eval_move.c -fPIC -shared

manual_player1.so: ${PLAYERS}/manual_player.c ${SRC}/board_game.c ${SRC}/bitboard.c ${SRC}/eval_move.c ${SRC}/eval_move.h
	${CC} ${CFLAGS} ${CPPFLAGS} -o manual_player1.so ${PLAYERS}/manual_player.c ${SRC}/board_game.c ${SRC}/bitboard.c ${SRC}/eval_move.c -fPIC -shared

test_player.so: ${PLAYERS}/test_player.c
	${CC} ${CFLAGS} ${CPPFLAGS} -o test_player.so ${PLAYERS}/test_player.c -fPIC -shared

you_lose.so : ${PLAYERS}/you_lose.c 
	${CC} ${CFLAGS} ${CPPFLAGS} -o you_lose.so ${PLAYERS}/you_lose.c ${SRC}/eval_move.c ${SRC}/board_game.c ${SRC}/bitboard.c -fPIC -shared

you_lose_again.so : ${PLAYERS}/you_lose_again.c 
	${CC} ${CFLAGS} ${CPPFLAGS} -o you_lose_again.so ${PLAYERS}/you_lose_again.c ${SRC}/eval_move.c ${SRC}/board_game.c ${SRC}/bitboard.c -fPIC -shared

damn.so : ${PLAYERS}/damn.c display.o
	${CC} ${CFLAGS} ${CPPFLAGS} -o damn.so ${PLAYERS}/damn.c ${SRC}/eval_move.c ${SRC}/display.c ${SRC}/board_game.c ${SRC}/bitboard.c -fPIC -shared

you_lose_againx2.so : ${PLAYERS}/you_lose_againx2.c display.o
	${CC} ${CFLAGS} ${CPPFLAGS} -o you_lose_againx2.so ${PLAYERS}/you_lose_againx2.c ${SRC}/eval_move.c ${SRC}/display.c ${SRC}/board_game.c ${SRC}/bitboard.c -fPIC -shared

#install -------------------------------------------------------------
install: build
	mkdir -p install
	mv server ${INSTALL}/server
	mv *.so ${INSTALL}/.
	mv *.o ${INSTALL}/.

#tests ----------------------------------------------------------------
test: test_list_players test_bitboard test_board_game test_players test_options

test_list_players.o :${TEST}/test_list_players.c
	${CC} ${CFLAGS} ${CPPFLAGS} -c ${TEST}/test_list_players.c

test_list_players : test_list_players.o list_players.o test_player.so
	${CC} ${CFLAGS} -o test_players test_list_players.o list_players.o -ldl
	./test_players

test_bitboard.o : ${TEST}/bitboard_test.c
	${CC} ${CFLAGS} ${CPPFLAGS} -c ${TEST}/bitboard_test.c

test_bitboard : test_bitboard.o bitboard.o
	${CC} ${CFLAGS} -o test_bitboard bitboard.o bitboard_test.o 
	./test_bitboard

test_board_game.o : ${TEST}/board_game_test.c
	${CC} ${CFLAGS} ${CPPFLAGS} -c ${TEST}/board_game_test.c

test_board_game : test_board_game.o board_game.o bitboard.o
	${CC} ${CFLAGS} -o test_board_game board_game.o bitboard.o board_game_test.o 
	./test_board_game

test_options : option.o  ${TEST}/test_option_struc.c
	${CC} ${CFLAGS} ${CPPFLAGS} -o test_options ${TEST}/test_option_struc.c option.o
	./test_options

#test_players ----------------------------------------------------------
test_players: test_same_move test_linear test_random

test_same_move: ${TESTPLAYERS}/test_same_move.c same_move_player.o
	${CC} ${CFLAGS} ${CPPFLAGS} ${TESTPLAYERS}/test_same_move.c same_move_player.o ${SRC}/board_game.c ${SRC}/bitboard.c -o test_same_move
	./test_same_move

test_linear: ${TESTPLAYERS}/test_linear.c linear_player.o
	${CC} ${CFLAGS} ${CPPFLAGS} ${TESTPLAYERS}/test_linear.c linear_player.o ${SRC}/board_game.c ${SRC}/bitboard.c -o test_linear
	./test_linear

test_random: ${TESTPLAYERS}/test_random.c random_player.o board_game.o bitboard.o
	${CC} ${CFLAGS} ${CPPFLAGS} ${TESTPLAYERS}/test_random.c random_player.o board_game.o bitboard.o -o test_random
	./test_random

#display ---------------------------------------------------------------
display : build_display install

build_display : server_display same_move_player.so linear_player.so you_lose.so manual_player.so manual_player1.so board_game.o bitboard.o you_lose_again.so you_lose_againx2.so

precompile:
	${CC} ${CFLAGS} -E -DDISP ${SRC}/server.c -ldl

server_display:${SRC}/server.c display.o list_moves.o list_players.o option.o bitboard.o board_game.o
	${CC} ${CFLAGS} -DDISP -o server display.o list_moves.o list_players.o option.o bitboard.o board_game.o ${SRC}/server.c -ldl

display.o: ${SRC}/display.c
ifdef SLW
	${CC} ${CFLAGS} -DSLW -c ${SRC}/display.c
else
	${CC} ${CFLAGS} -c ${SRC}/display.c
endif 

#clean ----------------------------------------------------------------
clean:
	rm -f *.so *.o *~ install/* src/*.o test_options test_same_move test_players test_bitboard test_board_game test_linear test_random
