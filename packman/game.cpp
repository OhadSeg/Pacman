#include "game.h"

void Game::start()
{
	system("cls");

	int mapIdx = 0;

	while (mapIdx < _filenames.size() != 0 && !ifLost(_lives)) {

		Board b(_filenames[mapIdx] + ".screen");

		_breadCrumbsCounter = b.initBreadVec();
		Point pacStartLocation;
		system("cls");
		b.printBoard(_ifColored);
		b.printStats(_lives, _score);

		std::vector<Ghost> ghostVec;

		initGameObj(b, pacStartLocation, ghostVec);

		Pacman player(pacStartLocation);

		Fruit f(b.findFreePlace(), drawNum5to9());

		run(b, player, ghostVec, f);

		ghostVec.clear();

		mapIdx++;

		printDiffMesg(mapIdx);
	}
}

void Game::run(Board& board, Pacman& player, std::vector<Ghost>& ghostVec, Fruit& fruit) {

	bool ghostTouchPac = false;
	int slowerTheGhost = 1;
	int fruitLifeCycle = 1;
	char c = 0, lastMove = 0;
	int numOfGhosts = ghostVec.size();

	int countNormalMoves = 0;


	Board::gotoxy(player.getObjLocation().colm, player.getObjLocation().row);

	while (!ifWon(_breadCrumbsCounter) && !ifLost(_lives)) {

		lastMove = c;
		if (_kbhit())
			c = _getch();

		Sleep(90);

		while (!ifValidMove(c) && (lastMove == 0)) {
			c = _getch();
		}

		if (c == ESC)
			pauseMode(board);

		if (!ifValidMove(c))
			c = lastMove;

		player.setPacmanC(c);

		if (countNormalMoves >= 20 && _gameLevel == '2') {

			for (int i = 0; i < numOfGhosts; i++) {
				Strategy* s = nullptr;
				if (countNormalMoves == 24) {
					s = new SmartestMove;
					ghostVec[i].setLevel(s);
				}
				else {
					s = new NoviceMove;
					ghostVec[i].setLevel(s);
				}
			}

			if (countNormalMoves == 24)
				countNormalMoves = 0;
		}
		countNormalMoves++;

		if (slowerTheGhost % 2 == 0) {
			for (int i = 0; i < numOfGhosts; i++) {
				ghostVec[i].GameObjMove(board);
				moveObjOnBoard(board, ghostVec[i]);
				if (player.getObjLocation() == ghostVec[i].getObjLocation()) { //operator overloading
					ghostTouchPac = true;
				}
				if (ghostVec[i].getObjLocation() == fruit.getObjLocation() && !fruit.getIfFruitShown())
					fruitLifeCycle = 0;
			}
		}slowerTheGhost++;

		if (fruit.getIfFruitShown() && fruitLifeCycle != 0) {
			fruit.GameObjMove(board);
			moveObjOnBoard(board, fruit);
			board.printChanges(fruit.getObjPreviousLocation(), fruit.getObjLocation(), _ifColored);
		}

		for (int i = 0; i < numOfGhosts; i++) {

			board.printChanges(ghostVec[i].getObjPreviousLocation(), ghostVec[i].getObjLocation(), _ifColored);
		}

		if (!ghostTouchPac) {

			player.GameObjMove(board);


			if (board.getCellFromBoard(player.getObjLocation()) == '$') {
				ghostTouchPac = true;

				player.setObjLocation(player.getObjPreviousLocation());
			}
			else {
				board.printChanges(fruit.getObjPreviousLocation(), fruit.getObjLocation(), _ifColored);
				moveObjOnBoard(board, player);
			}
		}
		else {

			_lives--;

			resetGameObj(board, player);

			for (int i = 0; i < numOfGhosts; i++) {

				resetGameObj(board, ghostVec[i]);
			}
			system("cls");
			fruitLifeCycle = 0;
			board.printBoard(_ifColored);
			ghostTouchPac = false;
			c = 0;
		}

		pacEatBread(board, player);

		if (fruitLifeCycle % 70 == 0) {

			board.setCellFromBoard(board.getCellFromBreadVec(fruit.getObjLocation()), fruit.getObjLocation());
			board.printChanges(fruit.getObjPreviousLocation(), fruit.getObjLocation(), _ifColored);
			fruit.setIfFruitShown(!fruit.getIfFruitShown());
			fruit.setObjLocation(board.findFreePlace());

			fruit.setObjSymbol(drawNum5to9());

		}fruitLifeCycle++;

		if (player.getObjLocation() == fruit.getObjLocation() && fruit.getIfFruitShown()) {

			_score += fruit.getObjSymbol() - '0';
			fruitLifeCycle = 0;
		}

		board.printChanges(player.getObjPreviousLocation(), player.getObjLocation(), _ifColored);
		for (int i = 0; i < numOfGhosts; i++) {
			board.printChanges(ghostVec[i].getObjPreviousLocation(), ghostVec[i].getObjLocation(), _ifColored);
		}
		board.printStats(_lives, _score);
	}
}

	void Game::pauseMode(Board& b) {

	char c = 0;
	while (c != ESC) {
		Board::gotoxy(b.getstatsPrintPosition().colm, b.getstatsPrintPosition().row);
		std::cout << " GAME PAUSED ";

		Board::gotoxy(b.getstatsPrintPosition().colm, b.getstatsPrintPosition().row + 1);
		std::cout << " press ESC to back ";
		c = _getch();
	}
	Board::gotoxy(b.getstatsPrintPosition().colm, b.getstatsPrintPosition().row);
	std::cout << "####################";
	Board::gotoxy(b.getstatsPrintPosition().colm, b.getstatsPrintPosition().row + 1);
	std::cout << "####################";
}

