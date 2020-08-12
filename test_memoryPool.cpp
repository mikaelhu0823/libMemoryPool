#include <iostream>
#include <ctime>
#include <cassert>
#include <vector>
#include "MemoryPool.h"
#include "MemoryStorage.h"
#include "NMemoryPool.h"
#include "NMemoryStorage.h"

#ifdef _DEBUG
#include "vld.h"
#endif //_DEBUG


// the numbers of wanted pieces of memory.
#define MEM_NUM 64
// loops of each memory test and total memory times of alloc-and-dealloc are LOOP * MEM_NUM.
#define LOOP 500

struct Dem {
	char data[64]; //64 byte
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

//test nmemory pool
void test_nmemoryPool() {
	NMemoryPool<Dem> pool(1);
	auto start = clock();
	for (int j = 0; j < LOOP; j++) {
		for (int i = 0; i < MEM_NUM; i++) {
			auto p = pool.Alloc();
			/*pool.Construct(p);
			pool.Destroy(p);*/
			pool.Free(p);
		}
	}
	std::cout << "\nNMemoryPool Time ms: ";
	std::cout << (((double)clock() - start)) << std::endl;
}

//test nmemory storage
void test_nmemoryStorage() {
	NMemoryStorage<Dem> storage({ 1 });
	auto start = clock();
	for (int j = 0; j < LOOP; j++) {
		for (int i = 0; i < MEM_NUM; i++) {
			auto p = storage.Alloc(1);
			storage.Free(p);
		}
	}
	std::cout << "\nNMemoryStorage Time ms: ";
	std::cout << (((double)clock() - start)) << std::endl;
}

void test_memStorage1() {
	MemoryStorage<char> storage(MEM_NUM, { 16, 32, 64, 256, 512 });
	std::vector<char*> v_mem;
	v_mem.reserve(32);

	auto start = clock();
	for (int j = 0; j < LOOP; j++) {
		for (int i = 0; i < MEM_NUM * 1; i++) {
			char* p = storage.Alloc(32);
			assert(p);
			memset(p, 0, 32);
			v_mem.emplace_back(p);
			//storage.Free(p);
		}
		for (auto it = v_mem.begin(); it != v_mem.end();) {
			if (*it) {
				auto ret = storage.Free(*it);
				if (ret < 0)
					std::cout << "storage.Free failure, ret = " << ret << std::endl;
				it = v_mem.erase(it);
			}
			else {
				std::cout << "v_mem, *it = " << *it << std::endl;
			}
		}
		v_mem.clear();
	}
	
	std::cout << "\nMemoryStorage Time ms: ";
	std::cout << (((double)clock() - start)) << std::endl;
}
void test_nmemStorage1() {
	NMemoryStorage<char> storage({ 16, 32, 64, 256, 512 });
	std::vector<char*> v_mem;
	v_mem.reserve(32);

	auto start = clock();
	for (int j = 0; j < LOOP; j++) {
		for (int i = 0; i < MEM_NUM * 1; i++) {
			char* p = storage.Alloc(32);
			assert(p);
			memset(p, 0, 32);
			v_mem.emplace_back(p);
			//storage.Free(p);
		}
		for (auto it = v_mem.begin(); it != v_mem.end();) {
			if (*it) {
				auto ret = storage.Free(*it);
				if (ret < 0)
					std::cout << "storage.Free failure, ret = " << ret << std::endl;
				it = v_mem.erase(it);
			}
			else {
				std::cout << "v_mem, *it = " << *it << std::endl;
			}
		}
		v_mem.clear();
	}

	std::cout << "\nNMemoryStorage Time ms: ";
	std::cout << (((double)clock() - start)) << std::endl;
}

int main(int argc, char** argv) {
	try
	{
		test_new_delete();
		test_vectorMemPool();
		test_memoryPool();
		test_memoryStorage();
		test_nmemoryPool();
		test_nmemoryStorage();
		test_memStorage1();
		test_nmemStorage1();
	}
	catch (const std::exception& e)
	{ 
		return -1;
	}

	return 0;
}