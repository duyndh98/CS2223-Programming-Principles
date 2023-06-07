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

template<class State, class Heuristic>
void Solve(State src, State dst)
{	
	AStar<State, Heuristic> astar;

	auto begin_clock = std::chrono::high_resolution_clock::now();
	astar.Solve(src, dst);
	auto end_clock = std::chrono::high_resolution_clock::now();

	astar.Print();
	std::cout << "Found solution in " << std::chrono::duration_cast<std::chrono::milliseconds>(end_clock - begin_clock) << std::endl;
}

int main()
{
	const int WIDTH = 3;

	std::cout << "================================================================" << std::endl;
	std::cout << "TACI 8 PUZZLE SOLVER" << std::endl;
	std::cout << "================================================================" << std::endl;
	std::cout << std::endl;

	// 4 1 3 0 2 5 7 8 6 1 2 3 4 5 6 7 8 0

	// { 4 1 3 0 2 5 7 8 6 };
	std::cout << "Source " << WIDTH << "x" << WIDTH << std::endl;
	TaciState src = RequestUserState(WIDTH);

	// { 1 2 3 4 5 6 7 8 0 };
	std::cout << "Destination " << WIDTH << "x" << WIDTH << std::endl;
	TaciState dst = RequestUserState(WIDTH);

	std::cout << std::endl;
	std::cout << "----------------------------------------------------------------" << std::endl;
	std::cout << "SOLVING USING MANHATTAN HEURISTIC" << std::endl;
	std::cout << "----------------------------------------------------------------" << std::endl;
	std::cout << std::endl;
	Solve<TaciState, ManhattanTaciHeuristic>(src, dst);

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
