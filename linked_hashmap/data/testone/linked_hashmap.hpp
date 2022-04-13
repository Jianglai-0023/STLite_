/**
 * implement a container like std::linked_hashmap
 */
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::equal_to<T> and std::hash<T>
#include <functional>
#include <cstddef>
#include "../../utility.hpp"
#include "../../exceptions.hpp"

namespace sjtu {
    /**
     * In linked_hashmap, iteration ordering is differ from map,
     * which is the order in which keys were inserted into the map.
     * You should maintain a doubly-linked list running through all
     * of its entries to keep the correct iteration order.
     *
     * Note that insertion order is not affected if a key is re-inserted
     * into the map.
     */
    
template<
	class Key,
	class T,
	class Hash = std::hash<Key>, 
	class Equal = std::equal_to<Key>
> class linked_hashmap {
public:
	/**
	 * the internal type of data.
	 * it should have a default constructor, a copy constructor.
	 * You can use sjtu::linked_hashmap as value_type by typedef.
	 */
	typedef pair<const Key, T> value_type;
     struct node{
         node *nxt = nullptr;
         //for linklist
         node *pre_l = nullptr;
         node *nxt_l = nullptr;
         linked_hashmap :: value_type* data = nullptr;
         node(){}
         node(const value_type &t){
             data = new value_type (t);
         }
         ~node(){
             delete data;
         }
     };
private:
    node ** array;
    Hash hash;
    Equal equal;
    int current_size = 0;
    int Capacity = 16;
    node *head = nullptr;//linklist
    node *tail = nullptr;
    const float Loadfactor = 0.75;
    node *find_key(const Key &key)const{
        int index = hash(key) % Capacity;
        if(array[index]== nullptr) return nullptr;
        node *tmp = array[index];
        while(!equal(*(tmp->data)->first,key)){
            tmp = tmp->nxt;
            if(tmp == nullptr) break;
        }
        return tmp;
    }
    void doubledspace(){
            Capacity = Capacity << 1;
            delete array [];
            array = new node*[Capacity];
            node *tmp = head;
            while(tmp!= tail){
                int index = hash(*(tmp->data)->first)%Capacity;
                if(array[index]== nullptr) array[index] = tmp;
                else{
//                    array[index]->pre = tmp;
                    tmp->nxt = array[index];
                    array[index] = tmp;
                }
                tmp = tmp->nxt_l;//link_list
            }
        }
    node *insert_key(value_type t){//can't find origin key
        ++current_size;
        int index = hash(t.first) % Capacity;
        node *tmp = new node(t);//insert in head
        if(array[index]== nullptr) array[index] = tmp;
        else{//insert in head
//            array[index]->pre = tmp;
            tmp->nxt = array[index];
            array[index] = tmp;
        }
        if(head==tail){//insert in linklist
            head = tmp;
            head->nxt_l = tail;
            tail->pre_l = head;
        }
        else{
            tail->pre_l->nxt_l = tmp;
            tmp->pre_l = tail->pre_l;
            tmp->nxt_l = tail;
            tail->pre_l = tmp;
        }
        if(current_size > Capacity * Loadfactor)doubledspace();
        return tmp;
    }

public:
	/**
	 * see BidirectionalIterator at CppReference for help.
	 *
	 * if there is anything wrong throw invalid_iterator.
	 *     like it = linked_hashmap.begin(); --it;
	 *       or it = linked_hashmap.end(); ++end();
	 */
	class const_iterator;
	class iterator {
        friend class linked_hashmap;
	private:
		node *point;
        linked_hashmap* me;
	public:
		// The following code is written for the C++ type_traits library.
		// Type traits is a C++ feature for describing certain properties of a type.
		// For instance, for an iterator, iterator::value_type is the type that the 
		// iterator points to. 
		// STL algorithms and containers may use these type_traits (e.g. the following 
		// typedef) to work properly. 
		// See these websites for more information:
		// https://en.cppreference.com/w/cpp/header/type_traits
		// About value_type: https://blog.csdn.net/u014299153/article/details/72419713
		// About iterator_category: https://en.cppreference.com/w/cpp/iterator
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::output_iterator_tag;


		iterator() {
			point = nullptr;
            me = nullptr;
		}
		iterator(const iterator &other) {
			point = other.point;
            me = other.me;
		}
        iterator(node * p, linked_hashmap * m){
            point = p;
            me = m;
        }
		/**
		 * TODO iter++
		 */
		iterator operator++(int) {
            if(point == me->tail)throw invalid_iterator();
            iterator tmp = *this;
            point = point->nxt_l;
            return tmp;
        }
		/**
		 * TODO ++iter
		 */
		iterator & operator++() {
            if(point == me->tail)throw invalid_iterator();
            point = point->nxt_l;
            return *this;
        }
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {
            if(point==me->head) throw invalid_iterator();
            iterator tmp = *this;
            point = point->pre_l;
            return tmp;
        }
		/**
		 * TODO --iter
		 */
		iterator & operator--() {
            if(point==me->head) throw invalid_iterator();
            point = point->pre_l;
            return *this;
        }
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		value_type & operator*() const {
            return *(point->data);
        }
		bool operator==(const iterator &rhs) const {
            return me == rhs.me && point == rhs.point;
        }
		bool operator==(const const_iterator &rhs) const {
            return me == rhs.me && point == rhs.point;
        }
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
            return me != rhs.me || point != rhs.point;
        }
		bool operator!=(const const_iterator &rhs) const {
            return me != rhs.me || point != rhs.point;
        }

		/**
		 * for the support of it->first. 
		 * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
		 */
		value_type* operator->() const noexcept {
            return point->data;
        }
	};
 
