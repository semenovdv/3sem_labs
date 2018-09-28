#include <memory>
#include <limits>
#include <iterator>
#include <type_traits>

namespace example
{


	template<typename IteratorType, typename Container>
	class general_iterator
	{
		using iterator_traits = std::iterator_traits<IteratorType>;
	public:
		using iterator_category = typename iterator_traits::iterator_category;
		using iterator_type = IteratorType;
		using value_type = typename iterator_traits::value_type;
		using difference_type = typename iterator_traits::difference_type;
		using pointer = typename iterator_traits::pointer;
		using reference = typename iterator_traits::reference;

		general_iterator() noexcept :
			m_current(IteratorType{})
		{
		}

		general_iterator(IteratorType const & src) noexcept :
			m_current(src)
		{
		}

		template<typename SourceIterator>
		general_iterator(general_iterator<
			SourceIterator,
			typename std::enable_if<
			std::is_same<
			SourceIterator,
			typename Container::pointer
			>::value,
			Container
			>::type
		> const & src
		) :
			m_current(src.base())
		{
		}

		reference operator*() const noexcept
		{
			return *m_current;
		}
		pointer operator->() const noexcept
		{
			return m_current;
		}

		general_iterator & operator++() noexcept
		{
			++m_current;
			return *this;
		}

		general_iterator operator++(int) noexcept
		{
			general_iterator old(m_current);
			++m_current;
			return old;
		}

		general_iterator & operator--() noexcept
		{
			--m_current;
			return *this;
		}

		general_iterator operator--(int) noexcept
		{
			general_iterator old(m_current);
			--m_current;
			return old;
		}

		general_iterator operator+(difference_type diff) const noexcept
		{
			return m_current + diff;
		}

		general_iterator & operator+=(difference_type diff) noexcept
		{
			m_current += diff;
			return *this;
		}

		general_iterator operator-(difference_type diff) const noexcept
		{
			return m_current - diff;
		}

		general_iterator & operator-=(difference_type diff) noexcept
		{
			m_current -= diff;
			return *this;
		}


		iterator_type const & base() const noexcept
		{
			return m_current;
		}


	private:
		IteratorType m_current;
	};//class general_iterator


	template<
		typename IteratorTypeLeft,
		typename IteratorTypeRight,
		typename Container
	>
		bool operator==(
			general_iterator<IteratorTypeLeft, Container> const & left,
			general_iterator<IteratorTypeRight, Container> const & right
			)
	{
		return left.base() == right.base();
	}

	template<
		typename IteratorTypeLeft,
		typename IteratorTypeRight,
		typename Container
	>
		bool operator!=(
			general_iterator<IteratorTypeLeft, Container> const & left,
			general_iterator<IteratorTypeRight, Container> const & right
			)
	{
		return left.base() != right.base();
	}


	template<
		typename IteratorTypeLeft,
		typename IteratorTypeRight,
		typename Container
	>
		bool operator<(
			general_iterator<IteratorTypeLeft, Container> const & left,
			general_iterator<IteratorTypeRight, Container> const & right
			)
	{
		return left.base() < right.base();
	}

	template<
		typename IteratorTypeLeft,
		typename IteratorTypeRight,
		typename Container
	>
		bool operator<=(
			general_iterator<IteratorTypeLeft, Container> const & left,
			general_iterator<IteratorTypeRight, Container> const & right
			)
	{
		return left.base() <= right.base();
	}

	template<
		typename IteratorTypeLeft,
		typename IteratorTypeRight,
		typename Container
	>
		bool operator>(
			general_iterator<IteratorTypeLeft, Container> const & left,
			general_iterator<IteratorTypeRight, Container> const & right
			)
	{
		return left.base() > right.base();
	}

	template<
		typename IteratorTypeLeft,
		typename IteratorTypeRight,
		typename Container
	>
		bool operator>=(
			general_iterator<IteratorTypeLeft, Container> const & left,
			general_iterator<IteratorTypeRight, Container> const & right
			)
	{
		return left.base() >= right.base();
	}


