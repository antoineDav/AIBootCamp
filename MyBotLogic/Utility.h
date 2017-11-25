#pragma once
#include <queue>
#include <vector>
#include <algorithm>

//According to @skypjack https://stackoverflow.com/questions/41904374/priority-queues-in-c
template<class T, class C = std::vector<T>, class P = std::less<typename C::value_type> >
struct MyPriorityQueue : std::priority_queue<T, C, P> {
	typename C::iterator begin() { return std::priority_queue<T, C, P>::c.begin(); }
	typename C::iterator end() { return std::priority_queue<T, C, P>::c.end(); }
};