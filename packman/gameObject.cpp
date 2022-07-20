#include "gameObject.h"

void GameObject::setObjStartingLocation(Point p) {

	_gameObjStartingLocation.row = p.row;
	_gameObjStartingLocation.colm = p.colm;
}

void GameObject::setObjLocation(Point p) {

	_gameObjLocation.row = p.row;
	_gameObjLocation.colm = p.colm;
}

void GameObject::setObjPreviousLocation(Point p) {

	_gameObjPreviousLocation.row = p.row;
	_gameObjPreviousLocation.colm = p.colm;
}

void GameObject::setObjSymbol(char symbol) {

	_symbol = symbol;

}

Point GameObject::getObjStartingLocation() const {

	return _gameObjStartingLocation;
}

Point GameObject::getObjLocation() const {

	return _gameObjLocation;
}

char GameObject::getObjSymbol() const {

	return _symbol;
}

Point GameObject::getObjPreviousLocation() const {

	return _gameObjPreviousLocation;
}