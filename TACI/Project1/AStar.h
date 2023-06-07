#pragma once
#include <set>
#include <map>
#include <queue>
#include <functional>
#include <stack>
#include <iostream>

template <class State, class Heuristic>
class AStar;

template <class State, class Heuristic>
class StateComparator
{
    AStar<State, Heuristic>* _astar;
public:
    StateComparator(AStar<State, Heuristic>* astar) : _astar(astar) {}
    bool operator() (const uint64_t& left_state_hash, const uint64_t& right_state_hash) const
    {
        return _astar->GetTotalCost(left_state_hash) < _astar->GetTotalCost(right_state_hash);
    }
};

template <class State, class Heuristic>
class AStar
{
private:
    std::map<uint64_t, State> _states;
    std::map<uint64_t, uint32_t> _g_costs;
    std::map<uint64_t, uint32_t> _h_costs;
    std::map<uint64_t, uint64_t> _traces;

    std::set<uint64_t, StateComparator<State, Heuristic>> _opening_set;
    std::set<uint64_t> _closed_set;

    State _src_state, _dst_state;

    void AddState(State state, State dst_state, uint32_t g_cost);
    uint32_t GetTotalCost(uint64_t state_hash) { return _g_costs[state_hash] + _h_costs[state_hash]; }

    uint64_t GetTopState() { return *_opening_set.begin(); }
    void OpenState(uint64_t state_hash);
    void CloseTopState();

    friend class StateComparator<State, Heuristic>;

public:
    AStar() : _opening_set(StateComparator<State, Heuristic>(this)) {}

    void Solve(State src_state, State dst_state);
    void Print();
};

template<class State, class Heuristic>
inline void AStar<State, Heuristic>::AddState(State state, State dst_state, uint32_t g_cost)
{
    auto state_hash = state.GetHash();

    if (_states.contains(state_hash))
        return;

    _states[state_hash] = state;
    _g_costs[state_hash] = g_cost;
    _h_costs[state_hash] = Heuristic::CalculateCost(state, dst_state);

    return;
}

template<class State, class Heuristic>
inline void AStar<State, Heuristic>::OpenState(uint64_t state_hash)
{
    if (std::find(_opening_set.begin(), _opening_set.end(), state_hash) != _opening_set.end())
        return;

    _opening_set.insert(state_hash);

    std::cout << "Open " << state_hash << std::endl;
    std::cout << std::endl;
    return;
}

template<class State, class Heuristic>
inline void AStar<State, Heuristic>::CloseTopState()
{
    if (_opening_set.empty())
        return;

    uint64_t state_hash = *_opening_set.begin();

    _opening_set.erase(state_hash);
    _closed_set.insert(state_hash);

    std::cout << "Close " << state_hash << std::endl;
    std::cout << std::endl;
    return;
}

template<class State, class Heuristic>
inline void AStar<State, Heuristic>::Solve(State src_state, State dst_state)
{
    _states.clear();
    _g_costs.clear();
    _h_costs.clear();
    _traces.clear();
    _opening_set.clear();
    _closed_set.clear();

    _src_state = src_state;
    _dst_state = dst_state;

    auto src_state_hash = src_state.GetHash();
    AddState(src_state, dst_state, 0);

    auto dst_state_hash = dst_state.GetHash();
    AddState(dst_state, dst_state, UINT32_MAX);

    OpenState(src_state_hash);
    
    while (!_opening_set.empty())
    {
        auto current_state_hash = GetTopState();
        if (current_state_hash == dst_state_hash)
            break;

        CloseTopState();

        auto current_state = _states[current_state_hash];
        std::cout << "Current" << std::endl;
        current_state.Print();
        
        std::vector<State> next_states = _states[current_state_hash].GetNextStates();

        for (auto next_state : next_states)
        {
            auto next_state_hash = next_state.GetHash();
            AddState(next_state, dst_state, UINT32_MAX);

            std::cout << "Next" << std::endl;
            next_state.Print();

            auto src_to_next_cost = _g_costs[current_state_hash] + Heuristic::CalculateCost(_states[current_state_hash], next_state);
            if (src_to_next_cost < _g_costs[next_state_hash])
            {
                _g_costs[next_state_hash] = src_to_next_cost;
                _traces[next_state_hash] = current_state_hash;

                if (!_closed_set.contains(next_state_hash))
                    OpenState(next_state_hash);
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

    auto src_state_hash = _src_state.GetHash();
    auto dst_state_hash = _dst_state.GetHash();

    // begin with dst state
    uint64_t current_state_hash = dst_state_hash;
    path.push(current_state_hash);

    do
    {
        // trace not found
        if (!_traces.contains(current_state_hash))
            break;

        // add state to path
        current_state_hash = _traces[current_state_hash];
        path.push(current_state_hash);

        // meet src state
    } while (current_state_hash != src_state_hash);

    // no path found
    if (current_state_hash != src_state_hash)
    {
        std::cout << "NOT FOUND" << std::endl;
        return;
    }

    std::cout << "Solution has " << path.size() << " steps" << std::endl;
    std::cout << std::endl;

    // print
    for (size_t step = 0; !path.empty(); step++)
    {
        auto state_hash = path.top();
        path.pop();

        std::cout << "Step " << step + 1 << std::endl;
        _states[state_hash].Print();
    }

    return;
}