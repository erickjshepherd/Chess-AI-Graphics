
#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include "Header1.h"
#include "windows.h"
#include "Color.h"
#include <string>
#include <ctime>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <string>

#undef main

#define Depth 5 // Sets the depth the alpha beta function will search

using namespace eku;
using namespace std;


void set_board(Tile* board) { // Set the initial board state
	int n = 0;
	
	for (n = 0; n < 64; n++) { // set up char values and weights for initial position
	
		if (n == 0 || n == 7 || n == 56 || n == 63) { // Rooks
		
			board[n].type = 'R';
			board[n].weight = 50;
		}

		else if (n == 1 || n == 6 || n == 57 || n == 62) { // Knights
		
			board[n].type = 'N';
			board[n].weight = 30;
		}

		else if (n == 2 || n == 5 || n == 58 || n == 61) { // Bishops
		
			board[n].type = 'B';
			board[n].weight = 30;
		}

		else if (n == 3 || n == 59) { // Queens
		
			board[n].type = 'Q';
			board[n].weight = 90;
		}

		else if (n == 4 || n == 60) { // Kings
		
			board[n].type = 'K';
			board[n].weight = 7000;
		}

		else if ((n >= 8 && n <= 15) || (n >= 48 && n <= 55)) { // Pawns
		
			board[n].type = 'P';
			board[n].weight = 10;
		}

		else { // Empty
		
			board[n].type = '#';
			board[n].weight = 0;
		}
	}

	for (n = 0; n < 64; n++) { // Define color of initial pieces and board tiles
	
		if (n >= 0 && n <= 15) { 
		
			board[n].color = 1; // 1 = black
		}

		else if (n >= 48 && n <= 63) {
		
			board[n].color = 0; // 0 = white
		}

		else {
		
			board[n].color = 2; // 2 = empty
		}

		if ((n >= 0 && n <= 7) || (n >= 16 && n <= 23) || (n >= 32 && n <= 39) || (n >= 48 && n <= 55)) { // set colors of even rows
		
			if (n % 2 == 0) {
			
				board[n].color_b = 0;
			}

			else {
			
				board[n].color_b = 1;
			}
		}

		if ((n >= 8 && n <= 15) || (n >= 24 && n <= 31) || (n >= 40 && n <= 47) || (n >= 56 && n <= 63)) { // set colors of odd rows

			if (n % 2 == 0) {

				board[n].color_b = 1;
			}

			else {

				board[n].color_b = 0;
			}
		}

		board[n].enpassant = 0;
	}

	board[0].castle = 1; //set up castling
	board[7].castle = 1;
	board[56].castle = 1;
	board[63].castle = 1;
	board[4].castle = 1;
	board[60].castle = 1;
}

void print_board_white(Tile * board) { // Print the board from whites perspective

	int n;
	char row = 0x38;
	char letter = 0x41;

	concolinit();

	cout << yellow << row << ' ';
	row--;
	
	for (n = 0; n < 64; n++) {
	
		if (board[n].type == '#') { // output tile color
		
			if (board[n].color_b == 0) {
			
				cout << white << board[n].type;
			}
			
			else {
			
				cout << blue << board[n].type;
			}
		}

		else {
		
			if (board[n].color == 0) {
			
				cout << white << board[n].type;
			}

			else {
			
				cout << blue << board[n].type;
			}
		}

		if (n % 8 == 7) { // new line every 8 tiles
		
			cout << '\n';
			if (row > 48) {
			
				cout << yellow << row << ' ';
				row--;
			}

		}

		else {
		
			cout << ' ';
		}
	}

	cout << ' ';

	for (n = 0; n < 8; n++) {
	
		cout << yellow << ' ' << letter;
		letter++;
	}

	cout << endl << endl;
	
	setcolor(white, defbackcol);
}

void print_board_black(Tile* board) { // Print the board from blacks perspective

	int n;
	char row = 0x31;
	char letter = 0x48;

	concolinit();

	cout << yellow << row << ' ';
	row++;

	for (n = 0; n < 64; n++) {

		if (board[63 - n].type == '#') { // output tile color

			if (board[63 - n].color_b == 0) {

				cout << white << board[63 - n].type;
			}

			else {

				cout << blue << board[63 - n].type;
			}
		}

		else {

			if (board[63 - n].color == 0) {

				cout << white << board[63 - n].type;
			}

			else {

				cout << blue << board[63 - n].type;
			}
		}

		if (n % 8 == 7) { // new line every 8 tiles

			cout << '\n';
			if (row < 57) {

				cout << yellow << row << ' ';
				row++;
			}

		}

		else {

			cout << ' ';
		}
	}

	cout << ' ';

	for (n = 0; n < 8; n++) {

		cout << yellow << ' ' << letter;
		letter--;
	}

	cout << endl << endl;

	setcolor(white, defbackcol);
}

void print_board(Tile* board) { // Print the board to the console


	if (player_color_p == 0) {
	
		print_board_white(board);
	}

	else {
	
		print_board_black(board);
	}
}

void render_board_w(Tile* board) { // renders the current board state if player is white

	int count;
	int x;
	int y;

	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, Board, NULL, NULL);

	for (count = 0; count < 64; count++) {

		x = (count % 8) * 50; // get x/y coordinates of the piece texture
		y = (count / 8) * 50;

		piecesquare.x = x;
		piecesquare.y = y;

		if (board[count].color == 0) { // for white pieces


			if (board[count].type == 'P') {

				SDL_RenderCopy(renderer, WPawn, NULL, &piecesquare);
			}

			if (board[count].type == 'B') {

				SDL_RenderCopy(renderer, WBishop, NULL, &piecesquare);
			}

			if (board[count].type == 'N') {

				SDL_RenderCopy(renderer, WKnight, NULL, &piecesquare);
			}

			if (board[count].type == 'R') {

				SDL_RenderCopy(renderer, WRook, NULL, &piecesquare);
			}

			if (board[count].type == 'K') {

				SDL_RenderCopy(renderer, WKing, NULL, &piecesquare);
			}

			if (board[count].type == 'Q') {

				SDL_RenderCopy(renderer, WQueen, NULL, &piecesquare);
			}
		}

		else { // for black pieces

			if (board[count].type == 'P') {

				SDL_RenderCopy(renderer, BPawn, NULL, &piecesquare);
			}

			if (board[count].type == 'B') {

				SDL_RenderCopy(renderer, BBishop, NULL, &piecesquare);
			}

			if (board[count].type == 'N') {

				SDL_RenderCopy(renderer, BKnight, NULL, &piecesquare);
			}

			if (board[count].type == 'R') {

				SDL_RenderCopy(renderer, BRook, NULL, &piecesquare);
			}

			if (board[count].type == 'K') {

				SDL_RenderCopy(renderer, BKing, NULL, &piecesquare);
			}

			if (board[count].type == 'Q') {

				SDL_RenderCopy(renderer, BQueen, NULL, &piecesquare);
			}
		}
	}
}

