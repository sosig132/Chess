//TODO: king check(the check function doesn't work, but if the king gets taken the game closes, but there is no checkmate so no option to keep the score)


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <array>
#include <string.h>

using namespace std;

//class Board;
//class Piece;
//class King : |
//class Pawn;  |
//class Queen; |
//class Bishop;|	PUBLIC PIECES
//class Knight;|
//class Rook;  |
//class Game   

class Game;

//----------------------------------------------------------------
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string.h>
#include <iostream>
#include <string>

using namespace std;

class Board{
private:
	friend class Game;
	friend class Piece;
	//Game window dimensions
	
	const int WINDOW_WIDTH;
	const int WINDOW_HEIGHT;
	const int CELL_WIDTH;
	const int CELL_HEIGHT;

	int cellNumber;

	char* boardName;

	//Game window

	SDL_Window* window;

	//Game renderer
	SDL_Renderer* renderer;


	//Window surface

	SDL_Surface* surface;


public:	
	int getCellNumber(){
		return cellNumber;
	}

	void setCellNumber(int cel){
		cellNumber=cel;
	}

	int getCellHeight(){
		return CELL_HEIGHT;
	}

	int getCellWidth(){
		return CELL_WIDTH;
	}

	int getWindowHeight(){
		return WINDOW_HEIGHT;
	}

	int getwindowWidth(){ 
		return WINDOW_WIDTH;
	}

	void setBoardName(char* boardnam){
		strcpy(boardName, boardnam);
	}

	char* getBoardName(){
		return boardName;
	}

	//Events like mouse click
	SDL_Event event;


	//For loading images

	SDL_Texture* loadImage(string filename);

	//default constructor

	Board();

	//all parameter constructor

	Board(SDL_Window* window, SDL_Renderer* renderer, SDL_Surface* surface, const int WINDOW_WIDTH, const int WINDOW_HEIGHT, const int CELL_WIDTH, const int CELL_HEIGHT);

	//copy constructor

	Board(Board& board);

	//initializing SDL

	bool init();

	//cleaning any SDL stuff

	void clean();

	//drawing rectangles(for rendering pieces)

	void drawRect(SDL_Rect source, SDL_Rect destination, SDL_Texture* texture);

	//Rendering the background

	void renderBackground();
	
	//deleting pieces from graphical board
	
	void undoPiece(int coordX, int coordY);

	//destructor

	~Board();

	//>>operator

	friend istream& operator>>(istream& in, Board& board);

	//<<operator

	friend ostream& operator<<(ostream& out, const Board& board);

	//=operator

	Board& operator=(const Board& board);

	//==operator

	bool operator==(const Board& board);

	//>operator

	bool operator>(const Board& board);

	//cast explicit operator

	explicit operator double(){
		return double(this->cellNumber);
	}

	//+operator

	Board operator+(int x);
	friend Board operator+(Board board, int x){
		board.cellNumber= board.cellNumber+x;
		return board;
	}
	//-operator

	Board operator-(int x);
	friend Board operator-(Board board, int x){
		board.cellNumber=board.cellNumber-x;
		return board;
	}
	//++operator

	Board operator++();
	Board operator++(int);

	//[]operator

	char operator[](int index);
};
//================ OPERATOR OVERLOADING ===============

char Board::operator[](int index){
	if(0<=index&&index<=strlen(this->boardName)){
		return boardName[index];
	} 
	std::cout<<"Wrong index";
}

Board Board::operator++(int){
	Board aux(*this);
	
	this->cellNumber++;

	return aux;
}

Board Board::operator++(){
	this->cellNumber++;
	return *this;
}

Board Board::operator-(int x){
	this->cellNumber = this->cellNumber-x;
	return *this;
}

Board Board::operator+(int x){
	this->cellNumber = this->cellNumber+x;
	return *this;
}

bool Board::operator>(const Board& board){
	return this->WINDOW_WIDTH*this->WINDOW_HEIGHT>board.WINDOW_WIDTH*board.WINDOW_HEIGHT;
}

bool Board::operator==(const Board& board){
	
	return this->window==board.window&&this->renderer==board.renderer&&this->surface==board.surface;

}

Board& Board::operator=(const Board& board){
	if(this != &board){
		if(this->boardName!=NULL){
			delete[] boardName;
		}
		this->boardName = new char[strlen(board.boardName)+1];
		strcpy(this->boardName, board.boardName);

		this->window = board.window;
		this->renderer = board.renderer;
		this->surface = board.surface;
	}
	
	return *this;
}

ostream& operator<<(ostream& out, const Board& board){
	out<<"\nBoard Height: "<< board.WINDOW_HEIGHT;
	out<<"\nBoard Width: "<<board.WINDOW_WIDTH;
	out<<"\nCell Height: "<<board.CELL_HEIGHT;
	out<<"\nCell Width: "<<board.CELL_WIDTH;

	return out;
}

istream& operator>>(istream& in, Board& board){
	std::cout<<"Game name: ";
	char aux[500];
	in>>aux;

	if(board.boardName != NULL){
		delete[] board.boardName;
	}
	board.boardName = new char[strlen(aux)+1];
	strcpy(board.boardName, aux);
	return in;
}

//================END OF OPERATOR OVERLOADING===================

//================ DEFINITIONS OF SDL FUNCTIONS ================

Board::Board(Board& board) : WINDOW_WIDTH(board.WINDOW_WIDTH), WINDOW_HEIGHT(board.WINDOW_HEIGHT), CELL_WIDTH(board.CELL_WIDTH), CELL_HEIGHT(board.CELL_HEIGHT){
	window = NULL;
	surface = NULL;
	renderer = NULL;
	
	init();
}

Board::Board(): WINDOW_WIDTH(640), WINDOW_HEIGHT(640), CELL_WIDTH(WINDOW_WIDTH/8), CELL_HEIGHT(WINDOW_HEIGHT/8){
	
	window = NULL;
	surface = NULL;
	renderer = NULL;
	
	init();
}

Board::Board(SDL_Window* window, SDL_Renderer* renderer, SDL_Surface* surface, const int WINDOW_WIDTH, const int WINDOW_HEIGHT, const int CELL_WIDTH, const int CELL_HEIGHT) : WINDOW_WIDTH(640), WINDOW_HEIGHT(640), CELL_WIDTH(WINDOW_WIDTH/8), CELL_HEIGHT(WINDOW_HEIGHT/8) {
	
	window = NULL;
	surface = NULL;
	renderer = NULL;
	init();
}

void Board::drawRect(SDL_Rect source, SDL_Rect destination, SDL_Texture* texture) {
	if(texture){
		SDL_RenderCopy(renderer, texture, &source, &destination);
		SDL_RenderPresent(renderer);
	}
}

void Board::renderBackground(){
	bool white = true;
	SDL_SetRenderDrawColor(renderer,255,255,255,255);	

	for(int i = 0; i < 8; i++){

		for(int j = 0; j < 8; j++){

			if(white){
				//white
				SDL_SetRenderDrawColor(renderer,160,255,255,255);

			}

			else{
				//green
				SDL_SetRenderDrawColor(renderer, 155, 103, 60, 255);

			}

			white=!white;

			SDL_Rect square = {i*CELL_WIDTH, j*CELL_HEIGHT, CELL_WIDTH, CELL_HEIGHT};
			
			SDL_RenderFillRect(renderer, &square);
		}
		white=!white;
	}

}

void Board::undoPiece(int coordX, int coordY){
	
	if((coordX + coordY)%2==0){

		SDL_SetRenderDrawColor(renderer,160,255,255,255);
	
	}

	else{

		SDL_SetRenderDrawColor(renderer, 155, 103, 60, 255);

	}

	SDL_Rect square = {coordX * CELL_WIDTH, coordY * CELL_HEIGHT, CELL_WIDTH, CELL_HEIGHT};

	SDL_RenderFillRect(renderer, &square);

}

SDL_Texture* Board::loadImage(string filename){
	//loading image
	if(IMG_Init(IMG_INIT_PNG)<0){
		std::cout<<"Couldn't initialize SDL_Image: "<<SDL_GetError()<<endl; 
	}

	SDL_Surface* image=NULL;
	image=IMG_Load(filename.c_str());

	if(image==NULL){
		
		std::cout<<"Couldn't load "<<filename<<":"<<SDL_GetError<<endl;

	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);

	return texture;

}

//INITIALIZING SDL(window, renderer, and surface)

