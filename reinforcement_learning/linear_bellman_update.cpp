#include<iostream>
#include<vector>
#include<stdlib.h>
#include<time.h>
#include<algorithm>
#include<math.h>

#define START_X (1)
#define MAX_X (4)
#define START_Y (1)
#define MAX_Y (3)
#define GOAL_X (4)
#define GOAL_Y (3)
#define FAIL_X (4)
#define FAIL_Y (2)
#define N_OF_TRIES (3*1000)
#define GO_UP (0)
#define GO_RIGHT (1)
#define GO_DOWN (2)
#define GO_LEFT (3)
#define N_UTILITIES (10000)
#define REWARD (-0.04)
#define GAMMA (1)
#define NO_PERTURBATION_PROB (0.8)
#define PERTURBATION_PROB_SIDE (0.1)
#define MAX_DIFF 0.005

using namespace std;

int get_max_index(const vector<double> &v){
	if(v.size() == 0) return -1;
	int max_index = 0;
	for(int i = 0; i<v.size(); i++){
		if(v[i] > v[max_index]){
			max_index = i;
		}
	}
	return max_index;
}

bool is_goal(int x, int y){
	return x == GOAL_X && y == GOAL_Y;
}

bool is_fail(int x, int y){
	return x == FAIL_X && y == FAIL_Y;
}

bool is_legal(int x, int y){
	return x >= START_X
		&& y >= START_Y
		&& x <= MAX_X
		&& y <= MAX_Y
		&& !(x == 2 && y == 2);
}

double get_action_utility_go_up(double utilities[MAX_X+1][MAX_Y+1][N_UTILITIES], int x, int y, int n_utility){
	double utility = 0;
	if(is_legal(x, y+1))
		utility += NO_PERTURBATION_PROB * utilities[x][y+1][n_utility];
	else
		utility += NO_PERTURBATION_PROB * utilities[x][y][n_utility];
	if(is_legal(x-1, y))
		utility += PERTURBATION_PROB_SIDE * utilities[x-1][y][n_utility];
	else
		utility += PERTURBATION_PROB_SIDE * utilities[x][y][n_utility];
	if(is_legal(x+1, y))
		utility += PERTURBATION_PROB_SIDE * utilities[x+1][y][n_utility];
	else
		utility += PERTURBATION_PROB_SIDE * utilities[x][y][n_utility];
	return utility;
}

double get_action_utility_go_right(double utilities[MAX_X+1][MAX_Y+1][N_UTILITIES], int x, int y, int n_utility){
	double utility = 0;
	if(is_legal(x+1, y))
		utility += NO_PERTURBATION_PROB * utilities[x+1][y][n_utility];
	else
		utility += NO_PERTURBATION_PROB * utilities[x][y][n_utility];
	if(is_legal(x, y+1))
		utility += PERTURBATION_PROB_SIDE * utilities[x][y+1][n_utility];
	else
		utility += PERTURBATION_PROB_SIDE * utilities[x][y][n_utility];
	if(is_legal(x, y-1))
		utility += PERTURBATION_PROB_SIDE * utilities[x][y-1][n_utility];
	else
		utility += PERTURBATION_PROB_SIDE * utilities[x][y][n_utility];
	return utility;
}

double get_action_utility_go_down(double utilities[MAX_X+1][MAX_Y+1][N_UTILITIES], int x, int y, int n_utility){
	double utility = 0;
	if(is_legal(x, y-1))
		utility += NO_PERTURBATION_PROB * utilities[x][y-1][n_utility];
	else
		utility += NO_PERTURBATION_PROB * utilities[x][y][n_utility];
	if(is_legal(x-1, y))
		utility += PERTURBATION_PROB_SIDE * utilities[x-1][y][n_utility];
	else
		utility += PERTURBATION_PROB_SIDE * utilities[x][y][n_utility];
	if(is_legal(x+1, y))
		utility += PERTURBATION_PROB_SIDE * utilities[x+1][y][n_utility];
	else
		utility += PERTURBATION_PROB_SIDE * utilities[x][y][n_utility];
	return utility;
}

