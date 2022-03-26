#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>

namespace sjtu {
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 * 连续空间，随机访问
 * 用数组实现
 *
 */
    template<typename T>
    class vector {
    public:
        /**
         * TODO
         * a type for actions of the elements of a vector, and you should write
         *   a class named const_iterator with same interfaces.接口
         */
        /**
         * you can see RandomAccessIterator at CppReference for help.
         */
        class const_iterator;

        class iterator {
            // The following code is written for the C++ type_traits类型特点 library.
            // Type traits is a C++ feature for describing certain properties of a type.
            // For instance, for an iterator, iterator::value_type is the type that the
            // iterator points to.
            // STL algorithms and containers may use these type_traits (e.g. the following
            // typedef) to work properly. In particular, without the following code,
            // @code{std::sort(iter, iter1);} would not compile.
            // See these websites for more information:
            // https://en.cppreference.com/w/cpp/header/type_traits
            // About value_type: https://blog.csdn.net/u014299153/article/details/72419713
            // About iterator_category: https://en.cppreference.com/w/cpp/iterator
        public:
            friend class const_iterator;
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = T *;
            using reference = T &;
            using iterator_category = std::output_iterator_tag;
        private:
            /**
             * TODO add data members
             *   just add whatever you want.
             *///可用浅拷贝
            pointer i_now = nullptr;
            vector * from = nullptr;

        public:
            /**
             * return a new iterator which pointer n-next elements
             * as well as operator-
             */
            iterator(){}
            iterator(const iterator * &a){
                i_now = a->i_now;
                from = a->from;
            }
            iterator(pointer p,  vector * f){
                i_now = p;
                from = f;
            }
            ~iterator(){
                i_now = nullptr;
                from = nullptr;
            }
            iterator operator+(const int &n) const {
                //TODO
                return iterator(i_now + n,from);
            }

            iterator operator-(const int &n) const {
                //TODO
                return iterator(i_now - n,from);
            }

            // return the distance between two iterators,
            // if these two iterators point to different vectors, throw invaild_iterator.
            int operator-(const iterator &rhs) const {
                //TODO
                if(from != rhs.from) throw invalid_iterator();
                return i_now - rhs.i_now;
            }

            iterator &operator+=(const int &n) {
                //TODO
                i_now += n;
                return *this;
            }

            iterator &operator-=(const int &n) {
                //TODO
                i_now -= n;
                return *this;
            }

            /**
             * TODO iter++
             */
            iterator operator++(int) {
                iterator tmp(this);
                ++i_now;
                return tmp;
            }

            /**
             * TODO ++iter
             */
            iterator &operator++() {
                ++i_now;
                return *this;
            }

            /**
             * TODO iter--
             */
            iterator operator--(int) {
                iterator tmp(this);
                --i_now;
                return tmp;
            }

            /**
             * TODO --iter
             */
            iterator &operator--() {
                --i_now;
                return *this;
            }

            /**
             * TODO *it
             */
            T &operator*() const {
                return *i_now;
            }

            /**
             * a operator to check whether two iterators are same (pointing to the same memory address).
             */
            bool operator==(const iterator &rhs) const {
                return i_now == rhs.i_now;
            }

            bool operator==(const const_iterator &rhs) const {
                return i_now == rhs.i_now_c;
            }

            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const {
                return i_now != rhs.i_now;
            }

            bool operator!=(const const_iterator &rhs) const {
                return i_now != rhs.i_now_c;
            }
        };

        /**
         * TODO
         * has same function as iterator, just for a const object.
         */
        class const_iterator {
        public:
            friend class iterator;
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = T *;
            using reference = T &;
            using iterator_category = std::output_iterator_tag;

            const_iterator(){}
            const_iterator(const const_iterator &a):i_now_c(a.i_now_c),from_c(a.from_c){
            }
            const_iterator(T * p, const vector * f):i_now_c(p),from_c(f){
            }
            ~const_iterator(){}
            const_iterator operator+(const int &n) const {
                return const_iterator(i_now_c + n,from_c);
            }

            const_iterator operator-(const int &n) const {
                return const_iterator(i_now_c - n,from_c);
            }

            int operator-(const iterator &rhs) const {
                if(from_c != rhs.from) throw invalid_iterator();
                return i_now_c - rhs.i_now;
            }

