#include "../toy_traits_test/map.hpp"
#include <iostream>
#include <cassert>
#include <string>

class Integer {
public:
	static int counter;
	int val;
	
	Integer(int val) : val(val) {
		counter++;
	}

	Integer(const Integer &rhs) {
		val = rhs.val;
		counter++;
	}

	Integer& operator = (const Integer &rhs) {
		assert(false);
	}

	~Integer() {
		counter--;
	}
};

int Integer::counter = 0;

class Compare {
public:
	bool operator () (const Integer &lhs, const Integer &rhs) const {
		return lhs.val < rhs.val;
	}
};

void tester(void) {
	//	test: constructor
	sjtu::map<Integer, std::string, Compare> map;
//    std::cout << "hewew"<<std::endl;
	//	test: empty(), size()
	assert(map.empty() && map.size() == 0);
//    std::cout << "hewew"<<std::endl;
	//	test: operator[], insert()
	for (int i = 0; i < 100000; ++i) {
		std::string string = "";
		for (int number = i; number; number /= 10) {
			char digit = '0' + number % 10;
			string = digit + string;
		}
		if (i & 1) {
			map[Integer(i)] = string;
			auto result = map.insert(sjtu::pair<Integer, std::string>(Integer(i), string));
			assert(!result.second);
		} else {
			auto result = map.insert(sjtu::pair<Integer, std::string>(Integer(i), string));
			assert(result.second);
		}
	}
//    std::cout << "here" << std::endl;
	//	test: count(), find(), erase()
	for (int i = 0; i < 100000; ++i) {
		if (i > 1896 && i <= 2016) {
			continue;
		}
//        std::cout << "1"<< std::endl;
		assert(map.count(Integer(i)) == 1);
//        std::cout << "2"<< std::endl;
		assert(map.find(Integer(i)) != map.end());
//        std::cout << "3"<< std::endl;
//        std::cout << i << std::endl;
//        map.Intraverse();
		map.erase(map.find(Integer(i)));
//        std::cout << "4"<< std::endl;
	}
//    std::cout << "HIHI"<< std::endl;
	//	test: constructor, operator=, clear();
	for (int i = 0; i < (int)map.size(); ++i) {
//        std::cout << "$$"<<i<< std::endl;
		sjtu::map<Integer, std::string, Compare> copy(map);
		map.clear();
		std::cout << map.size() << " " << copy.size() << " ";
		map = copy;
		copy.clear();
		std::cout << map.size() << " " << copy.size() << " ";
		copy = map;
		map.clear();
		std::cout << map.size() << " " << copy.size() << " ";
		map = copy;
		copy.clear();
		std::cout << map.size() << " " << copy.size() << " ";
	}
//    std::cout << "HIHI"<< std::endl;
	std::cout << std::endl;
	//	test: const_iterator, cbegin(), cend(), operator++, at()
	sjtu::map<Integer, std::string, Compare>::const_iterator const_iterator;
	const_iterator = map.cbegin();
	while (const_iterator != map.cend()) {
		const Integer integer(const_iterator->first);
		const_iterator++;
		std::cout << map.at(integer) << " ";
	}
	std::cout << std::endl;
	//	test: iterator, operator--, operator->
	sjtu::map<Integer, std::string, Compare>::iterator iterator;
	iterator = map.end();
	while (true) {
		sjtu::map<Integer, std::string, Compare>::iterator peek = iterator;
		if (peek == map.begin()) {
			std::cout << std::endl;
			break;
		}
		std::cout << (--iterator)->second << " ";
	}
	//	test: erase()
	while (map.begin() != map.end()) {
		map.erase(map.begin());
	}
//    std::cout << "HUHUHHU" << std::endl;
	assert(map.empty() && map.size() == 0);
	//	test: operator[]
	for (int i = 0; i < 100000; ++i) {
		std::cout << map[Integer(i)];
	}
	std::cout << map.size() << std::endl;
}

int main(void) {
    std::cout << "UUU";
	tester();
	std::cout << Integer::counter << std::endl;
}
