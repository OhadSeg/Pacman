#include "pacman.h"

void Pacman::GameObjMove(Board& b) {

	int dirR = 0, dirC = 0;

	determineDirections(_c, dirR, dirC);

	ifInTunnel(b, _c, dirR, dirC);

	if (b.getCellFromBoard({ _gameObjLocation.row + dirR, _gameObjLocation.colm + dirC }) != '#') {

		_gameObjPreviousLocation.row = _gameObjLocation.row;
		_gameObjPreviousLocation.colm = _gameObjLocation.colm;
		_gameObjLocation.row += dirR;
		_gameObjLocation.colm += dirC;
	}
}

void Pacman::setPacmanC(char c) {

	_c = c;
}

bool Pacman::ifInTunnel(Board& b, char c, int& dirR, int& dirC) {

	if (_gameObjLocation.colm == 0 && (c == LEFT || c == left)) {
		dirC += b.getColmsOfBoard() - 1;
	}
	else if (_gameObjLocation.colm == (b.getColmsOfBoard() - 2) && (c == RIGHT || c == right)) {
		dirC = -(b.getColmsOfBoard() - 2);
	}
	else if (_gameObjLocation.row == 0 && (c == UP || c == up)) {
		dirR += b.getRowsOfBoard();
	}
	else if (_gameObjLocation.row == b.getRowsOfBoard() - 1 && (c == DOWN || c == down)) {
		dirR = -(b.getRowsOfBoard() - 1);
	}
	return true;
}