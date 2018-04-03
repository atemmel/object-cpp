#include "linked_list.h"
#include <random>
#include <list>

template <class T>
void print_list(const linked_list<T> & list)
{
	list.print();	//???
}

template <class T>
linked_list<T> merge(linked_list<T> A, linked_list<T> B)
{
	linked_list<T> list;

	T a, b, c;

	//Om någon av listorna är tomma, skicka tillbaka
	//den andra meddetsamma
	if(A.empty()) 	   return B;
	else if(B.empty()) return A;

	//Ge variablerna starvärden
	a = A.pop_front();	
	b = B.pop_front();

	//Så länge det finns element i listorna
	while(!A.empty() && !B.empty())
	{
		if(a < b)
		{
			c = a;
			a = A.pop_front();
		}
		else
		{
			c = b;
			b = B.pop_front();
		}

		list.push_back(c);
	}

	list.push_back(a);
	list.push_back(b);

	while(!A.empty()) list.push_back(A.pop_front());
	while(!B.empty()) list.push_back(B.pop_front());

	return list;
}

void trials();

int main()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dist(0,19);

	linked_list<int> llA, llB;

	llA.push_back(dist(gen));
	llB.push_back(dist(gen));

	for(int i = 1; i < 100; i++)
	{
		llA.push_back(llA.back() + dist(gen));
		llB.push_back(llB.back() + dist(gen));
	}

	puts("\n--1--\n");

	llA.print();

	puts("\n--2--\n");

	llB.print();


	llA[49] > llB[49] ? llA.remove(49) : llB.remove(49);

	auto llC = llA;

	for(int i = 0; i < 50; i++)
	{
		llC.pop_back();
		llC.push_front(llC.pop_back());
	}

	puts("\n--3--\n");

	print_list<int>(llC);

	puts("\n--4---\n");

	auto llD = merge<int>(llA, llB);

	llD.print();

	std::cout << llD.size() << " = " << llA.size() << 
		" + " << llB.size() << '\n';

	return 0;
}

void trials()
{
	linked_list<int> list;

	list.push_back(0.0);

	std::cout << list.front() << " or " << list.back() << " is its value.\n";

	list.front() = 5;

	std::cout << list.front() << " or " << list.back() << " is its value.\n";

	list.push_back(2);
	list.push_back(3);

	std::cout << "Front is " << list.front() << " while back is " << list.back() << '\n';
	
	std::cout << "Element no 2 is " << list[1] << '\n';

	list[1] = 4;

	std::cout << "Element no 2 is " << list[1] << '\n';

	std::cout << "Value " << list.pop_front() << " popped from list\n";

	list.pop_back();

	std::cout << "Front is " << list.front() << " Back is " << list.back() << '\n';

	list.pop_front();

	std::cout << "Is list empty? " << std::boolalpha << list.empty() << '\n';

	list.print();

	for(int i = 0; i < 10; i++)
		list.push_back(i);

	list.print();

	list.print_reverse();

	auto list2(list);
	list2.push_front(-4);

	list2.print();

	std::cout << "List2 front " << list2.front() << " List2 back " << list2.back() << '\n';

	list.remove(4);
	list.remove(0);

	list.print();

	list.remove(list.size() - 1);

	list.print();

	list2.print();

	list2 = list;

	list2.print();

	list2 += list2;

	list2.print();
	
	std::cout << "List contains " << list.size() << " elements\n";
}