            const_iterator &operator+=(const int &n) {
                i_now_c += n;
                return *this;
            }

            const_iterator &operator-=(const int &n) {
                i_now_c -= n;
                return *this;
            }

            /**
             * TODO iter++
             */
            const_iterator operator++(int) {
                const_iterator tmp(this);
                ++i_now_c;
                return tmp;
            }

            /**
             * TODO ++iter
             */
            const_iterator &operator++() {
                ++i_now_c;
                return *this;
            }

            /**
             * TODO iter--
             */
            const_iterator operator--(int) {
                const_iterator tmp(this);
                --i_now_c;
                return tmp;
            }

            /**
             * TODO --iter
             */
            iterator &operator--() {
                i_now_c -= 1;
                return *this;
            }

            /**
             * TODO *it
             */
            const T &operator*() const {
                return *i_now_c;
            }

            /**
             * a operator to check whether two iterators are same (pointing to the same memory address).
             */
            bool operator==(const iterator &rhs) const {
                return i_now_c == rhs.i_now;
            }

            bool operator==(const const_iterator &rhs) const {
                return i_now_c == rhs.i_now_c;
            }

            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const {
                return i_now_c != rhs.i_now;
            }

            bool operator!=(const const_iterator &rhs) const {
                return i_now_c != rhs.i_now_c;
                //rhs.i_now 不会报错？
            }

        private:
            const T * i_now_c = nullptr;
            const vector * from_c = nullptr;
        };

        /**
         * TODO Constructs
         * Atleast two: default constructor, copy constructor
         */
        vector(int initSize = 10) {
            v_first_c = v_last = v_first = (T *)malloc(initSize * sizeof(T));
            v_end = v_last + initSize;
            maxSize = initSize;
        }
        vector(const vector &other) {
            maxSize = other.maxSize;
            v_first_c = v_last = v_first =(T *)malloc(maxSize * sizeof(T));
            v_end = v_first + maxSize;
            T *tmp = other.v_first;
            while(tmp != other.v_last){
                new(v_last) T(*tmp);//用new的第二部分功能，在分配的空间上调用了T的拷贝构造函数
                ++tmp;
                ++v_last;
            }
        }

        /**
         * TODO Destructor
         */
        ~vector() {
            T *tmp = v_first;
            while(tmp != v_last){
                (*tmp).~T();//显示调用析构函数
                ++tmp;
            }
            free(v_first);
            v_first_c =v_last = v_end = v_first = nullptr;
        }

        /**
         * TODO Assignment operator
         */
        vector &operator=(const vector &other) {
            if(this == &other) return *this;
            maxSize = other.maxSize;
            T *tmp0 = v_first;
            while(tmp0 != v_last){
                (*tmp0).~T();//显示调用析构函数
                ++tmp0;
            }
            free(v_first);
            v_first_c = v_last = v_first = (T *)malloc(maxSize * sizeof(T));
            v_end = v_first + maxSize;
            T *tmp = other.v_first;
            while(tmp != other.v_last){
                new (v_last) T(*tmp);
                ++tmp;
                ++v_last;
            }
            return *this;
        }

        /**
         * assigns specified element with bounds checking
         * throw index_out_of_bound if pos is not in [0, size)
         */
        T &at(const size_t &pos) {
            if(pos < 0 || pos >= v_last - v_first) throw index_out_of_bound();
            else return v_first[pos];
        }

        const T &at(const size_t &pos) const {
            if(pos < 0 || pos >= v_last - v_first) throw index_out_of_bound();
            else return v_first_c[pos];
        }

        /**
         * assigns specified element with bounds checking
         * throw index_out_of_bound if pos is not in [0, size)
         * !!! Pay attentions
         *   In STL this operator does not check the boundary but I want you to do.
         */
        T &operator[](const size_t &pos) {
            if(pos < 0 || pos >= v_last - v_first) throw index_out_of_bound();
            else return v_first[pos];
        }

        const T &operator[](const size_t &pos) const {
            if(pos < 0 || pos >= v_last - v_first) throw index_out_of_bound();
            else return v_first_c[pos];
        }

        /**
         * access the first element.
         * throw container_is_empty if size == 0
         */
        const T &front() const {
            if(size() == 0) throw container_is_empty();
            else return *v_first_c;
        }