void render_board_b(Tile* board) { // renders the current board state if player is white

	int count;
	int x;
	int y;

	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, BoardB, NULL, NULL);

	for (count = 0; count < 64; count++) {

		x = (count % 8) * 50; // get x/y coordinates of the piece texture
		y = (count / 8) * 50;

		piecesquare.x = x;
		piecesquare.y = y;

		if (board[63 - count].color == 0) { // for white pieces


			if (board[63 - count].type == 'P') {

				SDL_RenderCopy(renderer, WPawn, NULL, &piecesquare);
			}

			if (board[63 - count].type == 'B') {

				SDL_RenderCopy(renderer, WBishop, NULL, &piecesquare);
			}

			if (board[63 - count].type == 'N') {

				SDL_RenderCopy(renderer, WKnight, NULL, &piecesquare);
			}

			if (board[63 - count].type == 'R') {

				SDL_RenderCopy(renderer, WRook, NULL, &piecesquare);
			}

			if (board[63 - count].type == 'K') {

				SDL_RenderCopy(renderer, WKing, NULL, &piecesquare);
			}

			if (board[63 - count].type == 'Q') {

				SDL_RenderCopy(renderer, WQueen, NULL, &piecesquare);
			}
		}

		else { // for black pieces

			if (board[63 - count].type == 'P') {

				SDL_RenderCopy(renderer, BPawn, NULL, &piecesquare);
			}

			if (board[63 - count].type == 'B') {

				SDL_RenderCopy(renderer, BBishop, NULL, &piecesquare);
			}

			if (board[63 - count].type == 'N') {

				SDL_RenderCopy(renderer, BKnight, NULL, &piecesquare);
			}

			if (board[63 - count].type == 'R') {

				SDL_RenderCopy(renderer, BRook, NULL, &piecesquare);
			}

			if (board[63 - count].type == 'K') {

				SDL_RenderCopy(renderer, BKing, NULL, &piecesquare);
			}

			if (board[63 - count].type == 'Q') {

				SDL_RenderCopy(renderer, BQueen, NULL, &piecesquare);
			}
		}
	}
}

void render_board(Tile* board) {

	if (player_color_p == 0) {

		render_board_w(board);
	}

	else {

		render_board_b(board);
	}
}

void get_player_color() { // Let player choose a color

	int choice;

	cout << "Do you want to play as white(0) or black(1)?" << endl;
	cin >> choice;

	if (choice == 0 || choice == 1) {
	
		player_color_p = choice;
		computer_color_p = !choice;
	}

	else {
		
		cout << "That is not a valid choice." << endl;
		get_player_color();
	}
}

void get_extra_info() {

	int choice;

	cout << "Should extra information be displayed? No(0)/Yes(1)" << endl;
	cin >> choice;

	if (choice == 0 || choice == 1) {
	
		extra_info = choice;
	}

	else {
	
		cout << "That is not a valid choice." << endl;
		get_extra_info();
	}
}

void get_coordinates(){ // Prompt player to input a move

	int n;

	string first;
	string next;

	cout << "What piece do you want to move?" << endl;
	cin >> first;
	cout << "Where do you want to move it?" << endl;
	cin >> next;

	for (n = 0; n < 64; n++) {
	
		if (first == conversion[n]) {
		
			player_currentp = n;
		}

		if (next == conversion[n]) {
		
			player_nextp = n;
		}
	}
}

int valid_move(int color, int check, Tile* board, int nextp, int currentp);

int in_check_tile(int color, int tile, Tile* board) { // is a certain tile in check?

	int n;
	int nextp;
	int currentp;


	nextp = tile;
	
	for (n = 0; n < 64; n++) { // for each opposing piece check if taking the tile is valid
							   // allow pieces to take king into check
		currentp = n;

		if (valid_move(!color, 0, board, nextp, currentp) == 1) {

			return 1;
		}
	}

	return 0;
}

int in_check(int color,Tile* board) { // is a player in check?

	int n;

	int currentp;
	int nextp;

	for (n = 0; n < 64; n++) { // get king location

		if (board[n].type == 'K' && board[n].color == color) {

			nextp = n;
		}
	}

	for (n = 0; n < 64; n++) { // for each opposing piece check if taking the king is valid
							   // allow pieces to take king into check
		currentp = n;

		if (valid_move(!color , 0, board, nextp, currentp) == 1) {

			return 1;
		}
	}

	return 0;
}

