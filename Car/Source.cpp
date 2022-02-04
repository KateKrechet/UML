#include<iostream>
#include<conio.h>
#include<chrono>
#include<Windows.h>
#include<thread>
using namespace std;

#define MAX_TANK_VOLUME 80
#define MIN_TANK_VOLUME 40
#define MIN_ENGINE_CONSUMPTION 4
#define MAX_ENGINE_CONSUMPTION 25


class Tank
{
	const unsigned int VOLUME;
	double fuel_level;
public:
	const unsigned int get_VOLUME()const
	{
		return VOLUME;
	}
	double get_fuel_level()const
	{
		return fuel_level;
	}
	double fill(double fuel)//позволяет пополнить бак;определяет сколько бензина можем залить
	{
		if (fuel_level + fuel < 0)return fuel_level = 0;
		if (fuel_level + fuel > VOLUME)return fuel_level = VOLUME;
		//if (fuel_level+fuel>=0&&fuel_level + fuel <= VOLUME) fuel_level += fuel;
		else return fuel_level += fuel;
	}
	double give_fuel(double fuel)//позволяет только брать бензин из бака и возвращает, сколько бензина осталось
	{
		fuel_level -= fuel;
		if (fuel_level < 0)fuel_level = 0;
		return fuel_level;
	}
	Tank(unsigned int volume)
		:VOLUME(volume >= MIN_TANK_VOLUME && volume <= MAX_TANK_VOLUME ? volume : MAX_TANK_VOLUME),
		fuel_level(0)
	{
		//this->VOLUME = volume;
		cout << "Tank id ready\t" << this << endl;
	}
	~Tank()
	{
		cout << "Tank id gone\t" << this << endl;
	}
	void info()const
	{
		cout << "Tank volume:\t" << VOLUME << endl;
		cout << "Fuel level:\t" << fuel_level << endl;
	}
};
class Engine
{
	double default_consumption;
	double consumption;//расход топлива
	double consumption_per_second;
	bool is_started;
public:
	double get_consumption()const
	{
		return consumption;
	}
	double get_consumption_per_second()const
	{
		return consumption_per_second;
	}
	bool started()const
	{
		return is_started;
	}
	bool start()
	{
		return is_started = true;
	}
	bool stop()
	{
		return is_started = false;
	}

	void set_consumption(double consumption)
	{
		if (consumption >= MIN_ENGINE_CONSUMPTION && consumption <= MAX_ENGINE_CONSUMPTION)
			this->consumption = consumption;
		else
			this->consumption = MAX_ENGINE_CONSUMPTION / 2;
		consumption_per_second = this->consumption * 0.3e-4;

	}

	explicit Engine(double consumption)
	{
		set_consumption(consumption);
		default_consumption = this->consumption;
		is_started = false;
		cout << "Engine is ready:\t" << this << endl;
	}
	~Engine()
	{
		cout << "Engine is gone:\t" << this << endl;
	}
	double set_consumption_by_speed(int speed)
	{
		set_consumption(get_consumption());
		if (speed > 0 && speed <= 60)consumption_per_second *= 6.6;
		else if (speed > 60 && speed <= 100)consumption_per_second *= 4.6;
		else if (speed > 100 && speed <= 140)consumption_per_second *= 6.6;
		else if (speed > 140 && speed <= 200)consumption_per_second *= 8.3;
		else if (speed > 200 && speed <= 300)consumption_per_second *= 8.3;
		return consumption_per_second;
	}
	void info()const
	{
		cout << "Consumption:\t" << consumption << endl;
		cout << "Consuption per sec:\t" << consumption_per_second << endl;
		cout << "Engine is:\t" << (is_started ? "started" : "stopped") << endl;

	}
};
#define Enter 13
#define Escape 27

#define MAX_SPEED_LOW 120
#define MAX_SPEED_HIGH 400

class Car
{
	Tank tank;
	Engine engine;
	bool driver_inside;
	struct Control
	{
		std::thread panel_thread;       //отобржение панели приборов
		std::thread engine_idle_thread;//холостой ход двигателя
		std::thread free_wheeling_thread;//движение машины по инерции
	}control;