bool Board::init(){

	if(SDL_Init(SDL_INIT_VIDEO)<0){

		std::cout<<"Couldn't initialize SDL: "<<SDL_GetError()<<endl;
		clean();
		return false;

	}
	std::cout<<"SDL initialized!"<<endl;
	window=SDL_CreateWindow("Chess(not finished)", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

	if(window==NULL){

		std::cout<<"Couldn't initialize window: "<<SDL_GetError<<endl;

	}

	else{
		std::cout<<"Window initialized!"<<endl;
		renderer=SDL_CreateRenderer(window, -1, 0);
		
		//  if (renderer) {
		//  	surface=SDL_GetWindowSurface(window);
		//  	std::std::cout << "Renderer created!"<<endl;
		//   	renderBackground();
		//  	SDL_RenderPresent(renderer);
		//  }
    }
	return true;
}
	

void Board::clean(){

	//destroying everything

	SDL_FreeSurface(surface);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	//quitting SDL

	SDL_Quit();

}

Board::~Board(){	
    clean();
}

//================END OF DEFINITIONS OF SDL FUNCTIONS================

//----------------------------------------------------------------
class King;
//----------------------------------------------------------------
class Piece{
public:
	//type of piece
	enum class PieceType {Empty, PAWN, ROOK, BISHOP, KNIGHT, QUEEN, KING};
	
	//piece's team
	enum class PieceTeam {Empty, WHITE, BLACK};

	//type of move a piece does
	enum class MoveType {Empty, NORMAL, CASTLE, EN_PASSANT, PROMOTE};


	//position of piece
	struct PiecePosition{
		int xCoord, yCoord;
		MoveType moveType;
	};
	//default constructor
	Piece();

	//parameter constructor
	Piece(PieceType type, PieceTeam team, PiecePosition position, Board* handler);

	//copy-constructor
	Piece(Piece& piece);

	//to be overridden by derived classes

	virtual void calculatePossibleMoves( Piece* pieceField[8][8], bool checkIfChecked) = 0;

	//setters and getters

	void setType(PieceType types){
		type=types;
	}

	PieceType getType(){
		return type;
	}

	void setTeam(PieceTeam teams){
		team=teams;
	}

	PieceTeam getTeam(){
		return team;
	}

	void setPosition(PiecePosition position){
		pos=position;
	}

	PiecePosition getPosition(){
		return pos;
	}

	vector<PiecePosition> getPossibleMoves(){
		return possibleMoves;
	}

	void setHasMoved(bool moved){
		hasMoved=moved;
	}

	bool getHasMoved(){
		return hasMoved;
	}

	void setMoveNumber(){
		moveNumber=0;
	}

	int getMoveNumber(){
		return moveNumber;
	}

	void setMoveColumn(char col){
		moveColumn=col;
	}

	char getMoveColumn(){
		return moveColumn;
	}

	void setMoveRow(char row){
		moveRow=row;
	}

	char getMoveRow(){
		return moveRow;
	}

	void setMovePiece(char piece){
		movePiece=piece;
	}

	char getMovePiece(){
		return movePiece;
	}

	//for rendering pieces
	void render();
	
	void piecePresent();

	//destructor
	~Piece();

	//=operator

	Piece& operator=(const Piece& p);

	//==operator

	bool operator==(const Piece& p);

	//>operator

	bool operator>(const Piece& p);

	//++operator(and operator++)

	Piece& operator++();

	Piece& operator++(int);

	//+operator

	Piece& operator+(int x);
	friend Piece& operator+(int x, Piece& p){
		p.moveNumber=p.moveNumber+x;
		return p;
	}
	//-operator

	Piece& operator-(int x);
	friend Piece& operator-(int x, Piece& p){
		p.moveNumber = p.moveNumber-x;
		return p;
	}

	//>>operator

	friend istream& operator>>(istream& in, Piece& p);

	//<<operator

	friend ostream& operator<<(ostream& out, const Piece& p);

	//[]operator

	char* operator[](int index);

	//explicit cast

	explicit operator double(){
		return (double)this->moveNumber;
	}	

	//move recording

	void moveRecord(char moveRow, char moveColumn, char movePiece);

	void setPoints(int point){
		points=point;
	}

	int getPoints(){
		return points;
	}

protected:
	
	//for 50 move rule

	static int movesFromLastCapture;

	//points of piece
	int points;

	//checks if piece has moved

	bool hasMoved;

	//for chess engine(not implemented)

	int moveNumber;

	//for recording moves, pawn = p, bishop = b etc

	char moveColumn;
	char moveRow;
	char movePiece;
	vector<char*> moveRecording;

	//where a piece can go

	vector<PiecePosition> possibleMoves;

	//if move is allowed, pushes it 
	//checks if the king is still in check after the move is made and if so, doesn't allow it
	//if checkIfChecked is true then checks if the move is allowed after the above simulation

	vector<PiecePosition> makeMove(vector<PiecePosition> moves, PiecePosition move, King* king, Piece* backBoard[8][8], bool checkIfChecked);

	//piece's team

	PieceTeam team;

	//piece's type

	PieceType type;

	//piece's position

	PiecePosition pos;

	//SDL 
	SDL_Texture* texture;
	
	Board* univHandler;

	//getter for king of own team

	King* teamsKing(Piece* backBoard[8][8]);
};
//==================OPERATOR OVERLOADING=============
char* Piece::operator[](int index){
	if(0<=index && index < this->moveRecording.size())
		return this->moveRecording[index];
	cout<<"Wrong index";
}

istream& operator>>(istream& in, Piece& p){
	cout<<"\n Introdu numele piesei(prima litera): ";
	in>>p.movePiece;

	cout<<"\n Introdu coloana pe care ai mutat piesa(a,b,c etc): ";
	in>>p.moveColumn;

	cout<<"\n Introdu randul pe care ai mutat piesa(1,2,3 etc): ";
	in>>p.moveRow;

	return in;
}

ostream& operator<<(ostream& out, const Piece& p){
	out<<p.movePiece;
	out<<p.moveColumn;
	out<<p.moveRow;	

	return out;
}

Piece& Piece::operator+(int x){
	this->moveNumber = this->moveNumber+x;
	return *this;
}

Piece& Piece::operator-(int x){
	this->moveNumber=this->moveNumber-x;
	return *this;
}

Piece& Piece::operator++(){
	Piece& aux = *this;
	this->moveNumber++;
	return aux;
}

Piece& Piece::operator++(int){
	this->moveNumber++;
	return *this;
}

bool Piece::operator>(const Piece& p){
	return this->moveNumber>p.moveNumber;
}

bool Piece::operator==(const Piece& p){
	return this->team == p.team&&this->type == p.type&&this->pos.xCoord == p.pos.xCoord&&this->pos.yCoord==p.pos.yCoord&&this->texture == p.texture&&this->univHandler == p.univHandler;
}

Piece& Piece::operator=(const Piece& p){
	if(this!=&p){
		this->team = p.team;
		this->type = p.type;
		this->pos = p.pos;
		this->texture = p.texture;
		this->univHandler = p.univHandler;
	}
	
	return *this;
}
//==================END OF OPERATOR OVERLOADING======

//==================PIECE FUNCTIONS==================
int Piece::movesFromLastCapture = 0;
//for 50 move rule(not implemented)

void Piece::moveRecord(char moveRow, char moveColumn, char movePiece){
	char aux[4];
	aux[0]=movePiece;
	aux[1]=moveColumn;
	aux[2]=moveRow;
	moveRecording.push_back(aux);
}


void Piece::piecePresent(){
	cout<<"There is no piece here!"<<endl;
}

Piece::Piece(PieceType type, PieceTeam team, PiecePosition position, Board* handler) : type(type), team(team), univHandler(handler), texture(NULL), hasMoved(false), pos(position){}

Piece::Piece(Piece& piece) :  type(piece.type), team(piece.team), univHandler(piece.univHandler), texture(NULL), hasMoved(false), pos(piece.pos){}

void Piece::render(){
	SDL_Rect source = {0,0,60,60};

	//the integers below are for centering the pieces on the squares

	SDL_Rect destination = {univHandler->getCellWidth()*pos.xCoord+10,univHandler->getCellHeight()*pos.yCoord+10, univHandler->getCellWidth()-20, univHandler->getCellHeight()-20};
	univHandler->drawRect(source, destination, texture);
}

Piece::~Piece(){
	SDL_DestroyTexture(texture);

	univHandler->undoPiece(pos.xCoord, pos.yCoord);
}

King* Piece::teamsKing(Piece* backBoard[8][8]){
	for(int i = 0; i < 8; i++){

		for (int j = 0; j < 8; j++){

			if(backBoard[i][j] && backBoard[i][j]->getTeam()==team && backBoard[i][j]->getType()==Piece::PieceType::KING){
				return (King*)(backBoard[i][j]);
			}
		}
	} 
	return nullptr;
}

//=====================END OF PIECE FUNCTIONS===============


//----------------------------------------------------------
class King : public Piece{
private:

	//if king is in check
	bool isChecked;

public:

	//Constructor with parameters
	King(PieceType type, PieceTeam team, PiecePosition position, Board* handler);

	//king possible moves

	void calculatePossibleMoves(Piece* backBoard[8][8], bool checkIfChecked);

	//makes king get checked
	
	void setCheck(Piece* backBoard[8][8], int x, int y);

	//if king is in check or no

	bool getCheck() { 
		return isChecked; 
	}

	~King();

};
//---------------------------------------------------------

//======================KING FUNCTIONS=====================
King::~King(){}

void King::setCheck(Piece* backBoard[8][8], int x, int y){
	bool check = false;

	for(int i = 0; i < 8; i++){
		for(int j = 0; j < 8; j++){
			if(backBoard[i][j]&&backBoard[i][j]->getTeam()!=team){
				if(backBoard[i][j]->getType()==PieceType::KING){
					
					//checks if king is next to king

					if(abs(backBoard[i][j]->getPosition().xCoord-x)<=1 && abs(backBoard[i][j]->getPosition().yCoord-y)<=1){
						check = true;
					}
					
					//checks if pawn can attack king
					
					else if(backBoard[i][j]->getType()==PieceType::PAWN){
						int pawnMovementDirection;
						if(backBoard[i][j]->getTeam()==PieceTeam::WHITE){
							pawnMovementDirection = 1;
						}
						else{
							pawnMovementDirection = -1;
						}
						if((backBoard[i][j]->getPosition().xCoord+1==x || backBoard[i][j]->getPosition().xCoord-1==x) && (backBoard[i][j]->getPosition().yCoord + pawnMovementDirection == y)){
							check = true;
						}
					}
					
					//calculates possible moves for everything else and checks if anything can attack king

					else{
						backBoard[i][j]->calculatePossibleMoves(backBoard, false);
						vector<PiecePosition> notPossible = backBoard[i][j]->getPossibleMoves();
						for(auto& move : notPossible){
							if(move.xCoord == x&&move.yCoord == y){
								check = true;
							}
						}
					}
				}
			}
		}
	}
	isChecked=check;
}


King::King(PieceType type, PieceTeam team, PiecePosition position,Board* handler) : Piece(PieceType::KING, team, position, handler){
	string filename;

	if(team == PieceTeam::WHITE){
		filename = "img/Chess_klt60.bmp"; 
	}

	else if(team == PieceTeam::BLACK){
		filename = "img/Chess_kdt60.bmp";
	}
	univHandler = handler;
	texture = handler->loadImage(filename);

	render();

	cout<<"King rendered"<<endl;
}

	
vector<Piece::PiecePosition> Piece::makeMove(vector<PiecePosition> movesList, PiecePosition move, King* king, Piece* backBoard[8][8], bool checkIfChecked){
	if(!checkIfChecked){
		movesList.push_back(move);
	}
	else{
		bool isEnemyHere = true;
		king->setCheck(backBoard, king->getPosition().xCoord, king->getPosition().yCoord);
		Piece* jman = &(*backBoard[move.xCoord][move.yCoord]);
		isEnemyHere = false;
		if(backBoard[move.xCoord][move.yCoord]){
			isEnemyHere = true;
			backBoard[move.xCoord][move.yCoord] = nullptr;
		}

		swap(backBoard[move.xCoord][move.yCoord], backBoard[pos.xCoord][pos.yCoord]);

		if(type == PieceType::KING){
			king->setCheck(backBoard, move.xCoord, move.yCoord);
		}	
		else{
			king->setCheck(backBoard, king->getPosition().xCoord, king->getPosition().yCoord);
		}

		swap(backBoard[move.xCoord][move.yCoord], backBoard[pos.xCoord][pos.yCoord]);

		if(isEnemyHere){
			backBoard[move.xCoord][move.yCoord] = &(*jman);
		}

		if(!king->getCheck()){
			movesList.push_back(move);
		}

		king->setCheck(backBoard, king->getPosition().xCoord, king->getPosition().yCoord);


	}
	return movesList;
}


void King::calculatePossibleMoves(Piece* backBoard[8][8], bool checkIfChecked){

	PiecePosition cx = pos;

	vector<PiecePosition> moves;

	bool castle = 0;

	//can move only in a square around him
	for(int x = -1; x <= 1; x++){

		for(int y = -1; y <= 1; y++){

			//checking bounds of board(same for every piece)

			if(cx.xCoord+x>=0 && cx.yCoord+y>=0 && cx.xCoord+x<=7 && cx.yCoord+y<=7){
				if(backBoard[cx.xCoord+x][cx.yCoord+y]){
					if(backBoard[cx.xCoord+x][cx.yCoord+y]->getTeam() != team){
						cx.xCoord+=x;
						cx.yCoord+=y;
						cx.moveType = Piece::MoveType::NORMAL;
						moves = makeMove(moves, cx, teamsKing(backBoard), backBoard, checkIfChecked);
						cx.xCoord-=x;
						cx.yCoord-=y;
					}
				}
				else{
					cx.xCoord+=x;
					cx.yCoord+=y;
					cx.moveType = Piece::MoveType::NORMAL;
					moves = makeMove(moves, cx, teamsKing(backBoard), backBoard, checkIfChecked);
					cx.xCoord-=x;
					cx.yCoord-=y;
				}
			}

		}
	}
	//checks if castling is permitted
	if(!hasMoved){
		for(int i = 0; i <= 7; i+=7){
			for(int j=0; j<= 7; j+=7){
				castle = true;
				
				//checks if there is anything between king and rook
				//or if any piece from the other team can move between them
				if(backBoard[i][j]){
					if(backBoard[i][j]->getTeam()==team&&backBoard[i][j]->getType()==Piece::PieceType::ROOK){
						int a, b, c;
						if(i==0){
							a=1;
							b=2;
							c=1;
						}
						else{
							a=5;
							b=6;
							c=4;
						}
						if(!backBoard[a][j]&&!backBoard[b][j]&&!backBoard[c][j]){
							for(int k =0;k<8;k++){
								for(int l = 0; l < 8; l++){
									if(backBoard[k][l]){
										if(backBoard[k][l]->getTeam()!=team){
											vector<PiecePosition> notPoss = backBoard[k][l]->getPossibleMoves();
											for(auto& val : notPoss){
												if(i==0){
													if((val.xCoord==2||val.xCoord==1||val.xCoord==4)&&val.yCoord==j){
														castle=false;
													}
													else if((val.xCoord==4||val.xCoord==5||val.xCoord==6)&&val.yCoord==j){
														castle=false;
													}
												}
											}
										}
									}
								}
							}
							if(castle){
								cx.xCoord=i;
								cx.yCoord=j;
								cx.moveType=Piece::MoveType::CASTLE;
								moves.push_back(cx);
							}
						}
					}
				}
			}
		}
	}
	possibleMoves=moves;
}

//===================END OF KING FUNCTIONS=======================

//---------------------------------------------------------------
class Pawn : public Piece{
	friend class Game;
private:
	int movementDirection;

	int numberOfMoves;

	int points;

	char* name;

	pair<bool, int> enPassant;
public:
	//constructor
	Pawn();

	pair<bool, int> getEnPassant(){
		return enPassant;
	}

	void setEnPassant(pair<bool, int> e){
		enPassant = e;
	}

	void setPoints(int point){points = point;}

	int getPoints(){return points;}

	int setMovementDirection(int direction){movementDirection = direction;}
	
	int getMovementDirection(){return movementDirection;}
	
	//all parameter constructor
	Pawn(PieceType type, PieceTeam team, PiecePosition position, Board* handler);

	//copy constructor
	Pawn(const Pawn& pawn);
	
	void calculatePossibleMoves(Piece* backBoard[8][8], bool checkIfChecked);
	
	//destructor
	~Pawn();

	//=operator

	Pawn operator=(const Pawn& p);

	//==operator

	bool operator==(const Pawn& p);

	//>operator

	bool operator>(const Pawn& p);

	//++operator(and operator++)

	Pawn operator++();

	Pawn operator++(int);

	//+operator

	Pawn operator+(int x);
	friend Pawn operator+(int x, Pawn& p){
		p.numberOfMoves=p.numberOfMoves+x;
		return p;
	}
	//-operator

	Pawn operator-(int x);
	friend Pawn operator-(int x, Pawn& p){
		p.numberOfMoves = p.numberOfMoves-x;
		return p;
	}

	//>>operator

	friend istream& operator>>(istream& in, Pawn& p);

	//<<operator

	friend ostream& operator<<(ostream& out, const Pawn& p);

	//[]operator

	char operator[](int index);

	//explicit cast

	explicit operator double(){
		return (double)this->numberOfMoves;
	}	


};
//---------------------------------------------------------------

//====================OPERATOR OVERLOADING=======================

Pawn Pawn::operator++(int){
	Pawn aux(*this);
	this->numberOfMoves++;
	return aux;
}

Pawn Pawn::operator++(){
	this->numberOfMoves++;
	return *this;
}

Pawn Pawn::operator=(const Pawn& p){
	this->points = p.points;
	this->numberOfMoves = p.numberOfMoves;
	return *this;
}

bool Pawn::operator==(const Pawn& p){
	this->numberOfMoves==p.numberOfMoves&&this->points==p.points;
}

bool Pawn::operator>(const Pawn& p){
	return (this->numberOfMoves>p.numberOfMoves&&this->points>p.points);
}

Pawn Pawn::operator+(int x){
	this->numberOfMoves = this->numberOfMoves+x;
	return *this;
}

Pawn Pawn::operator-(int x){
	this->numberOfMoves = this->numberOfMoves-x;
	return *this;
}

ostream& operator<<(ostream& out, const Pawn& p){
	out<<p.name;
	out<<p.numberOfMoves;
	out<<p.points;
	return out;
}

istream& operator>>(istream& in, Pawn& p){
	in.getline(p.name, 10);
	in>>p.points;
	return in;
}

char Pawn::operator[](int index){
	if(index>0&& index<strlen(this->name))
		return this->name[index];
	
}

//=======================PAWN FUNCTIONS==========================

Pawn::Pawn(const Pawn& pawn) : Piece(PieceType::PAWN, pawn.team, pawn.pos, pawn.univHandler), points(1){
	string filename;
	

	if(team==PieceTeam::WHITE){
		filename = "img/Chess_plt60.bmp";
	}

	else if(team==PieceTeam::BLACK){
		filename = "img/Chess_pdt60.bmp";
	}
	//cout<<SDL_GetError()<<endl;
	
	texture = univHandler->loadImage(filename);
	//cout<<SDL_GetError()<<endl;

	if(team==PieceTeam::BLACK){
		//from bottom to top
		setMovementDirection(-1);
	}
	else{
		// from top to bottom
		setMovementDirection(1);
	}

	render();
	

	cout<<"Pawn rendered"<<endl;
}

Pawn::~Pawn(){}

void Pawn::calculatePossibleMoves(Piece* backBoard[8][8], bool checkIfChecked){
	vector<PiecePosition> moves;
	Piece::PiecePosition posi = pos;

	//checks if move is promotion
	if((posi.yCoord+movementDirection==7||posi.yCoord+movementDirection==0 )&& !backBoard[posi.xCoord][posi.yCoord+movementDirection]){
		posi.yCoord+=movementDirection;
		posi.moveType= Piece::MoveType::PROMOTE;
		moves = makeMove(moves, posi, teamsKing(backBoard), backBoard, checkIfChecked);
		posi.yCoord-=movementDirection;
	}
	
	//if it isn't promotion then it's a normal move
	else if(!backBoard[posi.xCoord][posi.yCoord+movementDirection]){
		posi.yCoord+=movementDirection;
		posi.moveType= Piece::MoveType::NORMAL;
		moves = makeMove(moves, posi, teamsKing(backBoard), backBoard, checkIfChecked);
		posi.yCoord-=movementDirection;		
	}

	//if a pawn hasn't moved it can move 2 squares
	if(!hasMoved && !backBoard[posi.xCoord][posi.yCoord+2*movementDirection]&&!backBoard[posi.xCoord][posi.yCoord+movementDirection] && posi.yCoord+2*movementDirection>=0 && posi.yCoord+2*movementDirection<=7){
		posi.yCoord+=2*movementDirection;
		posi.moveType= Piece::MoveType::NORMAL;
		moves = makeMove(moves, posi, teamsKing(backBoard), backBoard, checkIfChecked);
		posi.yCoord-=2*movementDirection;		
	}
	
	//pawns can only take diagonally
	if(posi.xCoord+1<=7){
		if(backBoard[posi.xCoord+1][posi.yCoord+movementDirection]){
			if(backBoard[posi.xCoord+1][posi.yCoord+movementDirection]->getTeam() != team){
				if(posi.yCoord+movementDirection==0||posi.yCoord+movementDirection==7){
					posi.yCoord+=movementDirection;
					posi.xCoord++;
					posi.moveType= Piece::MoveType::PROMOTE;
					moves = makeMove(moves, posi, teamsKing(backBoard), backBoard, checkIfChecked);
					posi.yCoord-=movementDirection;
					posi.xCoord--;
				}
				else{
					posi.yCoord+=movementDirection;
					posi.xCoord++;
					posi.moveType= Piece::MoveType::NORMAL;
					moves = makeMove(moves, posi, teamsKing(backBoard), backBoard, checkIfChecked);
					posi.yCoord-=movementDirection;
					posi.xCoord--;					
				}
			}
		}
	}
	if(posi.xCoord-1>=0){
		if(backBoard[posi.xCoord-1][posi.yCoord+movementDirection]){
			if(backBoard[posi.xCoord-1][posi.yCoord+movementDirection]->getTeam() != team){
				if(posi.yCoord+movementDirection==0||posi.yCoord+movementDirection==7){
					posi.yCoord+=movementDirection;
					posi.xCoord--;
					posi.moveType= Piece::MoveType::PROMOTE;
					moves = makeMove(moves, posi, teamsKing(backBoard), backBoard, checkIfChecked);
					posi.yCoord-=movementDirection;
					posi.xCoord++; 
				}
				else{
					posi.xCoord--;
					posi.yCoord+=movementDirection;
					posi.moveType= Piece::MoveType::NORMAL;
					moves = makeMove(moves, posi, teamsKing(backBoard), backBoard, checkIfChecked);
					posi.yCoord-=movementDirection;
					posi.xCoord++;					
				}
			}
		}
	}

	//en passant
	if(enPassant==pair<bool, int>(true,-1)){
		posi.xCoord++;
		posi.yCoord+=movementDirection;
		posi.moveType= Piece::MoveType::EN_PASSANT;
		moves = makeMove(moves, posi, teamsKing(backBoard), backBoard, checkIfChecked);
		posi.xCoord--;
		posi.yCoord-=movementDirection;
	}
	if(enPassant==pair<bool, int>(true, 1)){
		posi.xCoord--;
		posi.yCoord+=movementDirection;
		posi.moveType = Piece::MoveType::EN_PASSANT;
		moves = makeMove(moves, posi, teamsKing(backBoard), backBoard, checkIfChecked);
		posi.xCoord++;
		posi.yCoord-=movementDirection;
	}

	possibleMoves = moves;
}

Pawn::Pawn() : Piece(PieceType::PAWN, team, pos, univHandler), points(1){
	string filename;
	

	if(team==PieceTeam::WHITE){
		filename = "img/Chess_plt60.bmp";
	}

	else if(team==PieceTeam::BLACK){
		filename = "img/Chess_pdt60.bmp";
	}
	//cout<<SDL_GetError()<<endl;
	
	texture = univHandler->loadImage(filename);
	//cout<<SDL_GetError()<<endl;

	if(team==PieceTeam::BLACK){
		//from bottom to top
		setMovementDirection(-1);
	}
	else{
		// from top to bottom
		setMovementDirection(1);
	}

	render();
	

	cout<<"Pawn rendered"<<endl;
}

Pawn::Pawn(PieceType type, PieceTeam team, PiecePosition position, Board* handler) : Piece(PieceType::PAWN, team, position, handler), points(1){
	string filename;
	

	if(team==PieceTeam::WHITE){
		filename = "img/Chess_plt60.bmp";
	}

	else if(team==PieceTeam::BLACK){
		filename = "img/Chess_pdt60.bmp";
	}
	//cout<<SDL_GetError()<<endl;
	univHandler=handler;
	texture = handler->loadImage(filename);
	//cout<<SDL_GetError()<<endl;


	if(team==PieceTeam::BLACK){
		//from bottom to top
		setMovementDirection(-1);
	}
	else{
		// from top to bottom
		setMovementDirection(1);
	}

	render();
	

	cout<<"Pawn rendered"<<endl;
	cout<<"Pawn movdir: "<< movementDirection<<endl;
}

//======================END OF PAWN FUNCTIONS=====================

//----------------------------------------------------------------
class Queen : public Piece{

public:	
	Queen(PieceType type, PieceTeam team, PiecePosition position, Board* handler);
	void calculatePossibleMoves(Piece* backBoard[8][8], bool checkIfChecked);
	~Queen();
};
//----------------------------------------------------------------

//============================QUEEN FUNCTIONS=====================

Queen::~Queen(){}

Queen::Queen(PieceType type, PieceTeam team, PiecePosition position, Board* handler) : Piece(PieceType::QUEEN, team, position, handler){
	string filename;

	if(team == PieceTeam::WHITE){
		filename = "img/Chess_qlt60.bmp"; 
	}

	else if(team == PieceTeam::BLACK){
		filename = "img/Chess_qdt60.bmp";
	}

	univHandler=handler;
	texture = handler->loadImage(filename);

	render();

	cout<<"Queen rendered"<<endl;
}

void Queen::calculatePossibleMoves(Piece* backBoard[8][8], bool checkIfChecked){
	vector<PiecePosition> moves;

	Piece::PiecePosition posi = pos;
	
	//queen can move in any direction, straight and diagonally
	//until something is in the way(logic applies to every piece from now on)
	for(int x = -1; x <= 1; x++){

		for(int y = -1; y <= 1; y++){
			int xCpy = x;
			int yCpy = y;

			while(!backBoard[posi.xCoord+xCpy][posi.yCoord+yCpy]){
				posi.xCoord+=xCpy;
				posi.yCoord+=yCpy;
				posi.moveType = Piece::MoveType::NORMAL;
					
				moves = makeMove(moves, posi, teamsKing(backBoard), backBoard, checkIfChecked);
				posi.xCoord-=xCpy;
					
				posi.yCoord-=yCpy;

				if(xCpy>0){
					xCpy++;
				}
				else if(xCpy<0){
					xCpy--;
				}
				if(yCpy>0){
					yCpy++;
				}
				else if(yCpy<0){
					yCpy--;
				}
			}
			if(backBoard[posi.xCoord+xCpy][posi.yCoord+yCpy]&&(posi.xCoord+xCpy>=0)&&(posi.xCoord+xCpy<=7)&&(posi.yCoord+yCpy>=0)&&posi.yCoord+yCpy<=7)
				if(backBoard[posi.xCoord+xCpy][posi.yCoord+yCpy]->getTeam() != team){
					posi.xCoord+=xCpy;
					posi.yCoord+=yCpy;
					posi.moveType = Piece::MoveType::NORMAL;
					moves = makeMove(moves, posi, teamsKing(backBoard), backBoard, checkIfChecked);
					posi.xCoord-=xCpy;
				
					posi.yCoord-=yCpy;
				}

		}

	}
	possibleMoves = moves;
}

//===========================END OF QUEEN FUNCTIONS===============

//----------------------------------------------------------------
class Bishop : public Piece{

private:
	int points;

public:
	Bishop(PieceType type, PieceTeam team, PiecePosition position, Board* handler);
	void calculatePossibleMoves(Piece* backBoard[8][8], bool checkIfChecked);
	~Bishop();
};
//----------------------------------------------------------------

//===========================BISHOP FUNCTIONS=====================

Bishop::~Bishop(){}

Bishop::Bishop(PieceType type, PieceTeam team, PiecePosition position, Board* handler) : Piece(PieceType::BISHOP, team, position, handler){
	string filename;

	if(team == PieceTeam::WHITE){
		filename = "img/Chess_blt60.bmp"; 
	}

	else if(team == PieceTeam::BLACK){
		filename = "img/Chess_bdt60.bmp";
	}
	univHandler=handler;
	texture = handler->loadImage(filename);

	render();

	cout<<"Bishop rendered"<<endl;
}

void Bishop::calculatePossibleMoves(Piece* backBoard[8][8], bool checkIfChecked){
	vector<PiecePosition> moves;

	Piece::PiecePosition posi=pos;

	//four directions((-1,-1) = down left, (-1,1) = up left etc.)
	for(int x = -1; x <= 1; x+=2){
		
		for(int y = -1; y <= 1; y+=2){

			int xCpy = x;

			int yCpy = y;

			while(!backBoard[pos.xCoord+xCpy][pos.yCoord+yCpy]&&(pos.xCoord+xCpy>=0)&&(pos.xCoord+xCpy<=7)&&(pos.yCoord+yCpy>=0)&&pos.yCoord+yCpy<=7){
				pos.xCoord+=xCpy;
				pos.yCoord+=yCpy;
				pos.moveType = Piece::MoveType::NORMAL;
				moves = makeMove(moves, pos, teamsKing(backBoard), backBoard, checkIfChecked);

				pos.xCoord-=xCpy;
				
				pos.yCoord-=yCpy;

				if(xCpy < 0){
					xCpy--;
				}

				else{
					xCpy++;
				}

				if(yCpy < 0){
					yCpy--;
				}

				else{
					yCpy++;
				}
			}

			if(backBoard[pos.xCoord+xCpy][pos.yCoord+yCpy]&&(pos.xCoord+xCpy>=0)&&(pos.xCoord+xCpy<=7)&&(pos.yCoord+yCpy>=0)&&pos.yCoord+yCpy<=7)
				if(backBoard[pos.xCoord+xCpy][pos.yCoord+yCpy]->getTeam() != team){
					pos.xCoord+=xCpy;
					pos.yCoord+=yCpy;
					pos.moveType = Piece::MoveType::NORMAL;
					moves = makeMove(moves, pos, teamsKing(backBoard), backBoard, checkIfChecked);

					pos.xCoord-=xCpy;
				
					pos.yCoord-=yCpy;
				}
		}
	}
	possibleMoves = moves;
}

//=========================END OF BISHOP FUNCTIONS================

//----------------------------------------------------------------
class Knight : public Piece{
public:
	Knight(PieceType type, PieceTeam team, PiecePosition position, Board* handler);
	void calculatePossibleMoves(Piece* backBoard[8][8], bool checkIfChecked);
	~Knight();
};
//----------------------------------------------------------------

//==========================KNIGHT FUNCTIONS======================

Knight::~Knight(){}

Knight::Knight(PieceType type, PieceTeam team, PiecePosition position, Board* handler) : Piece(PieceType::KNIGHT, team, position, handler){
	string filename;

	if(team == PieceTeam::WHITE){
		filename = "img/Chess_nlt60.bmp"; 
	}

	else if(team == PieceTeam::BLACK){
		filename = "img/Chess_ndt60.bmp";
	}

	univHandler=handler;
	texture = handler->loadImage(filename);

	render();

	cout<<"Knight rendered"<<endl;
}

void Knight::calculatePossibleMoves(Piece* backBoard[8][8], bool checkIfChecked){
	vector<PiecePosition> moves;

	Piece::PiecePosition posi = pos;

	//2 left 1 up, 2 left 1 down, 2 right 1 up, 2 right 1 down
	for(int x = -2; x <= 2; x += 4){

		for(int y = -1; y <= 1; y += 2){

			if((posi.xCoord+x<=7)&&(posi.xCoord+x>=0)&&(posi.yCoord+y>=0)&&(posi.yCoord+y<=7)){
				if(!backBoard[posi.xCoord+x][posi.yCoord+y]){
					posi.xCoord+=x;
					posi.yCoord+=y;
					posi.moveType = Piece::MoveType::NORMAL;
					moves = makeMove(moves, posi, teamsKing(backBoard), backBoard, checkIfChecked);

					posi.xCoord-=x;
				
					posi.yCoord-=y;
				}
				else{
					if(backBoard[posi.xCoord+x][posi.yCoord+y]->getTeam()!=team){
					posi.xCoord+=x;
					posi.yCoord+=y;
					posi.moveType = Piece::MoveType::NORMAL;
					moves = makeMove(moves, posi, teamsKing(backBoard), backBoard, checkIfChecked);

					posi.xCoord-=x;
				
					posi.yCoord-=y;
					}
				}
			}		
		}
	}

	//2 up 1 right, 2 up 1 left, 2 down 1 right, 2 down 1 left
	for(int y = -2; y <= 2; y += 4){

		for(int x = -1; x <= 1; x += 2){

			if((posi.xCoord + x <=7)&&(posi.xCoord+x>=0)&&(posi.yCoord+y>=0)&&(posi.yCoord+y<=7)){
				if(!backBoard[posi.xCoord+x][posi.yCoord+y]){
					posi.xCoord+=x;
					posi.yCoord+=y;
					posi.moveType = Piece::MoveType::NORMAL;
					moves = makeMove(moves, posi, teamsKing(backBoard), backBoard, checkIfChecked);

					posi.xCoord-=x;
				
					posi.yCoord-=y;				}
				else{
					if(backBoard[posi.xCoord+x][posi.yCoord+y]->getTeam()!=team){
					posi.xCoord+=x;
					posi.yCoord+=y;
					posi.moveType = Piece::MoveType::NORMAL;
					moves = makeMove(moves, posi, teamsKing(backBoard), backBoard, checkIfChecked);

					posi.xCoord-=x;
				
					posi.yCoord-=y;
					}
				}
			}		
		}
	}


	possibleMoves=moves;
}

//=========================END OF KNIGHT FUNCTIONS================

//----------------------------------------------------------------
class Rook : public Piece{
public:
	Rook(PieceType type, PieceTeam team, PiecePosition position, Board* handler);
	void calculatePossibleMoves(Piece* backBoard[8][8], bool checkIfChecked);
	~Rook();
};
//----------------------------------------------------------------

//========================ROOK FUNCTIONS==========================

Rook::~Rook(){}

void Rook::calculatePossibleMoves(Piece* backBoard[8][8], bool checkIfChecked){
	vector<PiecePosition> moves;

	Piece::PiecePosition posi = pos;

	for(int x = -1; x <= 1; x++){

		for(int y = -1; y <= 1; y++){

			if(x==0||y==0){
				int xCpy = x;
				int yCpy = y;

				while(!backBoard[posi.xCoord+xCpy][posi.yCoord+yCpy]&&(posi.xCoord+xCpy>=0)&&(posi.xCoord+xCpy<=7)&&(posi.yCoord+yCpy>=0)&&posi.yCoord+yCpy<=7){
					posi.xCoord+=xCpy;
					posi.yCoord+=yCpy;
					posi.moveType = Piece::MoveType::NORMAL;
					moves = makeMove(moves, posi, teamsKing(backBoard), backBoard, checkIfChecked);

					posi.xCoord-=xCpy;
					
					posi.yCoord-=yCpy;

					if(xCpy>0){
						xCpy++;
					}
					else if(xCpy<0){
						xCpy--;
					}
					if(yCpy>0){
						yCpy++;
					}
					else if(yCpy<0){
						yCpy--;
					}
				}
				if(backBoard[posi.xCoord+xCpy][posi.yCoord+yCpy]&&(posi.xCoord+xCpy>=0)&&(posi.xCoord+xCpy<=7)&&(posi.yCoord+yCpy>=0)&&posi.yCoord+yCpy<=7)
					if(backBoard[posi.xCoord+xCpy][posi.yCoord+yCpy]->getTeam() != team){
						posi.xCoord+=xCpy;
						posi.yCoord+=yCpy;
						posi.moveType = Piece::MoveType::NORMAL;
						moves = makeMove(moves, posi, teamsKing(backBoard), backBoard, checkIfChecked);

						posi.xCoord-=xCpy;
					
						posi.yCoord-=yCpy;
					}

			}

		}
	}
	possibleMoves=moves;
}

Rook::Rook(PieceType type, PieceTeam team, PiecePosition position, Board* handler) : Piece(PieceType::ROOK, team, position, handler){
	string filename;

	if(team == PieceTeam::WHITE){
		filename = "img/Chess_rlt60.bmp"; 
	}

	else if(team == PieceTeam::BLACK){
		filename = "img/Chess_rdt60.bmp";
	}
	univHandler=handler;
	texture = handler->loadImage(filename);

	render();

	cout<<"Rook rendered"<<endl;
}

//======================END OF ROOK FUNCTIONS=====================

//----------------------------------------------------------------
class Game{
public:

	//all parameter constructor
	Game(bool enPassant, Piece* backBoard[8][8], Piece::PieceTeam turen, Board* handleer);

	//parameter constructor
	Game(Board* handler);

	//default constructor
	Game();

	//destructor
	~Game();

	//copy constructor
	Game(const Game& game);

	Piece::PieceTeam getTurn(){
		return turn;
	}

	//determines what type of move you are making
	void move(Piece* piece, Piece::PiecePosition position);

	Piece* getPositionInField(int row, int column);

	bool isValidMove(int x, int y, Piece* piece);

	void calculateAllMoves();

	void renderPossibleMoves(Piece* piece);

	void undoRenderOfPossibleMoves(Piece* piece);

	void setCheckEnPassant(bool boo){
		checkEnPassant=boo;
	}

	void setTime(double tim){
		time=tim;
	}

	double getTime(){
		return time;
	}

	bool getCheckEnPassant(){
		return checkEnPassant;
	}

	int getAdvantage(){
		return advantage;
	}

	//=operator
	Game operator=(const Game& g);

	//>>operator
	friend istream& operator>>(istream& in, Game& game);

	//<<operator
	friend istream& operator>>(istream& in, const Game& game);

	//[]operator
	int operator[](int index);

	//++operator
	Game operator++();
	Game operator++(int);

	//==operator
	bool operator==(const Game& game);

	//+operator
	Game operator+(int x);
	friend Game operator+(int x, Game game){
		game.advantage = game.advantage + x;
		return game;
	}

	//-operator
	Game operator-(int x);
	friend Game operator-(int x, Game game){
		game.advantage = game.advantage + x;
		return game;
	}

	//>operator
	bool operator>(const Game& game);

	//+operator between classes
	friend Game operator+(Piece& p, Game game);
	friend Game operator+(Game game, Piece& p);

	explicit operator int(){
		return int(time);
	}

private:

	double time;

	int advantage;

	int* pointsArray;

	//SDL
	Board* univHandle;

	//whose turn is it
	Piece::PieceTeam turn;

	//normal move
	void normalMove(int xs, int ys, int xe, int ye);

	//makes the move PROMOTION
	void promoteMove(int xs, int ys, int xe, int ye);

	//makes the move CASTLE
	void castleMove(int xs, int ys, int xe, int ye);

	//makes the move EN PASSANT
	void enPassant(int xs, int ys, int xe, int ye);

	//background board
	Piece* pieceField[8][8];

	//state of game at the moment(is someone winning, losing, draw etc)
	void state();

	bool checkEnPassant;
	
	//as the name suggests, disables enPassant(sets enPassant to false)
	void disableEnPassant();

	//every piece
	Pawn* p10;
	Pawn* p11;
	Pawn* p12;
	Pawn* p13;
	Pawn* p14;
	Pawn* p15;
	Pawn* p16;
	Pawn* p17;
	Pawn* p60;
	Pawn* p61;
	Pawn* p62;
	Pawn* p63;
	Pawn* p64;
	Pawn* p65;
	Pawn* p66;
	Pawn* p67;
	Rook* r00;
	Rook* r07;
	Rook* r70;
	Rook* r77;
	Knight* n01;
	Knight* n06;
	Knight* n71;
	Knight* n76;
	Bishop* b02;
	Bishop* b05;
	Bishop* b72;
	Bishop* b75;
	King* k03;
	King* k73;
	Queen* q04;
	Queen* q74;
};
//----------------------------------------------------------------

//===============OPERATOR OVERLOADING=============================

bool Game::operator==(const Game& game){
	return this->advantage==game.advantage&&this->time==game.time;
}

Game Game::operator=(const Game& g){
	if(this!=&g){
		this->advantage = g.advantage;
		this->time= g.time;
	}
	return *this;
}

istream& operator>>(istream& in, Game& game){
	in>>game.advantage;
	return in;
}

ostream& operator<<(ostream& out, Game& game){
	out<<game.getAdvantage();
	return out;
}

bool Game::operator>(const Game& game){
	return this->advantage>game.advantage;
}

Game Game::operator-(int x){
	this->advantage = this->advantage - x;
	return *this;
}

Game Game::operator+(int x){
	this->advantage = this->advantage + x;
	return *this;
}

Game Game::operator++(){
	this->advantage++;
	return *this;
}

Game Game::operator++(int){
	Game game(*this);
	this->advantage++;
	return game;
}

int Game::operator[](int index){
	if(index>0)
		return pointsArray[index];
}

Game operator+(Piece& p, Game game){
	int* auxcc = new int[p.getMoveNumber()+1];
	for(int i = 0; i < p.getMoveNumber(); i++){
		auxcc[i]=game.pointsArray[i];
	}
	auxcc[p.getMoveNumber()+1]=p.getPoints();

	if(game.pointsArray!=NULL){
		delete[] game.pointsArray;
	}
	game.pointsArray = new int[p.getMoveNumber()+1];

	for(int i = 0; i<p.getMoveNumber()+1;i++){
		game.pointsArray[i]=auxcc[i];
	}
	return game;
}

Game operator+(Game game, Piece& p){
	int* auxcc = new int[p.getMoveNumber()+1];
	for(int i = 0; i < p.getMoveNumber(); i++){
		auxcc[i]=game.pointsArray[i];
	}
	auxcc[p.getMoveNumber()+1]=p.getPoints();

	if(game.pointsArray!=NULL){
		delete[] game.pointsArray;
	}
	game.pointsArray = new int[p.getMoveNumber()+1];

	for(int i = 0; i<p.getMoveNumber()+1;i++){
		game.pointsArray[i]=auxcc[i];
	}
	return game;
}


//========================GAME FUNCTIONS==========================

void Game::state(){
 	bool hasLost=true;

 	King* p = k03;//white king
 	if(turn == Piece::PieceTeam::BLACK){
		p = k73;//black king
	}

	p->setCheck(pieceField,p->getPosition().xCoord, p->getPosition().yCoord);

	for(int i = 0; i < 8; i++){
		
		for(int j = 0; j < 8; j++){
			
			if(pieceField[i][j] && pieceField[i][j]->getTeam()!=turn){
				
				//calculates moves for opponent
				pieceField[i][j]->calculatePossibleMoves(pieceField, true);
				
				//if there are any legal moves, team has not lost
				if(!pieceField[i][j]->getPossibleMoves().empty()){
					hasLost=false;;
				}
			}
		}
	}
	if(p->getCheck()&&hasLost){
		if(turn == Piece::PieceTeam::WHITE)	
			cout<<"WHITE WINS!";
	
		else if(turn == Piece::PieceTeam::BLACK){
			cout<<"BLACK WINS!";
		}
	}

	else if(hasLost){
		cout<<"DRAW";
	}

	
	//changes turn
	if(turn==Piece::PieceTeam::WHITE){
		turn = Piece::PieceTeam::BLACK;
	}
	else if(turn == Piece::PieceTeam::BLACK){
		turn = Piece::PieceTeam::WHITE;
	}
}

void Game::disableEnPassant(){
	for(int i = 0; i < 8; i++){
		for(int j = 0; j < 8; j++){
			if(pieceField[i][j]&&pieceField[i][j]->getType()==Piece::PieceType::PAWN){
				Pawn* pawn = (Pawn*)(pieceField[i][j]);
				pawn->setEnPassant(pair<bool, int>(false, 0));
			}
		}
	}
}

bool Game::isValidMove(int x, int y, Piece* piece){
	vector<Piece::PiecePosition> moves = piece->getPossibleMoves();
	//if the move to coordinates X and Y are in piece's possible moves, then it's a valid move
	for(auto& move : moves){
		if(move.xCoord==x&&move.yCoord==y){
			return true;
		}
	}
	return false;
}

void Game::move(Piece* piece, Piece::PiecePosition move){

	if(checkEnPassant){
		disableEnPassant();
	}

	else{
		checkEnPassant=true;
	}

	
	switch(move.moveType){
		case Piece::MoveType::NORMAL:
			normalMove(piece->getPosition().xCoord, piece->getPosition().yCoord, move.xCoord, move.yCoord);
			break;
		case Piece::MoveType::CASTLE:
			castleMove(piece->getPosition().xCoord, piece->getPosition().yCoord, move.xCoord, move.yCoord);
			break;
		case Piece::MoveType::PROMOTE:
			promoteMove(piece->getPosition().xCoord, piece->getPosition().yCoord, move.xCoord, move.yCoord);
			break;
		case Piece::MoveType::EN_PASSANT:
			enPassant(piece->getPosition().xCoord, piece->getPosition().yCoord, move.xCoord, move.yCoord);
			cout<<"\nENPASSANT?!?!?!?!?!!?!!?AYOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO!";
			break;
		default:
			break;
	}
	state();
}

void Game::normalMove(int xs, int ys, int xe, int ye){
	
	//basically, it takes the piece on the background board, moves it to where the player moves it, unrenders the piece from where it left, 
	//and renders it where the player moves it

	pieceField[xe][ye] = getPositionInField(xs, ys);
	pieceField[xe][ye]->setHasMoved(true);
	pieceField[xs][ys]=nullptr;
	univHandle->undoPiece(xs, ys);
	Piece::PiecePosition posi;
	
	posi.xCoord = xe;
	posi.yCoord = ye;
	pieceField[xe][ye]->setPosition(posi);

	if(pieceField[xe][ye]){
		univHandle->undoPiece(xe, ye);
	}
	pieceField[xe][ye]->render();

	//en passant rules
	if(pieceField[xe][ye]->getType() == Piece::PieceType::PAWN){
		if(abs(ye - ys)==2){
			if(xe - 1 >=0){
				if (pieceField[xe-1][ye]){
					if(pieceField[xe-1][ye]->getType()==Piece::PieceType::PAWN){
						Pawn* pawn = (Pawn*)(pieceField[xe-1][ye]);
						pawn->setEnPassant(pair<bool, int>(true, -1));
						checkEnPassant=false;
					}
				}
			}
			if(xe + 1 <=7){
				if(pieceField[xe+1][ye]){
					if(pieceField[xe+1][ye]->getType() == Piece::PieceType::PAWN){
						Pawn* pawn = (Pawn*)(pieceField[xe+1][ye]);
						pawn->setEnPassant(pair<bool, int>(true, 1));
						checkEnPassant = false;
					}
				}
			}
		}
	}
	for(int i = 0; i<8; i++){
		for(int j = 0; j < 8; j++){
			if(pieceField[i][j]){
				pieceField[i][j]->render();
			}
		}
	}
}

void Game::calculateAllMoves(){
	for(int i = 0; i < 8; i++){
		for(int j = 0; j < 8; j++){
			if(pieceField[j][i]){
				pieceField[j][i]->calculatePossibleMoves(pieceField, true);
			}
		}
	}
}

Piece* Game::getPositionInField(int row, int column){
	return pieceField[row][column];
}

Game::Game(bool enPassant, Piece* backBoard[8][8], Piece::PieceTeam turen, Board* handleer) : turn(turen), univHandle(handleer), checkEnPassant(enPassant){
	
	Piece::PiecePosition posi;
	posi.xCoord=0;
	posi.yCoord=1;

	//rendering the pawns
	p10 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::WHITE, posi, univHandle);

	posi.xCoord++;

	p11 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::WHITE, posi, univHandle);

	posi.xCoord++;

	p12 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::WHITE, posi, univHandle);

	posi.xCoord++;

	p13 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::WHITE, posi, univHandle);

	posi.xCoord++;

	p14 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::WHITE, posi, univHandle);

	posi.xCoord++;

	p15 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::WHITE, posi, univHandle);

	posi.xCoord++;

	p16 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::WHITE, posi, univHandle);

	posi.xCoord++;

	p17 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::WHITE, posi, univHandle);

	posi.xCoord=0;
	posi.yCoord=6;

	p60 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::BLACK, posi, univHandle);

	posi.xCoord++;
	
	p61 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::BLACK, posi, univHandle);

	posi.xCoord++;

	p62 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::BLACK, posi, univHandle);

	posi.xCoord++;

	p63 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::BLACK, posi, univHandle);

	posi.xCoord++;

	p64 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::BLACK, posi, univHandle);

	posi.xCoord++;

	p65 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::BLACK, posi, univHandle);

	posi.xCoord++;

	p66 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::BLACK, posi, univHandle);

	posi.xCoord++;

	p67 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::BLACK, posi, univHandle);

	//finish pawn rendering

	//rendering the rooks
	
	posi.xCoord=0;
	posi.yCoord=0;

	r00 = new Rook(Rook::PieceType::ROOK, Rook::PieceTeam::WHITE, posi, univHandle);

	posi.xCoord=7;

	r07 = new Rook(Rook::PieceType::ROOK, Rook::PieceTeam::WHITE, posi, univHandle);

	posi.yCoord=7;

	r77 = new Rook(Rook::PieceType::ROOK, Rook::PieceTeam::BLACK, posi, univHandle);

	posi.xCoord=0;

	r70 = new Rook(Rook::PieceType::ROOK, Rook::PieceTeam::BLACK, posi, univHandle);
	
	//finish rook rendering

	//rendering the knights

	posi.xCoord=1;
	posi.yCoord=0;

	n01 = new Knight(Knight::PieceType::KNIGHT, Knight::PieceTeam::WHITE, posi, univHandle);	
	
	posi.xCoord=6;
	n06 = new Knight(Knight::PieceType::KNIGHT, Knight::PieceTeam::WHITE, posi, univHandle);

	posi.yCoord = 7;
	n76 = new Knight(Knight::PieceType::KNIGHT, Knight::PieceTeam::BLACK, posi, univHandle);

	posi.xCoord=1;
	n71 = new Knight(Knight::PieceType::KNIGHT, Knight::PieceTeam::BLACK, posi, univHandle);

	//finish knight rendering

	//rendering the bishops

	posi.xCoord = 2;
	posi.yCoord = 0;

	b02 = new Bishop(Bishop::PieceType::BISHOP,	Bishop::PieceTeam::WHITE, posi, univHandle);

	posi.xCoord=5;
	b05 = new Bishop(Bishop::PieceType::BISHOP, Bishop::PieceTeam::WHITE, posi, univHandle);

	posi.yCoord = 7;
	b72 = new Bishop(Bishop::PieceType::BISHOP, Bishop::PieceTeam::BLACK, posi, univHandle);

	posi.xCoord = 2;
	b75 = new Bishop(Bishop::PieceType::BISHOP, Bishop::PieceTeam::BLACK, posi, univHandle);

	//finish bishop rendering

	//rendering the kings

	posi.xCoord = 3;
	posi.yCoord = 0;

	k03 = new King(King::PieceType::KING, King::PieceTeam::WHITE, posi, univHandle);

	posi.yCoord=7;

	k73 = new King(King::PieceType::KING, King::PieceTeam::BLACK, posi, univHandle);

	//finish king rendering

	//rendering the queens

	posi.xCoord=4;
	posi.yCoord=0;

	q04 = new Queen(Queen::PieceType::QUEEN, Queen::PieceTeam::WHITE, posi, univHandle);

	posi.yCoord = 7;

	q74 = new Queen(Queen::PieceType::QUEEN, Queen::PieceTeam::BLACK, posi, univHandle);

		
	pieceField[0][0] = r00;
	pieceField[1][0] = n01;
	pieceField[2][0] = b02;
	pieceField[3][0] = k03;
	pieceField[4][0] = q04;
	pieceField[5][0] = b05;
	pieceField[6][0] = n06;
	pieceField[7][0] = r07;

	pieceField[0][1] = p10;
	pieceField[1][1] = p11;
	pieceField[2][1] = p12;
	pieceField[3][1] = p13;
	pieceField[4][1] = p14;
	pieceField[5][1] = p15;
	pieceField[6][1] = p16;
	pieceField[7][1] = p17;

	pieceField[0][7] = r70;
	pieceField[1][7] = n71;
	pieceField[2][7] = b75;
	pieceField[3][7] = k73;
	pieceField[4][7] = q74;
	pieceField[5][7] = b72;
	pieceField[6][7] = n76;
	pieceField[7][7] = r77;

	pieceField[0][6] = p60;
	pieceField[1][6] = p61;
	pieceField[2][6] = p62;
	pieceField[3][6] = p63;
	pieceField[4][6] = p64;
	pieceField[5][6] = p65;
	pieceField[6][6] = p66;
	pieceField[7][6] = p67;

	for(int j = 2; j < 6; j++){
		for (int i = 0; i < 8; i++){
			pieceField[i][j] = nullptr;
		}
	}
	calculateAllMoves();
}

