#include <iostream>
#include <ctime>
#include <cassert>
#include <vector>
#include "MemoryPool.h"
#include "MemoryStorage.h"


// the numbers of wanted pieces of memory.
#define MEM_NUM 10000
// loops of each memory test and total memory times of alloc-and-dealloc are LOOP * MEM_NUM.
#define LOOP 500

struct Dem {
	char data[512]; //512 byte
};

//test new delete
void test_new_delete() {
	auto start = clock();
	for (int j = 0; j < LOOP; j++) {
		for (int i = 0; i < MEM_NUM; i++) {
			Dem* p = new Dem;
			delete p;
		}
	}
	std::cout << "\nNew_delete Time ms: ";
	std::cout << (((double)clock() - start)) << std::endl;
}

//test vector memory alloc and deallac.
void test_vectorMemPool() {
	std::vector<Dem> stackVector;
	stackVector.reserve(MEM_NUM);
	auto start = clock();
	for (int j = 0; j < LOOP; j++) {
		assert(stackVector.empty());
		for (int i = 0; i < MEM_NUM; i++)
			stackVector.emplace_back(Dem());
		for (int i = 0; i < MEM_NUM; i++)
			stackVector.pop_back();
	}
	std::cout << "\nVector Time ms: ";
	std::cout << (((double)clock() - start)) << std::endl;
}

//test memory pool
void test_memoryPool() {
	MemoryPool<Dem> pool(1, MEM_NUM);
	auto start = clock();
	for (int j = 0; j < LOOP; j++) {
		for (int i = 0; i < MEM_NUM; i++) {
			auto p = pool.Alloc();
			pool.Free(p);
		}
	}
	std::cout << "\nMemoryPool Time ms: ";
	std::cout << (((double)clock() - start)) << std::endl;
}

//test memory storage
void test_memoryStorage() {
	MemoryStorage<Dem> storage(MEM_NUM, { 1 });
	auto start = clock();
	for (int j = 0; j < LOOP; j++) {
		for (int i = 0; i < MEM_NUM; i++) {
			auto p = storage.Alloc(1);
			storage.Free(p);
		}
	}
	std::cout << "\nMemoryStorage Time ms: ";
	std::cout << (((double)clock() - start)) << std::endl;
}

int main(int argc, char** argv) {
	test_new_delete();
	test_vectorMemPool();
	test_memoryPool();
	test_memoryStorage();

	return 0;
}