	const int MAX_SPEED;
	int speed;

public:
	Car(double engine_consumption, unsigned int tank_volume, int max_speed) :
		engine(engine_consumption),
		tank(tank_volume),
		MAX_SPEED(max_speed >= MAX_SPEED_LOW && max_speed <= MAX_SPEED_HIGH ? max_speed : 200)
	{
		driver_inside = false;//когда машина сходит с конвейера в ней нет водителя
		speed = 0;
		cout << "Your car is ready to go\t" << this << endl;
	}
	~Car()
	{
		cout << "Your car is over" << endl;
	}
	void fill(double fuel)
	{
		tank.fill(fuel);//метод, как крышка топливного бака
	}
	void start_engine()
	{
		if (tank.get_fuel_level())
		{
			engine.start();
			control.engine_idle_thread = std::thread(&Car::engine_idle, this);
		}
	}
	void stop_engine()
	{
		engine.stop();
		if (control.engine_idle_thread.joinable())control.engine_idle_thread.join();
	}
	void get_in()
	{
		driver_inside = true;
		control.panel_thread = std::thread(&Car::control_panel, this);//запускаем метод control_panel в отдельном потоке
	}
	void get_out()
	{
		driver_inside = false;
		if (control.panel_thread.joinable())control.panel_thread.join();//Останавливаем выполнение потока panel_thread
		system("CLS");
		cout << "You are out of a car" << endl;
	}
	void free_wheeling()
	{
		//свободное торможение в результате силы трения

		while (speed)
		{
			speed--;
			if (speed < 0)speed = 0;
			std::this_thread::sleep_for(1s);
		}

	}

	void control_car()
	{
		char key;
		do
		{
			key = _getch();
			switch (key)
			{
			case Enter://сесть в машину, отобразить панель приборов
				if (driver_inside)get_out();
				else get_in();

				break;
			case 'F':
			case 'f'://заправить машину
				double fuel;
				cout << "Введите объем топлива: ";
				//cin.clear();
				cin >> fuel;
				fill(fuel);
				break;
			case 'I':case'i'://Ignition-зажигание
				if (engine.started())stop_engine();
				else start_engine();
				break;
			case 'W':case 'w':
				if (engine.started() && speed <= MAX_SPEED)speed += 10;
				if (!control.free_wheeling_thread.joinable())
				{
					control.free_wheeling_thread = std::thread(&Car::free_wheeling, this);
				}
				std::this_thread::sleep_for(1s);//чем больше задержка, тем разгоняемся быстрее
				break;
			case 'S':case 's':
				if (speed > 0)speed -= 10;
				if (speed < 0)speed -= 0;
				std::this_thread::sleep_for(.4s);
				break;
			case Escape:
				//if (control.free_wheeling_thread.joinable())control.free_wheeling_thread.join();
				stop_engine();
				get_out();
				if (control.free_wheeling_thread.joinable())
				{
					speed = 0;
					control.free_wheeling_thread.join();
				}
				break;
			}
			//если скорость=0, останавливаем поток трения, чтобы скорость не ушла в отрицание
			if (speed == 0 && control.free_wheeling_thread.joinable()) control.free_wheeling_thread.join();
			engine.set_consumption_by_speed(speed);
		} while (key != 27);
	}
	void engine_idle()
	{
		while (engine.started() &&
			tank.give_fuel(engine.get_consumption_per_second())
			)
			std::this_thread::sleep_for(1s);
	}
	void control_panel()
	{
		while (driver_inside)
		{
			system("CLS");
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			for (int i = 0; i < speed / 3; i++)
			{
				if (i > 150 / 3) SetConsoleTextAttribute(hConsole, 0x0E);
				if (i > 200 / 3) SetConsoleTextAttribute(hConsole, 0x0C);
				cout << "|";
				SetConsoleTextAttribute(hConsole, 0x07);
			}
			cout << endl;
			cout << "Fuel Level: " << tank.get_fuel_level() << "  liters.";
			if (tank.get_fuel_level() < 5)
			{
				HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
				SetConsoleTextAttribute(hConsole, 0x0C);//красный
				cout << "\tLOW FUEL";
				SetConsoleTextAttribute(hConsole, 0x07);//обычный цвет
			}
			cout << endl;
			cout << "Consumption: " << engine.get_consumption_per_second() << " liters.";
			cout << "Engine is " << (engine.started() ? "started" : "stopped") << endl;
			cout << "Speed: " << speed << " km/h.\n";
			std::this_thread::sleep_for(1s);
		}
	}

	void info()const
	{
		tank.info();
		engine.info();
	}
};

//#define TANK_CHECK
//#define ENGINE_CHECK

void main()
{
	setlocale(LC_ALL, "");
#ifdef TANK_CHECK
	Tank tank(120);
	tank.info();
	int fuel;
	while (true)
	{
		cout << "Введите объем:"; cin >> fuel;
		tank.fill(fuel);
		tank.info();
	}
#endif // TANK_CHECK
#ifdef ENGINE_CHECK
	Engine engine(9);
	engine.info();
#endif // ENGINE_CHECK

	Car bmw(20, 80, 250);
	cout << "Press Enter to get in" << endl;
	bmw.control_car();


}