	class const_iterator {
        friend class linked_hashmap;
		// it should has similar member method as iterator.
		//  and it should be able to construct from an iterator.
		private:
			const node *point = nullptr;
            const linked_hashmap * me = nullptr;
		public:
			const_iterator() {
			}
			const_iterator(const const_iterator &other) {
				point = other.point;
                me = other.me;
			}
			const_iterator(const iterator &other) {
                point = other.point;
                me = other.me;
			}
            const_iterator(node * p, linked_hashmap * m){
                point = p;
                me = m;
            }
        const_iterator operator++(int) {
            if(point == me->tail)throw invalid_iterator();
            const_iterator tmp = *this;
            point = point->nxt_l;
            return tmp;
        }
        /**
         * TODO ++iter
         */
        const_iterator & operator++() {
            if(point == me->tail)throw invalid_iterator();
            point = point->nxt_l;
            return *this;
        }
        /**
         * TODO iter--
         */
        const_iterator operator--(int) {
            if(point== me->head) throw invalid_iterator();
            const_iterator tmp = *this;
            point = point->pre_l;
            return tmp;
        }
        /**
         * TODO --iter
         */
        const_iterator & operator--() {
            if(point == me->head) throw invalid_iterator();
            point = point->pre_l;
            return *this;
        }
        /**
         * a operator to check whether two iterators are same (pointing to the same memory).
         */
        value_type & operator*() const {
            return *(point->data);
        }
        bool operator==(const iterator &rhs) const {
            return me == rhs.me && point == rhs.point;
        }
        bool operator==(const const_iterator &rhs) const {
            return me == rhs.me && point == rhs.point;
        }
        /**
         * some other operator for iterator.
         */
        bool operator!=(const iterator &rhs) const {
            return me != rhs.me || point != rhs.point;
        }
        bool operator!=(const const_iterator &rhs) const {
            return me != rhs.me || point != rhs.point;
        }

        /**
         * for the support of it->first.
         * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
         */
        value_type* operator->() const noexcept {
            return point->data;
        }
	};
 
	/**
	 * TODO two constructors
	 */
	linked_hashmap() {
        array = new node*[Capacity];
        for(int i = 0; i < Capacity; ++i){
            array[i] = nullptr;
        }
        head = tail = new node;//尾结点
    }
	linked_hashmap(const linked_hashmap &other) {
        Capacity = other.Capacity;
//        current_size = other.current_size;
        array = new node*[Capacity];
        node *tmp = other.head;
        while(tmp != other.tail){
            this->insert_key(*(tmp->data));
            tmp = tmp->nxt_l;
        }
//        for(int i = 0; i < Capacity; ++i){//无头节点
//            node *tmp = other.array[i];
//            if(tmp == nullptr) continue;
//            node *me = array[i];
//            me = new node(tmp->data);
//            while(tmp->nxt != nullptr){
//                tmp = tmp->nxt;
//                me->nxt = new node(tmp->data);
//                me->nxt->pre = me;
//                me = me->nxt;
//            }
//        }
    }
 
	/**
	 * TODO assignment operator
	 */
	linked_hashmap & operator=(const linked_hashmap &other) {
        if(this == other) return *this;
        Capacity = other.Capacity;
//        current_size = other.current_size;
        array = new node*[Capacity];
        node *tmp = other.head;
        while(tmp != other.tail){
            this->insert_key(*(tmp->data));
            tmp = tmp->nxt_l;
        }
        return *this;
    }
 
	/**
	 * TODO Destructors
	 */
	~linked_hashmap() {
        for(int i = 0; i < Capacity; ++i){
            node *tmp = array[i];
            if(tmp == nullptr) continue;
            while(tmp!= nullptr){
                node *h = tmp;
                tmp = tmp->nxt;
                delete h;
            }
        }
        delete array [];
    }
 
