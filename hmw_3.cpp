#include <iostream>
#include <deque>
#include <mutex>
#include <initializer_list>
#include <thread>
using namespace std;
mutex deque_mute;
class Safe_Deque : public deque<int>
{
	deque<int> Deque;
public:
	Safe_Deque(deque<int>& ordinary_deque)
	{
		for (int n : ordinary_deque)
		{
			Deque.push_back(n);
		}
	}
	bool empty() const
	{
		lock_guard<mutex> guard(deque_mute);
		return Deque.empty();
	}
	size_t size() const
	{
		lock_guard<mutex> guard(deque_mute);
		return Deque.size();
	}
	size_t max_size() const
	{
		lock_guard<mutex> guard(deque_mute);
		return Deque.max_size();
	}
	void push_back(const int value)
	{
		lock_guard<mutex> guard(deque_mute);
		Deque.push_back(value);
	}
	void push_front(const int value)
	{
		lock_guard<mutex> guard(deque_mute);
		Deque.push_front(value);
	}
	int front()
	{
		lock_guard<mutex> guard(deque_mute);
		return Deque.front();
	}
	int back()
	{
		lock_guard<mutex> guard(deque_mute);
		Deque.back();
	}
	int& operator[] (int index)
	{
		lock_guard<mutex> guard(deque_mute);
		return Deque[index];
	}
	const int& operator[] (int index) const
	{
		lock_guard<mutex> guard(deque_mute);
		return Deque[index];
	}
};

void add_deque(deque<int>& Deque, int n)
{
	Deque.push_back(n);
}

void add_safe_deque(Safe_deque& Deque, int n)
{
	Deque.push_back(n);
}

int main()
{
	deque<int> ordinary_deque = { 0 }; 
	Safe_deque safe_deque(ordinary_deque); 
	thread* thread_arrray = new thread[5];
	int k = 0;
	for (int i = 0; i < 10; i++)
	{
		if ((i + 1) % 2 == 0)
		{
			thread_array[k] = thread(add_deque, ref(not_safe_deque), i); 
			k++;
		}
	}
	for (int i = 0; i < k; ++i) 
	{
		thread_array[i].join();
	}
	for (int i = 0; i < ordinary_deque.size(); ++i) 
		cout << "not safe deque " << ordinary_deque[i] << endl;
	thread* thread_array_b = new thread[5]; 
	k = 0;
	for (int i = 0; i < 10; i++)
	{
		if ((i + 1) % 2 == 0)
		{
			thread_array_b[k] = thread(add_safe_deque, ref(safe_deque), i); 
			k++;
		}
	}
	for (int i = 0; i < k; ++i) 
	{
		thread_array_b[i].join();
	}
	for (int i = 0; i < safe_deque.size(); ++i) 
		cout << "safe deque " << safe_deque[i] << endl;
	return 0;
}