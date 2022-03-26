/**
 * implement a container like std::map
 */
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::less<T>
#include <functional>
#include <cstddef>
#include "utility.hpp"
#include "exceptions.hpp"

namespace sjtu {

    template<
            class Key,
            class T,
            class Compare = std::less<Key>
    > class map {

    public:
        /**
         * the internal type of data.
         * it should have a default constructor, a copy constructor.
         * You can use sjtu::map as value_type by typedef.
         */
        typedef pair<const Key, T> value_type;
        struct Node {
            value_type * data = nullptr;
            const value_type *data_c = nullptr;
            Node *ls = nullptr;
            Node *rs = nullptr;
            int height = 1;
            ~Node(){
                if(data != nullptr)delete data;
                if(data_c != nullptr)delete data_c;
            }
            Node(){}
            Node(const value_type &a,Node *ls_,Node *rs_,int height_):ls(ls_),rs(rs_),height(height_){
                data_c = data = new value_type(a);
            }

        };
        /**
         * see BidirectionalIterator at CppReference for help.
         *
         * if there is anything wrong throw invalid_iterator.
         *     like it = map.begin(); --it;
         *       or it = map.end(); ++end();
         */
        class const_iterator;
        class iterator {
        private:
            /**
             * TODO add data members
             *   just add whatever you want.
             */
            Node *point = nullptr;
        public:
            iterator() {
                // TODO
            }
            iterator(Node *p){
                point = p;
            }
            iterator(const iterator &other) {
                // TODO
                point = other.point;
            }
            /**
             * TODO iter++
             */
            iterator operator++(int) {}
            /**
             * TODO ++iter
             */
            iterator & operator++() {}
            /**
             * TODO iter--
             */
            iterator operator--(int) {}
            /**
             * TODO --iter
             */
            iterator & operator--() {}
            /**
             * a operator to check whether two iterators are same (pointing to the same memory).
             */
            value_type & operator*() const {}
            bool operator==(const iterator &rhs) const {}
            bool operator==(const const_iterator &rhs) const {}
            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const {}
            bool operator!=(const const_iterator &rhs) const {}

            /**
             * for the support of it->first.
             * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
             */
            value_type* operator->() const noexcept {}
        };
        class const_iterator {
            // it should has similar member method as iterator.
            //  and it should be able to construct from an iterator.
        private:
            // data members.
            const Node *point_c = nullptr;
        public:
            const_iterator() {
                // TODO
            }
            const_iterator(const Node *p){
                point_c = p;
            }
            const_iterator(const const_iterator &other) {
                // TODO
            }
            const_iterator(const iterator &other) {
                // TODO
            }
            // And other methods in iterator.
            // And other methods in iterator.
            // And other methods in iterator.
        };
        /**
         * TODO two constructors
         */
        map() {}
        map(const map &other) {
            head = Creat(other.head);
        }
        /**
         * TODO assignment operator
         */
        map & operator=(const map &other) {
            if(&other == this) return *this;
            Destroy(head);
            head = Creat(other.head);
            return *this;
        }
        /**
         * TODO Destructors
         */
        ~map() {
            Destroy(head);
        }
        /**
         * TODO
         * access specified element with bounds checking
         * Returns a reference to the mapped value of the element with key equivalent to key.
         * If no such element exists, an exception of type `index_out_of_bound'
         */
        T & at(const Key &key) {
            Node *x = find(key,head);
            if(x == nullptr) throw index_out_of_bound();
            return x->data->second;
        }
        const T & at(const Key &key) const {
            Node *x = find(key,head);
            if(x == nullptr) throw index_out_of_bound();
            return x->data_c->second;
        }
        /**
         * TODO
         * access specified element
         * Returns a reference to the value that is mapped to a key equivalent to key,
         *   performing an insertion if such key does not already exist.
         */
        T & operator[](const Key &key) {
            Node *tmp = find(key,head);
            if(tmp!= nullptr) return tmp->data->second;
            else{
                tmp->data = (value_type *)malloc(sizeof(value_type));
                new(tmp->data->first) Key(key);
                return tmp->data->first;
            }
        }
        /**
         * behave like at() throw index_out_of_bound if such key does not exist.
         */
        const T & operator[](const Key &key) const {}
        /**
         * return a iterator to the beginning
         */
        iterator begin() {}
        const_iterator cbegin() const {}
        /**
         * return a iterator to the end
         * in fact, it returns past-the-end.
         */
        iterator end() {}
        const_iterator cend() const {}
        /**
         * checks whether the container is empty
         * return true if empty, otherwise false.
         */
        bool empty() const {}
        /**
         * returns the number of elements.
         */
        size_t size() const {}
        /**
         * clears the contents
         */
        void clear() {}
        /**
         * insert an element.
         * return a pair, the first of the pair is
         *   the iterator to the new element (or the element that prevented the insertion),
         *   the second one is true if insert successfully, or false.
         */
        pair<iterator, bool> insert(const value_type &value) {

        }
        /**
         * erase the element at pos.
         *
         * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
         */
        void erase(iterator pos) {}
        /**
         * Returns the number of elements with key
         *   that compares equivalent to the specified argument,
         *   which is either 1 or 0
         *     since this container does not allow duplicates.
         * The default method of check the equivalence is !(a < b || b > a)
         */
        size_t count(const Key &key) const {}
        /**
         * Finds an element with key equivalent to key.
         * key value of the element to search for.
         * Iterator to an element with key equivalent to key.
         *   If no such element is found, past-the-end (see end()) iterator is returned.
         */
        iterator find(const Key &key) {
            if(find(key,head) == nullptr) return end();
            return iterator(find(key,head));
        }
        const_iterator find(const Key &key) const {
            if(find(key,head) == nullptr) return cend();
            return const_iterator(find(key,head));
        }
    private:
        Node *head;
        Compare compare;
        size_t current_size = 0;
        Node* find(const Key &key,const Node* x){//return nullptr if not found
            if(x == nullptr) return nullptr;
            if(x->data.first == key) return x;
            if(compare(key,x->data.first))find(x->ls);
            else find(x->rs);
        }
        Node Creat(Node *t){
            Node *tmp = new Node(t->data,t->ls,t->rs,t->height);
            if(t->ls!= nullptr) tmp->ls = Creat(t->ls);
            if(t->rs != nullptr) tmp->rs = Creat(t->rs);
            return tmp;
        }
        void Destroy(Node *t){
            if(t == nullptr) return ;
            Destroy(t->rs);
            Destroy(t->ls);
            delete t;
            t = nullptr;
        }

    };

}

#endif
