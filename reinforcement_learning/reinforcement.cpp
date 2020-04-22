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
#define N_OF_TRIES 3*1000
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
			if(y-1 >= START_Y)
				y--;
			break;
		case 3:
			if(x-1 >= START_X)
				x--;
			break;
		default: do_move(normalize_move(move), x, y);
	}
}

int choose_move(int x, int y, const vector<vector<double>>&learning_weights){
	int u=0, l=0, d=0, r=0;
	int cu, cl, cd, cr;

	if(y+1 <= MAX_Y){
		u = learning_weights[x][y+1]*1000;
	}
	if(x+1 <= MAX_X){
		r = learning_weights[x+1][y]*1000;
	}
	if(y-1 >= START_Y){
		d = learning_weights[x][y-1]*1000;
	}
	if(x-1 >= START_X){
		l = learning_weights[x-1][y]*1000;
	}
	cu = u; cr = cu + r; cd = cr + d; cl = cd + l;
	int random = rand()%(u+l+d+r);
	if(random < cu){
		return GO_UP;
	} else if(random < cr){
		return GO_RIGHT;
	} else if(random < cd){
		return GO_DOWN;
	} else {
		return GO_LEFT;
	}
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

void play_game(vector<int> &moves, vector<int> &xs, vector<int> &ys, const vector<vector<double>> &learning_weights){
	int x = START_X, y = START_Y;
	do{
		int move = choose_move(x, y, learning_weights);
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

void update_learning_weights(const vector<int> &xs, const vector<int> &ys, vector<vector<double>> &weights){
	if(xs.back() != GOAL_X || ys.back() != GOAL_Y){
		for (int i = 0; i < xs.size(); i++){
			weights[xs[i]][ys[i]] -= 1;
			if(weights[xs[i]][ys[i]] < 0.04){
				weights[xs[i]][ys[i]] = 0.04;
			}
		}
	} else {
		for (int i = 0; i < xs.size(); i++){
			weights[xs[i]][ys[i]] += 0.04;
		}
	}

}

int main(int argc, char** argv){
	srand(time(NULL));
	vector<vector<int>> solutions, solutions_x, solutions_y;
	vector<pair<double, int>> scores;
	vector<vector<double>> learning_weights;
	learning_weights.resize(MAX_X+1, vector<double>(MAX_Y+1, 0.04));

	for(auto &x : learning_weights){
		for (auto y : x){
			cout << y << ' ';
		}
		cout << endl;
	}
	cout << endl;

	int no_learning_succeed = 0;
	int in_learning_succeed = 0;
	int after_learning_succeed = 0;

	for(int i = 0; i < N_OF_TRIES; i++){
		vector<int> moves, solution_x, solution_y;
		double score;
		play_game(moves, solution_x, solution_y, learning_weights);
		score = calc_score(moves, solution_x.back(), solution_y.back());
		solutions.push_back(moves);
		solutions_x.push_back(solution_x);
		solutions_y.push_back(solution_y);
		scores.push_back({score, i});

		if(i <= N_OF_TRIES/3){
			if(solution_x.back() == GOAL_X && solution_y.back() == GOAL_Y)
				no_learning_succeed++;
		} else if(i <= 2*N_OF_TRIES/3){
			// learning_weights[GOAL_X][GOAL_Y] = 1000;
			// learning_weights[FAIL_X][FAIL_Y] = -1000;
			update_learning_weights(solution_x, solution_y, learning_weights);
			if(solution_x.back() == GOAL_X && solution_y.back() == GOAL_Y)
				in_learning_succeed++;
		} else {
			if(solution_x.back() == GOAL_X && solution_y.back() == GOAL_Y)
				after_learning_succeed++;

		}
	}


	// sort(scores.begin(), scores.end());
	
	// for(int i = 0; i<solutions.size(); i++){
	// 	auto &moves = solutions[scores[i].second];
	// 	cout << '['<<scores[i].first<<"] ";
	// 	for(auto move : moves){
	// 		cout << move << ' ';
	// 	}
	// 	cout << endl;
	// }
	// cout << endl;

	cout << "No learning succeeded: " << no_learning_succeed << endl;
	cout << "In learnign succeeded: " << in_learning_succeed << endl;
	cout << "After learnign succeeded: " << after_learning_succeed << endl;

	for(auto &x : learning_weights){
		for (auto y : x){
			cout << y << ' ';
		}
		cout << endl;
	}
	cout << endl;




	return 0;
}
