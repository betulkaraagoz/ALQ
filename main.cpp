
#include <iostream>
#include <string>
#include <memory>
#include <algorithm>
#include "AtomicEvent.h"
#include "Event.h"
#include <vector>
#include "ALadderQueue.h"
#include <iomanip>
#include <random>
#include <queue>
#include <ctime>
#include "main.h"

#include <fstream>

#include <cstdlib> 

constexpr auto SIZE = 5000000;

using std::ofstream;
using namespace std;
using std::cerr;
using std::endl;

void testALQ(std::vector<double> list) {
	ALadderQueue ladder;


	//ofstream outdata;
	//outdata.open("alq_result.txt"); // opens the file
	//if (!outdata) { // file couldn't be opened
	//	cerr << "Error: file could not be opened" << endl;
	//	exit(1);
	//}

	//ofstream enqdata;
	//enqdata.open("alq_enqueue.txt"); // opens the file
	//if (!enqdata) { // file couldn't be opened
	//	cerr << "Error: file could not be opened" << endl;
	//	exit(1);
	//}


	for (int i = 0; i < SIZE; i++) {
		//enqdata << list.back() << endl;
		ladder.enqueue(Event(list.back()));
		list.pop_back();
	}

	/*while (list.size() > 0) {
		ladder.enqueue(Event(list.back()));
		list.pop_back();
		ladder.dequeue();
		std::cout << "d+enqueued" << "\n";
	}*/

	//enqdata.close();

	int m = 0;
	while (ladder.getSize()>0) {
		m++;
		try {
			ladder.dequeue();
		}
		catch(exception & e){
			std::cout << e.what() << "\n";
		}
		/*if (ladder.getSize() < 5) {
			ladder.printStatus();
		}*/
	}

	//outdata.close();

	std::cout << "Total dequeue : " << m << "\n";
}

void testPQ(std::vector<double> list) {
	priority_queue<double> pq;

	for (int i = 0; i < SIZE; i++) {
		pq.push(list.back());
		list.pop_back();
	}

	/*while (list.size() > 0) {
		pq.push(list.back());
		list.pop_back();
		pq.pop();
	}*/

	while (pq.size() > 0) {
		pq.pop();
	}
}



int main()
{
	std::random_device rd;
	std::mt19937 gen(rd());

	std::exponential_distribution<> d(1.0);

	//ofstream outdata;
	//outdata.open("list.txt"); // opens the file
	//if (!outdata) { // file couldn't be opened
	//	cerr << "Error: file could not be opened" << endl;
	//	exit(1);
	//}

	std::vector<double> list;
	for (int n = 0; n < SIZE; n++) {
		list.push_back(d(gen));
		//outdata <<list.back() << endl;
	}

	//outdata.close();


	std::cout << list.size() << "\n";
	clock_t begin_time = clock();
	testALQ(list);
	std::cout << "ALQ: " << float(clock() - begin_time) / CLOCKS_PER_SEC << "\n";

	std::cout << list.size() << "\n";
	clock_t begin_time_2 = clock();
	testPQ(list);
	std::cout << "PQ: " << float(clock() - begin_time_2) / CLOCKS_PER_SEC;

}