int valid_move(int player_color, int check, Tile * board, int nextp, int currentp) { // checks if the current move is valid for a particular color
									   // if check = 0 dont look for check

	if (nextp < 0 || nextp > 63) {
	
		//cout << "You cannot move off of the board. Choose a different move." << endl;
		return 0;
	}

	if (currentp < 0 || currentp > 63) {
	
		//cout << "you must choose a piece on the board. Choose a different move." << endl;
		return 0;
	}

	if (board[currentp].type == '#') {
	
		//cout << "There is no piece there. Choose a different move." << endl;
		return 0;
	}

	if (board[currentp].color != player_color) {
	
		//cout << "You can not move that piece. Choose a different move." << endl;
		return 0;
	}

	if (check == 1) { // make sure you don't move into check
		
		char temptype = board[nextp].type; // store currentp and nextp
		int tempcolor = board[nextp].color;
		int tempweight = board[nextp].weight;

		if (board[nextp].type == 'K' && board[nextp].color == player_color) { //don't let the player take their own king though
		
			return 0;
		} 

		board[nextp].type = board[currentp].type; // update the board with the new move even if its illegal
		board[nextp].color = board[currentp].color;
		board[nextp].weight = board[currentp].weight;
		board[currentp].type = '#';
		board[currentp].weight = 0;

		int valid = !in_check(player_color,board); //would we be in check now?

		board[currentp].type = board[nextp].type; // undo the move
		board[currentp].color = board[nextp].color;
		board[currentp].weight = board[nextp].weight;
		board[nextp].type = temptype;
		board[nextp].weight = tempweight;
		board[nextp].color = tempcolor;

		if (valid == 0) { // if we would be in check move is invalid

			return 0;
		}
	}
	
	if (board[currentp].type == 'P') { // Valid pawn moves
		
		if (player_color == 0) { // for when player is white
			
			int difference = currentp - nextp;

			if (difference == 8 && board[nextp].type == '#') { // pawn moves forward if empty
			
				return 1;
			}

			else if (difference == 16 && board[nextp].type == '#' && board[nextp + 8].type == '#' && currentp >= 48 && currentp <= 55) { // pawn moves 2
				
				return 1;
			}

			else if (currentp % 8 == 0) { // if pawn is in left most column only check right
			
				if (difference == 7 && ((board[nextp].color == 1 && board[nextp].type != '#') || (board[nextp].enpassant == 1 && board[nextp + 8].color != player_color))) {
				
					return 1;
				}

				else {
				
					//cout << "That move is invalid. Choose a different move." << endl;
					return 0;
				}
			}

			else if (currentp % 8 == 7) { // if pawn is in right most column only check left
			
				if (difference == 9 && ((board[nextp].color == 1 && board[nextp].type != '#') || (board[nextp].enpassant == 1 && board[nextp + 8].color != player_color))) {
				
					return 1;
				}

				else {
				
					//cout << "That move is invalid. Choose a different move." << endl;
					return 0;
				}
			}

			else if (currentp % 8 > 0 || currentp % 8 < 7) { //if pawn is in the middle check both
			
				if (difference == 9 && ((board[nextp].color == 1 && board[nextp].type != '#') || (board[nextp].enpassant == 1 && board[nextp + 8].color != player_color))) { // check left

					return 1;
				}


				if (difference == 7 && ((board[nextp].color == 1 && board[nextp].type != '#') || (board[nextp].enpassant == 1 && board[nextp + 8].color != player_color))) { // check right

					return 1;
				}
			}

			else {
			
				//cout << "That move is invalid. Choose a different move." << endl;
				return 0;
			}
		}

		else { //when player is black
		
			int difference = nextp - currentp;

			if (difference == 8 && board[nextp].type == '#') { // pawn moves forward if empty

				return 1;
			}

			else if (difference == 16 && board[nextp].type == '#' && board[nextp - 8].type == '#' && currentp >= 8 && currentp <= 15) { // pawn moves 2

				return 1;
			}

			else if (currentp % 8 == 0) { // if pawn is in right most column only check left

				if (difference == 9 && ((board[nextp].color == 0 && board[nextp].type != '#') || (board[nextp].enpassant == 1 && board[nextp - 8].color != player_color))) {

					return 1;
				}

				else {

					//cout << "That move is invalid. Choose a different move." << endl;
					return 0;
				}
			}

			else if (currentp % 8 == 7) { // if pawn is in left most column only check right

				if (difference == 7 && ((board[nextp].color == 0 && board[nextp].type != '#') || (board[nextp].enpassant == 1 && board[nextp - 8].color != player_color))) {

					return 1;
				}

				else {

					//cout << "That move is invalid. Choose a different move." << endl;
					return 0;
				}
			}

			else if (currentp % 8 > 0 || currentp % 8 < 7) { //if pawn is in the middle check both

				if (difference == 9 && ((board[nextp].color == 0 && board[nextp].type != '#') || (board[nextp].enpassant == 1 && board[nextp - 8].color != player_color)) && board[nextp].type != '#') { // check left

					return 1;
				}

				if (difference == 7 && ((board[nextp].color == 0 && board[nextp].type != '#') || (board[nextp].enpassant == 1 && board[nextp - 8].color != player_color))) { // check right

					return 1;
				}

			}

			else {

				//cout << "That move is invalid. Choose a different move." << endl;
				return 0;
			}
		}
	}

	if (board[currentp].type == 'B') { // Valid Bishop moves
	
		int n;
		int p = -1;
		int difference = currentp - nextp;
		
		if (difference > 0 && difference % 7 == 0 && currentp % 8 < nextp % 8) { // top right diagonal. difference should be 7*something
		
			n = 1;
			
			while (p != nextp) {
			
				p = currentp - 7 * n;

				if (p != nextp && board[p].type != '#') { //return 0 if theres a piece in the way
				
					return 0;
				}

				n++;
			}

			if (board[nextp].color == player_color && board[p].type != '#') { // cant move onto your own piece
			
				return 0;
			}

			else {
			
				return 1;
			}
		}

		if (difference > 0 && difference % 9 == 0 && currentp % 8 > nextp % 8) { // top left diagonal. difference should be 9*something
		
			n = 1;

			while (p != nextp) {

				p = currentp - 9 * n;

				if (p != nextp && board[p].type != '#') { //return 0 if theres a piece in the way

					return 0;
				}

				n++;
			}

			if (board[nextp].color == player_color && board[p].type != '#') { // cant move onto your own piece

				return 0;
			}

			else {

				return 1;
			}
		}
	
		if (difference < 0 && difference % 7 == 0 && currentp % 8 > nextp % 8) { // bottom left diagonal. difference should be -7*something

			n = 1;

			while (p != nextp) {

				p = currentp + 7 * n;

				if (p != nextp && board[p].type != '#') { //return 0 if theres a piece in the way

					return 0;
				}

				n++;
			}

			if (board[nextp].color == player_color && board[p].type != '#') { // cant move onto your own piece

				return 0;
			}

			else {

				return 1;
			}
		}

		if (difference < 0 && difference % 9 == 0 && currentp % 8 < nextp % 8) { // bottom right diagonal. difference should be 9*something

			n = 1;

			while (p != nextp) {

				p = currentp + 9 * n;

				if (p != nextp && board[p].type != '#') { //return 0 if theres a piece in the way

					return 0;
				}

				n++;
			}

			if (board[nextp].color == player_color && board[p].type != '#') { // cant move onto your own piece

				return 0;
			}

			else {

				return 1;
			}
		}

	}

	if (board[currentp].type == 'N') { // Valid knight moves
	
		int difference = currentp - nextp;
		
		if (difference == 17 && currentp % 8 > 0) { // top left #1
		
			if (board[nextp].color == player_color && board[nextp].type != '#') {
			
				return 0;
			}

			else { 
				
				return 1; 
			}
		}

		else if (difference == 10 && currentp % 8 > 1) { // top left #2

			if (board[nextp].color == player_color && board[nextp].type != '#') {

				return 0;
			}

			else {

				return 1;
			}
		}

		else if (difference == 15 && currentp % 8 < 7) { // top right #1

			if (board[nextp].color == player_color && board[nextp].type != '#') {

				return 0;
			}

			else {

				return 1;
			}
		}

		else if (difference == 6 && currentp % 8 < 6) { // top right #2

			if (board[nextp].color == player_color && board[nextp].type != '#') {

				return 0;
			}

			else {

				return 1;
			}
		}

		else if (difference == -6 && currentp % 8 > 1) { // bottom left #1

			if (board[nextp].color == player_color && board[nextp].type != '#') {

				return 0;
			}

			else {

				return 1;
			}
		}

		else if (difference == -15 && currentp % 8 > 0) { // bottom left #2

			if (board[nextp].color == player_color && board[nextp].type != '#') {

				return 0;
			}

			else {

				return 1;
			}
		}

		else if (difference == -10 && currentp % 8 < 6) { // bottom right #1

			if (board[nextp].color == player_color && board[nextp].type != '#') {

				return 0;
			}

			else {

				return 1;
			}
		}

		else if (difference == -17 && currentp % 8 < 7) { // bottom right #2

			if (board[nextp].color == player_color && board[nextp].type != '#') {

				return 0;
			}

			else {

				return 1;
			}
		}

		else {
		
			return 0;
		}
	}

	if (board[currentp].type == 'R') { // Valid rook moves
	
		int n;
		int p = -1;
		int difference = currentp - nextp;

		if (difference > 0 && difference % 8 == 0) { // up
		
			n = 1;

			while (p != nextp) {

				p = currentp - 8 * n;

				if (p != nextp && board[p].type != '#') { //return 0 if theres a piece in the way

					return 0;
				}

				n++;
			}

			if (board[nextp].color == player_color && board[p].type != '#') { // cant move onto your own piece

				return 0;
			}

			else {

				return 1;
			}
		}

		if (difference < 0 && difference % 8 == 0) { // down

			n = 1;

			while (p != nextp) {

				p = currentp + 8 * n;

				if (p != nextp && board[p].type != '#') { //return 0 if theres a piece in the way

					return 0;
				}

				n++;
			}

			if (board[nextp].color == player_color && board[p].type != '#') { // cant move onto your own piece

				return 0;
			}

			else {

				return 1;
			}
		}

		if (difference > 0 && difference <= currentp % 8) { // left

			n = 1;

			while (p != nextp) {

				p = currentp - n;

				if (p != nextp && board[p].type != '#') { //return 0 if theres a piece in the way

					return 0;
				}

				n++;
			}

			if (board[nextp].color == player_color && board[p].type != '#') { // cant move onto your own piece

				return 0;
			}

			else {

				return 1;
			}
		}		
		
		if (difference < 0 && (-1 * difference) <= 7 - (currentp % 8)) { // right

			n = 1;

			while (p != nextp) {

				p = currentp + n;

				if (p != nextp && board[p].type != '#') { //return 0 if theres a piece in the way

					return 0;
				}

				n++;
			}

			if (board[nextp].color == player_color && board[p].type != '#') { // cant move onto your own piece

				return 0;
			}

			else {

				return 1;
			}
		}
	}

	if (board[currentp].type == 'Q') { // Valid Queen moves
	
		int n;
		int p = -1;
		int difference = currentp - nextp;

		if (difference > 0 && difference % 7 == 0 && currentp % 8 < nextp % 8) { // top right diagonal. difference should be 7*something

			n = 1;

			while (p != nextp) {

				p = currentp - 7 * n;

				if (p != nextp && board[p].type != '#') { //return 0 if theres a piece in the way

					return 0;
				}

				n++;
			}

			if (board[nextp].color == player_color && board[p].type != '#') { // cant move onto your own piece

				return 0;
			}

			else {

				return 1;
			}
		}

		if (difference > 0 && difference % 9 == 0 && currentp % 8 > nextp % 8) { // top left diagonal. difference should be 9*something

			n = 1;

			while (p != nextp) {

				p = currentp - 9 * n;

				if (p != nextp && board[p].type != '#') { //return 0 if theres a piece in the way

					return 0;
				}

				n++;
			}

			if (board[nextp].color == player_color && board[p].type != '#') { // cant move onto your own piece

				return 0;
			}

			else {

				return 1;
			}
		}

		if (difference < 0 && difference % 7 == 0 && currentp % 8 > nextp % 8) { // bottom left diagonal. difference should be -7*something

			n = 1;

			while (p != nextp) {

				p = currentp + 7 * n;

				if (p != nextp && board[p].type != '#') { //return 0 if theres a piece in the way

					return 0;
				}

				n++;
			}

			if (board[nextp].color == player_color && board[p].type != '#') { // cant move onto your own piece

				return 0;
			}

			else {

				return 1;
			}
		}

		if (difference < 0 && difference % 9 == 0 && currentp % 8 < nextp % 8) { // bottom right diagonal. difference should be 9*something

			n = 1;

			while (p != nextp) {

				p = currentp + 9 * n;

				if (p != nextp && board[p].type != '#') { //return 0 if theres a piece in the way

					return 0;
				}

				n++;
			}

			if (board[nextp].color == player_color && board[p].type != '#') { // cant move onto your own piece

				return 0;
			}

			else {

				return 1;
			}
		}

		if (difference > 0 && difference % 8 == 0) { // up

			n = 1;

			while (p != nextp) {

				p = currentp - 8 * n;

				if (p != nextp && board[p].type != '#') { //return 0 if theres a piece in the way

					return 0;
				}

				n++;
			}

			if (board[nextp].color == player_color && board[p].type != '#') { // cant move onto your own piece

				return 0;
			}

			else {

				return 1;
			}
		}

		if (difference < 0 && difference % 8 == 0) { // down

			n = 1;

			while (p != nextp) {

				p = currentp + 8 * n;

				if (p != nextp && board[p].type != '#') { //return 0 if theres a piece in the way

					return 0;
				}

				n++;
			}

			if (board[nextp].color == player_color && board[p].type != '#') { // cant move onto your own piece

				return 0;
			}

			else {

				return 1;
			}
		}

		if (difference > 0 && difference <= currentp % 8) { // left

			n = 1;

			while (p != nextp) {

				p = currentp - n;

				if (p != nextp && board[p].type != '#') { //return 0 if theres a piece in the way

					return 0;
				}

				n++;
			}

			if (board[nextp].color == player_color && board[p].type != '#') { // cant move onto your own piece

				return 0;
			}

			else {

				return 1;
			}
		}

		if (difference < 0 && (-1 * difference) <= 7 - (currentp % 8)) { // right

			n = 1;

			while (p != nextp) {

				p = currentp + n;

				if (p != nextp && board[p].type != '#') { //return 0 if theres a piece in the way

					return 0;
				}

				n++;
			}

			if (board[nextp].color == player_color && board[p].type != '#') { // cant move onto your own piece

				return 0;
			}

			else {

				return 1;
			}
		}
	}

	if (board[currentp].type == 'K') { // Valid King moves

		int difference = currentp - nextp;

		if (board[currentp].castle == 1) { // can the king castle?

			if (difference == 2 && board[currentp - 4].castle == 1) { // castle queenside, rook can't have moved

				if (board[currentp - 1].type == '#' && board[currentp - 2].type == '#' && board[currentp - 3].type == '#' && in_check_tile(player_color, currentp - 1, board) == 0) { // spaces must be open, can't castle through check

					if (in_check(player_color, board) == 1) { // Can't castle out of check
					
						return 0;
					}

					return 1;
				}

				else {

					return 0;
				}
			}

			if (difference == -2 && board[currentp + 3].castle == 1) {// Castle kingside, rook can't have moved

				if (board[currentp + 1].type == '#' && board[currentp + 2].type == '#' && in_check_tile(player_color, currentp + 1, board) == 0) { // spaces must be open, can't castle through check

					if (in_check(player_color, board) == 1) { // Can't castle out of check

						return 0;
					}
					
					return 1;
				}

				else {

					return 0;
				}
			}
		}

		if (currentp % 8 > 0 && currentp % 8 < 7) { //if king is in the center files

			if (abs(difference) == 8 || abs(difference) == 1 || abs(difference) == 7 || abs(difference) == 9) {

				if (board[nextp].type != '#' && board[nextp].color == player_color) { // Can't move on your own piece

					return 0;
				}

				else {

					return 1;
				}
			}
		}

		if (currentp % 8 == 7) { // king is in right file

			if (abs(difference) == 8 || difference == 1 || difference == -7 || difference == 9) { //dont check right tiles

				if (board[nextp].type != '#' && board[nextp].color == player_color) { // Can't move on your own piece

					return 0;
				}

				else {

					return 1;
				}
			}
		}

		if (currentp % 8 == 0) { // king is in left

			if (abs(difference) == 8 || difference == -1 || difference == 7 || difference == -9) { //dont check left tiles

				if (board[nextp].type != '#' && board[nextp].color == player_color) { // Can't move on your own piece

					return 0;
				}

				else {

					return 1;
				}
			}
		}
	}
	
	return 0;
}

