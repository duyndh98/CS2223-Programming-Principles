#pragma once

#include "AStar.h"
#include "TACI.h"

class ManhattanTaciHeuristic
{
public:
	static uint32_t CalculateCost(TaciState left_state, TaciState right_state);
};

inline uint32_t ManhattanTaciHeuristic::CalculateCost(TaciState left_state, TaciState right_state)
{
	size_t size = left_state.GetItemsSize();
	size_t width = (size_t)sqrt(size);

	std::vector<uint8_t> left_positions(size);
	std::vector<uint8_t> right_positions(size);

	for (uint8_t item_id = 0; item_id < size; item_id++)
	{
		left_positions[left_state.GetItemValue(item_id)] = item_id;
		right_positions[right_state.GetItemValue(item_id)] = item_id;
	}

	/*left_state.Print();
	right_state.Print();*/

	uint32_t cost = 0;
	for (int item = 1; item < size; item++)
	{
		auto h = abs((int)(left_positions[item] / width) - (int)(right_positions[item] / width));
		auto w = abs((int)(left_positions[item] % width) - (int)(right_positions[item] % width));

		cost += h + w;
	}

	return cost;
}

class BinaryTaciHeuristic
{
public:
	static uint32_t CalculateCost(TaciState left_state, TaciState right_state);
};

inline uint32_t BinaryTaciHeuristic::CalculateCost(TaciState left_state, TaciState right_state)
{
	size_t size = left_state.GetItemsSize();
	size_t width = (size_t)sqrt(size);

	uint32_t cost = 0;
	for (uint8_t item_id = 0; item_id < size; item_id++)
	{
		if (!right_state.GetItemValue(item_id))
			continue;

		if (left_state.GetItemValue(item_id) != right_state.GetItemValue(item_id))
			cost++;
	}

	return cost;
}