Game::Game(const Game& game) : turn(game.turn), univHandle(game.univHandle), checkEnPassant(game.checkEnPassant) {

	Piece::PiecePosition posi;
	posi.xCoord=0;
	posi.yCoord=1;

	//rendering the pawns
	p10 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::WHITE, posi, univHandle);

	posi.xCoord++;

	p11 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::WHITE, posi, univHandle);

	posi.xCoord++;

	p12 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::WHITE, posi, univHandle);

	posi.xCoord++;

	p13 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::WHITE, posi, univHandle);

	posi.xCoord++;

	p14 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::WHITE, posi, univHandle);

	posi.xCoord++;

	p15 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::WHITE, posi, univHandle);

	posi.xCoord++;

	p16 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::WHITE, posi, univHandle);

	posi.xCoord++;

	p17 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::WHITE, posi, univHandle);

	posi.xCoord=0;
	posi.yCoord=6;

	p60 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::BLACK, posi, univHandle);

	posi.xCoord++;
	
	p61 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::BLACK, posi, univHandle);

	posi.xCoord++;

	p62 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::BLACK, posi, univHandle);

	posi.xCoord++;

	p63 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::BLACK, posi, univHandle);

	posi.xCoord++;

	p64 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::BLACK, posi, univHandle);

	posi.xCoord++;

	p65 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::BLACK, posi, univHandle);

	posi.xCoord++;

	p66 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::BLACK, posi, univHandle);

	posi.xCoord++;

	p67 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::BLACK, posi, univHandle);

	//finish pawn rendering

	//rendering the rooks
	
	posi.xCoord=0;
	posi.yCoord=0;

	r00 = new Rook(Rook::PieceType::ROOK, Rook::PieceTeam::WHITE, posi, univHandle);

	posi.xCoord=7;

	r07 = new Rook(Rook::PieceType::ROOK, Rook::PieceTeam::WHITE, posi, univHandle);

	posi.yCoord=7;

	r77 = new Rook(Rook::PieceType::ROOK, Rook::PieceTeam::BLACK, posi, univHandle);

	posi.xCoord=0;

	r70 = new Rook(Rook::PieceType::ROOK, Rook::PieceTeam::BLACK, posi, univHandle);
	
	//finish rook rendering

	//rendering the knights

	posi.xCoord=1;
	posi.yCoord=0;

	n01 = new Knight(Knight::PieceType::KNIGHT, Knight::PieceTeam::WHITE, posi, univHandle);	
	
	posi.xCoord=6;
	n06 = new Knight(Knight::PieceType::KNIGHT, Knight::PieceTeam::WHITE, posi, univHandle);

	posi.yCoord = 7;
	n76 = new Knight(Knight::PieceType::KNIGHT, Knight::PieceTeam::BLACK, posi, univHandle);

	posi.xCoord=1;
	n71 = new Knight(Knight::PieceType::KNIGHT, Knight::PieceTeam::BLACK, posi, univHandle);

	//finish knight rendering

	//rendering the bishops

	posi.xCoord = 2;
	posi.yCoord = 0;

	b02 = new Bishop(Bishop::PieceType::BISHOP,	Bishop::PieceTeam::WHITE, posi, univHandle);

	posi.xCoord=5;
	b05 = new Bishop(Bishop::PieceType::BISHOP, Bishop::PieceTeam::WHITE, posi, univHandle);

	posi.yCoord = 7;
	b72 = new Bishop(Bishop::PieceType::BISHOP, Bishop::PieceTeam::BLACK, posi, univHandle);

	posi.xCoord = 2;
	b75 = new Bishop(Bishop::PieceType::BISHOP, Bishop::PieceTeam::BLACK, posi, univHandle);

	//finish bishop rendering

	//rendering the kings

	posi.xCoord = 3;
	posi.yCoord = 0;

	k03 = new King(King::PieceType::KING, King::PieceTeam::WHITE, posi, univHandle);

	posi.yCoord=7;

	k73 = new King(King::PieceType::KING, King::PieceTeam::BLACK, posi, univHandle);

	//finish king rendering

	//rendering the queens

	posi.xCoord=4;
	posi.yCoord=0;

	q04 = new Queen(Queen::PieceType::QUEEN, Queen::PieceTeam::WHITE, posi, univHandle);

	posi.yCoord = 7;

	q74 = new Queen(Queen::PieceType::QUEEN, Queen::PieceTeam::BLACK, posi, univHandle);

		
	pieceField[0][0] = r00;
	pieceField[1][0] = n01;
	pieceField[2][0] = b02;
	pieceField[3][0] = k03;
	pieceField[4][0] = q04;
	pieceField[5][0] = b05;
	pieceField[6][0] = n06;
	pieceField[7][0] = r07;

	pieceField[0][1] = p10;
	pieceField[1][1] = p11;
	pieceField[2][1] = p12;
	pieceField[3][1] = p13;
	pieceField[4][1] = p14;
	pieceField[5][1] = p15;
	pieceField[6][1] = p16;
	pieceField[7][1] = p17;

	pieceField[0][7] = r70;
	pieceField[1][7] = n71;
	pieceField[2][7] = b75;
	pieceField[3][7] = k73;
	pieceField[4][7] = q74;
	pieceField[5][7] = b72;
	pieceField[6][7] = n76;
	pieceField[7][7] = r77;

	pieceField[0][6] = p60;
	pieceField[1][6] = p61;
	pieceField[2][6] = p62;
	pieceField[3][6] = p63;
	pieceField[4][6] = p64;
	pieceField[5][6] = p65;
	pieceField[6][6] = p66;
	pieceField[7][6] = p67;

	for(int j = 2; j < 6; j++){
		for (int i = 0; i < 8; i++){
			pieceField[i][j] = nullptr;
		}
	}
	calculateAllMoves();
}