int no_moves(int color,Tile * board) { // checks if one player has no valid moves 

	int n;
	int m;
	int valid;
	int currentp;
	int nextp;

	for (n = 0; n < 64; n++) {
	
		if (board[n].color == color && board[n].type != '#') { // only check possible moves if the piece is one of yours
		
			currentp = n; // update current location
			
			for (m = 0; m < 64; m++) { // scan for possible moves
			
				nextp = m; // update next location

				valid = valid_move(color , 1, board, nextp, currentp);

				if (valid == 1) { // if a valid move is found then restore currentp and nextp and return 0
				
					return 0;
				}
			}
		}
	}

	return 1;
}

int checkmate(int color, Tile* board) { // checks if a player is in checkmate


	
	if (in_check(color, board) == 1) {
	
		if (no_moves(color, board) == 1) {
		
			return 1;
		}
	}

	return 0;
}

void promotion(Tile* board) { // promotes any pawn on a back rank

	int n;
	char p;
	int correctp = 0;
	
	for (n = 0; n < 64; n++) {
	
		if (n >= 0 && n <= 7 && board[n].type == 'P') { // if the player gets a pawn to back rank (White)
			
			if (player_color_p == 0) { // player promotes
				
				while (correctp != 1) { // get new type

					cout << "What should the pawn promote too? (R) (N) (B) (Q)" << endl;
					cin >> p;

					if (p == 'Q' || p == 'R' || p == 'B' || p == 'N') {

						correctp = 1;
					}

					else {

						cout << "That is not a valid promotion." << endl;
					}
				}

				board[n].type = p; // update type
				
				if (p == 'Q') { //set weights
				
					board[n].weight = 90;
				}

				else if (p == 'R' || p == 'B') {
				
					board[n].weight = 50;
				}

				else {
				
					board[n].weight = 30;
				}
			}

			else { // computer promotes to queen
			
				board[n].type = 'Q';
				board[n].weight = 90;
			}

		}

		if (n >= 56 && n <= 63 && board[n].type == 'P') {
		
			if (player_color_p == 1) { // player promotes

				while (correctp != 1) { // get new type

					cout << "What should the pawn promote too? (R) (N) (B) (Q)" << endl;
					cin >> p;

					if (p == 'Q' || p == 'R' || p == 'B' || p == 'N') {

						correctp = 1;
					}

					else {

						cout << "That is not a valid promotion." << endl;
					}
				}

				board[n].type = p; // update type

				if (p == 'Q') { //set weights

					board[n].weight = 90;
				}

				else if (p == 'R' || p == 'B') {

					board[n].weight = 50;
				}

				else {

					board[n].weight = 30;
				}
			}

			else { // computer promotes to queen

				board[n].type = 'Q';
				board[n].weight = 90;
			}
		}
	}
}