	template<
		typename IteratorTypeLeft,
		typename IteratorTypeRight,
		typename Container
	>
		auto operator-(
			general_iterator<IteratorTypeLeft, Container> const & left,
			general_iterator<IteratorTypeRight, Container> const & right
			) -> decltype(left.base() - right.base())
	{
		return left.base() - right.base();
	}


}//namespace example

namespace example
{

	template<typename T, typename Alloc = std::allocator<T>>
	class vector
	{
	public:
		using allocator_type = typename Alloc::template rebind<T>::other;//����� ��� ���������
		using value_type = typename allocator_type::value_type;//��� ���������� ���������
		using reference = typename allocator_type::reference;//������ �� ��� ���������� ���������
		using const_reference = typename allocator_type::const_reference;//������ �� ����������� ��� ���������� ���������
		using pointer = typename allocator_type::pointer;//��������� �� ��� ���������� ���������
		using const_pointer = typename allocator_type::const_pointer;//��������� �� ����������� ��� ���������� ���������
		using size_type = typename allocator_type::size_type;//��� ��� ������� � ��������
		using difference_type = typename allocator_type::difference_type;//��� ��� ������� ���������� � ����������
		using iterator = general_iterator<pointer, vector>;
		using const_iterator = general_iterator<const_pointer, vector>;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;
		vector() noexcept :
			m_impl()
		{
		}

		vector(vector const & src)
			: m_impl(src.size())
		{
			m_impl.set_finish(uninitialized_copy(src.m_impl.start(), src.m_impl.finish(), m_impl.start()));
		}

		vector & operator=(vector const & rhv)
		{
			if (this == &rhv) {
				return *this;
			}
			if (rhv.empty()) {
				clear();
				return *this;
			}
			size_type new_size = rhv.size();
			if (capacity() < new_size) {
				//������-�������� �� ����� �������� ��� �������� �������-��������� ��� ������������� ������.
				//���������� � ������ ����� ����� ������ ���� ��� ��������, �.�. �� ���� ��� ���������� �� reserve.
				create_copy_from(rhv.m_impl.start(), rhv.m_impl.finish(), new_size);
			}
			else {
				//������-�������� ����� �������� ��� �������� �������-��������� ��� ������������� ������.
				pointer dest_current = m_impl.start();
				pointer dest_finish = m_impl.finish();
				//��������� �� ������� ���������� ������������ ��-�� �������� ��������
				//������� ��� ����� ������� ������
				pointer dest_middle = dest_current + std::min(new_size, m_impl.size());
				const_pointer source_current = rhv.m_impl.start();
				const_pointer source_finish = rhv.m_impl.finish();
				//��������� ������������ �� ������� �����
				//���� ���� ��������� ���������� ������ ������ ��������
				for (; dest_current != dest_middle; ++dest_current) {
					*dest_current = *source_current;
					++source_current;
				}
				/*
					//���������� ������ ��������
					//���� �� ��� (dest_current == dest_finish), �� destroy ������ �� ���������
					destroy(dest_current, dest_finish);
					//������� ����������� �������� � ������������� ����� ����� ������������������
					//���� �� ��� (source_current == source_finish), �� uninitialized_copy ������ �� �������
					m_impl.set_finish(uninitialized_copy(source_current, source_finish, dest_current));
				*/

				//���� ������� ������� �������
				if (new_size != m_impl.size()) {
					if (source_current == source_finish) {
						//���������� ������ ��������
						destroy(dest_current, dest_finish);
					}
					else {
						//������� ����������� ��������
						dest_current = uninitialized_copy(source_current, source_finish, dest_current);
					}
					//������������� ����� ����� �������
					m_impl.set_finish(dest_current);
				}
			}
			return *this;
		}
		~vector()
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

		bool empty() const noexcept
		{
			return m_impl.empty();
		}

