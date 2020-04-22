#include<iostream>
#include<vector>
#include<stdlib.h>
#include<time.h>
#include<algorithm>

#define START_X 1
#define MAX_X 4
#define START_Y 1
#define MAX_Y 3
#define GOAL_X 4
#define GOAL_Y 3
#define FAIL_X 4
#define FAIL_Y 2
#define N_OF_TRIES 10
#define GO_UP 0
#define GO_RIGHT 1
#define GO_DOWN 2
#define GO_LEFT 3

using namespace std;

bool is_goal(int x, int y){
	return x == GOAL_X && y == GOAL_Y;
}

bool is_fail(int x, int y){
	return x == FAIL_X && y == FAIL_Y;
}

int normalize_move(int move){
	if(move < 0){
		return 4 + (move % 4);
	}
	if(move > 3){
		return move % 4;
	}
	return move;
}

void do_move(int move, int &x, int &y){
	switch(move){
		case 0:
			if(y+1 <= MAX_Y)
				y++;
			break;
		case 1:
			if(x+1 <= MAX_X)
				x++;
			break;
		case 2:
			if(y-1 >= 0)
				y--;
			break;
		case 3:
			if(x-1 >= 0)
				x--;
			break;
		default: do_move(normalize_move(move), x, y);
	}
}

int choose_move(){
	return rand() % 4;
}

void perturbe_move(int &move){
	int random = rand() % 10;
	if(random == 0){
		move = normalize_move(move-1);
	}
	if(random == 9){
		move = normalize_move(move+1);
	}
}

void play_game(vector<int> &moves, vector<int> &xs, vector<int> &ys){
	int x = START_X, y = START_Y;
	do{
		int move = choose_move();
		perturbe_move(move);
		moves.push_back(move);
		do_move(move, x, y);
		xs.push_back(x);
		ys.push_back(y);
	} while(!is_goal(x, y) && !is_fail(x, y));
}

double calc_score(const vector<int> moves, int last_x, int last_y){
	double score = -0.04 * (moves.size()-1);
	if(is_fail(last_x, last_y)){
		score -= 1;
	} else {
		score += 1;
	}
	return score;
}

int main(int argc, char** argv){
	srand(time(NULL));
	vector<vector<int>> solutions, solutions_x, solutions_y;
	vector<pair<double, int>> scores;
	
	for(int i = 0; i < N_OF_TRIES; i++){
		vector<int> moves, solution_x, solution_y;
		double score;
		play_game(moves, solution_x, solution_y);
		score = calc_score(moves, solution_x.back(), solution_y.back());
		solutions.push_back(moves);
		solutions_x.push_back(solution_x);
		solutions_y.push_back(solution_y);
		scores.push_back({score, i});
	}

	sort(scores.begin(), scores.end());
	
	for(int i = 0; i<solutions.size(); i++){
		auto &moves = solutions[scores[i].second];
		cout << '['<<scores[i].first<<"] ";
		for(auto move : moves){
			cout << move << ' ';
		}
		cout << endl;
	}
	return 0;
}