void move_piece(Tile* board, int nextp, int currentp, int Auto_promote) { // Move piece data on board

	if (board[currentp].type == 'P' && abs(currentp - nextp) == 16) { // if a pawn was moved and it moved 2 set the enpassant tile

		if (board[currentp].color == 0) {

			board[nextp + 8].enpassant = 1;
		}

		else {

			board[nextp - 8].enpassant = 1;
		}
	}

	if (board[currentp].type == 'P' && board[nextp].enpassant == 1) { // For taking en passant
	
		if (board[nextp + 8].type == 'P') { // For white player
		
			board[nextp + 8].type = '#';
			board[nextp + 8].weight = 0;
			board[nextp + 8].color = 2;
		}

		if (board[nextp - 8].type == 'P') { // For black player

			board[nextp - 8].type = '#';
			board[nextp - 8].weight = 0;
			board[nextp - 8].color = 2;
		}
	}

	if (board[currentp].type == 'K' && currentp - nextp == 2) { // use for castling queenside

		board[currentp - 1].type = 'R'; // move the rook
		board[currentp - 1].color = board[currentp].color;
		board[currentp - 1].weight = 50;
		board[currentp - 1].castle = 0;
		board[currentp - 4].type = '#';
		board[currentp - 4].color = 2;
		board[currentp - 4].weight = 0;
		board[currentp - 4].castle = 0;
		board[currentp].castle = 0;
	}

	if (board[currentp].type == 'K' && currentp - nextp == -2) {

		board[currentp + 1].type = 'R'; // move the rook
		board[currentp + 1].color = board[currentp].color;
		board[currentp + 1].weight = 50;
		board[currentp + 1].castle = 0;
		board[currentp + 3].type = '#';
		board[currentp + 3].color = 2;
		board[currentp + 3].weight = 0;
		board[currentp + 3].castle = 0;
		board[currentp].castle = 0;
	}

	board[nextp].type = board[currentp].type; // update board data with the new move
	board[nextp].color = board[currentp].color;
	board[nextp].weight = board[currentp].weight;
	board[nextp].castle = 0;
	board[currentp].type = '#';
	board[currentp].color = 2;
	board[currentp].weight = 0;
	board[currentp].castle = 0;

	if (board[nextp].type == 'P' && (nextp / 8 == 0 || nextp / 8 == 7) && Auto_promote == 1) { // automatically promote
	
		board[nextp].type = 'Q';
		board[nextp].weight = 90;
	}
}

