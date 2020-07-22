#pragma once
//========================================================================
//[File Name]:MemoryStorage.h
//[Description]: A implemetation of memory storage based on MemoryPool,
//               which allowed to Alloc different size of memorys. for
//               example: typename T = int, size list = {1, 16, 8, 64, 128, 10},
//              MemoryStorage will create all these size of MemoryPool, and you can
//              call Alloc(16) to get the memory of sizeof(int)*16. if the input size is
//              not include in the size list, the MemoryStorage will create this kind of 
//              MemoryPool, and it will aslo return the wanted size of memory.
//[Author]:Nico Hu
//[Date]:2020-07-22
//[Other]:
//========================================================================
#include <initializer_list>
#include <memory>
#include "MemoryPool.h"


#define SAFE_DELETE(p) {if(p){delete (p); (p) = nullptr;}}
#define SAFE_DELETE_ARRAY(p) {if(p){delete[] (p); (p) = nullptr;}}


template<typename T, typename SizeType = uint16_t>
class MemoryStorage {
    enum { e_RESV_SIZE = 16 };

    struct MemPoolNode {
        SizeType sizeType{ 0 };
        MemoryPool<T, SizeType>* pMemPool{ nullptr };
        MemPoolNode* pNext{ nullptr };
    };

public:
    typedef T* pT;

    explicit MemoryStorage(size_t init_size, std::initializer_list<size_t> il_type_sizes) {
        for (auto it = il_type_sizes.begin(); it != il_type_sizes.end(); ++it) {
            auto p_mem_pool_node = new MemPoolNode();
            p_mem_pool_node->sizeType = *it;
            p_mem_pool_node->pMemPool = new MemoryPool<T, SizeType>(*it, init_size);
            AddMemPool(&mpMems_, p_mem_pool_node);
        }
    }
    ~MemoryStorage() { Release(); }

    pT Alloc(size_t size)
    {
        MemPoolNode* p_node = mpMems_;
        while (p_node) {
            if (p_node->sizeType >= size) {
                return p_node->pMemPool->Alloc();
            }
            p_node = p_node->pNext;
        }

        auto p_mem_pool_node = new MemPoolNode();
        p_mem_pool_node->sizeType = size;
        p_mem_pool_node->pMemPool = new MemoryPool<T, SizeType>(size);
        AddMemPool(&mpMems_, p_mem_pool_node);

        return p_mem_pool_node->pMemPool->Alloc();
    }
    int Free(pT p)
    {
        SizeType* p_s = reinterpret_cast<SizeType*>(p) - 1;
        SizeType type_size = *p_s;
        pT p_mem = reinterpret_cast<pT>(p_s + 1);
        MemPoolNode* p_node = mpMems_;
        while (p_node) {
            if (p_node->sizeType == type_size) {
                p_node->pMemPool->Free(p_mem);
                return 0;
            }
            p_node = p_node->pNext;
        }
        return -1;
    }

protected:
    void Release() {
        MemPoolNode* p_node = mpMems_;
        while (p_node) {
            MemPoolNode* p_delete = p_node;
            p_node = p_node->pNext;
            SAFE_DELETE(p_delete->pMemPool);
            SAFE_DELETE(p_delete);
        }
    }

    void AddMemPool(MemPoolNode** p_head, MemPoolNode* p_new_node) {
        if (*p_head == nullptr) {
            p_new_node->pNext = *p_head;
            *p_head = p_new_node;
        }
        else {
            MemPoolNode* p_node = *p_head;
            MemPoolNode* p_pre = nullptr;
            while (p_node) {
                if (p_node->sizeType > p_new_node->sizeType) {
                    if (p_pre) {
                        p_new_node->pNext = p_node;
                        p_pre->pNext = p_new_node;
                    }
                    else {
                        p_new_node->pNext = p_node;
                        *p_head = p_new_node;
                    }
                    break;
                }
                p_pre = p_node;
                p_node = p_node->pNext;
            }
            if (p_node == nullptr)
                p_pre->pNext = p_new_node;
        }
    }

protected: 
    MemPoolNode* mpMems_{ nullptr };
};