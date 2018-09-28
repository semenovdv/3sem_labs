// часть 1
namespace example
{

	template<typename T, typename Alloc = std::allocator<T>>
	class vector
	{
	public:
		using allocator_type = typename Alloc::template rebind<T>::other;//Новый тип аллокатор
		using value_type = typename allocator_type::value_type;//Тип хранящихся элементов
		using reference = typename allocator_type::reference;//Ссылка на тип хранящихся элементов
		using const_reference = typename allocator_type::const_reference;//Ссылка на константный тип хранящихся элементов
		using pointer = typename allocator_type::pointer;//Указатель на тип хранящихся элементов
		using const_pointer = typename allocator_type::const_pointer;//Указатель на константный тип хранящихся элементов
		using size_type = typename allocator_type::size_type;//Тип для размера и индексов
		using difference_type = typename allocator_type::difference_type;//Тип для разницы указателей и итераторов
		vector() noexcept :
			m_impl()
		{
		}

		vector(vector const & src)
			: m_impl(src.size())
		{
			m_impl.set_finish(uninitialized_copy(src.m_impl.start(), src.m_impl.finish(), m_impl.start()));
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

		void reserve(size_type new_capacity)
		{
			vector_data new_data = reserve_storage(new_capacity);
			try {
				new_data.set_finish(uninitialized_copy(m_impl.start(), m_impl.finish(), new_data.start()));
				destroy(m_impl.start(), m_impl.finish());
				m_impl = new_data;
			}
			catch (...) {
				m_impl.destroy_storage(new_data);
				throw;
			}
		}

		void resize(size_type new_size);
		void resize(size_type new_size, const_reference value);

		void push_back(const_reference value)
		{
			size_type current_size = m_impl.size();
			if (!(current_size < m_impl.capacity())) {
				size_type new_size = current_size * 1.6;
				if (new_size == current_size) {
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


		void clear() noexcept
		{
			destroy(m_impl.start(), m_impl.finish());//Уничтожаем объекты, но не освобождаем память
			m_impl.set_finish(m_impl.start());
		}


		//...
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
				return static_cast<size_type>(m_finish - m_start);//Разница между указателем на конец и начало
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

			//Взаимная замена данных
			void swap_data(vector_data & src) noexcept
			{
				//Меняем внутренние данные местами
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
				//Максимальное количество элементов для которых мы можем корректно отслеживать размер контейнера
				return static_cast<size_type>(
					std::numeric_limits<difference_type>::max() / sizeof(value_type) - sizeof(value_type)
					);
			}

			size_type capacity() const noexcept
			{
				return static_cast<size_type>(m_end_of_storage - m_start);
			}

			pointer m_start;//Начало вектора
			pointer m_finish;//Конец вектора
			pointer m_end_of_storage;//Конец выделенной памяти
		};//struct vector_data

		struct vector_impl : allocator_type, vector_data //Если аллокатор пустой, то размер объекта не увеличится
		{
			vector_impl()//Инициализируем объект в "нулевое" состояние
			{
			}

			vector_impl(size_type size) ://Делегирующий конструктор
				vector_impl() //Инициализируем объект в "нулевое" состояние
			{
				create_storage(size);//Создаем буфер для хранения size элементов
			}

			~vector_impl()
			{
				destroy_storage();//Освобождаем буфер
			}


			void set_finish(pointer p) noexcept
			{
				vector_data::m_finish = p;
			}

			//Создание буфера для заданного количества элементов
			void create_storage(size_type size)
			{
				if (size == 0) {
					//Переводим объект в "нулевое" состояние
					vector_data::set_null();
				}
				else {
					vector_data::m_start = allocator_type::allocate(size);//Создаем буфер для хранения size элементов
					//Настраиваем конец вектора и окнец буфера
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
			//Уничтожение буфера
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


		pointer uninitialized_copy(const_pointer start, const_pointer finish, pointer destination)
		{
			pointer current = destination;
			try {
				for (; start != finish; ++start) {//Создаем элементы в указанном участке памяти
					m_impl.construct(current, *start);//construct вызовет конструктор для объекта
					++current;
				}
				return current;
			}
			catch (...) {
				destroy(destination, current);
				throw;//Выбрасываем текущее исключение снова
			}
			//Участок не достижим, поэтому не будет писать здесь return current
		}

		pointer uninitialized_fill(pointer start, pointer finish, const_reference source)
		{
			pointer current = start;
			try {
				for (; current != finish; ++current) {//Создаем элементы в указанном участке памяти
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

}

// часть 2

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
		using allocator_type = typename Alloc::template rebind<T>::other;//Новый тип аллокатор
		using value_type = typename allocator_type::value_type;//Тип хранящихся элементов
		using reference = typename allocator_type::reference;//Ссылка на тип хранящихся элементов
		using const_reference = typename allocator_type::const_reference;//Ссылка на константный тип хранящихся элементов
		using pointer = typename allocator_type::pointer;//Указатель на тип хранящихся элементов
		using const_pointer = typename allocator_type::const_pointer;//Указатель на константный тип хранящихся элементов
		using size_type = typename allocator_type::size_type;//Тип для размера и индексов
		using difference_type = typename allocator_type::difference_type;//Тип для разницы указателей и итераторов
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
				//Вектор-приемник не может вместить все элементы вектора-источника без перевыделения памяти.
				//Устройство и работа этого блока должны быть уже известны, т.к. он мало чем отличается от reserve.
				create_copy_from(rhv.m_impl.start(), rhv.m_impl.finish(), new_size);
			}
			else {
				//Вектор-приемник может вместить все элементы вектора-источника без перевыделения памяти.
				pointer dest_current = m_impl.start();
				pointer dest_finish = m_impl.finish();
				//Указатель на котором закончится присваивание из-за разности размеров
				//Назовем это место средней точкой
				pointer dest_middle = dest_current + std::min(new_size, m_impl.size());
				const_pointer source_current = rhv.m_impl.start();
				const_pointer source_finish = rhv.m_impl.finish();
				//Выполняем присваивание до средней точки
				//Этот цикл полностью обработает случай равных размеров
				for (; dest_current != dest_middle; ++dest_current) {
					*dest_current = *source_current;
					++source_current;
				}
				/*
					//Уничтожаем лишние элементы
					//Если их нет (dest_current == dest_finish), то destroy ничего не уничтожит
					destroy(dest_current, dest_finish);
					//Создаем недостающие элементы и устанавливаем новый конец последовательности
					//Если их нет (source_current == source_finish), то uninitialized_copy ничего не сделает
					m_impl.set_finish(uninitialized_copy(source_current, source_finish, dest_current));
				*/

				//Если вектора разного размера
				if (new_size != m_impl.size()) {
					if (source_current == source_finish) {
						//Уничтожаем лишние элементы
						destroy(dest_current, dest_finish);
					}
					else {
						//Создаем недостающие элементы
						dest_current = uninitialized_copy(source_current, source_finish, dest_current);
					}
					//Устанавливаем новый конец вектора
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

		void reserve(size_type new_capacity)
		{
			if (capacity() < new_capacity) {
				create_copy_from(m_impl.start(), m_impl.finish(), new_capacity);
			}
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
			//Если размер меньше, то просто обрезаем конец вектора:
			if (new_size <= size()) {
				crop(new_size);
			}
			else {
				//Резервируем достаточное количество памяти
				reserve(new_size);
				//Заполняем конец вектора
				m_impl.set_finish(uninitialized_fill_n(m_impl.finish(), new_size - size(), value));
			}
		}

		void push_back(const_reference value)
		{
			size_type current_size = m_impl.size();
			if (!(current_size < m_impl.capacity())) {
				size_type new_size = static_cast<size_type>(current_size * 1.6);
				if (new_size == current_size) {
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
				return static_cast<size_type>(m_finish - m_start);//Разница между указателем на конец и начало
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

			//Взаимная замена данных
			void swap_data(vector_data & src) noexcept
			{
				//Меняем внутренние данные местами
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
				//Максимальное количество элементов для которых мы можем корректно отслеживать размер контейнера
				return static_cast<size_type>(
					std::numeric_limits<difference_type>::max() / sizeof(value_type) - sizeof(value_type)
					);
			}

			size_type capacity() const noexcept
			{
				return static_cast<size_type>(m_end_of_storage - m_start);
			}

			pointer m_start;//Начало вектора
			pointer m_finish;//Конец вектора
			pointer m_end_of_storage;//Конец выделенной памяти
		};//struct vector_data

		struct vector_impl : allocator_type, vector_data //Если аллокатор пустой, то размер объекта не увеличится
		{
			vector_impl()//Инициализируем объект в "нулевое" состояние
			{
			}

			vector_impl(size_type size) ://Делегирующий конструктор
				vector_impl() //Инициализируем объект в "нулевое" состояние
			{
				create_storage(size);//Создаем буфер для хранения size элементов
			}

			~vector_impl()
			{
				destroy_storage();//Освобождаем буфер
			}


			void set_finish(pointer p) noexcept
			{
				vector_data::m_finish = p;
			}

			//Создание буфера для заданного количества элементов
			void create_storage(size_type size)
			{
				if (size == 0) {
					//Переводим объект в "нулевое" состояние
					vector_data::set_null();
				}
				else {
					vector_data::m_start = allocator_type::allocate(size);//Создаем буфер для хранения size элементов
					//Настраиваем конец вектора и окнец буфера
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
			//Уничтожение буфера
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

		void create_copy_from(const_pointer start, const_pointer finish, size_type new_capacity)
		{
			vector_data new_data = create_initialized_storage(start, finish, new_capacity);
			destroy(m_impl.start(), m_impl.finish());
			m_impl = new_data;
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
					throw;
				}
			}
			return result;
		}


		pointer uninitialized_copy(const_pointer start, const_pointer finish, pointer destination)
		{
			pointer current = destination;
			try {
				for (; start != finish; ++start) {//Создаем элементы в указанном участке памяти
					m_impl.construct(current, *start);//construct вызовет конструктор для объекта
					++current;
				}
				return current;
			}
			catch (...) {
				destroy(destination, current);
				throw;//Выбрасываем текущее исключение снова
			}
			//Участок не достижим, поэтому не будет писать здесь return current
		}

		pointer uninitialized_fill(pointer start, pointer finish, const_reference source)
		{
			pointer current = start;
			try {
				for (; current != finish; ++current) {//Создаем элементы в указанном участке памяти
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

// часть 3

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



	template<typename T>
	void destroy_at(T * ptr) noexcept
	{
		ptr->~T();
	}

	template<typename ForwardIterator>
	void destroy(ForwardIterator first, ForwardIterator last) noexcept
	{
		for (; first != last; ++first) {
			::example::destroy_at(std::addressof(*first));
		}
	}

	template<typename T, typename ... Args>
	void construct_at(T * p, Args && ...args)
	{
		::new (static_cast<void*>(p)) T(std::forward<Args>(args)...);
	}


	template<typename InputIterator, typename ForwardIterator>
	ForwardIterator uninitialized_copy(InputIterator start, InputIterator finish, ForwardIterator destination)
	{
		ForwardIterator current = destination;
		try {
			for (; start != finish; ++start) {
				::example::construct_at(std::addressof(*current), *start);
				++current;
			}
			return current;
		}
		catch (...) {
			::example::destroy(destination, current);
			throw;
		}
	}

	template<typename T, typename ForwardIterator>
	ForwardIterator uninitialized_fill(ForwardIterator start, ForwardIterator finish, T const & source)
	{
		ForwardIterator current = start;
		try {
			for (; current != finish; ++current) {//Создаем элементы в указанном участке памяти
				::example::construct_at(std::addressof(*current), source);
			}
			return current;
		}
		catch (...) {
			::example::destroy(start, current);
			throw;
		}
	}


	template<typename T, typename ForwardIterator>
	ForwardIterator uninitialized_fill_n(ForwardIterator start, std::size_t count, T const & source)
	{
		ForwardIterator current = start;
		try {
			for (; count != 0; --count) {
				::example::construct_at(std::addressof(*current), source);
				++current;
			}
			return current;
		}
		catch (...) {
			::example::destroy(start, current);
			throw;
		}
	}


	template<typename InputIterator, typename OutputIterator>
	OutputIterator copy(InputIterator source_begin, InputIterator source_end, OutputIterator dest_begin)
	{
		for (; source_begin != source_end; ++source_begin) {
			*dest_begin = *source_begin;
			++dest_begin;
		}
		return dest_begin;
	}


	template<typename IteratorCategory, typename IteratorType>
	constexpr bool minimal_iterator_category =
		std::is_base_of<
		IteratorCategory,
		typename std::iterator_traits<IteratorType>::iterator_category
		>::value
		;



	namespace details_
	{
		template<
			typename Iterator,
			bool IsRandomAccess = minimal_iterator_category<
			std::random_access_iterator_tag,
			Iterator
			>
		>
			struct distance_impl
		{
			typename std::iterator_traits<Iterator>::difference_type
				operator()(Iterator first, Iterator last)
			{
				typename std::iterator_traits<Iterator>::difference_type count = 0;
				for (; first != last; ++first) {
					++count;
				}
				return count;
			}
		};

		template<typename Iterator>
		struct distance_impl<Iterator, true>
		{
			typename std::iterator_traits<Iterator>::difference_type
				operator()(Iterator first, Iterator last)
			{
				return last - first;
			}
		};
	}//namespace details_


	template<typename Iterator>
	typename std::iterator_traits<Iterator>::difference_type
		distance(Iterator first, Iterator last)
	{
		return ::example::details_::distance_impl<Iterator>()(first, last);
	}

	template<typename ForwardIterator>
	void rotate(ForwardIterator begin, ForwardIterator first, ForwardIterator end)
	{
		while (begin != first && first != end) {
			ForwardIterator beginCopy = first;
			for (; begin != first && beginCopy != end; ++begin, ++beginCopy) {
				std::iter_swap(begin, beginCopy);
			}
			if (begin == first) {
				first = beginCopy;
			}
		}
	}


	template<typename T, typename = void>
	struct is_iterator
	{
		static constexpr bool value = false;
	};

	template<typename T>
	struct is_iterator<T, typename std::enable_if<!std::is_same<typename std::iterator_traits<T>::value_type, void>::value, void>::type>
	{
		static constexpr bool value = true;
	};


}//namespace example

namespace example
{

	template<typename T, typename Alloc = std::allocator<T>>
	class vector
	{
	public:
		using allocator_type = typename Alloc::template rebind<T>::other;//Новый тип аллокатор
		using value_type = typename allocator_type::value_type;//Тип хранящихся элементов
		using reference = typename allocator_type::reference;//Ссылка на тип хранящихся элементов
		using const_reference = typename allocator_type::const_reference;//Ссылка на константный тип хранящихся элементов
		using pointer = typename allocator_type::pointer;//Указатель на тип хранящихся элементов
		using const_pointer = typename allocator_type::const_pointer;//Указатель на константный тип хранящихся элементов
		using size_type = typename allocator_type::size_type;//Тип для размера и индексов
		using difference_type = typename allocator_type::difference_type;//Тип для разницы указателей и итераторов
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
			m_impl.set_finish(::example::uninitialized_copy(src.m_impl.start(), src.m_impl.finish(), m_impl.start()));
		}

		vector(size_type size, const_reference src = value_type{})
			: m_impl(size)
		{
			m_impl.set_finish(::example::uninitialized_fill_n(m_impl.start(), size, src));
		}

		template<typename InputIterator>
		vector(InputIterator first, InputIterator last)
		{
			create_copy_from(first, last);
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
				//Вектор-приемник не может вместить все элементы вектора-источника без перевыделения памяти.
				//Устройство и работа этого блока должны быть уже известны, т.к. он мало чем отличается от reserve.
				create_copy_from(rhv.m_impl.start(), rhv.m_impl.finish(), new_size);
			}
			else {
				//Вектор-приемник может вместить все элементы вектора-источника без перевыделения памяти.
				pointer dest_current = m_impl.start();
				pointer dest_finish = m_impl.finish();
				//Указатель на котором закончится присваивание из-за разности размеров
				//Назовем это место средней точкой
				pointer dest_middle = dest_current + std::min(new_size, m_impl.size());
				const_pointer source_current = rhv.m_impl.start();
				const_pointer source_finish = rhv.m_impl.finish();
				//Выполняем присваивание до средней точки
				//Этот цикл полностью обработает случай равных размеров
				for (; dest_current != dest_middle; ++dest_current) {
					*dest_current = *source_current;
					++source_current;
				}
				/*
					//Уничтожаем лишние элементы
					//Если их нет (dest_current == dest_finish), то destroy ничего не уничтожит
					destroy(dest_current, dest_finish);
					//Создаем недостающие элементы и устанавливаем новый конец последовательности
					//Если их нет (source_current == source_finish), то uninitialized_copy ничего не сделает
					m_impl.set_finish(uninitialized_copy(source_current, source_finish, dest_current));
				*/

				//Если вектора разного размера
				if (new_size != m_impl.size()) {
					if (source_current == source_finish) {
						//Уничтожаем лишние элементы
						destroy(dest_current, dest_finish);
					}
					else {
						//Создаем недостающие элементы
						dest_current = ::example::uninitialized_copy(source_current, source_finish, dest_current);
					}
					//Устанавливаем новый конец вектора
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

		void reserve(size_type new_capacity)
		{
			if (capacity() < new_capacity) {
				create_copy_from(m_impl.start(), m_impl.finish(), new_capacity);
			}
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
			//Если размер меньше, то просто обрезаем конец вектора:
			if (new_size <= size()) {
				crop(new_size);
			}
			else {
				//Резервируем достаточное количество памяти
				reserve(new_size);
				//Заполняем конец вектора
				m_impl.set_finish(uninitialized_fill_n(m_impl.finish(), new_size - size(), value));
			}
		}
		void push_back(const_reference value)
		{
			if (m_impl.size() == m_impl.capacity()) {
				reserve(get_next_size(m_impl.size()));
			}
			m_impl.set_finish(::example::uninitialized_fill_n(m_impl.finish(), 1, value));
		}
		void pop_back()
		{
			m_impl.set_finish(m_impl.finish() - 1);
			m_impl.destroy(m_impl.finish());
		}
		iterator erase(const_iterator first) noexcept(std::is_nothrow_assignable<value_type, value_type>::value)
		{
			return erase(first, first + 1);
		}
		iterator erase(const_iterator first, const_iterator last) noexcept(std::is_nothrow_assignable<value_type, value_type>::value)
		{
			iterator destination = begin() + ::example::distance(cbegin(), first);
			iterator new_end = ::example::copy(last, cend(), destination);
			crop(static_cast<size_type>(new_end - begin()));
			return new_end;
		}
		void clear() noexcept
		{
			crop(0);
		}

		template<typename InputIterator>
		typename std::enable_if<::example::is_iterator<InputIterator>::value, iterator>::type
			insert(const_iterator insert_position, InputIterator first, InputIterator last)
		{
			size_type insert_index = static_cast<size_type>(::example::distance(cbegin(), insert_position));
			size_type old_size = m_impl.size();
			try {
				while (first != last) {
					push_back(*first);
					++first;
				}
				::example::rotate(m_impl.start() + insert_index, m_impl.start() + old_size, m_impl.finish());
			}
			catch (...) {
				crop(old_size);
				throw;
			}
			return iterator(m_impl.start() + static_cast<difference_type>(insert_index));
		}

		iterator insert(const_iterator insert_position, T const & value)
		{
			return insert(insert_position, std::addressof(value), std::addressof(value) + 1);
		}

		iterator insert(const_iterator insert_position, size_type count, T const & value)
		{
			size_type insert_index = static_cast<size_type>(::example::distance(cbegin(), insert_position));
			size_type old_size = m_impl.size();
			try {
				reserve(m_impl.size() + count);
				m_impl.set_finish(::example::uninitialized_fill_n(m_impl.finish(), count, value));
				::example::rotate(m_impl.start() + insert_index, m_impl.start() + old_size, m_impl.finish());
			}
			catch (...) {
				crop(old_size);
				throw;
			}
			return iterator(m_impl.start() + static_cast<difference_type>(insert_index));
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
		const_iterator end() const noexcept {
			return m_impl.finish();
		}
		const_iterator cend() const noexcept
		{
			return end();
		}
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
				return static_cast<size_type>(m_finish - m_start);//Разница между указателем на конец и начало
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

			//Взаимная замена данных
			void swap_data(vector_data & src) noexcept
			{
				//Меняем внутренние данные местами
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
				//Максимальное количество элементов для которых мы можем корректно отслеживать размер контейнера
				return static_cast<size_type>(
					std::numeric_limits<difference_type>::max() / sizeof(value_type) - sizeof(value_type)
					);
			}

			size_type capacity() const noexcept
			{
				return static_cast<size_type>(m_end_of_storage - m_start);
			}

			pointer m_start;//Начало вектора
			pointer m_finish;//Конец вектора
			pointer m_end_of_storage;//Конец выделенной памяти
		};//struct vector_data

		struct vector_impl : allocator_type, vector_data //Если аллокатор пустой, то размер объекта не увеличится
		{
			vector_impl()//Инициализируем объект в "нулевое" состояние
			{
			}

			vector_impl(size_type size) ://Делегирующий конструктор
				vector_impl() //Инициализируем объект в "нулевое" состояние
			{
				create_storage(size);//Создаем буфер для хранения size элементов
			}

			~vector_impl()
			{
				destroy_storage();//Освобождаем буфер
			}


			void set_finish(pointer p) noexcept
			{
				vector_data::m_finish = p;
			}

			//Создание буфера для заданного количества элементов
			void create_storage(size_type size)
			{
				if (size == 0) {
					//Переводим объект в "нулевое" состояние
					vector_data::set_null();
				}
				else {
					vector_data::m_start = allocator_type::allocate(size);//Создаем буфер для хранения size элементов
					//Настраиваем конец вектора и окнец буфера
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
			//Уничтожение буфера
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

		size_type get_next_size(size_type old_size) const noexcept
		{
			return static_cast<size_type>(old_size * 1.6) + 1u;
		}

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

		template<typename Iterator>
		void create_copy_from(Iterator start, Iterator finish, size_type new_capacity)
		{
			vector_data new_data = create_initialized_storage(start, finish, new_capacity);
			destroy(m_impl.start(), m_impl.finish());
			m_impl = new_data;
		}

		template<typename Iterator>
		typename std::enable_if<::example::minimal_iterator_category<
			std::forward_iterator_tag,
			Iterator
		>,
			void
		>::type
			create_copy_from(Iterator start, Iterator finish)
		{
			size_type new_capacity = static_cast<size_type>(::example::distance(start, finish));
			create_copy_from(start, finish, new_capacity);
		}

		template<typename Iterator>
		typename std::enable_if<!::example::minimal_iterator_category<
			std::forward_iterator_tag,
			Iterator
		>,
			void
		>::type
			create_copy_from(Iterator start, Iterator finish)
		{
			while (start != finish) {
				push_back(*start);
			}
		}


		template<typename Iterator>
		vector_data create_initialized_storage(Iterator start, Iterator finish, size_type new_capacity)
		{
			vector_data result;
			if (new_capacity) {
				result = reserve_storage(new_capacity);
				try {
					result.set_finish(::example::uninitialized_copy(start, finish, result.start()));
				}
				catch (...) {
					m_impl.destroy_storage(result);
					throw;
				}
			}
			return result;
		}

		vector_impl m_impl;
	};//class vector

}//namespace example

//часть 4

#include <memory>
#include <limits>
#include <iterator>
#include <algorithm>
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



	template<typename T>
	void destroy_at(T * ptr) noexcept
	{
		ptr->~T();
	}

	template<typename ForwardIterator>
	void destroy(ForwardIterator first, ForwardIterator last) noexcept
	{
		for (; first != last; ++first) {
			::example::destroy_at(std::addressof(*first));
		}
	}

	template<typename T, typename ... Args>
	void construct_at(T * p, Args && ...args)
	{
		::new (static_cast<void*>(p)) T(std::forward<Args>(args)...);
	}


	template<typename InputIterator, typename ForwardIterator>
	ForwardIterator uninitialized_move(InputIterator start, InputIterator finish, ForwardIterator destination)
	{
		ForwardIterator current = destination;
		try {
			for (; start != finish; ++start) {
				::example::construct_at(std::addressof(*current), std::move(*start));
				++current;
			}
			return current;
		}
		catch (...) {
			::example::destroy(destination, current);
			throw;
		}
	}

	template<typename IteratorCategory, typename IteratorType>
	constexpr bool minimal_iterator_category =
		std::is_base_of<
		IteratorCategory,
		typename std::iterator_traits<IteratorType>::iterator_category
		>::value
		;



	template<typename T, typename = void>
	struct is_iterator
	{
		static constexpr bool value = false;
	};

	template<typename T>
	struct is_iterator<T, typename std::enable_if<!std::is_same<typename std::iterator_traits<T>::value_type, void>::value, void>::type>
	{
		static constexpr bool value = true;
	};


}//namespace example

namespace example
{

	template<typename T, typename Alloc = std::allocator<T>>
	class vector
	{
	public:
		using allocator_type = typename Alloc::template rebind<T>::other;
		using value_type = typename allocator_type::value_type;
		using reference = typename allocator_type::reference;
		using const_reference = typename allocator_type::const_reference;
		using rvalue_reference = T && ;
		using pointer = typename allocator_type::pointer;
		using const_pointer = typename allocator_type::const_pointer;
		using size_type = typename allocator_type::size_type;
		using difference_type = typename allocator_type::difference_type;
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
			m_impl.set_finish(::std::uninitialized_copy(src.m_impl.start(), src.m_impl.finish(), m_impl.start()));
		}

		vector(vector && src) noexcept
			: m_impl()
		{
			m_impl.swap_data(src.m_impl);
		}

		vector(size_type size, const_reference src = value_type{})
			: m_impl(size)
		{
			m_impl.set_finish(::std::uninitialized_fill_n(m_impl.start(), size, src));
		}

		vector(::std::initializer_list<value_type> init)
			: m_impl(init.size())
		{
			m_impl.set_finish(::std::uninitialized_copy(init.begin(), init.end(), m_impl.start()));
		}

		template<typename InputIterator>
		vector(InputIterator first, InputIterator last)
		{
			create_copy_from(first, last);
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
				create_copy_from(rhv.m_impl.start(), rhv.m_impl.finish(), new_size);
			}
			else {
				pointer dest_current = m_impl.start();
				pointer dest_finish = m_impl.finish();
				pointer dest_middle = dest_current + std::min(new_size, m_impl.size());
				const_pointer source_current = rhv.m_impl.start();
				const_pointer source_finish = rhv.m_impl.finish();
				for (; dest_current != dest_middle; ++dest_current) {
					*dest_current = *source_current;
					++source_current;
				}

				if (new_size != m_impl.size()) {
					if (source_current == source_finish) {
						::example::destroy(dest_current, dest_finish);
					}
					else {
						dest_current = ::std::uninitialized_copy(source_current, source_finish, dest_current);
					}
					m_impl.set_finish(dest_current);
				}
			}
			return *this;
		}

		vector & operator=(vector && rhv) noexcept
		{
			if (this != &rhv) {
				clear();
				m_impl.destroy_storage();
				m_impl.swap_data(rhv.m_impl);
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

		void reserve(size_type new_capacity)
		{
			//Если тип имеет конструктор копирования и не умеет перемещаться без исключений,
			//то будем использовать указатель для обхода, иначе будем иметь дело с std::move_iterator
			using move_iterator_type = typename std::conditional<
				std::is_copy_constructible<T>::value && !std::is_nothrow_move_constructible<T>::value,
				pointer,
				std::move_iterator<pointer>
			>::type;
			if (capacity() < new_capacity) {
				create_copy_from(move_iterator_type(m_impl.start()), move_iterator_type(m_impl.finish()), new_capacity);
			}
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
			//Если размер меньше, то просто обрезаем конец вектора:
			if (new_size <= size()) {
				crop(new_size);
			}
			else {
				//Резервируем достаточное количество памяти
				reserve(new_size);
				//Заполняем конец вектора
				m_impl.set_finish(uninitialized_fill_n(m_impl.finish(), new_size - size(), value));
			}
		}
		void push_back(const_reference value)
		{
			add_to_back(value);
		}

		void push_back(rvalue_reference value)
		{
			add_to_back(std::move(value));
		}

		template<typename ... Args>
		reference emplace_back(Args && ...args)
		{
			add_to_back(std::forward<Args>(args)...);
			return back();
		}

		void pop_back()
		{
			m_impl.set_finish(m_impl.finish() - 1);
			m_impl.destroy(m_impl.finish());
		}
		iterator erase(const_iterator first) noexcept(std::is_nothrow_assignable<value_type, value_type>::value)
		{
			return erase(first, first + 1);
		}
		iterator erase(const_iterator first, const_iterator last) noexcept(std::is_nothrow_assignable<value_type, value_type>::value)
		{
			iterator destination = begin() + ::std::distance(cbegin(), first);
			iterator new_end = ::std::copy(last, cend(), destination);
			crop(static_cast<size_type>(new_end - begin()));
			return new_end;
		}
		void clear() noexcept
		{
			crop(0);
		}

		template<typename InputIterator>
		typename std::enable_if<::example::is_iterator<InputIterator>::value, iterator>::type
			insert(const_iterator insert_position, InputIterator first, InputIterator last)
		{
			size_type insert_index = static_cast<size_type>(::std::distance(cbegin(), insert_position));
			size_type old_size = m_impl.size();
			try {
				while (first != last) {
					push_back(*first);
					++first;
				}
				::std::rotate(m_impl.start() + insert_index, m_impl.start() + old_size, m_impl.finish());
			}
			catch (...) {
				crop(old_size);
				throw;
			}
			return iterator(m_impl.start() + static_cast<difference_type>(insert_index));
		}

		iterator insert(const_iterator insert_position, T const & value)
		{
			return insert(insert_position, std::addressof(value), std::addressof(value) + 1);
		}

		iterator insert(const_iterator insert_position, size_type count, T const & value)
		{
			size_type insert_index = static_cast<size_type>(::std::distance(cbegin(), insert_position));
			size_type old_size = m_impl.size();
			try {
				reserve(m_impl.size() + count);
				m_impl.set_finish(::std::uninitialized_fill_n(m_impl.finish(), count, value));
				::std::rotate(m_impl.start() + insert_index, m_impl.start() + old_size, m_impl.finish());
			}
			catch (...) {
				crop(old_size);
				throw;
			}
			return iterator(m_impl.start() + static_cast<difference_type>(insert_index));
		}

		template<typename ... Args>
		iterator emplace(const_iterator insert_position, Args && ... args)
		{
			using move_iterator_type = typename std::conditional<
				std::is_copy_constructible<T>::value && !std::is_nothrow_move_constructible<T>::value,
				pointer,
				std::move_iterator<pointer>
			>::type;
			//Если вставка в конец, то просто используем add_to_back
			if (insert_position == cend()) {
				add_to_back(std::forward<Args>(args)...);
				return end() - 1;
			}
			size_type insert_index = static_cast<size_type>(::std::distance(cbegin(), insert_position));
			if (m_impl.size() == m_impl.capacity()) {
				//Если требуется расширение, то создаем новый vector_data,
				//сразу инициализируя элементы, копируя (перемещая), элементы до позиции вставки
				vector_data new_data = create_initialized_storage(
					move_iterator_type(m_impl.start()),
					move_iterator_type(m_impl.start() + insert_index),
					get_next_size(m_impl.size())
				);
				try {
					//Создаем новый элемент, передавая в конструктор переданный в emplace аргументы
					::example::construct_at(new_data.finish(), std::forward<Args>(args)...);
					new_data.set_finish(new_data.finish() + 1);
					//Копируем оставшиеся элементы в конец новой последовательности
					new_data.set_finish(::std::uninitialized_copy(
						move_iterator_type(m_impl.start() + insert_index),
						move_iterator_type(m_impl.finish()),
						new_data.finish())
					);
					//Уничтожаем старые элементы и присваиваем новый vactor_data нашему vector_impl
					::example::destroy(m_impl.start(), m_impl.finish());
					m_impl = new_data;
				}
				catch (...) {
					::example::destroy(new_data.start(), new_data.finish());
					m_impl.destroy_storage(new_data);
					throw;
				}
			}
			else {
				//Если расширение не требуется
				pointer insert_pointer = m_impl.start() + insert_index;
				//Создаем в конце новый элемент, копируя (перемещая) последний элемент вектора
				::example::construct_at(m_impl.finish(), std::move(*(m_impl.finish() - 1)));
				m_impl.set_finish(m_impl.finish() + 1);
				//Сдвигаем оставшиеся элементы вправо
				::std::copy_backward(
					move_iterator_type(insert_pointer),
					move_iterator_type(m_impl.finish() - 2),
					m_impl.finish() - 1
				);
				//Уничтожаем элемент в позиции вставки
				::example::destroy_at(insert_pointer);
				//Создаем новый элемент в позиции вставки
				::example::construct_at(insert_pointer, std::forward<Args>(args)...);
			}
			return m_impl.start() + insert_index;
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
		const_iterator end() const noexcept {
			return m_impl.finish();
		}
		const_iterator cend() const noexcept
		{
			return end();
		}
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
				return static_cast<size_type>(m_finish - m_start);//Разница между указателем на конец и начало
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

			//Взаимная замена данных
			void swap_data(vector_data & src) noexcept
			{
				//Меняем внутренние данные местами
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
				//Максимальное количество элементов для которых мы можем корректно отслеживать размер контейнера
				return static_cast<size_type>(
					std::numeric_limits<difference_type>::max() / sizeof(value_type) - sizeof(value_type)
					);
			}

			size_type capacity() const noexcept
			{
				return static_cast<size_type>(m_end_of_storage - m_start);
			}

			pointer m_start;//Начало вектора
			pointer m_finish;//Конец вектора
			pointer m_end_of_storage;//Конец выделенной памяти
		};//struct vector_data

		struct vector_impl : allocator_type, vector_data //Если аллокатор пустой, то размер объекта не увеличится
		{
			vector_impl()//Инициализируем объект в "нулевое" состояние
			{
			}

			vector_impl(size_type size) ://Делегирующий конструктор
				vector_impl() //Инициализируем объект в "нулевое" состояние
			{
				create_storage(size);//Создаем буфер для хранения size элементов
			}

			~vector_impl()
			{
				destroy_storage();//Освобождаем буфер
			}


			void set_finish(pointer p) noexcept
			{
				vector_data::m_finish = p;
			}

			//Создание буфера для заданного количества элементов
			void create_storage(size_type size)
			{
				if (size == 0) {
					//Переводим объект в "нулевое" состояние
					vector_data::set_null();
				}
				else {
					vector_data::m_start = allocator_type::allocate(size);//Создаем буфер для хранения size элементов
					//Настраиваем конец вектора и окнец буфера
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
			//Уничтожение буфера
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

		size_type get_next_size(size_type old_size) const noexcept
		{
			return static_cast<size_type>(old_size * 1.6) + 1u;
		}

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

		template<typename Iterator>
		void create_copy_from(Iterator start, Iterator finish, size_type new_capacity)
		{
			vector_data new_data = create_initialized_storage(start, finish, new_capacity);
			destroy(m_impl.start(), m_impl.finish());
			m_impl = new_data;
		}

		template<typename Iterator>
		typename std::enable_if<::example::minimal_iterator_category<
			std::forward_iterator_tag,
			Iterator
		>,
			void
		>::type
			create_copy_from(Iterator start, Iterator finish)
		{
			size_type new_capacity = static_cast<size_type>(::std::distance(start, finish));
			create_copy_from(start, finish, new_capacity);
		}

		template<typename Iterator>
		typename std::enable_if<!::example::minimal_iterator_category<
			std::forward_iterator_tag,
			Iterator
		>,
			void
		>::type
			create_copy_from(Iterator start, Iterator finish)
		{
			clear();
			while (start != finish) {
				push_back(*start);
			}
		}

		template<typename Iterator>
		vector_data create_initialized_storage(Iterator start, Iterator finish, size_type new_capacity)
		{
			vector_data result;
			if (new_capacity) {
				result = reserve_storage(new_capacity);
				try {
					result.set_finish(::std::uninitialized_copy(start, finish, result.start()));
				}
				catch (...) {
					m_impl.destroy_storage(result);
					throw;
				}
			}
			return result;
		}

		template<typename ... Args>
		void add_to_back(Args && ... args)
		{
			if (m_impl.size() == m_impl.capacity()) {
				reserve(get_next_size(m_impl.size()));
			}
			::example::construct_at(m_impl.finish(), std::forward<Args>(args)...);
			m_impl.set_finish(m_impl.finish() + 1);
		}

		vector_impl m_impl;
	};//class vector

}//namespace example