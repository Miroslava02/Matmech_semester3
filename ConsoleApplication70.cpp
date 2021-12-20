#include <deque>
#include <mutex>
#include <future>
#include <thread>
#include <utility>
#include <iostream>
using namespace std;
//выплата налогов
//в массиве содержится информация о зарплате
//вычитаем 13%

const int N = 100;
mutex m;
deque<packaged_task<void()>> tasks;

bool stop_message_received(int x) {
	if (x == 1) return 1;
	else return 0;
}

void get_and_proc() {
	
}

void taxes_thread(int* salary_array, int x) {
	while (!stop_message_received(x)){
		get_and_proc();
		packaged_task<void()> task;
		{lock_guard<mutex> lk(m);
		if (tasks.empty())
			continue;
		task = move(tasks.front());
		tasks.pop_front();
		}
		task();
	}
}

thread bg_thread(taxes_thread);

template <typename Func>
future <void> post_task_for_taxes_thread(Func f) {
	packaged_task<void()> task(f);
	future <void> res = task.get_future();
	lock_guard<mutex> lk(m);
	tasks.push_back(move(task));
	return res;
}

void f(int* salary_array) {
	for (int i = 0; i < N; i++) {
		salary_array[i] = salary_array[i] * 0, 87;
	}
}

int main() {
	int x=1;
	int* salary_array = new int[5];
	while (x != 0) {
		cin >> x;
		for (int i = 0; i < 5; i++) {
			salary_array[i] = rand();
		}
		post_task_for_taxes_thread(f);
		taxes_thread(salary_array, x);
	}
	return 0;
}