void Game::pacEatBread(Board& b, Pacman& player) {

	if (b.getCellFromBreadVec(player.getObjLocation()) == '*') {
		_score++;
		_breadCrumbsCounter--;
		b.setCellFromBreadVec(player.getObjLocation());
	}
}

void Game::moveObjOnBoard(Board& b, GameObject& creature) {

	b.setCellFromBoard(b.getCellFromBreadVec(creature.getObjPreviousLocation()), creature.getObjPreviousLocation());
	b.setCellFromBoard(creature.getObjSymbol(), creature.getObjLocation());
}

void Game::resetGameObj(Board& b, GameObject& creature) {

	creature.setObjPreviousLocation(creature.getObjLocation());
	creature.setObjLocation(creature.getObjStartingLocation());
	moveObjOnBoard(b, creature);
}

void Game::initGameObj(Board& b, Point& pacStartLocation, std::vector<Ghost>& ghostVec) {

	int currRow, currColm;

	int bRows = b.getRowsOfBoard();
	int bColms = b.getColmsOfBoard();

	for (int i = 0; i < bRows * bColms; i++) {

		currRow = i / bColms;
		currColm = i - (currRow * bColms);

		if (b.getCellFromBoard({ currRow ,currColm }) == '@') {

			pacStartLocation = { currRow, currColm };
		}
		else if (b.getCellFromBoard({ currRow ,currColm }) == '$') {
			Strategy* s = nullptr;
			if (_gameLevel == '1') {
	
				s = new NoviceMove;
			}
			else {
		
				s = new SmartestMove;
			}
			ghostVec.push_back({ { currRow ,currColm }, '$' , s });

		}		
	}
}

void Game::printDiffMesg(int& mapIdx) {

	system("cls");

	if (_lives == 0) {

		std::cout << "~GAME OVER~" << std::endl;
		std::cout << "Press any key in order to return to the menu";
		char c = 0;
		c = _getch();
	}
	else {
		if (mapIdx < _filenames.size()) {
			std::cout << "You have passed stage " << mapIdx + 1 << std::endl;
			std::cout << "you are mooving to the next stage";
			Sleep(2000);

		}
		else {
			std::cout << "Congratulations, You Won!" << std::endl;
			std::cout << "Press any key in order to return to the menu";
			char c = 0;
			c = _getch();
		}
	}

}