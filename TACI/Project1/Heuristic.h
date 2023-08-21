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

	uint32_t cost = 0;
	for (int item = 1; item < size; item++)
	{
		auto h = abs((int)(left_positions[item] / width) - (int)(right_positions[item] / width));
		auto w = abs((int)(left_positions[item] % width) - (int)(right_positions[item] % width));

		cost += h + w;
	}

	return cost;
}

class HammingTaciHeuristic
{
public:
	static uint32_t CalculateCost(TaciState left_state, TaciState right_state);
};

inline uint32_t HammingTaciHeuristic::CalculateCost(TaciState left_state, TaciState right_state)
{
	size_t size = left_state.GetItemsSize();

	uint32_t cost = 0;
	for (uint8_t item_id = 0; item_id < size; item_id++)
	{
		int left_item = left_state.GetItemValue(item_id);
		int right_item = right_state.GetItemValue(item_id);
		if (left_item != right_item && left_item && right_item)
			cost++;
	}

	return cost;
}
