// Project1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <chrono>

#include "AStar.h"
#include "TACI.h"
#include "Heuristic.h"

TaciState RequestUserState(int width)
{
	std::vector<uint8_t> items(width * width); 
	for (size_t item_id = 0; item_id < items.size(); item_id++)
	{
		int value;
		while (true)
		{  
			std::cout << "(" << item_id / width + 1 << ", " << item_id % width + 1 << ") ";
			std::cin >> value;

			if (value >= 0 && value < items.size())
				break;

			std::cout << "Retry" << std::endl;
		};

		items[item_id] = value;
	}
	TaciState state(items);
	state.Print();

	return state;
}

TaciState GenerateState(TaciState dst_state, int step_count)
{
	TaciState state = dst_state;

	uint64_t prev_hash = 0;

	for (size_t step = 0; step < step_count; step++)
	{
		std::vector<TaciState> next_states = state.GetNextStates();

		TaciState next_state;
		do { next_state = next_states[rand() % next_states.size()]; } while (next_state.GetHash() == prev_hash);

		prev_hash = state.GetHash();
		state = next_state;
	}

	return state;
}

template<class State, class Heuristic>
void Solve(State src, State dst)
{	
	AStar<State, Heuristic> astar(src, dst);

	auto begin_clock = std::chrono::high_resolution_clock::now();
	astar.Solve();
	auto end_clock = std::chrono::high_resolution_clock::now();

	astar.Print();
	std::cout << "Finished in " << std::chrono::duration_cast<std::chrono::milliseconds>(end_clock - begin_clock) << std::endl;
}

int main()
{
	srand(time(0));

	const int WIDTH = 3;

	std::cout << "================================================================" << std::endl;
	std::cout << "TACI 8 PUZZLE SOLVER" << std::endl;
	std::cout << "================================================================" << std::endl;
	std::cout << std::endl;

	// 1 2 3 4 5 6 7 8 0 4 1 3 0 2 5 7 8 6

	// 1 2 3 4 5 6 7 8 0
	std::cout << "Destination " << WIDTH << "x" << WIDTH << std::endl;
	TaciState dst_state = RequestUserState(WIDTH);
	
	// 4 1 3 0 2 5 7 8 6
	TaciState src_state;

	if (0)
	{
		std::cout << "Source " << WIDTH << "x" << WIDTH << std::endl;
		src_state = RequestUserState(WIDTH);
	}
	else
	{
		const int STEP_COUNT = 10;
		src_state = GenerateState(dst_state, STEP_COUNT);
		std::cout << "Source " << WIDTH << "x" << WIDTH << std::endl;
		src_state.Print();
	}

	std::cout << std::endl;
	std::cout << "----------------------------------------------------------------" << std::endl;
	std::cout << "USING MANHATTAN HEURISTIC" << std::endl;
	std::cout << "----------------------------------------------------------------" << std::endl;
	std::cout << std::endl;
	Solve<TaciState, ManhattanTaciHeuristic>(src_state, dst_state);

	std::cout << std::endl;
	std::cout << "----------------------------------------------------------------" << std::endl;
	std::cout << "USING HAMMING HEURISTIC" << std::endl;
	std::cout << "----------------------------------------------------------------" << std::endl;
	std::cout << std::endl;
	Solve<TaciState, HammingTaciHeuristic>(src_state, dst_state);

	system("pause");

	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
