#include<iostream>
#include <fstream>
#include <thread>
#include <mutex>
using namespace std;

const char* F = "filename.txt";

mutex mutex_1;

mutex mutex_2;

char read_first_symbol() {
    char a;
    ifstream in(F);
    if (in.is_open())
    {
        in >> a;
    }
    return a;
}

char read_last_symbol() {
    char b;
    ifstream in(F);
    if (in.is_open()) {
        while (in) {
            in >> b;
        }
    }
    return b;
}

void add_symbol(char c) {
    lock_guard<mutex> guard(mutex_1);
    ofstream out(F);
    if (out.is_open()) {
        out << c;
    }
}

void add_symbol_2(char c, char d) {
    lock_guard<mutex> guard(mutex_2);
    ofstream out(F);
    if (out.is_open()) {
        out << c;
        out << d;
    }
}


int main() {
    thread read_thread_1(read_first_symbol);
    thread read_thread_2(read_last_symbol);
    thread write_thread_1(add_symbol);
    thread write_thread_2(add_symbol_2);
    return 0;
}