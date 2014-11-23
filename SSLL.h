#ifndef _SSLL_H_
#define _SSLL_H_
#include <iostream>
#include <stdexcept>

namespace COP3530 {

    /**
    * Class Description: Holds a list of type T objects or primitives in a linked list of nodes that each contain a pointer
    * to the next node.
    */
	template <typename T> class SSLL
	{
	private:

        /**
        * Description: A private container that will hold values of type T and a pointer
        * to the next node in the list.
        */
		struct Node {
			T data;
			Node* next;

			Node() {
				next = NULL;
			}
			~Node() {
                next = NULL;
            }

            /**
            * Construct node with already available data.
            */
			Node(const T& data) {
				this->data = data;
				this->next = NULL;
			}

            /**
            * Construct node with already available next node and data.
            */
			Node(const T& data, const T& next) {
				this->data = data;
				this->next = next;
			}
		};
		Node* head;
		Node* tail;
		size_t listSize;

	public:

        /**
        * Class Description: Iterates through the list that it belongs to and returns values in the list from start to end.
        * The values in the list that it returns can be manipulated since they are not constant and they are passed by reference.
        */
		class SSLL_Iter
		{
		public:
			typedef T value_type;
			typedef std::ptrdiff_t difference_type;
			typedef T& reference;
			typedef T* pointer;
			typedef std::forward_iterator_tag iterator_category;

			typedef SSLL_Iter self_type;
			typedef SSLL_Iter& self_reference;

		private:
			Node* here;

		public:
            /**
            * Constructor. Unless the start node is explicitly specified, it returns an iterator pointing to the end of
            * the list.
            */
			explicit SSLL_Iter(Node* start = NULL) : here(start) {}

            /**
            * Copy Constructor.
            */
			SSLL_Iter(const SSLL_Iter& src) : here(src.here) {}

            /**
            * Dereference operator. It will return the item in the list that the iterator is pointing to
            * unless the iterator is out of range, in which case it will throw an out of range exception.
            */
			reference operator*() const {
                if (here == NULL)
                    throw std::out_of_range("You have reached the end of the list");
				return here->data;
			}

            /**
            * Member operator for structs and classes. Similarly to the above method, it will return the item in the list
            * that the iterator is pointing to unless the iterator is out of range, in which case it will throw an out of
            * range exception. Since it returns a pointer to the item in the list, its individual members and properties
            * can be accessed on the right-hand side of the operator.
            */
			pointer operator->() const {
                if (here == NULL)
                    throw std::out_of_range("You have reached the end of the list");
				return &here->data;
			}

            /**
            * Overloaded Equals operator so as to return a proper copy construction of the iterator.
            */
			self_reference operator=(const SSLL_Iter& src) {
				if (*this == src)
					return *this;

				delete this;
				return new SSLL_Iter(src);
			}

            /**
            * Pre-increment overloaded to step the iterator one element up in the list. Pre-increment will continue to
            * return the list until it is one past the last element. Once it is one past the last element and the user
            * attempts to use the operator again, an out of range exception will be thrown since it is already at the
            * maximum length.
            */
			self_reference operator++() {
                if (here == NULL)
                    throw std::out_of_range("You have reached the end of the list");
				here = here->next;
				return *this;
			}

            /**
            * Post-increment operator is overloaded similarly to the pre-increment, but another iterator is copied before
            * this iterator points to the next element, and that copied iterator is returned for use. Post-increment will
            * continue to return the list until it is one past the last element. Once it is one past the last element and
            * the user attempts to use the operator again, an out of range exception will be thrown since it is already
            * at the maximum length.
            */
			self_type operator++(int) {
                if (here == NULL)
                    throw std::out_of_range("You have reached the end of the list");
				SSLL_Iter* results = new SSLL_Iter(*this);
				here = here->next;
				return *results;
			}

            /**
            * Comparison operator is overloaded to return true IFF two SSLL iterators are pointing to the same element in
            * the same list.
            */
			bool operator==(const SSLL_Iter& rhs) const {
				return here == rhs.here;
			}

            /**
            * Comparison operator is overloaded to return true IFF two SSLL operators are pointing to different elements.
            */
			bool operator!=(const SSLL_Iter& rhs) const {
				return here != rhs.here;
			}
		};

        /**
        * Class Description: Iterates through the list that it belongs to and returns values in the list from start to end.
        * The values in the list that it returns cannot be manipulated since they are constant.
        */
		class SSLL_Const_Iter
		{
		public:
			typedef T value_type;
			typedef std::ptrdiff_t difference_type;
			typedef const T& reference;
			typedef const T* pointer;
			typedef std::forward_iterator_tag iterator_category;

			typedef SSLL_Const_Iter self_type;
			typedef SSLL_Const_Iter& self_reference;

		private:
			const Node* here;

		public:
			explicit SSLL_Const_Iter(Node* start = NULL) : here(start) {}
			SSLL_Const_Iter(const SSLL_Const_Iter& src) : here(src.here) {}

            /**
            * Dereference operator. It will return the item in the list that the iterator is pointing to
            * unless the iterator is out of range, in which case it will throw an out of range exception. The referenced
            * item cannot be manipulated because it is constant.
            */
			reference operator*() const {
				if (here == NULL)
					throw std::out_of_range("You have reached the end of the list");
				return here->data;
			}

            /**
            * Member operator for structs and classes. Similarly to the above method, it will return the item in the list
            * that the iterator is pointing to unless the iterator is out of range, in which case it will throw an out of
            * range exception. Since it returns a pointer to the item in the list, its individual members and properties
            * can be accessed on the right-hand side of the operator. Any class or struct in this list cannot be changed
            * to a new one since it is const.
            */
			pointer operator->() const {
				if (here == NULL)
					throw std::out_of_range("You have reached the end of the list");
				return &here->data;
			}

            /**
            * Overloaded Equals operator so as to return a proper copy construction of the constant iterator.
            */
			self_reference operator=(const SSLL_Const_Iter& src) {
				if (*this == src)
					return *this;

				delete this;
				return new SSLL_Const_Iter(src);
			}

            /**
            * Pre-increment overloaded to step the iterator one element up in the list. Pre-increment will continue to
            * return the list until it is one past the last element. Once it is one past the last element and the user
            * attempts to use the operator again, an out of range exception will be thrown since it is already at the
            * maximum length.
            */
			self_reference operator++() {
                if (here == NULL)
                    throw std::out_of_range("You have reached the end of the list");
				here = here->next;
				return *this;
			}

            /**
            * Post-increment operator is overloaded similarly to the pre-increment, but another iterator is copied before
            * this iterator points to the next element, and that copied iterator is returned for use. Post-increment will
            * continue to return the list until it is one past the last element. Once it is one past the last element and
            * the user attempts to use the operator again, an out of range exception will be thrown since it is already
            * at the maximum length.
            */
			self_type operator++(int) {
                if (here == NULL)
                    throw std::out_of_range("You have reached the end of the list");
				SSLL_Const_Iter* results = new SSLL_Const_Iter(*this);
				here = here->next;
				return *results;
			}

            /**
            * Comparison operator is overloaded to return true IFF two SSLL iterators are pointing to the same element in
            * the same list.
            */
			bool operator==(const SSLL_Const_Iter& rhs) const {
				return here == rhs.here;
			}

            /**
            * Comparison operator is overloaded to return true IFF two SSLL operators are pointing to different elements.
            */
			bool operator!=(const SSLL_Const_Iter& rhs) const {
				return here != rhs.here;
			}
		};

		//--------------------------------------------------
		// types
		//--------------------------------------------------
		//typedef std::size_t size_t;
		typedef T value_type;
		typedef SSLL_Iter iterator;
		typedef SSLL_Const_Iter const_iterator;

        /**
        * Default constructor for the SSLL. It sets the head and tail pointers to NULL and the size to zero because there
        * are zero elements in the list.
        */
		SSLL() {
			this->head = NULL;
			this->tail = NULL;
			this->listSize = 0;
		}

        /**
        * Copy constructor for the SSLL. It is a deep copy constructor in that it recreates the nodes with the values from
        * all of the original nodes, so the original list is not copied by reference. Thus, if the original list is altered
        * after a new list is copied, the copied list will remain the same.
        */
		SSLL(const SSLL& copy) {
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
		}

        /**
        * Deconstructor that destroys all nodes in the chain by calling clear().
        */
		~SSLL() {
			clear();
		}

        /**
        * Equals operator overloaded so that it will return the same object if it is being "reassigned" to the same object.
        * Otherwise it will copy the list that it is newly assigned to and destroy itself.
        */
		SSLL& operator=(const SSLL& src) {
			if (&src == this)
				return *this;

			clear();
			delete this;
			return new SSLL(src);
		}

        /**
        * The subscript operator is overloaded so that each element in the list can be retrieved by putting its position
        * in the list in the subscript. Ideally, it will work for any number 0 through n-1, where n is the number of
        * items in the list. If any number outside of this range is entered, a domain error will get thrown infroming
        * the user that it is an invalid location.
        */
        T& operator[](int pos) {
            if (pos < 0 || pos >= listSize)
                throw std::domain_error("Position does not exist in the list.");
            Node* temp = head;
            for (int i = 0; i < pos; ++i) {
                temp = temp->next;
            }
            return temp->data;
        }

        /**
        * This const subscript operator overload is exactly the same as the above operator, but this one is called when
        * the list is declared as const. Thus, items that are returned using this operator are
        */
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
					Node* ins = new Node(ele);
					temp->next = ins;
					ins->next = push;
					++listSize;
				}
			}
			else {
				Node* ins = new Node(ele);
				ins->next = head;
				head = ins;
				++listSize;
			}
		}

		void push_back(const T& ele) {
			Node* newNode = new Node(ele);
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
			Node* ins = new Node(ele);
			ins->next = head;
            if (head == NULL)
                tail = ins;
			head = ins;
			++listSize;
		}

		T remove(int pos) {
			if (pos < 1 || pos >= listSize)
				throw std::domain_error("Position does not exist in the list.");
			Node* temp = head;
            T myData;
			if (pos != 0) {
                for (int i = 0; i < (pos - 1); ++i) {
                    temp = temp->next;
                }
                myData = temp->next->data;
                Node* newNext = temp->next->next;
                Node* deleted = temp->next;
                delete deleted;
                temp->next = newNext;
			}
			else {
				myData = temp->data;
				Node* newNext = temp->next;
				delete temp;
				head = newNext;
			}
			--listSize;
			return myData;
		}

		T pop_back() {
			if (listSize == 0)
				throw std::length_error("The list is empty");
			T data = tail->data;
			Node* temp = head;
			if (head == tail) { // 1 element in the list
				clear();
			}
			else {
				Node* temp = head;
				while (temp->next != tail)
					temp = temp->next;
				tail = temp;
				delete temp->next;
				temp->next = NULL;
			}
			--listSize;
			return data;
		}

		T pop_front() {
			if (listSize == 0)
				throw std::length_error("The list is empty");
			Node* temp = head;
			T data = temp->data;
			head = temp->next;
			delete temp;
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
                delete temp;
                temp = next;
                if (temp != NULL)
                    next = temp->next;
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

		iterator begin() { return SSLL_Iter(head); }
		iterator end() { return SSLL_Iter(); }

		const_iterator begin() const { return SSLL_Const_Iter(head); }
		const_iterator end() const { return SSLL_Const_Iter(); }
	};
}
#endif