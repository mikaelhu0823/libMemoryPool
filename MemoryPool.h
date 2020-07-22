#pragma once
//========================================================================
//[File Name]:MemoryPool.h
//[Description]: A implemetation of memory pool based on linked list,
//              which allow to alloc any standerd type(like:char,int etc.)
//               and user-defined POD-type data,with wanted size_type, 
//              for example: typename T = int, and SizeType size_type = 16,
//              when call Alloc(), it will return a memory of the size of sizeof(int)*16;
//[Author]:Nico Hu
//[Date]:2020-07-22
//[Other]:
//========================================================================


template<typename T, typename SizeType = uint16_t>
class MemoryPool
{
    enum { e_EXPAND_SIZE = 32 };
public:
    explicit MemoryPool(SizeType size_type, size_t pool_size = e_EXPAND_SIZE):sizeType_(size_type){
        Expand(pool_size);
    }
    ~MemoryPool(){ Release(); }

    T* Alloc(){
        if (listMem_ == nullptr){
            Expand();
        }

        MemoryPool<T>* p_head = listMem_;
        listMem_ = listMem_->listMem_;
        return reinterpret_cast<T*>(p_head);
    }
    void Free(T* p){
        MemoryPool<T>* p_head = reinterpret_cast<MemoryPool<T>*>(p);
        p_head->listMem_ = listMem_;
        p_head->sizeType_ = sizeType_;
        listMem_ = p_head;
    }

protected:
    void Expand(size_t pool_size = e_EXPAND_SIZE){
        try
        {
            unsigned int node_size = sizeof(T) > (sizeof(MemoryPool<T>*) + sizeof(SizeType)) ? sizeof(T) : (sizeof(MemoryPool<T>*) + sizeof(SizeType));
            node_size += sizeof(SizeType);
            SizeType* p_mem = reinterpret_cast<SizeType*>(new char[node_size]);
            *p_mem = sizeType_;
            auto p_last_node = reinterpret_cast<MemoryPool<T>*>(p_mem + 1);            

            listMem_ = p_last_node;
            for (int i = 0; i < pool_size - 1; ++i) {
                p_mem = reinterpret_cast<SizeType*>(new char[node_size]);
                *p_mem = sizeType_;
                p_last_node->sizeType_ = sizeType_;
                p_last_node->listMem_ = reinterpret_cast<MemoryPool<T>*>(p_mem + 1);
                p_last_node = p_last_node->listMem_;
            }
            p_last_node->sizeType_ = 0;
            p_last_node->listMem_ = nullptr;
        }
        catch (const std::exception& e)
        {
            std::cout << "MemoryPool raise ex:" << e.what() << std::endl;
        }
    }
    void Release() {
        try
        {
            MemoryPool<T>* p_head = nullptr;
            for (p_head = this->listMem_; p_head != nullptr; p_head = this->listMem_) {
                this->listMem_ = this->listMem_->listMem_;
                SizeType* p_mem = reinterpret_cast<SizeType*>(p_head) - 1;
                delete[] reinterpret_cast<char*>(p_mem); p_mem = nullptr;
            }
        }
        catch (const std::exception& e)
        {
            std::cout << "MemoryPool raise ex:" << e.what() << std::endl;
        }        
    }

public:
    MemoryPool<T>* listMem_{ nullptr };
    SizeType sizeType_{ 0 };
};