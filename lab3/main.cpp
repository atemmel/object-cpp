#include "linked_list.h"

int main()
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

	std::cout << "Element at index 1 is " << list.at(1) << '\n';

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
	
	std::cout << "List contains " << list.size() << " elements\n";

	return 0;
}
