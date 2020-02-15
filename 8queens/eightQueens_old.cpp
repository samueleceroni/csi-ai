#include<iostream>
#include<vector>
#define COLUMNS 8
#define ROWS 8

using namespace std;

void print_board(int board[ROWS][COLUMNS]){
	for(int i = 0; i < ROWS; i++){
		for(int j = 0; j < COLUMNS; j++){
			cout << board[i][j];
			if(j != COLUMNS-1)
				cout<< ' ';
		}
		cout<<endl;
	}
}

vector<int> get_possible_rows(int board [ROWS][COLUMNS], int column){
	vector<int> solution;
	for (int row = 0; row < ROWS; row++){
		if(board[row][column] == 0)
			solution.push_back(row);
	}
	return solution;
}

void place_queen(int board [ROWS][COLUMNS], int row, int column, vector<pair<int,int>> &occupied_cells){
	if(board[row][column] != 0){
		cout<<"error, can't put queen in " << row << ':' << column << endl;
		print_board(board);
		exit(-1);
	}
	board[row][column] = 2;
	for(int i = 0; i < ROWS; i++){
		if(board[i][column] == 0){
			occupied_cells.push_back({i, column});
			board[i][column] == 1;
		}
	}
	for(int j = column + 1; j < COLUMNS; j++){ // all previous column are already set to 1 or 2
		if(board[row][j] == 0){
			occupied_cells.push_back({row, j});
			board[row][j] == 1;
		}
	}
	for (int i = 1; i <= 7; i++){ // 7 is max number of steps from A1 to H8
		if(row + i < ROWS && column + i < COLUMNS && board[row + i][column + i] == 0){
			board[row + i][column + i] = 1;
			occupied_cells.push_back({row + i, column + i});
		}
		if(row - i >= 0 && column + i < COLUMNS && board[row - i][column + i] == 0){
			board[row - i][column + i] = 1;
			occupied_cells.push_back({row - i, column + i});
		}
	}
}

void remove_queen(int board [ROWS][COLUMNS], vector<pair<int,int>> &occupied_cells, int occupied_cells_previous_size){
	while(occupied_cells.size() > occupied_cells_previous_size){
		board[occupied_cells.back().first][occupied_cells.back().second] = 0;
		occupied_cells.pop_back();
	}
}

bool can_go_on(int board [ROWS][COLUMNS], int curr_column){
	for(int column = curr_column + 1; column < COLUMNS; column++){
		if(get_possible_rows(board, column).size() <= 0){
			return false;
		}
	}
	return true;
}
//TODO keep through recursion placed queend
void handle_column(int board [ROWS][COLUMNS], int column, vector<pair<int,int>> &occupied_cells, int& n_solutions){
	auto possible_rows = get_possible_rows(board, column);
	int occupied_cells_size = occupied_cells.size();
	for(auto row : possible_rows){
		place_queen(board, row, column, occupied_cells);
		//pruning, if I don't have solutions I don't go further in this branch.
		if(column < COLUMNS - 1 && can_go_on(board, column)){
			handle_column(board, column + 1, occupied_cells, n_solutions);
		}
		if(column == COLUMNS - 1){
			cout<< "found solution " << ++n_solutions << endl;
			print_board(board);
		}
		remove_queen(board, occupied_cells, occupied_cells_size);
	}
}

int main(){
	int board[ROWS][COLUMNS] = {0}; // 0 -> free, 1 -> can't go, 2 -> queen
	int solutions_found = 0;
	vector<pair<int,int>> occupied_cells;
	handle_column(board, 0, occupied_cells, solutions_found);

}