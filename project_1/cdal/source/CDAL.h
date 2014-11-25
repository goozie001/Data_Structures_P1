#ifndef _CDAL_H_
#define _CDAL_H_

#include <iostream>
#include <stdexcept>

namespace COP3530 {
	template <typename T> class CDAL
	{
	private:
		struct Node{
			T* mArray;
			Node* next;

			Node() {
				mArray = new T[50]();
				next = NULL;
			}
			Node(T* data) {
				this->mArray = data;
				this->next = NULL;
			}
			~Node() {
                if (next != NULL) {
                    delete next;
                    next = NULL;
                }
                if (mArray != NULL) {
                    delete[] mArray;
                    mArray = NULL;
                }
			}
			void clearArray() {
				delete[] mArray;
				mArray = new T[50]();
			}
		};
		Node* head;
		size_t arraySize;
		int usedNodes;

		int totalNodes() {
			int i = 0;
			Node* temp = head;
			while (temp != NULL) {
				temp = temp->next;
				++i;
			}
			return i;
		}

		void addArrayNode() {
			if (arraySize == (totalNodes() * 50 + 1)) {
				Node* temp = head;
				while (temp->next != NULL) {
					temp = temp->next;
				}
				temp->next = new Node();
			}
		}

		// Check for and delete unused arrays if there are too many.
		void cutTheFat() {
			if (totalNodes() / 2 > usedNodes) {
				Node* temp = head;
				for (int i = 0; i < usedNodes; ++i) {
					temp = temp->next;
				}
				Node* next = temp->next;
				delete next;
			}
		}

	public:

		class CDAL_Iter
		{
		public:
			typedef T value_type;
			typedef std::ptrdiff_t difference_type;
			typedef T& reference;
			typedef T* pointer;
			typedef std::forward_iterator_tag iterator_category;

			typedef CDAL_Iter self_type;
			typedef CDAL_Iter& self_reference;

		private:
			Node* current;
			int total;
            const CDAL* ref;

		public:
			explicit CDAL_Iter(Node* current = NULL, int pos = 0, const CDAL* src = NULL) : current(current), total(pos), ref(src) {}
			CDAL_Iter(const CDAL_Iter& src) : current(src.current), total(src.total), ref(src.ref) {}

			reference operator*() const {
                if (total == ref->arraySize)
                    throw std::out_of_range("The position of the iterator is out of range of its list.");
				int pos = total % 50;
				return current->mArray[pos];
			}
			pointer operator->() const {
                if (total == ref->arraySize)
                    throw std::out_of_range("The position of the iterator is out of range of its list.");
				int pos = total % 50;
				return &current->mArray[pos];
			}

			self_reference operator=(const CDAL_Iter& src) {
				if (*this == src)
					return *this;

				delete this;
				return *new CDAL_Iter(src);
			}

			self_reference operator++() {
                if (total == ref->arraySize)
                    throw std::out_of_range("The position of the iterator is out of range of its list.");
                int pos = total % 50;
				++total;
				pos = total % 50;
				if (total == 0)
					current = current->next;
				return *this;
			} // preincrement

			self_type operator++(int) {
                if (total == ref->arraySize)
                    throw std::out_of_range("The position of the iterator is out of range of its list.");
                int pos = total % 50;
				CDAL_Iter* results = new CDAL_Iter(*this);
				++total;
				pos = total % 50;
				if (total == 0)
					current = current->next;
				return *results;
			} // postincrement

			bool operator==(const CDAL_Iter& rhs) const {
				return (current == rhs.current && total == rhs.total);
			}

			bool operator!=(const CDAL_Iter& rhs) const {
				return (current != rhs.current || total != rhs.total);
			}
		};

        class CDAL_Const_Iter
        {
        public:
            typedef T value_type;
            typedef std::ptrdiff_t difference_type;
            typedef const T& reference;
            typedef const T* pointer;
            typedef std::forward_iterator_tag iterator_category;

            typedef CDAL_Const_Iter self_type;
            typedef CDAL_Const_Iter& self_reference;

        private:
            const Node* current;
            int total;
            const CDAL* ref;

        public:
            explicit CDAL_Const_Iter(Node* current = NULL, int pos = 0, const CDAL* src = NULL) : current(current), total(pos), ref(src) {}
            CDAL_Const_Iter(const CDAL_Const_Iter& src) : current(src.current), total(src.total), ref(src.ref) {}

            reference operator*() const {
                if (total == ref->arraySize)
                    throw std::out_of_range("The position of the iterator is out of range of its list.");
                int pos = total % 50;
                return current->mArray[pos];
            }
            pointer operator->() const {
                if (total == ref->arraySize)
                    throw std::out_of_range("The position of the iterator is out of range of its list.");
                int pos = total % 50;
                return &current->mArray[pos];
            }

            self_reference operator=(const CDAL_Const_Iter& src) {
                if (*this == src)
                    return *this;

                delete this;
                return *new CDAL_Const_Iter(src);
            }

            self_reference operator++() {
                if (total == ref->arraySize)
                    throw std::out_of_range("The position of the iterator is out of range of its list.");
                int pos = total % 50;
                ++total;
                pos = total % 50;
                if (total == 0)
                    current = current->next;
                return *this;
            } // preincrement

            self_type operator++(int) {
                if (total == ref->arraySize)
                    throw std::out_of_range("The position of the iterator is out of range of its list.");
                int pos = total % 50;
                CDAL_Const_Iter* results = new CDAL_Const_Iter(*this);
                ++total;
                pos = total % 50;
                if (total == 0)
                    current = current->next;
                return *results;
            } // postincrement

            bool operator==(const CDAL_Const_Iter& rhs) const {
                return (current == rhs.current && total == rhs.total);
            }

            bool operator!=(const CDAL_Const_Iter& rhs) const {
                return (current != rhs.current || total != rhs.total);
            }
        };

		//--------------------------------------------------
		// types
		//--------------------------------------------------
		//typedef std::size_t size_t;
		typedef T value_type;
		typedef CDAL_Iter iterator;
		typedef CDAL_Const_Iter const_iterator;

		CDAL() {
			this->head = new Node();
			this->arraySize = 0;
			this->usedNodes = 0;
		}

		CDAL(const CDAL& copy) {
			this->arraySize = copy.arraySize;
			this->head = new Node();
            Node* temp = head;
            Node* foreign = copy.head;
            int prevOut = 0;
            for (int i = 0; i < arraySize; ++i) {
                int in = i % 50;
                int out = i / 50;
                if (out > prevOut) {
                    ++prevOut;
                    temp->next = new Node();
                    temp = temp->next;
                    foreign = foreign->next;
                }
                temp->mArray[in] = foreign->mArray[in];
            }
			this->usedNodes = copy.usedNodes;
		}

		~CDAL() {
			delete head;
			head = NULL;
		}

		CDAL& operator=(const CDAL& src) {
			if (&src == this)
				return *this;

			clear();
			delete this;
			return new CDAL(src);
		}

        T& operator[](int pos) {
            if (pos >= arraySize || pos < 0)
                throw std::domain_error("Location is out of range of this list");
            int in = pos % 50;
            int out = (int)(pos / 50);
            Node* temp = head;
            for (int i = 0; i < out; ++i) {
                temp = temp->next;
            }
            return temp->mArray[in];
        }

        T const& operator[](int pos) const {
            if (pos >= arraySize || pos < 0)
                throw std::domain_error("Location is out of range of this list");
            int in = pos % 50;
            int out = (int)(pos / 50);
            Node* temp = head;
            for (int i = 0; i < out; ++i) {
                temp = temp->next;
            }
            return temp->mArray[in];
        }

		T replace(const T& ele, int pos) {
            if (pos < 0 || pos >= arraySize)
                throw std::domain_error("Location is out of range of this list");
			int insidePos = pos % 50;
			int outsidePos = (int)(pos / 50);
			Node* temp = head;
			for (int i = 0; i < outsidePos; ++i) {
				temp = temp->next;
			}
			T obj = temp->mArray[insidePos];
			temp->mArray[insidePos] = ele;
		}

		void push_back(const T& ele) {
			++arraySize;
			if (arraySize % 50 == 1)
				++usedNodes;
			addArrayNode();
			int in = (arraySize - 1) % 50;
			int out = (int)((arraySize - 1) / 50);
			Node* temp = head;
			for (int i = 0; i < out; ++i) {
				temp = temp->next;
			}
			temp->mArray[in] = ele;
		}

		void insert(const T& ele, int pos) {
			if (pos == arraySize) {
				push_back(ele);
			}
            else if (pos > arraySize || pos < 0)
                throw std::domain_error("Postition out of range");
			else {
				++arraySize;
				if (arraySize % 50 == 1) {
					++usedNodes;
					addArrayNode();
				}
				int in = pos % 50;
				int out = (int)(pos / 50);
				Node* temp = head;
				for (int i = 0; i < out; ++i) {
					temp = temp->next;
				}
				T current = temp->mArray[in];
				temp->mArray[in] = ele;
				T moved;
				++pos;
				for (pos; pos < arraySize; ++pos) {
					in = pos % 50;
					int newOut = (int)(pos / 50);
					if (newOut > out) {
						temp = temp->next;
						++out;
					}
					moved = temp->mArray[in];
					temp->mArray[in] = current;
					current = moved;
				}
			}
		}

		void push_front(const T& ele) {
			insert(ele, 0); // There would be no optimization for implementing
			// the exact same thing as insert(T&, int) a second time.
		}

		T remove(int pos) {
			T obj;
            if (pos >= arraySize || pos < 0)
                throw std::domain_error("Location is out of range of this list");
			if (pos == (--arraySize))
				obj = pop_back();
			else {
				int in = pos % 50;
				int out = (int)(pos / 50);
				Node* temp = head;
				for (int i = 0; i < out; ++i) {
					temp = temp->next;
				}
				obj = temp->mArray[pos];
				for (pos; pos < arraySize; ++pos) {
					in = pos % 50;
					if (in == 49) { // Last element in the array needs to get the
									// first element of the next array.
						temp->mArray[in] = temp->next->mArray[0];
						temp = temp->next;
						if (pos == (arraySize - 1)) {
							--usedNodes;
						}
					}
					else {
						temp->mArray[in] = temp->mArray[in + 1];
					}
				}
				if (in == 49) {
					cutTheFat();
				}
			}
			return obj;
		}

		T pop_back() {
            if (arraySize == 0)
                throw std::length_error("Cannot pop an empty list.");
			int in = --arraySize % 50;
			int out = (int)(arraySize / 50);
			Node* temp = head;
			for (int i = 0; i < out; ++i) {
				temp = temp->next;
			}
			T obj = temp->mArray[in];
            cutTheFat();
			return obj;
		}

		T pop_front() {
            if (arraySize == 0)
                throw std::length_error("Cannot pop an empty list.");
			remove(0);	// Similar to push_front, the implementation would only
			// optimize a single if statement, and would otherwise be
			// an exact repeat of the implementation of remove.
		}

		T item_at(int pos) {
            if (pos >= arraySize || pos < 0)
                throw std::domain_error("Location is out of range of this list");
			int in = pos % 50;
			int out = (int)(pos / 50);
			Node* temp = head;
			for (int i = 0; i < out; ++i) {
				temp = temp->next;
			}
			return temp->mArray[in];
		}

		bool is_empty() {
			return arraySize == 0;
		}

		size_t size() {
			return arraySize;
		}

		void clear() {
			cutTheFat();
			Node* temp = head;
			while (temp != NULL) {
				temp->clearArray();
				temp = temp->next;
			}
            arraySize = 0;
		}

		bool contains(const T& ele, bool equals(const T& a, const T& b)) {
            Node* temp = head;
            for (int i = 0; i < arraySize; ++i) {
                int in = i % 50;
                if (in == 0 && i != 0) {
                    temp = temp->next;
                }
                if (equals(ele, temp->mArray[in]))
                    return true;
            }
			return false;
		}

		std::ostream& print(std::ostream& out) const {
			int in;
			Node* temp = head;
			for (int i = 0; i < arraySize; ++i) {
				in = i % 50;
				if (in == 0 && i != 0) {
					temp = temp->next;
				}
				out << temp->mArray[in] << std::endl;
			}
			return out;
		}

		iterator begin() { return CDAL_Iter(head, 0, this); }
		iterator end() { return CDAL_Iter(head, arraySize, this); }

		const_iterator begin() const { return CDAL_Const_Iter(head, 0, this); }
		const_iterator end() const { return CDAL_Const_Iter(head, arraySize, this); }
	};
}
#endif