		size_type capacity() const noexcept
		{
			return m_impl.capacity();
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

		void swap(vector & src) noexcept
		{
			m_impl.swap_data(src.m_impl);
		}

		bool reserve(size_type new_capacity)
		{
			if (capacity() < new_capacity) {
				if(!create_copy_from(m_impl.start(), m_impl.finish(), new_capacity)){
					return false;
				}
			}
			return true;
		}

		void resize(size_type new_size)
		{
			if (new_size <= size()) {
				crop(new_size);
			}
			else {
				resize(new_size, value_type{});
			}
		}

		void resize(size_type new_size, const_reference value)
		{
			//���� ������ ������, �� ������ �������� ����� �������:
			if (new_size <= size()) {
				crop(new_size);
			}
			else {
				//����������� ����������� ���������� ������
				reserve(new_size);
				//��������� ����� �������
				m_impl.set_finish(uninitialized_fill_n(m_impl.finish(), new_size - size(), value));
			}
		}

		bool push_back(const_reference value)
		{
			size_type current_size = m_impl.size();
			if (!(current_size < m_impl.capacity())) {
				size_type new_size = static_cast<size_type>(current_size * 1.6);
				if (new_size == current_size) {
					++new_size;
				}
				if(!reserve(new_size)){
					return false;
				}
			}
			m_impl.set_finish(uninitialized_fill_n(m_impl.finish(), 1, value));
			return true;
		}

		void pop_back()
		{
			m_impl.set_finish(m_impl.finish() - 1);
			m_impl.destroy(m_impl.finish());
		}


		void clear() noexcept
		{
			crop(0);
		}

		iterator begin() noexcept
		{
			return m_impl.start();
		}

		const_iterator begin() const noexcept
		{
			return m_impl.start();
		}

		const_iterator cbegin() const noexcept
		{
			return begin();
		}

		iterator end() noexcept
		{
			return m_impl.finish();
		}

		const_iterator end() const noexcept
		{
			return m_impl.finish();
		}

		const_iterator cend() const noexcept
		{
			return end();
		}



		//........

		reverse_iterator rbegin() noexcept
		{
			return reverse_iterator(m_impl.finish());
		}

		const_reverse_iterator rbegin() const noexcept
		{
			return const_reverse_iterator(m_impl.finish());
		}

		const_reverse_iterator crbegin() const noexcept
		{
			return rbegin();
		}

		reverse_iterator rend() noexcept
		{
			return reverse_iterator(m_impl.start());
		}

		const_reverse_iterator rend() const noexcept
		{
			return const_reverse_iterator(m_impl.start());
		}

		const_reverse_iterator crend() const noexcept
		{
			return rend();
		}
	private:
		struct vector_data
		{
			vector_data() :
				m_start(),
				m_finish(),
				m_end_of_storage()
			{
			}

			size_type size() const noexcept
			{
				return static_cast<size_type>(m_finish - m_start);//������� ����� ���������� �� ����� � ������
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

			//�������� ������ ������
			void swap_data(vector_data & src) noexcept
			{
				//������ ���������� ������ �������
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
				//������������ ���������� ��������� ��� ������� �� ����� ��������� ����������� ������ ����������
				return static_cast<size_type>(
					std::numeric_limits<difference_type>::max() / sizeof(value_type) - sizeof(value_type)
					);
			}

			size_type capacity() const noexcept
			{
				return static_cast<size_type>(m_end_of_storage - m_start);
			}

			pointer m_start;//������ �������
			pointer m_finish;//����� �������
			pointer m_end_of_storage;//����� ���������� ������
			bool my_is_null = false;
		};//struct vector_data

		struct vector_impl : allocator_type, vector_data //���� ��������� ������, �� ������ ������� �� ����������
		{
			vector_impl()//�������������� ������ � "�������" ���������
			{
			}

			vector_impl(size_type size) ://������������ �����������
				vector_impl() //�������������� ������ � "�������" ���������
			{
				create_storage(size);//������� ����� ��� �������� size ���������
			}

			~vector_impl()
			{
				destroy_storage();//����������� �����
			}


			void set_finish(pointer p) noexcept
			{
				vector_data::m_finish = p;
			}

			//�������� ������ ��� ��������� ���������� ���������
			void create_storage(size_type size)
			{
				if (size == 0) {
					//��������� ������ � "�������" ���������
					vector_data::set_null();
				}
				else {
					vector_data::m_start = allocator_type::allocate(size);//������� ����� ��� �������� size ���������
					//����������� ����� ������� � ����� ������
					vector_data::m_finish = vector_data::m_start;
					vector_data::m_end_of_storage = vector_data::m_start + size;
				}
			}

			void create_storage_to(vector_data & data, size_type size)
			{
				if (size == 0) {
					data.set_null();
				}
				else {
					data.m_start = allocator_type::allocate(size);
					data.m_finish = data.m_start;
					data.m_end_of_storage = data.m_start + size;
				}
			}
			//����������� ������
			void destroy_storage()
			{
				if (!vector_data::is_null()) {
					destroy_storage(*this);
				}
			}

			void destroy_storage(vector_data & data)
			{
				allocator_type::deallocate(data.m_start, data.size());
				data.set_null();
			}

			vector_impl & operator=(vector_data const & data) noexcept
			{
				destroy_storage();
				static_cast<vector_data&>(*this) = data;
				return *this;
			}
		};//struct vector_impl

		vector_data reserve_storage(size_type new_capacity)
		{
			vector_data data;
			if (m_impl.max_capacity() < new_capacity) {
				throw std::length_error("vector::reserve: invalid size");
			}
			if (m_impl.capacity() < new_capacity) {
				m_impl.create_storage_to(data, new_capacity);
			}
			return data;
		}

		void crop(size_type new_size)
		{
			destroy(m_impl.start() + new_size, m_impl.finish());
			m_impl.set_finish(m_impl.start() + new_size);
		}

		bool create_copy_from(const_pointer start, const_pointer finish, size_type new_capacity)
		{
			vector_data new_data = create_initialized_storage(start, finish, new_capacity);
			destroy(m_impl.start(), m_impl.finish());
			if(new_data.my_is_null == true){
				return false;
			} else{
				m_impl = new_data;	
			}
			return true;
		}


		vector_data create_initialized_storage(const_pointer start, const_pointer finish, size_type new_capacity)
		{
			vector_data result;
			if (new_capacity) {
				result = reserve_storage(new_capacity);
				try {
					result.set_finish(uninitialized_copy(start, finish, result.start()));
				}
				catch (...) {
					m_impl.destroy_storage(result);
					result.my_is_null = true;
					return result;
				}
			}
			return result;
		}


		pointer uninitialized_copy(const_pointer start, const_pointer finish, pointer destination)
		{
			pointer current = destination;
			try {
				for (; start != finish; ++start) {//������� �������� � ��������� ������� ������
					m_impl.construct(current, *start);//construct ������� ����������� ��� �������
					++current;
				}
				return current;
			}
			catch (...) {
				destroy(destination, current);
				throw;//����������� ������� ���������� �����
			}
			//������� �� ��������, ������� �� ����� ������ ����� return current
		}

		pointer uninitialized_fill(pointer start, pointer finish, const_reference source)
		{
			pointer current = start;
			try {
				for (; current != finish; ++current) {//������� �������� � ��������� ������� ������
					m_impl.construct(current, source);
				}
				return current;
			}
			catch (...) {
				destroy(start, current);
				throw;
			}
		}

		pointer uninitialized_fill_n(pointer start, size_type n, const_reference source)
		{
			return uninitialized_fill(start, start + n, source);
		}


		void destroy(pointer start, pointer finish) noexcept
		{
			while (start != finish) {
				m_impl.destroy(--finish);
			}
		}

		vector_impl m_impl;
	};//class vector

}//namespace example