Game::Game() : turn(Piece::PieceTeam::WHITE), univHandle(), checkEnPassant(true){
	
	Piece::PiecePosition posi;
	posi.xCoord=0;
	posi.yCoord=1;

	//rendering the pawns
	p10 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::WHITE, posi, univHandle);

	posi.xCoord++;

	p11 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::WHITE, posi, univHandle);

	posi.xCoord++;

	p12 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::WHITE, posi, univHandle);

	posi.xCoord++;

	p13 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::WHITE, posi, univHandle);

	posi.xCoord++;

	p14 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::WHITE, posi, univHandle);

	posi.xCoord++;


	posi.xCoord=7;

	r07 = new Rook(Rook::PieceType::ROOK, Rook::PieceTeam::WHITE, posi, univHandle);

	posi.yCoord=7;

	r77 = new Rook(Rook::PieceType::ROOK, Rook::PieceTeam::BLACK, posi, univHandle);

	posi.xCoord=0;

	r70 = new Rook(Rook::PieceType::ROOK, Rook::PieceTeam::BLACK, posi, univHandle);
	
	//finish rook rendering

	//rendering the knights

	posi.xCoord=1;
	posi.yCoord=0;

	n01 = new Knight(Knight::PieceType::KNIGHT, Knight::PieceTeam::WHITE, posi, univHandle);	
	
	posi.xCoord=6;
	n06 = new Knight(Knight::PieceType::KNIGHT, Knight::PieceTeam::WHITE, posi, univHandle);

	posi.yCoord = 7;
	n76 = new Knight(Knight::PieceType::KNIGHT, Knight::PieceTeam::BLACK, posi, univHandle);

	posi.xCoord=1;
	n71 = new Knight(Knight::PieceType::KNIGHT, Knight::PieceTeam::BLACK, posi, univHandle);

	//finish knight rendering

	//rendering the bishops

	posi.xCoord = 2;
	posi.yCoord = 0;

	b02 = new Bishop(Bishop::PieceType::BISHOP,	Bishop::PieceTeam::WHITE, posi, univHandle);

	posi.xCoord=5;
	b05 = new Bishop(Bishop::PieceType::BISHOP, Bishop::PieceTeam::WHITE, posi, univHandle);

	posi.yCoord = 7;
	b72 = new Bishop(Bishop::PieceType::BISHOP, Bishop::PieceTeam::BLACK, posi, univHandle);

	posi.xCoord = 2;
	b75 = new Bishop(Bishop::PieceType::BISHOP, Bishop::PieceTeam::BLACK, posi, univHandle);

	//finish bishop rendering

	//rendering the kings

	posi.xCoord = 3;
	posi.yCoord = 0;

	k03 = new King(King::PieceType::KING, King::PieceTeam::WHITE, posi, univHandle);

	posi.yCoord=7;

	k73 = new King(King::PieceType::KING, King::PieceTeam::BLACK, posi, univHandle);

	//finish king rendering

	//rendering the queens

	posi.xCoord=4;
	posi.yCoord=0;

	q04 = new Queen(Queen::PieceType::QUEEN, Queen::PieceTeam::WHITE, posi, univHandle);

	p15 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::WHITE, posi, univHandle);

	posi.xCoord++;

	p16 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::WHITE, posi, univHandle);

	posi.xCoord++;

	p17 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::WHITE, posi, univHandle);

	posi.xCoord=0;
	posi.yCoord=6;

	p60 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::BLACK, posi, univHandle);

	posi.xCoord++;
	
	p61 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::BLACK, posi, univHandle);

	posi.xCoord++;

	p62 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::BLACK, posi, univHandle);

	posi.xCoord++;

	p63 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::BLACK, posi, univHandle);

	posi.xCoord++;

	p64 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::BLACK, posi, univHandle);

	posi.xCoord++;

	p65 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::BLACK, posi, univHandle);

	posi.xCoord++;

	p66 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::BLACK, posi, univHandle);

	posi.xCoord++;

	p67 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::BLACK, posi, univHandle);

	//finish pawn rendering

	//rendering the rooks
	
	posi.xCoord=0;
	posi.yCoord=0;

	r00 = new Rook(Rook::PieceType::ROOK, Rook::PieceTeam::WHITE, posi, univHandle);
	posi.yCoord = 7;

	q74 = new Queen(Queen::PieceType::QUEEN, Queen::PieceTeam::BLACK, posi, univHandle);

		
	pieceField[0][0] = r00;
	pieceField[1][0] = n01;
	pieceField[2][0] = b02;
	pieceField[3][0] = k03;
	pieceField[4][0] = q04;
	pieceField[5][0] = b05;
	pieceField[6][0] = n06;
	pieceField[7][0] = r07;

	pieceField[0][1] = p10;
	pieceField[1][1] = p11;
	pieceField[2][1] = p12;
	pieceField[3][1] = p13;
	pieceField[4][1] = p14;
	pieceField[5][1] = p15;
	pieceField[6][1] = p16;
	pieceField[7][1] = p17;

	pieceField[0][7] = r70;
	pieceField[1][7] = n71;
	pieceField[2][7] = b72;
	pieceField[3][7] = k73;
	pieceField[4][7] = q74;
	pieceField[5][7] = b75;
	pieceField[6][7] = n76;
	pieceField[7][7] = r77;

	pieceField[0][6] = p60;
	pieceField[1][6] = p61;
	pieceField[2][6] = p62;
	pieceField[3][6] = p63;
	pieceField[4][6] = p64;
	pieceField[5][6] = p65;
	pieceField[6][6] = p66;
	pieceField[7][6] = p67;

	for(int j = 2; j < 6; j++){
		for (int i = 0; i < 8; i++){
			pieceField[i][j] = nullptr;
		}
	}
	calculateAllMoves();
}

