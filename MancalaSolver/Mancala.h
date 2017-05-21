#pragma once
#include <iostream>
#include <stdint.h>

int opposite[14] = {
	12,11,10,9,8,7,0,
	5,4,3,2,1,0,0
};

using namespace std;

struct Board {
	union {
		int8_t raw[14];
		struct {
			int8_t side1[6];
			int8_t pot1;
			int8_t side2[6];
			int8_t pot2;
		};
	};
	bool isValid;
	bool isRoot;
	bool player;
	bool isLast;
	Board * parent;
	int8_t lastMove;
	bool isGen;
	Board * moves[6];
	bool livingChildren[6];
	Board() {
		for (int i = 0; i < 14; i++) {
			if (i == 6 || i == 13){
				raw[i] = 0;
			}
			else {
				raw[i] = 4;
			}
		}
		player = 1;
		isLast = 0;
		isRoot = 1;
		parent = nullptr;
		lastMove = 0;
		isGen = false;
	}
	Board(Board * p_board) {
		for (int i = 0; i < 14; i++) {
			raw[i] = p_board->raw[i];
		}
		isLast = 0;
		isRoot = 0;
		parent = p_board;
		isGen = false;
	}
	void move(int i_move) {
		moves[i_move] = new Board(this);
		moves[i_move]->lastMove = i_move;
		int start = (player) ? (i_move) : (i_move + 7);
		int spaces = moves[i_move]->raw[start];
		if (!spaces) {
			moves[i_move]->isValid = 0;
			moves[i_move]->player = 0;
			return;
		}
		else {
			moves[i_move]->isValid = 1;
		}
		int hand_pos = start;
		moves[i_move]->raw[start] = 0;
		for (int i = 0; i < spaces; i++) {
			hand_pos++;
			hand_pos %= 14;
			hand_pos = (player && hand_pos == 13) ? (0) : (hand_pos);
			hand_pos = (!player && hand_pos == 6) ? (7) : (hand_pos);
			moves[i_move]->raw[hand_pos]++;
		}
		if (hand_pos == 13 || hand_pos == 6) {
			moves[i_move]->player = player;
		}
		else if (raw[hand_pos] == 1 && hand_pos < ((player)?(6):(13)) && hand_pos > ((player) ? (-1) : (6))){
			moves[i_move]->raw[(player) ? (6) : (13)] += moves[i_move]->raw[hand_pos];
			moves[i_move]->raw[(player) ? (6) : (13)] += moves[i_move]->raw[opposite[(player) ? (6) : (13)]];
			moves[i_move]->raw[opposite[(player) ? (6) : (13)]] = 0;
			moves[i_move]->raw[hand_pos] = 0;
			moves[i_move]->player = !player;
		}
		else {
			moves[i_move]->player = !player;
		}
		if (!moves[i_move]->raw[0] &&
			!moves[i_move]->raw[1] &&
			!moves[i_move]->raw[2] &&
			!moves[i_move]->raw[3] &&
			!moves[i_move]->raw[4] &&
			!moves[i_move]->raw[5])
		{
			for (int i = 7; i < 13; i++) {
				moves[i_move]->raw[13] += moves[i_move]->raw[i];
				moves[i_move]->raw[i] = 0;
			}
			moves[i_move]->isLast = 1;
			moves[i_move]->player = 0;
		}
		if (!moves[i_move]->raw[7] &&
			!moves[i_move]->raw[8] &&
			!moves[i_move]->raw[9] &&
			!moves[i_move]->raw[10] &&
			!moves[i_move]->raw[11] &&
			!moves[i_move]->raw[12])
		{
			for (int i = 0; i < 6; i++) {
				moves[i_move]->raw[13] += moves[i_move]->raw[i];
				moves[i_move]->raw[i] = 0;
			}
			moves[i_move]->isLast = 1;
			moves[i_move]->player = 0;
		}
	}
	void genMoves() {
		for (int i = 0; i < 6; i++) {
			move(i);
			livingChildren[i] = true;
		}
		isGen = true;
	}
	~Board() {
		parent->livingChildren[lastMove] = false;
		if (isGen) {
			for (int i = 0; i < 6; i++) {
				if (livingChildren[i]) {
					delete moves[i];
				}
			}
		}
	}
};