void Handle_Events(bool exit);

void player_move(Tile* board) { // lets player move
	
	int valid = 0;
	int nextp;
	int currentp;
	
	while (valid == 0) {
	
		get_coordinates(); //sets global variables to new player coordinates

		nextp = player_nextp; //set local variables to new global ones
		currentp = player_currentp;

		valid = valid_move(player_color_p , 1, board, nextp, currentp);
	}
	
	move_piece(board, nextp, currentp, 0);
	
	promotion(board); // check for promotion

	Handle_Events(exit); // Handle window events
	print_board(board); // print new board
	render_board(board); // render the new board
	SDL_RenderPresent(renderer); // update the screen
}

double board_score(Tile* board) { // Evaluate the current board state

	double white_score = 0;
	double black_score = 0;
	int n;
	double m;

	for (n = 0; n < 64; n++) { // go over each tile
	
		if (n >= 0 && n <= 7 && board[n].color == 0 && board[n].type == 'P') { // Give white a bonus for promotion
		
			white_score += 81;
		}

		if (n <= 63 && n >= 56 && board[n].color == 1 && board[n].type == 'P') { // Give black a bonus for promotion
		
			black_score += 81;
		}

		if (board[n].type == 'P') { // Give points for advanced pawns
		
			if (board[n].color == 0) { // Give .3 points for moving pawns forward
			
				m = n / 8; 
				//m = m * .3;
				m = 5 - m;
				white_score += m;
			}

			if (board[n].color == 1) { // Give .3 points for moving pawns forward
			
				m = n / 8;
				//m *= .3;
				black_score += m;
			}
		}

		if (n >= 0 && n <= 7 && board[n].color == 1 && board[n].type != '#' && board[n].type != 'K') { // Subtract points for pieces on back row
		
			black_score -= 2.5;
		}

		if (n >= 56 && n <= 63 && board[n].color == 0 && board[n].type != '#' && board[n].type != 'K') { // Subtract points for pieces on back row

			white_score -= 2.5;
		}
		
		if (board[n].color == 0 && board[n].type != '#') { // for each white piece
		
			white_score += board[n].weight; // add their weight too total score
		}

		if (board[n].color == 1 && board[n].type != '#') { // for each black piece
		
			black_score += board[n].weight; //add their weight too total score
		}
	}
	
	if (checkmate(0, board) == 1) { // Increase black_score if white is in checkmate
	
		black_score += 50000;
	}

	if (checkmate(1, board) == 1) {// Increase white_score if black is in checkmate
	
		white_score += 50000;
	}

	if (computer_color_p == 0) {
	
		return white_score - black_score;
	}

	else {
	
		return black_score - white_score;
	}
}

