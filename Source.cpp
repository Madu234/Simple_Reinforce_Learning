#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

int map[10][10];
float q[10][10][4];
int size_map = 4;
int agent_x, agent_y;
int objective_x, objective_y;
bool game_won=false;
int valids[4];

// Reinforce learning

int num_episodes = 100000, max_steps_per_episode = 100;
float learning_rate = 0.1, discount_rate = 0.9;

float exploration_rate = 1, max_exploration_rate = 1;
float min_exploration_rate = 0.01, exploration_decay_rate = 0.001;

void fill_map() {
	for (int i = 0; i < size_map; i++)
		for (int j = 0; j < size_map; j++)
			map[i][j] = 0;
}

void set_agent(int i, int j) {
	agent_x = i; agent_y = j;
	map[i][j] = 1;
}

void set_goal(int i, int j) {
	objective_x = i; objective_y = j;
	map[i][j] = 2;
}

void print_map(int x) {
	for (int i = size_map-1; i >= 0; i--) {
		for (int j = 0; j < size_map; j++) {
			cout << map[i][j] << " ";
		}
		cout << endl;
	}

}
// Returns 1 if move is valid, Returns 0 if move is invalid
int moveLeft() {
	if ( agent_y > 0) {
		map[agent_x][agent_y] = 0;
		agent_y--;
		map[agent_x][agent_y] = 1;
		
		return 1;
	}
	return 0;
}

int moveRight() {
	if (agent_y < size_map -1) {
		map[agent_x][agent_y] = 0;
		agent_y++;
		map[agent_x][agent_y] = 1;

		return 1;
	}
	return 0;
}

int moveDown() {
	if (agent_x > 0) {
		map[agent_x][agent_y] = 0;
		agent_x--;
		map[agent_x][agent_y] = 1;

		return 1;
	}
	return 0;
}

int moveUp() {
	if (agent_x < size_map-1) {
		map[agent_x][agent_y] = 0;
		agent_x++;
		map[agent_x][agent_y] = 1;
		return 1;
	}
	return 0;
}

void findValidMoves() {
	// Up
	if (agent_x < size_map - 1) {
		valids[0] = 1;
	}
	if (agent_y < size_map - 1) {
		valids[3] = 1;
	}
	if (agent_x > 0) {
		valids[1] = 1;
	}
	if (agent_y > 0) {
		valids[2] = 1;
	}
}

void map_reset() {
	fill_map();
	set_agent(size_map - 1, 0);
	set_goal(0, size_map - 1);
}

void initialize_q() {
	for (int i = 0; i < size_map; i++)
		for (int j = 0; j < size_map; j++)
			for (int k = 0; k < 4; k++)
				q[i][j][k] = 0;
}

float find_q_max(int state_i, int state_j) {
	int max = 0;
	for (int k = 0; k < 4;k++) {
		if (q[state_i][state_j][k] >= q[state_i][state_j][max] && valids[k]) {
			max = k;
		}
	}
	return max;
}

int move(int direction) {
	switch (direction)
	{
	case 0:
		return moveUp();
		break;
	case 1:
		return moveDown();
		break;
	case 2:
		return moveLeft();
		break;
	case 3:
		return moveRight();
		break;
	default:
		break;
	}
}

void print_q() {
	for (int k = 0; k < 4; k++) {
		cout << "k= " << k << endl;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				cout << q[i][j][k] << " ";
			}
			cout << endl;
		}
	}
}

int main() {

	srand(time(0));  // Initialize random number generator.
	map_reset();
	print_map(size_map);

	// Reinforce Learning
	initialize_q();
	for (int episode = 0; episode < num_episodes; episode++) {
		map_reset();
		bool done = false;
		int reward_current_episode = 0;
		
		//cout << (rand() % 10) << " ";
		for (int step = 0; step < max_steps_per_episode; step++) {
			float exploration_threshold = float(rand() % 100) / 100;
			bool free_move = false;
			int action;
			if (exploration_threshold < exploration_rate) {
				free_move = true;
			}
			findValidMoves();
			if (free_move) {
				do {
					action = rand() % 4;
				} while (!valids[action]);
			}
			else {
				action = find_q_max(agent_x, agent_y);
			}
			float reward = 0;
			int old_agent_x = agent_x;
			int old_agent_y = agent_y;
			move(action);

			if (agent_x == objective_x && agent_y == objective_y) {
				done = true;
				reward = 1;
			}
			int next_action = find_q_max(agent_x, agent_y);
			q[old_agent_x][old_agent_y][action] = q[old_agent_x][old_agent_y][action] * (1 - learning_rate) +
				learning_rate * (reward + discount_rate * q[agent_x][agent_y][next_action]);
			if (done) {
				//print_q();
				break;
			}
		}

	}
	print_q();
	/*
	// Manual Play
	while (!game_won) {
		int move;
		cout << "Choose move ";
		cin >> move;
		switch (move)
		{
		case 0:
			moveDown(); break;
		case 1:
			moveUp(); break;
		case 2:
			moveLeft(); break;
		case 3:
			moveRight(); break;
		default:
			break;
		}
		if (agent_x == objective_x && agent_y == objective_y) {
			game_won = true;
			cout << "Game Completed";
		}
		else {
			print_map(size_map);
		}
	}
	*/
	return 0;
}