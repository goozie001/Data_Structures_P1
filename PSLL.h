#ifndef _PSLL_H_
#define _PSLL_H_

#include <iostream>

namespace COP3530 {
	template <typename T> class PSLL
	{
	private:
		struct Node {
			T data;
			Node* next;

			Node() {
				next = NULL;
			}
			Node(const T& data) {
				this->data = data;
				this->next = NULL;
			}
			Node(const T& data, const T& next) {
				this->data = data;
				this->next = &next;
			}
			~Node() {
                if (next != NULL)
				    next = NULL;
			}
		};
		Node* head;
		Node* tail;
		Node* free;
        size_t listSize;

        void clearAll() {
            clear();
            if (free == NULL) return;
            Node* temp = free;
            Node* next = free->next;
            while (temp != NULL) {
                delete temp;
                temp = NULL;
                temp = next;
                if (temp != NULL)
                    next = temp->next;
            }
            head = NULL;
            tail = NULL;
            free = NULL;
            listSize = 0;
        }



        Node* createNode(const T& ele) {
            Node* myNode;
            if (free != NULL) { // There is a node to use.
                myNode = free;
                free = free->next;
            }
            else {
                myNode = new Node();
            }
            myNode->data = ele;
            myNode->next = NULL;
            return myNode;
        }

        void checkPool() {
            int listSize = size();
            int poolSize = 0;
            if (free != NULL) {
                ++poolSize;
                Node* temp = free;
                while (temp->next != NULL) {
                    temp = temp->next;
                    ++poolSize;
                }
            }
            if (listSize >= 100 && poolSize > (listSize / 2)) {
                int newSize = (int)(poolSize / 2);
                Node* temp;
                for (poolSize; poolSize > newSize; --poolSize) {
                    temp = free;
                    free = free->next;
                    delete temp;
                    temp = NULL;
                }
            }
        }

        void recycleNode(Node* point) {
            checkPool();
            point->next = NULL;
            if (free != NULL) {
                point->next = free;
                free = point;
            }
            else {
                free = point;
            }
        }

	public:

		class PSLL_Iter
		{
		public:
			typedef T value_type;
			typedef std::ptrdiff_t difference_type;
			typedef T& reference;
			typedef T* pointer;
			typedef std::forward_iterator_tag iterator_category;

			typedef PSLL_Iter self_type;
			typedef PSLL_Iter& self_reference;

		private:
			Node* here;

		public:
			explicit PSLL_Iter(Node* start = NULL) : here(start) {}
			PSLL_Iter(const PSLL_Iter& src) : here(src.here) {}

			reference operator*() const {
                if (here == NULL)
                    throw std::out_of_range("You have reached the end of the list");
				return here->data;
			}
			pointer operator->() const {
                if (here == NULL)
                    throw std::out_of_range("You have reached the end of the list");
				&here->data;
			}

			self_reference operator=(const PSLL_Iter& src) {
				if (*this == src)
					return this;

				delete this;
				return new PSLL_Iter(src);
			}

			self_reference operator++() {
                if (here == NULL)
                    throw std::out_of_range("You have reached the end of the list");
				here = here->next;
				return *this;
			} // preincrement

			self_type operator++(int) {
                if (here == NULL)
                    throw std::out_of_range("You have reached the end of the list");
				PSLL_Iter* results = new PSLL_Iter(*this);
				here = here->next;
				return *results;
			} // postincrement

			bool operator==(const PSLL_Iter& rhs) const {
				return here == rhs.here;
			}
			bool operator!=(const PSLL_Iter& rhs) const {
				return here != rhs.here;
			}
		};

		class PSLL_Const_Iter
		{
		public:
			typedef T value_type;
			typedef std::ptrdiff_t difference_type;
			typedef const T& reference;
			typedef const T* pointer;
			typedef std::forward_iterator_tag iterator_category;

			typedef PSLL_Const_Iter self_type;
			typedef PSLL_Const_Iter& self_reference;

		private:
			const Node* here;

		public:
			explicit PSLL_Const_Iter(Node* start = NULL) : here(start) {}
			PSLL_Const_Iter(const PSLL_Const_Iter& src) : here(src.here) {}

			reference operator*() const {
                if (here == NULL)
                    throw std::out_of_range("You have reached the end of the list");
				return here->data;
			}
			pointer operator->() const {
                if (here == NULL)
                    throw std::out_of_range("You have reached the end of the list");
				return &here->data;
			}

			self_reference operator=(const PSLL_Const_Iter& src) {
				if (*this == src)
					return this;

				delete this;
				return new PSLL_Const_Iter(src);
			}

			self_reference operator++() {
                if (here == NULL)
                    throw std::out_of_range("You have reached the end of the list");
				here = here->next;
				return *this;
			} // preincrement

			self_type operator++(int) {
                if (here == NULL)
                    throw std::out_of_range("You have reached the end of the list");
				PSLL_Const_Iter* results = new PSLL_Const_Iter(*this);
				here = here->next;
				return *results;
			} // postincrement

			bool operator==(const PSLL_Const_Iter& rhs) const {
				return here == rhs.here;
			}
			bool operator!=(const PSLL_Const_Iter& rhs) const {
				return here != rhs.here;
			}
		};

		//--------------------------------------------------
		// types
		//--------------------------------------------------
		//typedef std::size_t size_t;
		typedef T value_type;
		typedef PSLL_Iter iterator;
		typedef PSLL_Const_Iter const_iterator;

		PSLL() {
			this->head = NULL;
			this->free = NULL;
			this->tail = NULL;
            this->listSize = 0;
		}

		PSLL(const PSLL& copy) {
            if (copy.head != NULL) {
                this->head = new Node(copy.head->data);
                Node* temp = head;
                Node* foreign = copy.head->next;
                while (foreign != NULL) {
                    temp->next = new Node(foreign->data);
                    if (foreign->next == NULL)
                        tail = temp->next;
                    temp = temp->next;
                    foreign = foreign->next;
                }
            } else {
                this->head = NULL;
                this->tail = NULL;
            }
            this->listSize = copy.listSize;
            this->free = NULL;
		}

		~PSLL() {
			clearAll();
		}

		PSLL& operator=(const PSLL& src) {
			if (&src == this)
				return *this;

			delete this;
			return new PSLL(src);
		}

        T& operator[](int pos) {
            if (pos < 0 || pos >= listSize)
                throw std::domain_error("Position does not exist in the list.");
            Node* temp = head;
            for (int i = 0; i < pos; ++i) {
                temp = temp->next;
            }
            return temp->data;
        }

        T const& operator[](int pos) const {
            if (pos < 0 || pos >= listSize)
                throw std::domain_error("Position does not exist in the list.");
            Node* temp = head;
            for (int i = 0; i < pos; ++i) {
                temp = temp->next;
            }
            return temp->data;
        }

		T replace(const T& ele, int pos) {
            if (pos < 0 || pos >= listSize)
                throw std::domain_error("Position does not exist in the list.");
			Node *temp;
            T orig;
            if (pos == listSize - 1) {
                temp = tail;
            }
            else {
                temp = head;
                for (int i = 0; i < pos; ++i) {
                    temp = temp->next;
                }
            }
			orig = temp->data;
			temp->data = ele;
			return orig;
		}

		void insert(const T& ele, int pos) {
            if (pos < 0 || pos > listSize)
                throw std::domain_error("Position does not exist in the list.");
            Node *temp;
            if (pos != 0) {
                if (pos == listSize) {
                    push_back(ele);
                }
                else {
                    temp = head;
                    for (int i = 0; i < (pos - 1); ++i) { // pos - 1 because I want to stop at one early.
                        temp = temp->next;
                    }
                    Node* push = temp->next;
                    Node* ins = createNode(ele);
                    temp->next = ins;
                    ins->next = push;
                    ++listSize;
                }
            }
            else {
                Node* ins = createNode(ele);
                ins->next = head;
                head = ins;
                ++listSize;
            }
		}

		void push_back(const T& ele) {
			Node* newNode = createNode(ele);
			if (head == NULL) {
				head = newNode;
				tail = newNode;
			}
			else {
				tail->next = newNode;
				tail = newNode;
			}
            ++listSize;
		}

		void push_front(const T& ele) {
			Node* ins = createNode(ele);
			ins->next = head;
            if (head == NULL)
                tail = ins;
			head = ins;
            ++listSize;
		}

		T remove(int pos) {
            if (pos < 0 || pos >= listSize)
                throw std::domain_error("Position does not exist in the list.");
			Node* temp = head;
			T myData;
			if (pos != 0) {
                if (pos == listSize - 1) {
                    pop_back();
                    ++listSize; // To counteract the other decrement
                }
                else {
                    for (int i = 0; i < (pos - 1); ++i) {
                        temp = temp->next;
                    }
                    myData = temp->next->data;
                    Node *newNext = temp->next->next;
                    Node *deleted = temp->next;
                    temp->next = newNext;
                    recycleNode(deleted);
                }
			}
			else {
				myData = temp->data;
				Node* newNext = temp->next;
                head = newNext;
				recycleNode(temp);
			}
            --listSize;
			return myData;
		}

		T pop_back() {
            if (listSize == 0)
                throw std::length_error("The list is empty");
			Node* temp = head;
			T data = tail->data;
			if (head == tail) { // 1 element in the list
				clear();
			}
			else {
				Node* temp = head;
				while (temp->next != tail)
					temp = temp->next;
				tail = temp;
                Node* recycled = temp->next;
				temp->next = NULL;
                recycleNode(recycled);
			}
            --listSize;
			return data;
		}

		T pop_front() {
            if (listSize == 0)
                throw std::length_error("There is nothing in the list to remove.");
			Node* temp = head;
			T data = temp->data;
			head = temp->next;
			recycleNode(temp);
            --listSize;
			return data;
		}

		T item_at(int pos) {
            if (pos < 0 || pos >= listSize)
                throw std::domain_error("Position does not exist in the list.");
            if (pos == listSize - 1)
                return tail->data;
			Node* temp = head;
			for (int i = 0; i < pos; ++i) {
				temp = temp->next;
			}
			return temp->data;
		}

		bool is_empty() {
			return (head == NULL);
		}

		size_t size() {
			return listSize;
		}

		void clear() {
			if (head == NULL) return;
			Node* temp = head;
			Node* next = head->next;
            while (temp != NULL) {
                recycleNode(temp);
                temp = next;
                if (temp != NULL)
                    next = temp->next;
                --listSize;
			}
			head = NULL;
            tail = NULL;
            listSize = 0;
		}

		bool contains(const T& ele, bool equals(const T& a, const T& b)) const {
			Node* temp = head;

			while (temp != NULL) {
				if (equals(ele, temp->data))
					return true;
				temp = temp->next;
			}
			return false;
		}

		std::ostream& print(std::ostream& out) const {
			Node* temp = head;
			while (temp != NULL) {
				out << temp->data << std::endl;
				temp = temp->next;
			}
			return out;
		}

		iterator begin() { return PSLL_Iter(head); }
		iterator end() { return PSLL_Iter(); }

		const_iterator begin() const { return PSLL_Const_Iter(head); }
		const_iterator end() const { return PSLL_Const_Iter(); }
	};
}
#endif