double alphabeta(chess_node current, int depth, double a, double b, int maximizingplayer, int color) { //alpha beta function, searches for best move

	double n;
	int m;
	int x;
	double v;
	double value;
	int size;
	chess_node child;
	
	if (current.currentn != -1) { // dont try to move a piece on the first node
		
		move_piece(current.new_board, current.nextn, current.currentn, 1); // update the board, auto promote
	}
	
	if (depth == 0) { // return the value of the board at the end of the tree
		
		value = board_score(current.new_board);
		return value;
		
	}

	for (n = 0; n < 64; n++) { // this section scans for possible moves and creates a child node for each one
	
		for (m = 0; m < 64; m++) { // check every tile for possible moves
		
			if (valid_move(color, 1, current.new_board, m, n) == 1) { 
				
				//cout << conversion[n] << " " << conversion[m] << endl;
				
					child.nextn = m; // store the move for the child board
					child.currentn = n;
								
				for (x = 0; x < 64; x++) { //make the child board equal to the current board
				
					child.new_board[x].castle = current.new_board[x].castle;
					child.new_board[x].color = current.new_board[x].color;
					child.new_board[x].color_b = current.new_board[x].color_b;
					child.new_board[x].enpassant = current.new_board[x].enpassant;
					child.new_board[x].type = current.new_board[x].type;
					child.new_board[x].weight = current.new_board[x].weight;
				}
				
				current.children.push_back(child); // push the child node into the vector
			}
		}
	}

	size = current.children.size(); // get the number of elements

	moves_considered += size; // Track number of moves considered

	//cout << size << " " << Depth + extra_depth<< " " << depth << endl;
	
	if (size == 0) { // also return if there are no possible moves
		
		value = board_score(current.new_board);
		return value;
	}

	if (maximizingplayer == 1) {
	
		v = -10000;
		
		for (x = 0; x < size; x++) { // for each child
		
			//v = max(v, alphabeta(current.children[x], depth - 1, a, b, 0));
			
			n = alphabeta(current.children[x], depth - 1, a, b, 0, !computer_color_p);
			
			if (n > v) { // set globals for maximizing layers to find the best move
				
				if (depth == Depth + extra_depth) {
					
					computer_currentp = current.children[x].currentn;
					computer_nextp = current.children[x].nextn;
				}
				
				v = n;
			}
			
			a = max(a, v);
			
			if (b <= a) {
			
				break;
			}
		}
			
		return v;
	}

	else {
	
		v = 10000;

		for (x = 0; x < size; x++) { // for each child
		
			v = min(v, alphabeta(current.children[x], depth - 1, a, b, 1, computer_color_p));
			b = min(b, v);

			if (b <= a) {
				
				break;
			}
		}

		return v;
	}

	return 0;
}

void computer_move(Tile* board) { // Computer takes its turn
	
	int x;
	double best;
	int valid = 0;
	int m;
	int n;
	int moves = 0;
	int moves_p = 0;
	

	chess_node root;
	root.currentn = -1;
	root.nextn = -1;
	
	for (x = 0; x < 64; x++) { // make the root board the current board state

		root.new_board[x].castle = board[x].castle;
		root.new_board[x].color = board[x].color;
		root.new_board[x].color_b = board[x].color_b;
		root.new_board[x].enpassant = board[x].enpassant;
		root.new_board[x].type = board[x].type;
		root.new_board[x].weight = board[x].weight;
	}

	moves_considered = 0;
	
	best = alphabeta(root, 5, -10000, 10000, 1, computer_color_p); // calculate the best move, second input is # of layers

	while (valid != 1) { // output an invalid move

		valid = 0;
		valid = valid_move(computer_color_p, 1, board, computer_nextp, computer_currentp);
		
		if (valid == 0) {
			
			cout << conversion[computer_currentp] << " " << conversion[computer_nextp] << endl;
		}
	}

	if (extra_info == 1) {	// output the information of the current and next squares to help with debug
		
		cout << "Current Square Info:" << endl << "Location: " << computer_currentp << endl << "Type: " << board[computer_currentp].type << endl << "Color: " << board[computer_currentp].color << endl <<
			"Enpassant: " << board[computer_currentp].enpassant << endl << endl;

		cout << "Next Square Info:" << endl << "Location: " << computer_nextp << endl << "Type: " << board[computer_nextp].type << endl << "Color: " << board[computer_nextp].color << endl <<
			"Enpassant: " << board[computer_nextp].enpassant << endl << endl;

		cout << moves_considered << " moves considered" << endl << endl;
	}
	
	move_piece(board, computer_nextp, computer_currentp, 1); // move the piece
	
	promotion(board); // check for promotion

	Handle_Events(exit); // Handle window events
	print_board(board); // print new board
	render_board(board); // render the new board
	SDL_RenderPresent(renderer); // update the screen

	cout << "The computer has played " << conversion[computer_currentp] << " to " << conversion[computer_nextp] << endl;
}

void clear_enpassant(Tile* board,int color) { // Reset en passant markers

	int x = 0;
	
	for (x = 0; x < 64; x++) { // clear enpassants of certain color
	
		if (board[x].enpassant == 1 && x > 15 && x < 24 && color == 1) { // black enpassant
		
			board[x].enpassant = 0;
		}

		if (board[x].enpassant == 1 && x > 39 && x < 48 && color == 0) {
		
			board[x].enpassant = 0;
		}
	}
}

