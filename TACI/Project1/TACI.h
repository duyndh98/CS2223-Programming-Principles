#pragma once
#include "AStar.h"

#include <list>
#include <vector>
#include <algorithm>
#include <iostream>
#include <random>
#include <set>
#include <functional>
#include <tuple>

class TaciState
{
private:
	std::vector<uint8_t> _items;

public:
	TaciState() {}
	TaciState(const std::vector<uint8_t>& items) { _items = items; }
	TaciState(bool shuffle, size_t width);

	int GetItemValue(size_t id) { return _items[id]; }
	void SetItemValue(size_t id, int value) { _items[id] = value; }
	size_t GetItemsSize() { return _items.size(); }
	uint64_t GetHash();

	void Print();
	std::vector<TaciState> GetNextStates();
	int GetMoveCost();
};

inline TaciState::TaciState(bool shuffle, size_t width)
{
	_items.resize(width * width);
	for (size_t id = 0; id < _items.size(); id++)
		_items[id] = id;

	if (shuffle)
	{
		srand(time(0));
		std::shuffle(_items.begin(), _items.end(), std::default_random_engine(rand()));
	}

	return;
}

inline uint64_t TaciState::GetHash()
{
	uint64_t hash = 0;

	for (size_t item_id = 0; item_id < _items.size(); item_id++)
	{
		hash <<= 4;
		hash |= _items[item_id];
	}

	return hash;
}

inline void TaciState::Print()
{
	size_t width = std::sqrt(_items.size());

	std::cout << std::endl;
	
	size_t item_id = 0;
	for (size_t row = 0; row < width; row++)
	{
		for (size_t column = 0; column < width; column++, item_id++)
		{
			if (column == 0)
				std::cout << '[';

			std::cout << (int)_items[item_id];

			if (column == width - 1)
				std::cout << ']';
			else
				std::cout << ' ';
		}

		std::cout << std::endl;
	}

	std::cout << std::endl;

	return;
}

inline std::vector<TaciState> TaciState::GetNextStates()
{
	size_t width = std::sqrt(_items.size());

	size_t zero_id = std::find(_items.begin(), _items.end(), 0) - _items.begin();
	int zero_row = zero_id / width;
	int zero_column = zero_id % width;

	std::vector<std::tuple<int, int>> next_offsets =
	{
		{-1, 0},	// left
		{1, 0},		// right
		{0, -1},	// up
		{0, 1},		// down
	};

	std::vector<TaciState> next_states;

	for (auto offset : next_offsets)
	{
		int next_row = zero_row + std::get<0>(offset);
		if (next_row < 0 || next_row >= width)
			continue;

		int next_column = zero_column + std::get<1>(offset);
		if (next_column < 0 || next_column >= width)
			continue;

		size_t next_id = next_row * width + next_column;

		std::vector<uint8_t> items = _items;
		items[zero_id] = _items[next_id];
		items[next_id] = _items[zero_id];

		TaciState state(items);
		next_states.push_back(state);
	}

	return next_states;
}

inline int TaciState::GetMoveCost()
{
	return 1;
}
