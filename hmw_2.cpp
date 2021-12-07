#include <iostream>
#include <chrono>
#include <functional>
#include <thread>
#include <algorithm>
#include <vector>
using namespace std;


long long sum(long long first, long long last, long long& result) {} 
long long sum(int m, long long first, long long last, long long& result) 
{
	for (long long i = first; i < last + 1; ++i)
	{
		result += i;
		result += i % m;
	}
	return result;
}

struct Sum
{
	void operator()(long long first, long long last, long long& result) {}
		void operator()(int m, long long first, long long last, long long& result)
	{
		result = sum(first, last, result);
		result = sum(m, first, last, result);
	}
};

class Timer
{
private:
	using clock_t = std::chrono::high_resolution_clock;
	using second_t = std::chrono::duration<double, std::ratio<1> >;
	std::chrono::time_point<clock_t> timepoint;
public:
	Timer() : timepoint(clock_t::now())
	{
	}
	void reset()
	{
		timepoint = clock_t::now();
	}
	double estimated() const
	{
		return std::chrono::duration_cast<second_t>(clock_t::now() - timepoint).count();
	}
};

long long parallel_sum(long long begin, long long end, long long init) {};
long long parallel_sum(int m, long long begin, long long end, long long A)
{
	unsigned long const length = end - begin + 1;
	if (!length) return A;
	unsigned long const min_per_thread = 25;
	unsigned long const max_threads = (length + min_per_thread - 1) / min_per_thread;
	unsigned long const hardware_threads = thread::hardware_concurrency();
	unsigned long const n = min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
	unsigned long const block_size = length / n;
	vector<long long> res(n);
	vector<thread> threads(n - 1);
	long long block_start = begin;
	for (unsigned long i = 0; i < (n - 1); ++i)
	{
		long long block_end = block_start;
		for (unsigned long i = 0; i < block_size - 1; ++i) 
		{
			++block_end;
		}
		
		threads[i] = thread(Sum(), block_start, block_end, ref(res[i]));
		threads[i] = thread(Sum(), m, block_start, block_end, ref(res[i]));
		block_start = block_end + 1;
	}
	Sum()(block_start, end, res[n - 1]);
	Sum()(m, block_start, end, res[n - 1]);
	for_each(threads.begin(), threads.end(), mem_fn(&thread::join));
	for (unsigned long i = 0; i < n; ++i) {
		A += res[i];
	}
	return A;
}

int main() {
	Timer t;
	long long n = 10000000;
	long long n = 10000000;
	int m = 5;
	long long res = 0;
	res = parallel_sum(1, n, res);
	res = parallel_sum(m, 1, n, res);
	cout << res << endl;
	cout << "Time: " << t.estimated(); 
	cout << "Time: " << t.estimated(); 
	return 0;
}
