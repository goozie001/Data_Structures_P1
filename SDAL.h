#ifndef _SDAL_H_
#define _SDAL_H_

#include <iostream>

namespace COP3530 {
	template <typename T> class SDAL
	{
	private:
		T* myArray;
		int tail;
		size_t arraySize;

		void increaseArraySize() {
			if (tail == (arraySize - 1)) {
				T* oldArray = myArray;
				int newSize = (int)(arraySize * 1.5);
				myArray = new T[newSize]();
				for (int i = 0; i < arraySize; ++i) {
					myArray[i] = oldArray[i];
				}
				arraySize = newSize;
				delete[] oldArray;
			}
		}

		void decreaseArraySize() {
			if (arraySize > 99 && tail < (arraySize/ 2)) {
				T* oldArray = myArray;
				int newSize = arraySize / 2;
				myArray = new T[newSize]();
				for (int i = 0; i < tail + 1; ++i) {
					myArray[i] = oldArray[i];
				}
				arraySize = newSize;
				delete[] oldArray;
			}
		}

	public:

		class SDAL_Iter
		{
		public:
			typedef T value_type;
			typedef std::ptrdiff_t difference_type;
			typedef T& reference;
			typedef T* pointer;
			typedef std::forward_iterator_tag iterator_category;

			typedef SDAL_Iter self_type;
			typedef SDAL_Iter& self_reference;

		private:
			T* myArray;
			int pos;
            const SDAL* ref;

		public:
			explicit SDAL_Iter(T* backArray = NULL, int pos = NULL, const SDAL* src = NULL) : myArray(backArray), pos(pos), ref(src) {}
			SDAL_Iter(const SDAL_Iter& src) : myArray(src.myArray), pos(src.pos), ref(src.ref) {}

			reference operator*() const {
                if (pos > ref->tail)
                    throw std::out_of_range("The iterator has reached the end of the list");
				return myArray[pos];
			}
			pointer operator->() const {
                if (pos > ref->tail)
                    throw std::out_of_range("The iterator has reached the end of the list");
				return &myArray[pos];
			}

            self_reference operator++() {
                if (pos > ref->tail)
                    throw std::out_of_range("The iterator has already reached the end of the list");
                ++pos;
                return *this;
            } // preincrement

			self_type operator++(int) {
                if (pos > ref->tail)
                    throw std::out_of_range("The iterator has already reached the end of the list");
				SDAL_Iter* results = new SDAL_Iter(*this);
				++pos;
				return *results;
			} // postincrement

			bool operator==(const SDAL_Iter& rhs) const {
				return (myArray == rhs.myArray && pos == rhs.pos);
			}

			bool operator!=(const SDAL_Iter& rhs) const {
				return (myArray != rhs.myArray || pos != rhs.pos);
			}
		};

		class SDAL_Const_Iter
		{
		public:
			typedef T value_type;
			typedef std::ptrdiff_t difference_type;
			typedef const T& reference;
			typedef const T* pointer;
			typedef std::forward_iterator_tag iterator_category;

			typedef SDAL_Const_Iter self_type;
			typedef SDAL_Const_Iter& self_reference;

		private:
			const T* myArray;
			int pos;
            const SDAL* ref;

		public:
			explicit SDAL_Const_Iter(T* backArray = NULL, int start = NULL, const SDAL* src = NULL) : myArray(backArray), pos(start), ref(src) {}
			SDAL_Const_Iter(const SDAL_Const_Iter& src) : myArray(src.myArray), pos(src.pos), ref(src.ref) {}

			reference operator*() const {
                if (pos > ref->tail)
                    throw std::out_of_range("The iterator has reached the end of the list");
				return myArray[pos];
			}

			pointer operator->() const {
                if (pos > ref->tail)
                    throw std::out_of_range("The iterator has reached the end of the list");
				return &myArray[pos];
			}

			self_reference operator=(const SDAL_Const_Iter& src) {
				if (*this == src)
					return *this;

				delete this;
				return new SDAL_Const_Iter(src);
			}

			self_reference operator++() {
                if (pos > ref->tail)
                    throw std::out_of_range("The iterator has already reached the end of the list");
				++pos;
				return *this;
			} // preincrement

			self_type operator++(int) {
                if (pos > ref->tail)
                    throw std::out_of_range("The iterator has already reached the end of the list");
				SDAL_Const_Iter* results = new SDAL_Const_Iter(*this);
				++pos;
				return *results;
			} // postincrement

			bool operator==(const SDAL_Const_Iter& rhs) const {
				return (myArray == rhs.myArray && pos == rhs.pos);
			}

			bool operator!=(const SDAL_Const_Iter& rhs) const {
				return (myArray != rhs.myArray || pos != rhs.pos);
			}
		};

		//--------------------------------------------------
		// types
		//--------------------------------------------------
		//typedef std::size_t size_t;
		typedef T value_type;
		typedef SDAL_Iter iterator;
		typedef SDAL_Const_Iter const_iterator;

		SDAL() {
			this->arraySize = 50;
			myArray = new T[arraySize]();
			tail = -1;
		}

		SDAL(int size) {
			myArray = new T[size]();
			tail = -1;
			this->arraySize= size;
		}

		SDAL(const SDAL& copy) {
			this->arraySize = copy.arraySize;
            myArray = new T[arraySize]();
            for (int i = 0; i < arraySize; ++i) {
                myArray[i] = copy.myArray[i];
            }
			this->tail = copy.tail;
		}

		~SDAL() {
			delete[] myArray;
		}

		SDAL& operator=(const SDAL& src) {
			if (&src == this)
				return *this;

			clear();
			delete this;
			return new SDAL(src);
		}

        T& operator[](int pos) {
            if (pos > tail || pos < 0)
                throw std::domain_error("Location is out of range of this list");
            return myArray[pos];
        }

        T const& operator[](int pos) const {
            if (pos > tail || pos < 0)
                throw std::domain_error("Location is out of range of this list");
            return myArray[pos];
        }

		T replace(const T& ele, int pos) {
			T oldEle = myArray[pos];
			myArray[pos] = ele;
			return oldEle;
		}

		void push_back(const T& ele) {
			increaseArraySize();
			myArray[++tail] = ele;
		}

		void insert(const T& ele, int pos) {
            if (pos > tail + 1 || pos < 0)
                throw std::domain_error("Desired location is not within the bounds of the list");
			increaseArraySize();
			if (pos == tail + 1) {
				push_back(ele);
			}
			else {
				T move = myArray[pos];
				T temp;
				myArray[pos] = ele;
				++pos; ++tail;
				for (pos; pos < (tail + 1); ++pos) {
					temp = myArray[pos];
					myArray[pos] = move;
					move = temp;
				}
			}
		}

		void push_front(const T& ele) {
			insert(ele, 0); // There would be no optimization for implementing
							// the exact same thing as insert(T&, int) a second time.
		}

		T remove(int pos) {
            if (pos > tail || pos < 0)
                throw std::domain_error("Desired location is not within the bounds of the list");
			T obj;
			if (pos == tail) {
				obj = pop_back();
			}
			else {
				obj = myArray[pos];
				for (pos; pos < tail; ++pos) {
					myArray[pos] = myArray[pos + 1];
				}
				--tail;
			}
			decreaseArraySize();
			return obj;
		}

		T pop_back() {
            if (tail < 0)
                throw std::length_error("The list is empty");
			T obj = myArray[tail];
			--tail;
			decreaseArraySize();
			return obj;
		}

		T pop_front() {
            if (tail < 0)
                throw std::length_error("The list is empty");
			remove(0);	// Similar to push_front, the implementation would only
						// optimize a single if statement, and would otherwise be
						// an exact repeat of the implementation of remove.
		}

		T item_at(int pos) {
            if (pos > tail || pos < 0)
                throw std::domain_error("Desired location is not within the bounds of the list");
			return myArray[pos];
		}

		bool is_empty() {
			return (tail == -1);
		}

		size_t size() {
			return (size_t)(tail + 1);
		}

		void clear() {
			delete[] myArray;
			myArray = new T[arraySize]();
			tail = -1;
			decreaseArraySize();
		}

		bool contains(const T& ele, bool equals(const T& a, const T& b)) {
			for (int i = 0; i < tail + 1; ++i) {
				if (equals(ele, myArray[i]))
					return true;
			}
			return false;
		}

		std::ostream& print(std::ostream& out) const {
			if (tail != -1) {
				for (int i = 0; i <= tail; ++i) {
					out << myArray[i] << std::endl;
				}
			}
			return out;
		}

		iterator begin() { return SDAL_Iter(myArray, 0, this); }
		iterator end() { return SDAL_Iter(myArray, tail + 1, this); }

		const_iterator begin() const { return SDAL_Const_Iter(myArray, 0, this); }
		const_iterator end() const { return SDAL_Const_Iter(myArray, tail + 1, this); }
	};
}
#endif