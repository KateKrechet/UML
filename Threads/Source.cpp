#include<iostream>
#include<thread>
#include<chrono>
#include<Windows.h>
//using namespace std;
//using namespace std::chrono;
using namespace std::literals::chrono_literals;

bool finish = false;

void plus()
{
	while (!finish)
	{
		std::cout << "+ ";
		//Sleep(1000);
		std::this_thread::sleep_for(10ms);
	}
}

void minus()
{
	while (!finish)
	{
		std::cout << "- ";
		//Sleep(1000);
		std::this_thread::sleep_for(10ms);
	}
}
void main()
{
	//plus();
	//minus();
	std::thread plus_thread (plus);
	std::thread minus_thread(minus);

	std::cin.get();//ожидает нажатия Enter
	finish = true;

	minus_thread.join();
	plus_thread.join();
}