double get_action_utility_go_left(double utilities[MAX_X+1][MAX_Y+1][N_UTILITIES], int x, int y, int n_utility){
	double utility = 0;
	if(is_legal(x-1, y))
		utility += NO_PERTURBATION_PROB * utilities[x-1][y][n_utility];
	else
		utility += NO_PERTURBATION_PROB * utilities[x][y][n_utility];
	if(is_legal(x, y+1))
		utility += PERTURBATION_PROB_SIDE * utilities[x][y+1][n_utility];
	else
		utility += PERTURBATION_PROB_SIDE * utilities[x][y][n_utility];
	if(is_legal(x, y-1))
		utility += PERTURBATION_PROB_SIDE * utilities[x][y-1][n_utility];
	else
		utility += PERTURBATION_PROB_SIDE * utilities[x][y][n_utility];
	return utility;
}

void update_utility_cell(double utilities[MAX_X+1][MAX_Y+1][N_UTILITIES], int x, int y, int n_utility){
	if(!is_legal(x, y)){
		cout << "[WARNING] impossible to update cell x:"<< x << " y:" << y << endl;
		return;
	}

	if(is_fail(x, y)){
		utilities[x][y][n_utility] = -1;
		return;
	}

	if(is_goal(x, y)){
		utilities[x][y][n_utility] = 1;
		return;
	}

	if(n_utility == 0){
		utilities[x][y][n_utility] = (double)((int)random()%10000)/1000000;//number between 0 and 0.01
		return;
	}

	int max_utility_index_nearby = get_max_index({
		y < MAX_Y ? utilities[x][y+1][n_utility-1] : -10, // go up
		x < MAX_X ? utilities[x+1][y][n_utility-1] : -10, // go right
		y > 0     ? utilities[x][y-1][n_utility-1] : -10, // go down
		x > 0     ? utilities[x-1][y][n_utility-1] : -10  // go left
	});

	double max_utility_nearby;

	switch(max_utility_index_nearby){
		case GO_UP:
			max_utility_nearby = get_action_utility_go_up(utilities, x, y, n_utility-1);
			break;
		case GO_RIGHT:
			max_utility_nearby = get_action_utility_go_right(utilities, x, y, n_utility-1);
			break;
		case GO_DOWN:
			max_utility_nearby = get_action_utility_go_down(utilities, x, y, n_utility-1);
			break;
		case GO_LEFT:
			max_utility_nearby = get_action_utility_go_left(utilities, x, y, n_utility-1);
			break;
		default: exit(-1);
	}
	
	utilities[x][y][n_utility] = (REWARD) + (GAMMA) * max_utility_nearby;
}

void update_utilities(double utilities[MAX_X+1][MAX_Y+1][N_UTILITIES], int n_utility){
	for(int x = START_X; x<=MAX_X; x++){
    	for(int y = START_Y; y<=MAX_Y; y++){
    		if(is_legal(x, y))
	    		update_utility_cell(utilities, x, y, n_utility);
    	}
    }
}

void print_utilities(double utilities[MAX_X+1][MAX_Y+1][N_UTILITIES], int n_utility){
	cout << endl << "["<<n_utility<<"]"<<endl<<endl;
	for(int y = MAX_Y; y>=START_Y; y--){
    	for(int x = START_X; x<=MAX_X; x++){
    		cout << utilities[x][y][n_utility] << '\t';
    	}
    	cout << endl;
    }
}

bool utilities_are_changing(double utilities[MAX_X+1][MAX_Y+1][N_UTILITIES], int n_utility, double max_diff){
	if(n_utility == 0){
		return true;
	}
	for(int x = 0; x < MAX_X; x++){
		for(int y = 0; y < MAX_Y; y++){
			if(abs(utilities[x][y][n_utility] - utilities[x][y][n_utility-1]) > max_diff)
				return true;
		}
	}
	return false;
}

int main(int argc, char** argv){
	cout.precision(3);
    double utilities[MAX_X+1][MAX_Y+1][N_UTILITIES] = {0};
    int n_utility = 0;
    do{
    	update_utilities(utilities, n_utility);
    	n_utility++;
    } while (n_utility < N_UTILITIES && utilities_are_changing(utilities, n_utility-1, MAX_DIFF));
  	print_utilities(utilities, n_utility-1);
}
