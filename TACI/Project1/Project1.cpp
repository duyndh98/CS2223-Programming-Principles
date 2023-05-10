// Project1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "AStar.h"
#include "TACI.h"
#include "Heuristic.h"

int main()
{
	const int WIDTH = 3;

	//TaciState a(true, 16);
	//uint8_t a[] = {  };

	std::vector<uint8_t> src_items = { 4, 1, 3, 0, 2, 5, 7, 8, 6 };
	TaciState src(src_items);
	
	std::cout << "Src" << std::endl;
	src.Print();

	std::vector<uint8_t> dst_items = { 1, 2, 3, 4, 5, 6, 7, 8, 0 };
	TaciState dst(dst_items);
	
	std::cout << "Dst" << std::endl;
	dst.Print();

	AStar<TaciState, BinaryTaciHeuristic> astar;
	astar.Solve(src, dst);
	astar.Print();

	//bool solved = astar.Solve(src, dst);

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