        /**
         * access the last element.
         * throw container_is_empty if size == 0
         */
        const T &back() const {
            if(size() == 0) throw container_is_empty();
            else return *(v_first_c + size() - 1);
        }

        /**
         * returns an iterator to the beginning.
         */
        iterator begin() {
            return iterator(v_first,this);
        }

        const_iterator cbegin() const {
            return const_iterator(v_first,this);
        }

        /**
         * returns an iterator to the end.
         */
        iterator end() {
            return iterator(v_last,this);
        }

        const_iterator cend() const {
            return const_iterator(v_last,this);
        }

        /**
         * checks whether the container is empty
         */
        bool empty() const {
            return size() == 0;
        }

        /**
         * returns the number of elements
         */
        size_t size() const {
            return size_t (v_last - v_first);
        }

        /**
         * clears the contents
         */
        void clear() {
            v_last = v_first;
        }

        /**
         * inserts value before pos
         * returns an iterator pointing to the inserted value.
         */
        iterator insert(iterator pos, const T &value) {
            if(v_last == v_end){
                size_t pos_v = pos - begin();
                doubleSpace();
                T* tmp = v_last - 1;
                new (v_last) T(*tmp);
                ++v_last;
                while(tmp != v_first + pos_v){
                    *(tmp) = *(tmp - 1);
                    --tmp;
                }
                v_first[pos_v] = value;
                return iterator(v_first + pos_v,this);

            }
            else{
                size_t pos_v = pos - begin();
                T* tmp = v_last - 1;
                new (v_last) T(*tmp);
                ++v_last;
                while(tmp != v_first + pos_v){
                    *(tmp) = *(tmp - 1);
                    --tmp;
                }
                v_first[pos_v] = value;
                return iterator(v_first + pos_v,this);
            }
        }

        /**
         * inserts value at index ind.
         * after inserting, this->at(ind) == value
         * returns an iterator pointing to the inserted value.
         * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
         */
        iterator insert(const size_t &ind, const T &value) {
            if(ind > size()) throw index_out_of_bound();
            if(ind == size()){
                push_back(value);
                return iterator(v_last - 1,this);
            }
            T *tmp = v_last;
            T*tmp0 = (T *)malloc(sizeof(T));
            *tmp0 = *(v_last - 1);
            while(tmp != v_first + ind){
                *tmp = *(--tmp);
            }
            push_back(*tmp0);
            return iterator(v_first[ind],this);
        }

        /**
         * removes the element at pos.
         * return an iterator pointing to the following element.
         * If the iterator pos refers the last element, the end() iterator is returned.
         */
        iterator erase(iterator pos) {
            iterator tmp(pos);
            while(pos != end()){
                *pos = *(pos + 1);
                ++pos;
            }
            pop_back();
            return tmp;
        }

        /**
         * removes the element with index ind.
         * return an iterator pointing to the following element.
         * throw index_out_of_bound if ind >= size
         */
        iterator erase(const size_t &ind) {
            if(ind >= size()) throw index_out_of_bound();
            iterator tmp(v_first + ind,this);
            while(tmp != end()){
                *tmp = *(tmp + 1);
                ++tmp;
            }
            pop_back();
        }

        /**
         * adds an element to the end.
         */
        void push_back(const T &value) {
            if(v_last != v_end){
                new(v_last) T(value);
                ++v_last;
            }
            else{
                doubleSpace();
                new(v_last) T(value);
                ++v_last;
            }
        }

        /**
         * remove the last element from the end.
         * throw container_is_empty if size() == 0
         */
        void pop_back() {
            if(!size()) throw container_is_empty();
            --v_last;
            (*v_last).~T();
        }
    private:
        T* v_first;
        const T* v_first_c;
        T* v_last;
        T* v_end;
        long long maxSize = 0;
        void doubleSpace(){
            maxSize *= 2;
            T *t;
            T *tmp = v_first;
            t = tmp;
            T *last = v_last;
            v_first_c = v_last = v_first =(T *)malloc(maxSize * sizeof(T));
            v_end = v_first + maxSize;
            while(tmp != last){
                new(v_last) T(*tmp);
                (*tmp).~T();
                ++tmp;
                ++v_last;
            }
            free(t);
        }

    };

}

#endif