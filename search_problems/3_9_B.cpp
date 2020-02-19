#include<iostream>
#include<vector>

using namespace std;

struct State{
	int missionairs;
	int cannibals;
};

bool is_valid_state(const State &state){
	return (state.missionairs == 3 || state.missionairs == state.cannibals)
	 	 && state.missionairs >= 0
	 	 && state.cannibals   >= 0;
}

bool is_final_state(const State &state){
	return state.missionairs == 0
		&& state.cannibals == 0;
}

void print_solution(const vector<State> &solution_states){
	for (auto &state : solution_states){
		cout << "M" << state.missionairs << "C" << state.cannibals << ' ';
	}
	cout << '\n';
}

void find_solution(State curr_state, vector<State> &prev_states){
	if(is_final_state(curr_state)){
		prev_states.push_back(curr_state);
		print_solution(prev_states);
		prev_states.pop_back();
		return;
	}

	if(is_valid_state(curr_state)){
		prev_states.push_back(curr_state);
		find_solution({curr_state.missionairs - 1, curr_state.cannibals}, prev_states);
		find_solution({curr_state.missionairs - 2, curr_state.cannibals}, prev_states);
		find_solution({curr_state.missionairs - 1, curr_state.cannibals - 1}, prev_states);
		find_solution({curr_state.missionairs, curr_state.cannibals - 2}, prev_states);
		find_solution({curr_state.missionairs, curr_state.cannibals - 1}, prev_states);
		prev_states.pop_back();
	}

}

int main(){
	vector<State> prev_states;
	find_solution({3, 3}, prev_states);
}