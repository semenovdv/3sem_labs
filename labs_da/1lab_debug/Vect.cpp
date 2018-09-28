#include <iostream>
#include <iterator>
#include <memory>
#include <utility>
#include <initializer_list>
 
namespace example{
    template <typename T, typename Allocator = std::allocator<T>>
    class vector {
    public:
        typedef T value_type;
        typedef Allocator allocator_type;
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef value_type& reference;
        typedef const value_type& const_reference;
        typedef typename std::allocator_traits<Allocator>::pointer pointer;
        typedef typename std::allocator_traits<Allocator>::const_pointer const_pointer;
    
        typedef typename std::iterator<std::random_access_iterator_tag, T>::pointer iterator;
        typedef typename std::iterator<std::random_access_iterator_tag, const T>::pointer const_iterator;
        
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    
        vector() = default;
    
        vector(vector const& rhs) {
            auto newdata = alloc_n_copy(rhs.begin(), rhs.end());
            elements = newdata.first;
            first_free = cap = newdata.second;
        }
    
        vector(std::initializer_list<T> il){
            auto data = alloc_n_copy(il.begin(), il.end());
            elements = data.first;
            first_free = cap = data.second;
        }
    
        vector(vector && rhs) noexcept
            : elements(rhs.elements), first_free(rhs.first_free), cap(rhs.cap){
            rhs.elements = rhs.first_free = rhs.cap = nullptr;
        }
    
        vector &operator=(vector const& rhs) {
            auto newdata = alloc_n_copy(rhs.begin(), rhs.end());
            free();
            elements = newdata.first;
            first_free = cap = newdata.second;
            return *this;
        }
    
        vector &operator=(std::initializer_list<T> il) {
            auto data = alloc_n_copy(il.begin(), il.end());
            free();
            elements = data.first;
            first_free = cap = data.second;
            return *this;
        }
    
        vector &operator=(vector && rhs) noexcept {
            if (this != &rhs) {
                free();
                elements = rhs.elements;
                first_free = rhs.first_free;
                cap = rhs.cap;
                rhs.elements = rhs.first_free = rhs.cap = nullptr;
            }
            return *this;
        }
    
        ~vector() {free();}
    
        iterator begin() noexcept { return elements; }
        const_iterator begin() const noexcept { return elements; }
        const_iterator cbegin() const noexcept { return elements; }
    
        iterator end() noexcept { return first_free; }
        const_iterator end() const noexcept { return first_free; }
        const_iterator cend() const noexcept { return first_free; }
    
        reference operator [](size_type n) { return elements[n]; }
        const_reference operator [](size_type n) const { return elements[n]; }
        

        void push_back(T const& val) {
            chk_n_alloc();
            alloc.construct(first_free++, val);
        }
    
        void push_back(T &&val) {
            chk_n_alloc();
            alloc.construct(first_free++, std::move(val));
        }
    
        template <typename... Args>
        void emplace_back(Args&&... args) {
            chk_n_alloc();
            alloc.construct(first_free++, std::forward<Args>(args)...);
        }
    
        size_type size() const noexcept {return std::distance(begin(), end());}
        size_type capacity() const noexcept { return cap - elements;}
    
    private:
        std::allocator<T> alloc;
        T * elements = nullptr;
        T * first_free = nullptr;
        T * cap = nullptr;
    
        void free() {
            if (elements) {
                for (auto p = first_free; p != elements; /*...*/)
                alloc.destroy(--p);
                alloc.deallocate(elements, cap - elements);
            }
        }
    
        void reallocate() {
            auto newcapacity = size() ? 2 * size() : 1;
            auto first = alloc.allocate(newcapacity);
            auto last = std::uninitialized_copy(std::make_move_iterator(begin()),
                                            std::make_move_iterator(end()),
                                            first);
            free();
            elements = first;
            first_free = last;
            cap = elements + newcapacity;
        }
    
        std::pair<T *, T *> alloc_n_copy(const T * b, const T * e) {
            auto data = alloc.allocate(e - b);
            return {data, std::uninitialized_copy(b, e, data)};
        }
    
        void chk_n_alloc() { if (size() == capacity()) reallocate(); }
    };
    
   
 
}