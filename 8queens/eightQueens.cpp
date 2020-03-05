#include<iostream>
#include<vector>
#include<algorithm>
#include<chrono>
#define COLUMNS 8
#define ROWS 8

using namespace std;

struct state {
	vector<int> queens_row;
	vector<int> pos_diag;
	vector<int> neg_diag;
};

void print_solutions(vector<vector<int>> &solutions){
	cout << solutions.size() << " solutions found.\n";
	int i = 1;
	for (auto &solution : solutions){
		cout <<i++ << ": ";
		for(auto &x : solution){
			cout << x << ' ';
		}
		cout << '\n';
	}
}

int get_pos_diag(int row, int column){
	return row + column;
}

int get_neg_diag(int row, int column){
	return ROWS - row + column;
}

bool is_free_cell(const state &curr_state, int row, int column){
	return find(curr_state.queens_row.begin(), curr_state.queens_row.end(), row) == curr_state.queens_row.end()
		&& find(curr_state.pos_diag.begin(), curr_state.pos_diag.end(), get_pos_diag(row, column)) == curr_state.pos_diag.end()
		&& find(curr_state.neg_diag.begin(), curr_state.neg_diag.end(), get_neg_diag(row, column)) == curr_state.neg_diag.end();
}

vector<int> get_free_rows_next_column(const state &curr_state){
	vector<int> free_rows_next_column;
	int column = curr_state.queens_row.size();
	for(int row = 0; row < ROWS; row++){
		if(is_free_cell(curr_state, row, column)){
			free_rows_next_column.push_back(row);
		}
	}
	return free_rows_next_column;
}

void place_queen(state &curr_state, int row){
	int column = curr_state.queens_row.size();
	curr_state.queens_row.push_back(row);
	curr_state.pos_diag.push_back(get_pos_diag(row, column));
	curr_state.neg_diag.push_back(get_neg_diag(row, column));
}

void remove_queen(state &curr_state){
	curr_state.queens_row.pop_back();
	curr_state.pos_diag.pop_back();
	curr_state.neg_diag.pop_back();
}

bool column_has_some_free_cell(const state &curr_state, int column){
	for (int row = 0; row < ROWS; row++){
		if(is_free_cell(curr_state, row, column)){
			return true;
		}
	}
	return false;
}

void handle_curr_state(state &curr_state, vector<vector<int>> &solutions){
	if(curr_state.queens_row.size() == COLUMNS){
		solutions.push_back(curr_state.queens_row);
		return;
	}

	for(auto row : get_free_rows_next_column(curr_state)){
		place_queen(curr_state, row);
			handle_curr_state(curr_state, solutions);
		remove_queen(curr_state);
	}
}

void draw_board_old(const vector<int> &solution){
	for(int column = 0; column < COLUMNS; column++){
		cout << "--";
	}
	cout << endl;
	for(int row = 0; row < ROWS; row++){
		cout << "|";
		for(int column = 0; column < COLUMNS; column++){
			if(solution[column] == row){
				cout << "Q|";
			} else {
				cout << "*|";
			}
			
		}
		cout << endl;
		for(int column = 0; column < COLUMNS; column++){
			cout << "--";
		}
		cout << endl;
	}
}

void draw_board(const vector<int> &solution){
	cout << endl;
	for(int row = 0; row < ROWS; row++){
		for(int column = 0; column < COLUMNS; column++){
			if(solution[column] == row){
				cout << "\033[1;41m"<<"X"<<"\033[0m";
			} else {
				cout << "O";
			}
		}
		cout << endl;
	}
}

void draw_solutions_loop(const vector<vector<int>> &solutions){
	int board_selected = 0;
	do {
		cout << "Solution to draw (0 to exit): ";
		cin >> board_selected;
		if(board_selected < 0 || board_selected > solutions.size()){
			cout<<"Solution number must be between 1 and " << solutions.size() << " (0 to exit)" << endl;
			continue;
		}
		if(board_selected != 0)
		{
			draw_board(solutions[board_selected-1]);
		}
	} while(board_selected!=0);
	cout << "bye"<<endl;
}

int main(){
	// time start
	auto start = chrono::high_resolution_clock::now();
	state curr_state;
	vector<vector<int>> solutions;
	handle_curr_state(curr_state, solutions);
	// time end
	auto stop = chrono::high_resolution_clock::now();
	// print time
	auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
	cout<<"The program took "<< duration.count() << " microseconds to compute.\n";
	print_solutions(solutions);
	draw_solutions_loop(solutions);
}
