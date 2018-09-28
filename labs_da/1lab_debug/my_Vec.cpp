#include <memory>
#include <limits>


namespace example{
    template<typename T, typename Alloc = std::allocator<T>>
    class vect
    {
    public:
        using allocator_type = typename Alloc::template rebind<T>::other;//New allocator type
        using value_type = typename allocator_type::value_type;//Type of els in storage
        using reference = typename allocator_type::reference;
        using const_reference = typename allocator_type::const_reference;
        using pointer = typename allocator_type::pointer;
        using const_pointer = typename allocator_type::const_pointer; 
        using size_type = typename allocator_type::size_type;
        using difference_type = typename allocator_type::difference_type;
        vect() noexcept: 
            m_impl()
        {
        }

        vect(vect const  & src):
            m_impl(src.size())

        {
            m_impl.set_finish(uninitialized_copy(src.m_impl.start(), src.m_impl.finish(), m_impl.start()));
        }
        
        ~vect()
        {
            clear();
        }

        size_type max_size() const noexcept
        {
            return m_impl.max_size();
        }

        size_type size() const noexcept
        {
            return m_impl.size();
        }

        size_type capacity() const noexcept
        {
            return m_impl.capacity();
        }

        bool empty() const noexcept
        {
            return m_impl.empty();
        }

        pointer data() noexcept
        {
            return m_impl.start();
        }

        const_pointer data() const noexcept
        {
            return m_impl.start();
        }

        reference operator[](size_type index) noexcept
        {
            return m_impl.start()[index];
        }

        const_reference operator[](size_type index) const noexcept
        {
            return m_impl.start()[index];
        }

        reference front() noexcept
        {
            return *m_impl.start();
        }

        const_reference front() const noexcept
        {
            return *m_impl.start();
        }

        reference back() noexcept
        {
            return *(m_impl.finish() - 1);
        }

        const_reference back() const noexcept
        {
            return *(m_impl.finish() - 1);
        }

        void push_back(const_reference value){
            size_type current_size = m_impl.size();
            if(!(current_size < m_impl.capacity())) {
                size_type new_size = current_size * 1.6;
                if(new_size == current_size){
                    ++new_size;
                }
                reserve(new_size);
            }
            m_impl.set_finish(uninitialized_fill_n(m_impl.finish(), 1, value));
        }

        void pop_back()
        {
            m_impl.set_finish(m_impl.finish() - 1);
            m_impl.destroy(m_impl.finish());
        }

        void swap(vect & src) noexcept
        {
            m_impl.sqap_data(src.m_impl);
        }
        void clear() noexcept
        {
            destroy(m_impl.start(), m_impl.finish());
            m_impl.set_finish(m_impl.start());
        }

        void resize(size_type new_size);
        void resize(size_type new_size, const_reference value);

        void reserve(size_type new_capasity)
        {
            vect_data new_data = reserve_storage(new_capasity);
            try {
                new_data.set_finish(uninitialized_copy(m_impl.start(), m_impl.finish(), new_data.start()));
                destroy(m_impl.start(), m_impl.finish());
                m_impl = new_data;
            } catch (...) {
                m_impl.destroy_storage(new_data);
                throw;
            }
        }
        
    private:
        struct vect_data
        {
            vect_data():
                m_start(),
                m_finish(),
                m_end_of_storage()
            {
            }

            size_type size() const noexcept
            {
                return static_cast<size_type>(m_finish - m_start);
            }

            pointer start() noexcept
            {
                return m_start;
            }

            const_pointer start() const noexcept
            {
                return m_start;
            }

            pointer finish() noexcept
            {
                return m_finish;
            }

            const_pointer finish() const noexcept
            {
                return m_finish;
            }

            bool is_null() const noexcept
            {
                return m_start == nullptr;
            }

            void set_finish(pointer p) noexcept
            {
                m_finish = p;
            }

            void swap_data(vect_data & src) noexcept
            {
                std::swap(m_start, src.m_start);
                std::swap(m_finish, src.m_finish);
                std::swap(m_end_of_storage, src.m_end_of_storage);
            }

            void set_null() noexcept
            {
                m_start = m_finish = m_end_of_storage = nullptr;
            }

            bool empty() const noexcept
            {
                return m_start == m_finish;
            }

            constexpr size_type max_capacity() const noexcept
            {
                return static_cast<size_type>(
                    std::numeric_limits<difference_type>::max() / sizeof(value_type) - sizeof(value_type)
                );
            }

            size_type capacity() const noexcept
            {
                return static_cast<size_type>(m_end_of_storage - m_start);
            }

            pointer m_start;
            pointer m_finish;
            pointer m_end_of_storage;
        }; // struct vect_data

        struct vect_impl: allocator_type, vect_data
        {
            vect_impl()
            {
            }

            vect_impl(size_type size):
                vect_impl()
            {
                create_storage(size);
            }

            ~vect_impl()
            {
                destroy_storage();
            }

            void set_finish(pointer p) noexcept
            {
                vect_data::m_finish = p;
            }

            //making buffer
            void create_storage(size_type size)
            {
                if(size == 0){
                    vect_data::set_null();
                } else {
                    vect_data::m_start = allocator_type::allocate(size);
                    vect_data::m_finish = vect_data::m_start;
                    vect_data::m_end_of_storage = vect_data::m_start + size;
                }
            }

            void create_storage_to(vect_data & data, size_type size)
            {
                if(size == 0){
                    data.set_null();
                } else {
                    data.m_start = allocator_type::allocate(size);
                    data.m_finish = data.m_start;
                    data.m_end_of_storage = data.m_start + size;
                }
            }

            void destroy_storage()
            {
                if(!vect_data::is_null()){
                    destroy_storage(*this);
                }
            }

            void destroy_storage(vect_data & data)
            {
                allocator_type::deallocate(data.m_start, data.size());
                data.set_null();
            }

            vect_impl & operator=(vect_data const & data) noexcept
            {
                destroy_storage();
                static_cast<vect_data&>(*this) = data;
                return *this;
            }
        }; //struct vect_impl

        vect_data reserve_storage(size_type new_capacity)
        {
            vect_data data;
            if(m_impl.max_capacity() < new_capacity){
                throw std::length_error("vector::reserve: invalid size");
            } 
            if(m_impl.capacity() < new_capacity){
                m_impl.create_storage_to(data, new_capacity);
            }
            return data;
        } 

        pointer uninitialized_copy(const_pointer start, const_pointer finish, pointer destination)
        {
            pointer current = destination;
            try{
                for(;start != finish; ++start){
                    m_impl.construct(current, *start);
                    ++current;
                }
                return current;
            } catch (...) {
                destroy(destination, current);
                throw;
            }
            return current; // участок недостижим
        }

        pointer uninitialized_fill_n(pointer start, pointer finish, const_reference source)
        {
            pointer current = start;
            try{
                for(; current != finish; ++current){
                    m_impl.construct(current, source);
                }
                return current;
            } catch (...) {
                destroy(start, current);
                throw;
            }
        }

        pointer uninitialized_fill_n(pointer start, size_type n, const_reference source)
        {
            return uninitialized_fill_n(start, start + n, source);
        }

        void destroy(pointer start, pointer finish) noexcept
        {
            while(start != finish){
                m_impl.destroy(--finish);
            }
        }

        vect_impl m_impl;
    }; // class vect
}

