#include <memory>
#include <limits>
#include <iterator>
#include <type_traits>

namespace NVector
{
	template<typename T, typename Alloc = std::allocator<T>>
	class TVector
	{
	public:
		using allocator_type = typename Alloc::template rebind<T>::other;// Новый тип аллокатор
		using value_type = typename allocator_type::value_type;// Тип хранящихся элементов
		using reference = typename allocator_type::reference;// Ссылка на тип хранящихся элементов
		using const_reference = typename allocator_type::const_reference;// Такаяя же но const
		using pointer = typename allocator_type::pointer;// Указатель на тип 
		using const_pointer = typename allocator_type::const_pointer;
		using size_type = typename allocator_type::size_type;// Тип для размера и индексов 
		using difference_type = typename allocator_type::difference_type;// Тип для разницы указателей
		
		// Конструкторы

		TVector() noexcept :
			m_impl()
		{
		}

		TVector(TVector const & src)
			: m_impl(src.Size())
		{
			m_impl.Set_finish(Uninitialized_copy(src.m_impl.Start(), src.m_impl.Finish(), m_impl.Start()));
		}

		
		~TVector()
		{
			Clear();
		}

		size_type Max_size() const noexcept
		{
			return m_impl.Max_size();
		}

		size_type Size() const noexcept
		{
			return m_impl.Size();
		}

		bool Empty() const noexcept
		{
			return m_impl.Empty();
		}

		size_type Capacity() const noexcept
		{
			return m_impl.Capacity();
		}

		pointer Data() noexcept
		{
			return m_impl.Start();
		}

		const_pointer Data() const noexcept
		{
			return m_impl.Start();
		}

		reference operator[](size_type index) noexcept
		{
			return m_impl.Start()[index];
		}

		const_reference operator[](size_type index) const noexcept
		{
			return m_impl.Start()[index];
		}

		reference Front() noexcept
		{
			return *m_impl.Start();
		}

		const_reference Front() const noexcept
		{
			return *m_impl.Start();
		}

		reference Back() noexcept
		{
			return *(m_impl.Finish() - 1);
		}

		const_reference Back() const noexcept
		{
			return *(m_impl.Finish() - 1);
		}

		void Swap(TVector & src) noexcept
		{
			m_impl.Swap_data(src.m_impl);
		}

		bool Reserve(size_type new_capacity)
		{
			if (Capacity() < new_capacity) {
				if(!Create_copy_from(m_impl.Start(), m_impl.Finish(), new_capacity)){
					return false;
				}
			}
			return true;
		}

		void Resize(size_type new_size)
		{
			if (new_size <= Size()) {
				Crop(new_size);
			}
			else {
				Resize(new_size, value_type{});
			}
		}

		void Resize(size_type new_size, const_reference value)
		{
			
			if (new_size <= Size()) {
				Crop(new_size);
			}
			else {
				Reserve(new_size);
				// Заполняем конец
				m_impl.Set_finish(Uninitialized_fill_n(m_impl.Finish(), new_size - Size(), value));
			}
		}

		bool Push_back(const_reference value) // bool для корректного выхода // 12 тест
		{
			size_type current_size = m_impl.Size();
			if (!(current_size < m_impl.Capacity())) {
				size_type new_size = static_cast<size_type>(current_size * 1.6);
				if (new_size == current_size) {
					++new_size;
				}
				if(!Reserve(new_size)){
					return false;
				}
			}
			m_impl.Set_finish(Uninitialized_fill_n(m_impl.Finish(), 1, value));
			return true;
		}

		void Pop_back()
		{
			m_impl.Set_finish(m_impl.Finish() - 1);
			m_impl.Destroy(m_impl.Finish());
		}


		void Clear() noexcept
		{
			Crop(0);
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

			size_type Size() const noexcept
			{
				return static_cast<size_type>(m_finish - m_start);//������� ����� ���������� �� ����� � ������
			}

			pointer Start() noexcept
			{
				return m_start;
			}

			const_pointer Start() const noexcept
			{
				return m_start;
			}

			pointer Finish() noexcept
			{
				return m_finish;
			}

			const_pointer Finish() const noexcept
			{
				return m_finish;
			}

			bool Is_null() const noexcept
			{
				return m_start == nullptr;
			}

			void Set_finish(pointer p) noexcept
			{
				m_finish = p;
			}

			//�������� ������ ������
			void Swap_data(vector_data & src) noexcept
			{
				//������ ���������� ������ �������
				std::swap(m_start, src.m_start);
				std::swap(m_finish, src.m_finish);
				std::swap(m_end_of_storage, src.m_end_of_storage);
			}
			void Set_null() noexcept
			{
				m_start = m_finish = m_end_of_storage = nullptr;
			}

			bool Empty() const noexcept
			{
				return m_start == m_finish;
			}

			constexpr size_type Max_capacity() const noexcept
			{
				//������������ ���������� ��������� ��� ������� �� ����� ��������� ����������� ������ ����������
				return static_cast<size_type>(
					std::numeric_limits<difference_type>::max() / sizeof(value_type) - sizeof(value_type)
					);
			}

			size_type Capacity() const noexcept
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
				Create_storage(size);//������� ����� ��� �������� size ���������
			}

