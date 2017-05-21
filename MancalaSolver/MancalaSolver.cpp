// MancalaSolver.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Mancala.h"
using namespace std;

int main()
{
	Board * game = new Board();
	game->genMoves();
	Board * spider;
	float wins[6] = {0,0,0,0,0,0};
	float endings[6] = { 0,0,0,0,0,0};
	Board * temp;
	int level = 1;
	for (int i = 0; i < 6; i++) {
		spider = game->moves[i];
		while (!spider->isRoot) {
			if (!spider->isLast && spider->isValid) {
				spider->genMoves();
				spider = spider->moves[0];
				level++;
			}
			else {
				if (spider->isLast) {
					endings[i]++;
					if (spider->pot1 > spider->pot2) {
						wins[i]++;
					}
				}
				if (spider->lastMove != 5) {
					Board * temp = spider;
					spider = spider->parent->moves[spider->lastMove + 1];
					delete temp;
				}
				else {
					while (spider->lastMove == 5 && !spider->isRoot) {
						spider = spider->parent;
						level--;
					}
					if (!spider->isRoot) {
						Board * temp = spider;
						spider = spider->parent->moves[spider->lastMove + 1];
						delete temp;
					}
				}
			}
			cout << level << "\n";
		}
		cout << "Start " << i << " mapped.\n";
	}
    return 0;
}