Game::Game(Board* handler) : turn(Piece::PieceTeam::WHITE), univHandle(handler), checkEnPassant(true) {

	Piece::PiecePosition posi;
	posi.xCoord=0;
	posi.yCoord=1;

	//rendering the pawns
	p10 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::WHITE, posi, univHandle);

	posi.xCoord++;

	p11 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::WHITE, posi, univHandle);

	posi.xCoord++;

	p12 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::WHITE, posi, univHandle);

	posi.xCoord++;

	p13 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::WHITE, posi, univHandle);

	posi.xCoord++;

	p14 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::WHITE, posi, univHandle);

	posi.xCoord++;

	p15 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::WHITE, posi, univHandle);

	posi.xCoord++;

	p16 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::WHITE, posi, univHandle);

	posi.xCoord++;

	p17 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::WHITE, posi, univHandle);

	posi.xCoord=0;
	posi.yCoord=6;

	p60 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::BLACK, posi, univHandle);

	posi.xCoord++;
	
	p61 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::BLACK, posi, univHandle);

	posi.xCoord++;

	p62 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::BLACK, posi, univHandle);

	posi.xCoord++;

	p63 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::BLACK, posi, univHandle);

	posi.xCoord++;

	p64 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::BLACK, posi, univHandle);

	posi.xCoord++;

	p65 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::BLACK, posi, univHandle);

	posi.xCoord++;

	p66 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::BLACK, posi, univHandle);

	posi.xCoord++;

	p67 = new Pawn(Pawn::PieceType::PAWN, Pawn::PieceTeam::BLACK, posi, univHandle);

	//finish pawn rendering

	//rendering the rooks
	
	posi.xCoord=0;
	posi.yCoord=0;

	r00 = new Rook(Rook::PieceType::ROOK, Rook::PieceTeam::WHITE, posi, univHandle);

	posi.xCoord=7;

	r07 = new Rook(Rook::PieceType::ROOK, Rook::PieceTeam::WHITE, posi, univHandle);

	posi.yCoord=7;

	r77 = new Rook(Rook::PieceType::ROOK, Rook::PieceTeam::BLACK, posi, univHandle);

	posi.xCoord=0;

	r70 = new Rook(Rook::PieceType::ROOK, Rook::PieceTeam::BLACK, posi, univHandle);
	
	//finish rook rendering

	//rendering the knights

	posi.xCoord=1;
	posi.yCoord=0;

	n01 = new Knight(Knight::PieceType::KNIGHT, Knight::PieceTeam::WHITE, posi, univHandle);	
	
	posi.xCoord=6;
	n06 = new Knight(Knight::PieceType::KNIGHT, Knight::PieceTeam::WHITE, posi, univHandle);

	posi.yCoord = 7;
	n76 = new Knight(Knight::PieceType::KNIGHT, Knight::PieceTeam::BLACK, posi, univHandle);

	posi.xCoord=1;
	n71 = new Knight(Knight::PieceType::KNIGHT, Knight::PieceTeam::BLACK, posi, univHandle);

	//finish knight rendering

	//rendering the bishops

	posi.xCoord = 2;
	posi.yCoord = 0;

	b02 = new Bishop(Bishop::PieceType::BISHOP,	Bishop::PieceTeam::WHITE, posi, univHandle);

	posi.xCoord=5;
	b05 = new Bishop(Bishop::PieceType::BISHOP, Bishop::PieceTeam::WHITE, posi, univHandle);

	posi.yCoord = 7;
	b72 = new Bishop(Bishop::PieceType::BISHOP, Bishop::PieceTeam::BLACK, posi, univHandle);

	posi.xCoord = 2;
	b75 = new Bishop(Bishop::PieceType::BISHOP, Bishop::PieceTeam::BLACK, posi, univHandle);

	//finish bishop rendering

	//rendering the kings

	posi.xCoord = 3;
	posi.yCoord = 0;

	k03 = new King(King::PieceType::KING, King::PieceTeam::WHITE, posi, univHandle);

	posi.yCoord=7;

	k73 = new King(King::PieceType::KING, King::PieceTeam::BLACK, posi, univHandle);

	//finish king rendering

	//rendering the queens

	posi.xCoord=4;
	posi.yCoord=0;

	q04 = new Queen(Queen::PieceType::QUEEN, Queen::PieceTeam::WHITE, posi, univHandle);

	posi.yCoord = 7;

	q74 = new Queen(Queen::PieceType::QUEEN, Queen::PieceTeam::BLACK, posi, univHandle);

		
	pieceField[0][0] = r00;
	pieceField[1][0] = n01;
	pieceField[2][0] = b02;
	pieceField[3][0] = k03;
	pieceField[4][0] = q04;
	pieceField[5][0] = b05;
	pieceField[6][0] = n06;
	pieceField[7][0] = r07;

	pieceField[0][1] = p10;
	pieceField[1][1] = p11;
	pieceField[2][1] = p12;
	pieceField[3][1] = p13;
	pieceField[4][1] = p14;
	pieceField[5][1] = p15;
	pieceField[6][1] = p16;
	pieceField[7][1] = p17;

	pieceField[0][7] = r70;
	pieceField[1][7] = n71;
	pieceField[2][7] = b75;
	pieceField[3][7] = k73;
	pieceField[4][7] = q74;
	pieceField[5][7] = b72;
	pieceField[6][7] = n76;
	pieceField[7][7] = r77;

	pieceField[0][6] = p60;
	pieceField[1][6] = p61;
	pieceField[2][6] = p62;
	pieceField[3][6] = p63;
	pieceField[4][6] = p64;
	pieceField[5][6] = p65;
	pieceField[6][6] = p66;
	pieceField[7][6] = p67;

	for(int j = 2; j < 6; j++){
		for (int i = 0; i < 8; i++){
			pieceField[i][j] = nullptr;
		}
	}
	calculateAllMoves();
}

