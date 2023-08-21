#pragma once
#include <set>
#include <map>
#include <queue>
#include <functional>
#include <stack>
#include <iostream>
#include <concurrent_priority_queue.h>

template <class State, class Heuristic>
class AStar
{
private:
    std::map<uint64_t, State> _states;
    std::map<uint64_t, uint32_t> _g_costs;
    std::map<uint64_t, uint32_t> _h_costs;
    std::map<uint64_t, uint64_t> _traces;

    uint64_t _src_state_hash, _dst_state_hash;

    std::set<uint64_t> _opening_set;
    std::set<uint64_t> _closed_set;

    uint32_t GetTotalCost(uint64_t state_hash);    
    void Init(State src_state, State dst_state);
    void AddState(State state);
    void OpenState(uint64_t state_hash);
    void UpdateState(uint64_t state_hash, uint32_t g_cost);
    uint64_t PopTopState();

    AStar() {}
public:
    AStar(State src_state, State dst_state) { Init(src_state, dst_state); }

    void Solve();
    void Print();
};

template<class State, class Heuristic>
inline uint32_t AStar<State, Heuristic>::GetTotalCost(uint64_t state_hash)
{
    if (_g_costs[state_hash] == UINT32_MAX)
        return UINT32_MAX;
    else
        return _g_costs[state_hash] + _h_costs[state_hash];
}

template<class State, class Heuristic>
inline void AStar<State, Heuristic>::Init(State src_state, State dst_state)
{
    _src_state_hash = src_state.GetHash();
    _dst_state_hash = dst_state.GetHash();

    AddState(dst_state);
    _h_costs[_dst_state_hash] = 0;

    AddState(src_state);
    _g_costs[_src_state_hash] = 0;
}

template<class State, class Heuristic>
inline void AStar<State, Heuristic>::AddState(State state)
{
    auto state_hash = state.GetHash();
    if (_states.contains(state_hash))
        return;

    _states.insert({ state_hash, state });
    _g_costs[state_hash] = UINT32_MAX;
    _h_costs[state_hash] = Heuristic::CalculateCost(state, _states[_dst_state_hash]);

    return;
}

template<class State, class Heuristic>
inline void AStar<State, Heuristic>::OpenState(uint64_t state_hash)
{
    if (_opening_set.contains(state_hash) || _closed_set.contains(state_hash))
        return;

    _opening_set.insert(state_hash);
    return;
}

template<class State, class Heuristic>
uint64_t AStar<State, Heuristic>::PopTopState()
{
    if (_opening_set.empty())
        return -1;

    uint64_t best_state_hash = *_opening_set.begin();
    uint32_t best_cost = GetTotalCost(best_state_hash);
    for (auto state_hash : _opening_set)
    {
        uint32_t cost = GetTotalCost(state_hash);
        if (cost < best_cost)
        {
            best_state_hash = state_hash;
            best_cost = cost;
        }
    }

    _opening_set.erase(best_state_hash);
    _closed_set.insert(best_state_hash);

    return best_state_hash;
}

template<class State, class Heuristic>
inline void AStar<State, Heuristic>::UpdateState(uint64_t state_hash, uint32_t g_cost)
{
    if (!_opening_set.contains(state_hash))
        return;

    _g_costs[state_hash] = g_cost;
    return;
}

template<class State, class Heuristic>
inline void AStar<State, Heuristic>::Solve()
{
    OpenState(_src_state_hash);

    while (!_opening_set.empty())
    {
        auto current_state_hash = PopTopState();
        if (current_state_hash == _dst_state_hash)
            break;

        auto current_state = _states[current_state_hash];
        std::cout << "Current " << std::hex << std::setw(9) << std::setfill('0') << current_state_hash << std::dec << std::endl;
        current_state.Print();
        
        std::vector<State> next_states = current_state.GetNextStates();

        for (auto next_state : next_states)
        {
            auto next_state_hash = next_state.GetHash();
            AddState(next_state);
            
            std::cout << "Next " << std::hex << std::setw(9) << std::setfill('0') << next_state_hash << std::dec << std::endl;
            next_state.Print();

            auto g_next_cost = UINT32_MAX;
            if (_g_costs[current_state_hash] != UINT32_MAX)
                g_next_cost = _g_costs[current_state_hash] + current_state.GetMoveCost();

            if (g_next_cost < _g_costs[next_state_hash])
            {
                OpenState(next_state_hash);
                UpdateState(next_state_hash, g_next_cost);                

                _traces[next_state_hash] = current_state_hash;
            }
        }
    }

    return;
}

template<class State, class Heuristic>
inline void AStar<State, Heuristic>::Print()
{
    std::cout << "----------------" << std::endl;
    std::cout << "Path" << std::endl;
    std::cout << "----------------" << std::endl;

    std::stack<uint64_t> path;

    // begin with dst state
    uint64_t current_state_hash = _dst_state_hash;
    path.push(current_state_hash);

    do
    {
        _states[current_state_hash].Print();
        
        // trace not found
        if (!_traces.contains(current_state_hash))
            break;

        // add state to path
        current_state_hash = _traces[current_state_hash];
        path.push(current_state_hash);
        
        // meet src state
    } while (current_state_hash != _src_state_hash);

    // no path found
    if (current_state_hash != _src_state_hash)
    {
        std::cout << "NOT FOUND" << std::endl;
        return;
    }

    std::cout << "Solution contains " << path.size() - 1 << " steps" << std::endl;
    std::cout << std::endl;

    // print
    for (size_t step = 0; !path.empty(); step++)
    {
        auto state_hash = path.top();
        path.pop();

        std::cout << "Step #" << step << std::endl;
        _states[state_hash].Print();
    }

    return;
}                               