#include <iostream>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <fstream>
#include <string>
#include <windows.h>
using namespace std;

string fname = "res.log"; 
const int T_SIZE = 100; 
int* t_array = new int[T_SIZE]; 
int arr_iterator = 0; 
int data_size = T_SIZE / 10;
int k = 0; 
int num_of_prep = 1; 
int num_of_proc = 1;
mutex mute_out; 

class Timer 
{
	using clock_t = chrono::high_resolution_clock;
	using second_t = chrono::duration<double, ratio<1> >;
	chrono::time_point<clock_t> m_beg;
public:
	Timer() : m_beg(clock_t::now()) {}
	double elapsed() const
	{
		return std::chrono::duration_cast<second_t>(clock_t::now() - m_beg).count();
	}
};

void init_array() 
{
	for (int i = 0; i < T_SIZE; ++i)
	{
		t_array[i] = i;
	}
}
struct Data 
{
	int* data;
	int size = 0;
	Data(int* d, int i, int j)
	{
		size = j - i;
		data = new int[size];
		for (int n = 0; n < size; ++n)
		{
			data[n] = d[n];
			++n;
		}
	}
	Data(const Data& d)
	{
		size = d.size;
		data = new int[size];
		for (int i = 0; i < d.size; ++i)
		{
			data[i] = d.data[i];
		}
	}
};
bool data_prep() 
{
	return (arr_iterator < T_SIZE);
}
Data prepare_data()
{
	Data D(t_array, arr_iterator, arr_iterator + data_size);
	arr_iterator += data_size;
	return D;
}
void process(Data& data)
{
	for (int i = 0; i < data_size; ++i)
	{
		data.data[i] *= 100;
	}
	mute_out.lock();
	ofstream fout(fname, ofstream::app);
	fout << data.data[0] << endl;       
	for (int i = 0; i < data.size; ++i) 
	{
		++k;
	}
	fout << endl;
	fout.close();
	mute_out.unlock();
}
bool if_data() 
{
	if (k > T_SIZE - data_size)
		return true;
	else return false;
}

mutex mut; 
queue<Data> data_queue; 
condition_variable cond_var;

void preparation_thread() 
{
	while (data_prep())
	{
		Data const data = prepare_data(); 
		lock_guard<mutex> l_g(mut);
		data_queue.push(data);
		cond_var.notify_one();
	}
}

void processing_thread() 
{
	while (true)
	{
		unique_lock<mutex> l_g(mut);
		cond_var.wait(l_g, [] {return !data_queue.empty(); });
		Data data = data_queue.front();
		data_queue.pop();
		l_g.unlock();
		process(data);
		if (if_data())
			break;
	}
}

int main()
{
	ofstream fout(fname, ofstream::app);
	fout << "Array Size: " << T_SIZE << endl;
	fout << "Number of Prepare Threads: " << num_of_prep << endl;
	fout << "Number of Process Threads: " << num_of_proc << endl;
	fout << "Result:" << endl;
	init_array();
	Timer t; 
	thread* preparation_thread = new thread[num_of_prep];
	thread* processing_thread = new thread[num_of_proc];
	for (int i = 0; i < num_of_prep; i++)
	{
		preparation_thread[i] = thread(preparation_thread);
	}
	for (int i = 0; i < num_of_proc; i++)
	{
		processing_thread[i] = thread(processing_thread);
	}

	for (int i = 0; i < num_of_prep; i++)
	{
		preparation_thread[i].join();
	}
	for (int i = 0; i < num_of_proc; i++)
	{
		processing_thread[i].join();
	}
	fout << "Time elapsed: " << t.elapsed() << endl;
	fout << "-------------------------------------------------------------" << endl << endl;
	return 0;
}