	/**
	 * TODO
	 * access specified element with bounds checking
	 * Returns a reference to the mapped value of the element with key equivalent to key.
	 * If no such element exists, an exception of type `index_out_of_bound'
	 */
	T & at(const Key &key) {
        node *tmp = find_key(key);
        if(key == nullptr) throw index_out_of_bound();
        return *(tmp->data).second;
    }
	const T & at(const Key &key) const {
        node *tmp = find_key(key);
        if(key == nullptr) throw index_out_of_bound();
        return *(tmp->data).second;
    }
 
	/**
	 * TODO
	 * access specified element 
	 * Returns a reference to the value that is mapped to a key equivalent to key,
	 *   performing an insertion if such key does not already exist.
	 */
	T & operator[](const Key &key) {
        node *tmp = find_key(key);
        if(key == nullptr){
            pair<const Key, T> p(key,T());
            tmp = insert_key(p);
        }
        return *(tmp->data)->second;
    }
 
	/**
	 * behave like at() throw index_out_of_bound if such key does not exist.
	 */
	const T & operator[](const Key &key) const {
        node *tmp = find_key(key);
        if(key == nullptr) throw index_out_of_bound();
        return *(tmp->data)->second;
    }
 
	/**
	 * return a iterator to the beginning
	 */
	iterator begin() {
        iterator i(head,this);
        return i;
    }
	const_iterator cbegin() const {
        const_iterator i(head,this) ;
        return i;
    }
 
	/**
	 * return a iterator to the end
	 * in fact, it returns past-the-end.
	 */
	iterator end() {
        iterator i(tail,this);
        return i;
    }
	const_iterator cend() const {
        const_iterator i(tail,this);
        return i;
    }
 
	/**
	 * checks whether the container is empty
	 * return true if empty, otherwise false.
	 */
	bool empty() const {
        if(!current_size) return true;
        return false;
    }
 
	/**
	 * returns the number of elements.
	 */
	size_t size() const {
        return current_size;
    }
 
	/**
	 * clears the contents
	 */
	void clear() {
        current_size = 0;
        for(int i = 0; i < Capacity; ++i){
            node *tmp = array[i];
            if(tmp == nullptr) continue;
            while(tmp!= nullptr){
                h = tmp;
                tmp = tmp->nxt;
                delete h;
            }
            array[i] = nullptr;
        }
        head = tail = new node;
    }
 
	/**
	 * insert an element.
	 * return a pair, the first of the pair is
	 *   the iterator to the new element (or the element that prevented the insertion), 
	 *   the second one is true if insert successfully, or false.
	 */
	pair<iterator, bool> insert(const value_type &value) {
        node *tmp = find_key(value.first);
        iterator p(x,this);
        if(x == nullptr){
            iterator p0(insert_key(value),this);
            pair<iterator,bool> ai(p0,true);
            return ai;
        }
        pair<iterator,bool> ai(p,false);
        return ai;
    }
 
	/**
	 * erase the element at pos.
	 *
	 * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
	 */
	void erase(iterator pos) {
        --current_size;
        if(pos == this->end()|| pos.me != this) throw invalid_iterator();
        node *tmp = pos.point;
        //linklist
        if(head == tmp){//头节点特判
           head->nxt_l->pre_l = nullptr;
           head = head->nxt_l;
        }
        else{
            tmp->pre_l->nxt_l = tmp->nxt_l;
            tmp->nxt_l->pre_l = tmp->pre_l;
        }
        //array
        int index = hash(*(pos.point->data)->first)%Capacity;
        node *f = array[index];
        node *r = f;
        while(f->data != tmp->data){
            r = f;
            f = f->nxt;
        };
        if(f == array[index]){//头节点特判
            array[index] = f->nxt;
            delete f;
        }
        else{
            r->nxt = f->nxt;
            delete f;
        }
    }
 
	/**
	 * Returns the number of elements with key 
	 *   that compares equivalent to the specified argument,
	 *   which is either 1 or 0 
	 *     since this container does not allow duplicates.
	 */
	size_t count(const Key &key) const {
        if(find_key(key) == nullptr) return 0;
        return 1;
    }
 
	/**
	 * Finds an element with key equivalent to key.
	 * key value of the element to search for.
	 * Iterator to an element with key equivalent to key.
	 *   If no such element is found, past-the-end (see end()) iterator is returned.
	 */
	iterator find(const Key &key) {
        node *x = find_key(key);
        if(x == nullptr){
            return iterator(this->end(),this);
        }
        return iterator(x,this);
    }
	const_iterator find(const Key &key) const {
        node *x = find_key(key);
        if(x == nullptr){
            return const_iterator(this->cend(),this);
        }
        return const_iterator(x,this);
    }
};

}

#endif
