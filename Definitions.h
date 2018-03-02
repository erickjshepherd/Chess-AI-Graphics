#ifndef Definitions
#define Definitions

#include <SDL.h>
#include <SDL_image.h>
#include "Window.h"
#include <vector>

using namespace std;

struct Tile {

	int color; //color of piece
	int color_b; // color of board
	int weight; //piece worth
	char type; //type of piece
	int enpassant;
	int castle;
};

//Tile board[64];

int player_currentp;
int player_nextp;
int player_color_p;
int computer_color_p;
int checkmate_p;
int checkmate_c;
int computer_currentp;
int computer_nextp;
int extra_depth;
int moves_considered;
int extra_info;

LWindow window; // the window
SDL_Surface* ScreenSurface = NULL; // the window surface
//SDL_Renderer* renderer = NULL; // the renderer
SDL_Rect piecesquare = { 0, 0, 50, 50 }; // square with W/H 50, change x/y to render

SDL_Texture* Board = NULL; // the board
SDL_Texture* BoardB = NULL; // the black board
SDL_Texture* WKing = NULL; // White King
SDL_Texture* WQueen = NULL; // White Queen
SDL_Texture* WBishop = NULL; // White Bishop
SDL_Texture* WKnight = NULL; // White Knight
SDL_Texture* WRook = NULL; // White Rook
SDL_Texture* WPawn = NULL; // White Pawn
SDL_Texture* BKing = NULL; // Black King
SDL_Texture* BQueen = NULL; // Black Queen
SDL_Texture* BBishop = NULL; // Black Bishop
SDL_Texture* BKnight = NULL; // Black Knight
SDL_Texture* BRook = NULL; // Black Rook
SDL_Texture* BPawn = NULL; // Black Pawn

SDL_Event event;


//int screen_width = 400;
//int screen_height = 400;

char* conversion[] = { "a8" , "b8" , "c8" , "d8" , "e8" , "f8" , "g8" , "h8",
					   "a7" , "b7" , "c7" , "d7" , "e7" , "f7" , "g7" , "h7",
	                   "a6" , "b6" , "c6" , "d6" , "e6" , "f6" , "g6" , "h6", 
					   "a5" , "b5" , "c5" , "d5" , "e5" , "f5" , "g5" , "h5", 
	                   "a4" , "b4" , "c4" , "d4" , "e4" , "f4" , "g4" , "h4", 
	                   "a3" , "b3" , "c3" , "d3" , "e3" , "f3" , "g3" , "h3", 
	                   "a2" , "b2" , "c2" , "d2" , "e2" , "f2" , "g2" , "h2", 
	                   "a1" , "b1" , "c1" , "d1" , "e1" , "f1" , "g1" , "h1", };


struct chess_node { //node for board tree

	Tile new_board[64]; //boardstate
	
	int currentn; // store the move
	int nextn;
	int score;	
	int currentn_p; // store previous move
	int nextn_p; // store previous move

	Tile* parent;
	
	vector<chess_node> children;

};

#endif