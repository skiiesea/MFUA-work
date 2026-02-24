#include <chrono>
#include <iostream>
#include <vector>
template <typename T>
class allocator {
public:
    using value_type = T;

    T* allocate(size_t n){
        std::cout << "Items: " << n << "\nAllocate: " << n * sizeof(T) << " bytes\n";
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }
    void deallocate(T* ptr, size_t n){
        std::cout << "Deallocate " << n * sizeof(T) << " bytes\n"; 
        ::operator delete(ptr);
    }
    template <typename U, typename... Args>
    void construct(U* p, Args&&... args){
        new(p) U(std::forward<Args>(args)...);
    }
    
    template <typename U>
    void destroy(U* p) {
        p->~U();
    }
template <typename T, typename U>
bool operator==(const allocator <T>&, const allocator<U>&) {
    return true;
}
};


int  N = 1000000;

int main() {
    std::vector<int, allocator<int>> vec;
    auto start = std::chrono::steady_clock::now();

    auto end = std::chrono::steady_clock::now();
    for (int i = 0; i < N; ++i){
        vec.push_back(i);
    }
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::cout << "Reserve elapsed time: " << duration.count() / 1e9 << "s\n"; 
    return 0;
}