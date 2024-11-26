#pragma once
#include <stack>

using namespace std;

template<typename T>
class TQueue {
	int sz; // size of queue
	stack<T> leftStack;
	stack<T> rightStack;
public:
	TQueue() : sz(0) {}

	int getSize() const { return sz; }

	bool isEmpty() const {return (sz == 0);}

	void push(const T& elem) {
		leftStack.push(elem);
		sz++;
	}

	T top() {
		if (sz == 0) throw "Cannot take top of an empty queue";
		if (rightStack.empty()) {
			while (!leftStack.empty()) {
				rightStack.push(leftStack.top());
				leftStack.pop();
			}
		}
		return rightStack.top();
	}

	T pop() {
		if (sz == 0) throw "Cannot pop from an empty queue";
		T val = top();
		rightStack.pop();
		sz--;
		return val;
	}

};