void Game::renderPossibleMoves(Piece* piece){
	piece->calculatePossibleMoves(pieceField, true);
	vector<Piece::PiecePosition> poss = piece->getPossibleMoves();
	SDL_Rect square;
	for(auto& move : poss){
		if((move.xCoord+move.yCoord)%2==1){
			SDL_SetRenderDrawColor(univHandle->renderer, 0, 134, 139, 255);
		}
		else{
			SDL_SetRenderDrawColor(univHandle->renderer, 164, 211, 238, 255);
		}
		square = {move.xCoord*univHandle->CELL_WIDTH, move.yCoord*univHandle->CELL_HEIGHT, univHandle->CELL_WIDTH, univHandle->CELL_HEIGHT};
		SDL_RenderFillRect(univHandle->renderer, &square);
		for(int i = 0; i<8;i++){
			for(int j = 0; j < 8; j++){
				if(pieceField[i][j]){
					pieceField[i][j]->render();
				}
			}
		}
	}
}

void Game::enPassant(int xs, int ys, int xe, int ye){
	Piece::PiecePosition posit;
	Pawn* pawnStart = (Pawn*)pieceField[xs][ys];
	pieceField[xe][ye-pawnStart->movementDirection] = nullptr;	
	pieceField[xe][ye]=getPositionInField(xs, ys);
	pieceField[xe][ye]->setHasMoved(true);
	pieceField[xs][ys]=nullptr;
	univHandle->undoPiece(xs, ys);
	univHandle->undoPiece(xe, ye-pawnStart->movementDirection);
	posit.xCoord=xe;
	posit.yCoord=ye;
	pieceField[xe][ye]->setPosition(posit);
	pieceField[xe][ye]->render();
}

