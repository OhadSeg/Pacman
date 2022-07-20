#include "ghost.h"

void Ghost::GameObjMove(Board& b) {
	
	Point cur = _gameObjLocation;
	Point prev = _gameObjPreviousLocation;
	
	_level->moveStrategy(b, cur, prev);

	if (!ifInTunnel(b, 0, cur.row, cur.colm)) {
		_gameObjLocation = cur;
		_gameObjPreviousLocation = prev;
	}
	else {

		_gameObjPreviousLocation = _gameObjLocation;
	}
}

void Ghost::setLevel(Strategy* level) {

	_level = level;
}

bool Ghost::ifInTunnel(Board& b, char c, int& dirR, int& dirC) {

	if (dirC == 0 || dirC == b.getColmsOfBoard() - 2 ||
		dirR == 0 || dirR == b.getRowsOfBoard() - 1)
	{
		return true;
	}
	return false;
}