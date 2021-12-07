#include <iostream>
#include <chrono>
#include <thread>
using namespace std;


void process_array(int arr[], const int n)
{
	for (int i = 0; i < n; i++)
	{
		arr[i] = n - i;
	}

	int k = 0;
	for (int i = 0; i < n - 1; i++)
	{
		for (int j = n - 1; j > i; j--)
		{
			if (arr[j] < arr[j - 1])
			{
				k = arr[j - 1];
				arr[j - 1] = arr[j];
				arr[j] = k;
			}
		}
	}
}

void print(int arr[], const int n)
{
	for (int i = 0; i < n; i++)
	{
		cout << arr[i] << " ";
	}
	cout << endl;
}

int main() {
	double start_time = clock();
	int n = 100000;
	int m = 20;
	int** matrix = new int* [m];
	thread* array_th = new thread[m];
	for (int i = 0; i < m; i++)
	{
		matrix[i] = new int[n];
	}

	for (int i = 0; i < m; i++)
	{
		array_th[i] = thread(process_array, matrix[i], n);
	}

	for (int i = 0; i < m; i++)
	{
		array_th[i].join();
		print(matrix[i], n);
	}
	double end_time = clock(); // конечное время
	double search_time = end_time - start_time; // искомое время
	cout << search_time;
	return 0;
}