void Game::castleMove(int xs, int ys, int xe, int ye){

	Piece::PiecePosition posit;

	if(xe==0){
		pieceField[1][ye]= pieceField[3][ye];
		pieceField[2][ye]= pieceField[0][ye];
		pieceField[1][ye]->setHasMoved(true);
		pieceField[2][ye]->setHasMoved(true);
		posit.xCoord=2;
		posit.yCoord=ye;
		
		pieceField[2][ye]->setPosition(posit);
		posit.xCoord=1;
		pieceField[1][ye]->setPosition(posit);
		pieceField[3][ye]=nullptr;
		pieceField[0][ye]=nullptr;
		univHandle->undoPiece(3, ye);
		univHandle->undoPiece(0, ye);
		pieceField[2][ye]->render();
		pieceField[1][ye]->render();
	}

	else{
		pieceField[6][ye]= pieceField[3][ye];
		pieceField[5][ye]= pieceField[7][ye];
		pieceField[6][ye]->setHasMoved(true);
		pieceField[5][ye]->setHasMoved(true);
		posit.xCoord=6;
		posit.yCoord=ye;
		
		pieceField[6][ye]->setPosition(posit);
		posit.xCoord=5;
		pieceField[5][ye]->setPosition(posit);
		pieceField[3][ye]=nullptr;
		pieceField[7][ye]=nullptr;
		univHandle->undoPiece(3, ye);
		univHandle->undoPiece(7, ye);
		pieceField[6][ye]->render();
		pieceField[5][ye]->render();
	}
}