bool SDL_init() { // initializes SDL, creates window, creates renderer

	bool success = true;

	// initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) { // if initialization failed output the error message
	
		cout << "SDL could not initialize. SDL error:" << SDL_GetError() << endl;
		success = false;
	}

	else { // if it did not fail create the window
	
		//window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, SDL_WINDOW_SHOWN); // create the window
		
		if (!window.init()) {
		
			cout << "Window was not created. SDL Error:" << SDL_GetError() << endl;
			success = false;
		}
		
		else { // Create the Renderer
		
			//renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			renderer = window.createRenderer();

			if (renderer == NULL) {
			
				cout << "Failed to create renderer. SDL Error: " << SDL_GetError() << endl;
				success = false;
			}

			else { // if successful then initialize color and PNG loading

				SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

				int image_flag = IMG_INIT_PNG;

				if (!(IMG_Init(image_flag)) & image_flag) {

					cout << "SDL_Image could not initialize. SDL Error:" << SDL_GetError() << endl;
					success = false;
				}

				else {
					SDL_Window* temp = window.getWindow();
					ScreenSurface = SDL_GetWindowSurface(temp); // gets the windows surface
				}
			}
		}
		
	}

	return success;
}

SDL_Texture* get_texture(std::string path) {

	SDL_Texture* finaltexture = NULL;

	SDL_Surface* currentsurface = IMG_Load(path.c_str()); // load the surface from a given path

	if (currentsurface == NULL) {
	
		cout << "Image could not be loaded. Error:" << SDL_GetError() << endl;
	}

	else {
	
		SDL_SetColorKey(currentsurface, SDL_TRUE, SDL_MapRGB(currentsurface->format, 0, 0xFF, 0xFF)); // set cyan 0 255 255 as transparent
		finaltexture = SDL_CreateTextureFromSurface(renderer, currentsurface);
		SDL_FreeSurface(currentsurface);
	}

	return finaltexture;
}

void cleanup() { // cleans up SDL pointers and functions

	SDL_DestroyTexture(Board);
	Board = NULL;
	SDL_DestroyTexture(BoardB);
	BoardB = NULL;
	SDL_DestroyTexture(WKing);
	WKing = NULL;
	SDL_DestroyTexture(WQueen);
	WQueen = NULL;
	SDL_DestroyTexture(WBishop);
	WBishop = NULL;
	SDL_DestroyTexture(WKnight);
	WKnight = NULL;
	SDL_DestroyTexture(WRook);
	WRook = NULL;
	SDL_DestroyTexture(WPawn);
	WPawn = NULL;
	SDL_DestroyTexture(BKing);
	BKing = NULL;
	SDL_DestroyTexture(BQueen);
	BQueen = NULL;
	SDL_DestroyTexture(BBishop);
	BBishop = NULL;
	SDL_DestroyTexture(BKnight);
	BKnight = NULL;
	SDL_DestroyTexture(BRook);
	BRook = NULL;
	SDL_DestroyTexture(BPawn);
	BPawn = NULL;

	SDL_DestroyRenderer(renderer);
	renderer = NULL;
	//SDL_DestroyWindow(window);
	//window = NULL;
	window.free();

	IMG_Quit;
	SDL_Quit;
	
}

void load_images() { // load all of the chess graphics

	Board = get_texture("Chess_Pieces/Chessboard.png"); 
	BoardB = get_texture("Chess_Pieces/ChessboardB.png");
	WKing = get_texture("Chess_Pieces/WKing.png");
	WQueen = get_texture("Chess_Pieces/WQueen.png");
	WBishop = get_texture("Chess_Pieces/WBishop.png");
	WKnight = get_texture("Chess_Pieces/WKnight.png");
	WRook = get_texture("Chess_Pieces/WRook.png");
	WPawn = get_texture("Chess_Pieces/WPawn.png");
	BKing = get_texture("Chess_Pieces/BKing.png");
	BQueen = get_texture("Chess_Pieces/BQueen.png");
	BBishop = get_texture("Chess_Pieces/BBishop.png");
	BKnight = get_texture("Chess_Pieces/BKnight.png");
	BRook = get_texture("Chess_Pieces/BRook.png");
	BPawn = get_texture("Chess_Pieces/BPawn.png");

}

void Handle_Events(bool exit) {

	while (SDL_PollEvent(&event) != 0){
		//User requests quit
		if (event.type == SDL_QUIT)
		{
			exit = true;
		}

		//Handle window events
		window.handleEvent(event);
	}

}

int main(){

	Tile board[64]; // create the board
	
	bool init_flag;
	bool exit = false;

	init_flag = SDL_init(); // initialize SDL and create 400 x 400 window

	if (init_flag = false) {
	
		cout << "Initialization failed." << endl;
	}

	load_images(); // get all the chess images
	
	Handle_Events(exit); // Handle window events 
	SDL_RenderClear(renderer); // clear the screen
	SDL_RenderCopy(renderer, Board, NULL, NULL); // render the board to the screen
	SDL_RenderPresent(renderer); // update the screen
	
	checkmate_p = 0;
	checkmate_c = 0;
	
	
	set_board(board); // set up the board

	get_extra_info(); // Let player choose if extra information should be shown

	get_player_color(); // Let the player choose their pieces
	
	print_board(board); // print the board

	Handle_Events(exit); // Handle initial events
	render_board(board); // render board
	SDL_RenderPresent(renderer);
	
	if (player_color_p == 0) { // player moves first if white
		
		player_move(board);
	}
	
	while (checkmate_p == 0 && checkmate_c == 0 && exit == false) { // main game loop
	
		//Handle events on queue
		Handle_Events(exit);

		computer_move(board);
		clear_enpassant(board , player_color_p); // clear player enpassants. Was initially white

		if (checkmate(player_color_p, board) == 1) {
		
			checkmate_p = 1;
		}
		
		if (checkmate_p == 0) { // skip players turn if they are in checkmate
			
			player_move(board);
			clear_enpassant(board, computer_color_p); // clear computer en passants. Was initially black
		}

		if (checkmate(computer_color_p, board) == 1) {
		
			checkmate_c = 1;
		}
	}
	
	if (checkmate_p == 1) { // end game message
	
		cout << "Checkmate. You lose." << endl;
	}

	if (checkmate_c == 1) {
	
		cout << "Checkmate. You win." << endl;
	}
	
	cleanup();
	
	return 0;
}

