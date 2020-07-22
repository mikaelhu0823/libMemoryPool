# libMemoryPool
This depositary provides the implemetation of Memory pool and Memory storage, both are with high performance. They are all head files, coded with C++11.

User defined data struct(POD):

struct Dem {

	char data[1024*8]; //8k byte
	
};

[MemoryPool]: the usage is showed below:

MemoryPool<T>(SizeType size_type, size_t pool_size) ;//T: std data, or user-defined pod-type data struct; size_type can be any num of pieces of T type memory, like 1, 16,etc. Pool_size is the inited size of the pool.

These are some defines:

MemoryPool<Dem> pool(10, 128);
	
MemoryPool<int> pool(4, 1024);
	
MemoryPool<float> pool(64, 128);


Get memorys:

Auto p = pool.Alloc();

Free memorys:

Pool.Free(p);


[MemoryStorage]: the usage is showed below:

MemoryStorage<T>(size_t init_size, std::initializer_list<size_t> il_type_sizes); //T: std data, or user-defined pod-type data struct; init_size is the inited size of each size-type memory pool. il_type_sizes is the size-type init list, like:{2, 8, 16, 1024, 128, 31}.

These are some defines:

MemoryStorage<Dem> storage(10, { 1, 699, 102, 32});
	
MemoryStorage<double> storage(64, { 102, 32});
	
MemoryStorage<char> storage(1, { 1});
	

Get memorys:

Auto p = storage.Alloc(10);

Free memorys:

storage.Free(p);


Test results compared with new-delete and std::vector:

Test platform: cpu: i3, memory: 8G.

Test cases:

test_new_delete();

test_vectorMemPool();

test_memoryPool();

test_memoryStorage();



Inclusion, the performance of the MemoryPool and MemoryStorage are high and stable, no matter the size of memory allocated , according to the new-delete and std::vector.