			~vector_impl()
			{
				Destroy_storage();//����������� �����
			}


			void Set_finish(pointer p) noexcept
			{
				vector_data::m_finish = p;
			}

			//�������� ������ ��� ��������� ���������� ���������
			void Create_storage(size_type size)
			{
				if (size == 0) {
					//��������� ������ � "�������" ���������
					vector_data::Set_null();
				}
				else {
					vector_data::m_start = allocator_type::allocate(size);//������� ����� ��� �������� size ���������
					//����������� ����� ������� � ����� ������
					vector_data::m_finish = vector_data::m_start;
					vector_data::m_end_of_storage = vector_data::m_start + size;
				}
			}

			void Create_storage_to(vector_data & data, size_type size)
			{
				if (size == 0) {
					data.Set_null();
				}
				else {
					data.m_start = allocator_type::allocate(size);
					data.m_finish = data.m_start;
					data.m_end_of_storage = data.m_start + size;
				}
			}
			//����������� ������
			void Destroy_storage()
			{
				if (!vector_data::Is_null()) {
					Destroy_storage(*this);
				}
			}

			void Destroy_storage(vector_data & data)
			{
				allocator_type::deallocate(data.m_start, data.Size());
				data.Set_null();
			}

			vector_impl &operator=(vector_data const & data) noexcept
			{
				Destroy_storage();
				static_cast<vector_data&>(*this) = data;
				return *this;
			}
		};//struct vector_impl

		vector_data Reserve_storage(size_type new_capacity)
		{
			vector_data data;
			if (m_impl.Max_capacity() < new_capacity) {
				throw std::length_error("vector::reserve: invalid size");
			}
			if (m_impl.Capacity() < new_capacity) {
				m_impl.Create_storage_to(data, new_capacity);
			}
			return data;
		}

		void Crop(size_type new_size)
		{
			Destroy(m_impl.Start() + new_size, m_impl.Finish());
			m_impl.Set_finish(m_impl.Start() + new_size);
		}

		bool Create_copy_from(const_pointer start, const_pointer finish, size_type new_capacity)
		{
			vector_data new_data = Create_initialized_storage(start, finish, new_capacity);
			Destroy(m_impl.Start(), m_impl.Finish());
			if(new_data.my_is_null == true){
				return false;
			} else{
				m_impl = new_data;	
			}
			return true;
		}


		vector_data Create_initialized_storage(const_pointer start, const_pointer finish, size_type new_capacity)
		{
			vector_data result;
			if (new_capacity) {
				result = Reserve_storage(new_capacity);
				try {
					result.Set_finish(Uninitialized_copy(start, finish, result.Start()));
				}
				catch (...) {
					m_impl.Destroy_storage(result);
					result.my_is_null = true;
					return result;
				}
			}
			return result;
		}


		pointer Uninitialized_copy(const_pointer start, const_pointer finish, pointer destination)
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
				Destroy(destination, current);
				throw;//����������� ������� ���������� �����
			}
			//������� �� ��������, ������� �� ����� ������ ����� return current
		}

		pointer Uninitialized_fill(pointer start, pointer finish, const_reference source)
		{
			pointer current = start;
			try {
				for (; current != finish; ++current) {//������� �������� � ��������� ������� ������
					m_impl.construct(current, source);
				}
				return current;
			}
			catch (...) {
				Destroy(start, current);
				throw;
			}
		}

		pointer Uninitialized_fill_n(pointer start, size_type n, const_reference source)
		{
			return Uninitialized_fill(start, start + n, source);
		}


		void Destroy(pointer start, pointer finish) noexcept
		{
			while (start != finish) {
				m_impl.destroy(--finish);
			}
		}

		vector_impl m_impl;
	};//class TVector

}//namespace NVector

