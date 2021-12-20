#include <thread>
#include <future>
#include <cstdlib>
#include <iostream>
using namespace std;

//программа иммитирует вложение в валюту
//известно: сколько валюты и за сколько купили
//неизвестно: за сколько продали каждую единицу
//считаем: прибыль
const int price = 500;
const int NUMBER = 1024;

int selling() {   //программа генерирует стоимость валюты
	srand(1000);
	int k = 0;
	int S = 0; //прибыль
	int x;
	int number = NUMBER;
	while (number != 0) {
		x = rand();
		if (x >= (price * 1.1)) { //ждём, пока цена не станет по крайней мере на 10% больше изначальной
			number = number / 2;//продаём половину оставшейся валюты по этой цене
			S += (number)*x;
		}
		k += 1;
		if (k = 1000) { //если цена поменялась 1000 раз, то сдаёмся и продаём всё, что осталось
			S += number * x;
			number = 0;
		}
	}
}

int main() {
	future<int> ans = async(selling);
	cout << ans.get() - price * NUMBER;
}