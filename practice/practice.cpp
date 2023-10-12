#pragma warning(disable : 4996)

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <ctime>
#include <map>

#define SPACE_CONSOLE cout << '\n';

using namespace std;

void write_to_logs(string log)
{
	time_t now = time(0);
	tm* ltm = localtime(&now);

	ofstream out("logs.txt", ios::app);
	out << ltm->tm_mday << "." << 1 + ltm->tm_mon << "." << 1900 + ltm->tm_year << " " << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec << " : " << log << endl;
	out.close();
}

void read_from_logs()
{
	SPACE_CONSOLE
	string log;
	fstream in("logs.txt");
	while (getline(in, log)) cout << log << endl;
	in.close();
	SPACE_CONSOLE
}

void print_exception()
{
	cout << "oops.. choose another" << endl;
	this_thread::sleep_for(chrono::seconds(1));
	system("cls");
}

void create_log(string selected_product, int product_count, bool is_exception, string info)
{
	string log;
	log = selected_product + " -> " + to_string(product_count) + " : " + info;
	if (is_exception) print_exception();
	write_to_logs(log);
}

void get_action(int& action)
{
	cout << "---- SHOP MANAGEMENT ----\n\n"
		<< "1. buy product(s)\n"
		<< "2. order product(s)\n"
		<< "3. view logs\n" << endl;

	cout << "enter the action: ";
	cin >> action;
}

bool is_product_exist(string selected_product, map<string, unsigned> products)
{
	for (const auto& element : products) if (element.first == selected_product) return true;
	return false;
}

void get_product_info(string& selected_product, int& product_count, map<string, unsigned> products)
{
	SPACE_CONSOLE
	for (const auto& product : products) cout << product.first << "  ->  " << product.second << endl;

	cout << "\nchoose product: ";
	cin >> selected_product;

	cout << "enter count of this product: ";
	cin >> product_count;
}

inline void print_action_status(string status, string selected_product, int product_count) { cout << status << selected_product << " -> " << product_count << endl; }

void case_cycle(bool& action_type, string type, string& selected_product, int& product_count, map<string, unsigned>& products)
{
	string action_status, log_status;
	bool is_completed = false;

	while (!action_type)
	{
		get_product_info(selected_product, product_count, products);
		if (!is_product_exist(selected_product, products))
		{
			print_exception();
			continue;
		}

		if ((products.find(selected_product)->second <= 0 || products.find(selected_product)->second < product_count) && type == "purchase") log_status = "purchase canceled";
		else if (product_count <= 0 && type == "replenish") log_status = "replenish canceled";
		else
		{
			is_completed = true;
			if (type == "purchase")
			{
				action_status = "the purchase successfully completed: ";
				log_status = "purchase completed";
				products.find(selected_product)->second -= product_count;
			}
			else if (type == "replenish")
			{
				action_status = "the category has been successfully replenished: ";
				log_status = "replenish completed";
				products.find(selected_product)->second += product_count;
			}
		}

		create_log(selected_product, product_count, !is_completed, log_status);
		if (is_completed)
		{
			print_action_status(action_status, selected_product, product_count);
			action_type = true;
		}
		else continue;
	}
}

int main()
{
	int action;
	string log;
	int product_count;
	string selected_product;
	bool is_working = true, is_purchased = false, is_replenished = false;

	map<string, unsigned> products;
	products["meat"] = 100;
	products["drinks"] = 100;
	products["desserts"] = 100;
	products["vegetables"] = 100;

	do
	{
		get_action(action);
		switch (action)
		{
		case 1:
			case_cycle(is_purchased, "purchase", selected_product, product_count, products);
			break;
		case 2:
			case_cycle(is_replenished, "replenish", selected_product, product_count, products);
			break;
		case 3:
			read_from_logs();
			break;
		}
		cout << "would you like to continue? (1/0): ";
		cin >> is_working;

		is_purchased = is_replenished = false;
		system("cls");
	} while (is_working);

	return 0;
}