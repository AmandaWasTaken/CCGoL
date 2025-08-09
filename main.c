#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "color.h"

#define BLANK 		"0"
#define CHAR  		"1"
#define GREEN 		32
#define DEFAULT_COLOR 	 0

void graceful_exit(int** board, int** next, int** prev,
		   int** prev_prev, int rows){

	for(int i = 0; i < rows; i++){
		free(board[i]);
		free(next[i]);
		free(prev[i]);
		free(prev_prev[i]);
	}
	free(board);
	free(next);
	free(prev);
	free(prev_prev);
	exit(0);
}

void die(const char* restrict e){
	fprintf(stderr, "%s\n", e);
	exit(1);
} 

void clear(){
	puts("\033[2J\033[H");
}

void init_board(int** board, int rows, int cols){

	for(int i = 0; i < rows; i++){
		for(int j = 0; j < cols; j++){
			board[i][j] = rand() % 2;
		}
	}
}

bool is_alive(int x){
	return x == 1;
}

bool all_zeros(int** board, int rows, int cols){

	for(int i = 0; i < rows; i++){
		for(int j = 0; j < cols; j++){
			if(board[i][j] != 0) return false;
		}
	}
	return true;
}

void print_board(int** board, const int sz){

	for(int i = 0; i < sz; i++){
		for(int j = 0; j < sz; j++){
			if(board[i][j] == 0){
				printf(BLANK);
			} else {
				change_color(GREEN);
				printf(CHAR);
				change_color(DEFAULT_COLOR);
			}

			if(j < sz - 1)
				printf(" ");
		}
	printf("\n");
	}
}

bool is_stuck(int** next, int** prev_prev, int rows, int cols){

	for(int i = 0; i < rows; i++){
		for(int j = 0; j < cols; j++){
			if(next[i][j] != prev_prev[i][j]) return false;
		}
	}
	return true;
}

int nb_count(int** board, int rows, int cols,
		int row, int col){

	// Define all 8 surrounding indices
	int nb = 0;
	int idc[8][2] = {
		{-1, -1}, {-1, 0}, {-1, 1},
		{0,  -1},          {0,  1},
		{1,  -1}, {1,  0}, {1,  1}
	};

	for(int i = 0; i < 8; i++){
		int new_row = row + idc[i][0];
		int new_col = col + idc[i][1];

		if(new_row >= 0 && new_row < rows &&
		   new_col >= 0 && new_col < cols){
			if(board[new_row][new_col] == 1) nb++;
		}
	}
	return nb;
}

int** event_loop(int** board, const int rows, const int cols){

	// This is so inefficient but it works for now ig
	int** next = malloc(rows*sizeof(int *));
	int** prev = malloc(rows*sizeof(int *));
	int** prev_prev = malloc(rows*sizeof(int *));

	for(int i = 0; i < rows; i++){
		next[i] = malloc(cols*sizeof(int));
		prev[i] = malloc(cols*sizeof(int));
		prev_prev[i] = malloc(cols*sizeof(int));
	}

	for(int i = 0; i < rows; i++){
		for(int j = 0; j < rows; j++){
			prev[i][j] = board[i][j];
		}
	}

	int gen = 0;
	while(1){
		sleep(1);
		clear();
		printf("Generation: %i\n", ++gen);
		print_board(board, rows);

		if(all_zeros(board, rows, cols)){
			printf("Everyone died\n");
			graceful_exit(board, next, prev, prev_prev, rows);
		}

		for(int i = 0; i < rows; i++){
			for(int j = 0; j < cols; j++){

				// count neighboring living cells
				int cnt = nb_count(
					  board, rows, cols, i ,j); 
				
				// cell has value 1
				bool alive = is_alive(board[i][j]);

				if(alive){
					next[i][j] = (cnt == 2 || cnt == 3?
							1 : 0);
				} else {
					next[i][j] = (cnt == 3) ? 1 : 0;
				}
			}
		}

		// Check for looping
		if(is_stuck(next, prev_prev, rows, cols)){
			puts("Loop detected, stopping sim\n");
			graceful_exit(board, next, prev, prev_prev, rows);
		}

		// Shift prev state
		for(int i = 0; i < rows; i++){
			for(int j = 0; j < cols; j++){
				prev_prev[i][j] = prev[i][j];
				prev[i][j] = board[i][j];
				board[i][j] = next[i][j];
			}
		}
	}
	// we don't actually need to return here since we exit early
	// but i'm too lazy to change that rn
	return next;
}

int main(int argc, char** argv){

	srand(time(NULL));
	if(argc < 3){
		die("Not enough arguments");
	}

	char* end;
	int rows = strtol(argv[1], &end, 10);
	if(*end != '\0'){
		die("couldn't convert rows to long");
	}

	int cols = strtol(argv[2], &end, 10);
	if(*end != '\0'){
		die("couldn't convert cols to long");
	}

	if(rows != cols) die("Only n*n supported (for now)");


	// Allocate memory for entire board
	int** board = malloc(rows*sizeof(int *));	
	for(int i = 0; i < rows; i++){
		board[i] = malloc(cols*sizeof(int));
	}

	// Board is filled with random values
	// I will allow the user to choose the values SOONTM
	init_board(board, rows, cols);

	// the only reason this returns int** is so that i can
	// pass it to graceful_exit
	// this cannot be a smart approach but i'm too drunk to think
	int** next = event_loop(board, rows, cols);

	// graceful_exit(board, next, prev, prev_prev, rows);

	
        return 0;
}