void Game::promoteMove(int xs, int ys, int xe, int ye){
	SDL_Texture* rookTexture = univHandle->loadImage("img/Chess_rlt60.bmp");
	SDL_Texture* bishopTexture = univHandle->loadImage("img/Chess_blt60.bmp");
	SDL_Texture* knightTexture = univHandle->loadImage("img/Chess_nlt60.bmp");
	SDL_Texture* queenTexture = univHandle->loadImage("img/Chess_qlt60.bmp");
	int yDraw = 0;

	Piece::PieceTeam team = Piece::PieceTeam::WHITE;

	if(pieceField[xs][ys]->getTeam()==Piece::PieceTeam::BLACK){
		rookTexture = univHandle->loadImage("img/Chess_rdt60.bmp");
		bishopTexture = univHandle->loadImage("img/Chess_bdt60.bmp");
		knightTexture = univHandle->loadImage("img/Chess_ndt60.bmp");
		queenTexture = univHandle->loadImage("img/Chess_qdt60.bmp");
		int yDraw = 3*univHandle->WINDOW_HEIGHT/4;
		team = Piece::PieceTeam::BLACK;
	}

	SDL_SetRenderDrawColor(univHandle->renderer, 155, 103, 60, 255);
    SDL_Rect square = {0, yDraw, univHandle->WINDOW_WIDTH/4, univHandle->WINDOW_HEIGHT/4};
    SDL_RenderFillRect(univHandle->renderer, &square);
    SDL_Rect source = { 0, 0, 60, 60 };
    univHandle->drawRect(source, square, rookTexture);

    SDL_SetRenderDrawColor(univHandle->renderer, 255, 255, 255, 255);
    square.x = univHandle->WINDOW_WIDTH/4;
    SDL_RenderFillRect(univHandle->renderer, &square);
    univHandle->drawRect(source, square, knightTexture);

    SDL_SetRenderDrawColor(univHandle->renderer, 155, 103, 60, 255);
    square.x = 2 * univHandle->WINDOW_WIDTH/4;
    SDL_RenderFillRect(univHandle->renderer, &square);
    univHandle->drawRect(source, square, bishopTexture);

    SDL_SetRenderDrawColor(univHandle->renderer, 255, 255, 255, 255);
    square.x = 3 * univHandle->WINDOW_WIDTH/4;
    SDL_RenderFillRect(univHandle->renderer, &square);
    univHandle->drawRect(source, square, queenTexture);
	bool quit=false;
	int x = -1;
	int y = -1;

	Piece* click = nullptr;

	Piece::PiecePosition posit;

	posit.xCoord = xe;
	posit.yCoord = ye;
	while(!quit){
		while(SDL_WaitEvent(&univHandle->event)){
			if(univHandle->event.type==SDL_QUIT){
				quit=true;
				break;
			}
			if(univHandle->event.type==SDL_MOUSEBUTTONDOWN){
				x = univHandle->event.button.x/160;
				y = univHandle->event.button.y/160;
				if(y>=yDraw/160 && y<yDraw/160+1){
					if(x<univHandle->WINDOW_WIDTH/640){
						click = new Rook(Piece::PieceType::ROOK, team, posit, univHandle);
					}
					else if(x<2*univHandle->WINDOW_WIDTH/640){
						click = new Knight(Piece::PieceType::KNIGHT, team, posit, univHandle);
					}
					else if(x<3*univHandle->WINDOW_WIDTH/640){
						click = new Bishop(Piece::PieceType::BISHOP, team, posit, univHandle);
					}
					else if(x<4*univHandle->WINDOW_WIDTH/640){
						click = new Queen(Piece::PieceType::QUEEN, team, posit, univHandle);
					}
					cout<<"x"<<' '<<univHandle->WINDOW_WIDTH/640<<endl;

				}
			}
			if(univHandle->event.type == SDL_MOUSEBUTTONDOWN && click){
				quit = true;
				break;
			}
		}
	}
	pieceField[xe][ye]=click;
	pieceField[xs][ys]=nullptr;
	univHandle->undoPiece(xs, ys);
	univHandle->renderBackground();

	for(int i = 0; i<8;i++){
		for(int j = 0; j < 8; j++){
			if(pieceField[i][j]){
				pieceField[i][j]->render();
			}
		}
	}
	SDL_DestroyTexture(rookTexture);
	SDL_DestroyTexture(bishopTexture);
	SDL_DestroyTexture(knightTexture);
	SDL_DestroyTexture(queenTexture);

}

void Game::undoRenderOfPossibleMoves(Piece* piece){
	vector<Piece::PiecePosition> poss = piece->getPossibleMoves();
	SDL_Rect square;
	for(auto& move : poss){
		if((move.xCoord+move.yCoord)%2==1){
			SDL_SetRenderDrawColor(univHandle->renderer, 155, 103, 60, 255);
		}
		else{
			SDL_SetRenderDrawColor(univHandle->renderer, 160, 255, 255, 255);
		}
		square = {move.xCoord*univHandle->CELL_WIDTH, move.yCoord*univHandle->CELL_HEIGHT, univHandle->CELL_WIDTH, univHandle->CELL_HEIGHT};
		SDL_RenderFillRect(univHandle->renderer, &square);
		for(int i = 0; i<8;i++){
			for(int j = 0; j < 8; j++){
				if(pieceField[i][j]){
					pieceField[i][j]->render();
				}
			}
		}
	}
}

Game::~Game(){
}

//=============================END OF GAME FUNCTIONS===============

//  __       __         ______         ______        __    __ 
// |  \     /  \       /      \       |      \      |  \  |  \ 
// | $$\   /  $$      |  $$$$$$\       \$$$$$$      | $$\ | $$
// | $$$\ /  $$$      | $$__| $$        | $$        | $$$\| $$
// | $$$$\  $$$$      | $$    $$        | $$        | $$$$\ $$
// | $$\$$ $$ $$      | $$$$$$$$        | $$        | $$\$$ $$
// | $$ \$$$| $$      | $$  | $$       _| $$_       | $$ \$$$$
// | $$  \$ | $$      | $$  | $$      |   $$ \      | $$  \$$$
//  \$$      \$$       \$$   \$$       \$$$$$$       \$$   \$$
                                                           
                                                           
                                                           

int main(int argc, char* argv[]){

	cout<<"Type s to start or e to exit: ";
	char *c = new char[2];

	cin.getline(c, 2);

	if(c[0]=='e'){
		delete[] c;
		return 0;
	}

	else if(c[0]=='s'){

		Board* handle = new Board();

		handle->renderBackground();

		Game* game = new Game(handle);
	//	cout<<SDL_GetError()<<endl;
	//	cout<<SDL_GetError()<<endl;

		bool quit=false;
	//	cout<<SDL_GetError()<<endl;

		int xs=-1, ys=-1, xe=-1, ye=-1;
	//	cout<<SDL_GetError()<<endl;

		Piece* click = nullptr;
	//	cout<<SDL_GetError()<<endl;

		while(!quit){
			cout<<SDL_GetError()<<endl;
			while(SDL_WaitEvent(&handle->event)){
	//			cout<<SDL_GetError()<<endl;
				if(handle->event.type == SDL_QUIT){
	//				cout<<SDL_GetError()<<endl;
					quit=1;
					break;
				}
				if(handle->event.type == SDL_MOUSEBUTTONDOWN){
	//				cout<<SDL_GetError()<<endl;
					xs = handle->event.button.x/80;
					ys = handle->event.button.y/80;
					click = game->getPositionInField(xs, ys);
					if(click){
						if(click->getTeam() == game->getTurn()){
							game->renderPossibleMoves(click);
						}
					}
				}
			
				else if(handle->event.type == SDL_MOUSEBUTTONUP){
	//				cout<<SDL_GetError()<<endl;
	//				cout<<SDL_GetError()<<endl;
					xe = handle->event.button.x/80;
					ye = handle->event.button.y/80;
					if(click){
						if(click->getTeam() == game->getTurn()){
							game->undoRenderOfPossibleMoves(click);
						}
						if(xs!=-1&&ys!=-1&&xe!=-1&&ye!=-1&&click->getTeam() == game->getTurn()&&game->isValidMove(xe,ye,click)){
	//						cout<<SDL_GetError()<<endl;
							vector<Piece::PiecePosition> list = game->getPositionInField(xs, ys)->getPossibleMoves();
	//						cout<<SDL_GetError()<<endl;
							for(auto& val : list){
								if(val.xCoord == xe && val.yCoord == ye){
									game->move(click, val);
	//							cout<<SDL_GetError()<<endl;
								}

							}
							xs=-1;
							ys=-1;
							xe=-1;
							ye=-1;
		//					cout<<SDL_GetError()<<endl;
							game->calculateAllMoves();
			//				cout<<SDL_GetError()<<endl;
							click=nullptr;
						}
					}
				}
			}
		}
		delete[] c;
		delete game;
		delete handle;
	}
	else{
		cout<<"Not a valid input";
		delete[] c;
	}
	return 0;
}

    //                            ________        __    __        _______                                          
    //                           |        \      |  \  |  \      |       \                                         
    //                           | $$$$$$$$      | $$\ | $$      | $$$$$$$\                                        
    //                           | $$__          | $$$\| $$      | $$  | $$                                        
    //                           | $$  \         | $$$$\ $$      | $$  | $$                                        
    //                           | $$$$$         | $$\$$ $$      | $$  | $$                                        
    //                           | $$_____       | $$ \$$$$      | $$__/ $$                                        
    //                           | $$     \      | $$  \$$$      | $$    $$                                        
    //                            \$$$$$$$$       \$$   \$$       \$$$$$$$                                         
                                                                                                                
                                                                                                                
                                                                                                                
    //                                   ______         ________                                                   
    //                                  /      \       |        \                                                  
    //                                 |  $$$$$$\      | $$$$$$$$                                                  
    //                                 | $$  | $$      | $$__                                                      
    //                                 | $$  | $$      | $$  \                                                     
    //                                 | $$  | $$      | $$$$$                                                     
    //                                 | $$__/ $$      | $$                                                        
    //                                  \$$    $$      | $$                                                        
    //                                   \$$$$$$        \$$                                                        
                                                                                                                
                                                                                                                
                                                                                                                
    //    _______         _______          ______            _____        ________         ______         ________ 
    //   |       \       |       \        /      \          |     \      |        \       /      \       |        \
    //   | $$$$$$$\      | $$$$$$$\      |  $$$$$$\          \$$$$$      | $$$$$$$$      |  $$$$$$\       \$$$$$$$$
    //   | $$__/ $$      | $$__| $$      | $$  | $$            | $$      | $$__          | $$   \$$         | $$   
    //   | $$    $$      | $$    $$      | $$  | $$       __   | $$      | $$  \         | $$               | $$   
    //   | $$$$$$$       | $$$$$$$\      | $$  | $$      |  \  | $$      | $$$$$         | $$   __          | $$   
    //   | $$            | $$  | $$      | $$__/ $$      | $$__| $$      | $$_____       | $$__/  \         | $$   
    //   | $$            | $$  | $$       \$$    $$       \$$    $$      | $$     \       \$$    $$         | $$   
    //    \$$             \$$   \$$        \$$$$$$         \$$$$$$        \$$$$$$$$        \$$$$$$           \$$   
                                                                                                                
                                                                                                                
                                                                                                                