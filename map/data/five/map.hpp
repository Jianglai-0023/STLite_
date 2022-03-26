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
        map::value_type * data = nullptr;
//        const value_type *data_c = nullptr;
        Node *ls = nullptr;
        Node *rs = nullptr;
        Node *pa = nullptr;
        int height = 1;
        ~Node(){
            delete data;
        }
        Node(){}
        Node(const value_type &a,Node*p,int h):pa(p),height(h){
            data = new value_type(a);
        }
        Node(const value_type &a,Node *pa_):pa(pa_){
            data = new value_type(a);
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
        friend class map;
        friend class const_iterator;
	private:
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
		 Node *point ;
         const map* me;
         bool isend = false;
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
		// If you are interested in type_traits, toy_traits_test provides a place to 
		// practice. But the method used in that test is old and rarely used, so you
		// may explore on your own.
		// Notice: you may add some code in here and class const_iterator and namespace sjtu to implement toy_traits_test,
		// this part is only for bonus.


		iterator() {
			point = nullptr;
            me = nullptr;
		}
		iterator(const iterator &other) {
            point = other.point;
            me = other.me;
            isend = other.isend;
		}
        iterator(Node* t,const map*m,int isend_ = 0):point(t),me(m),isend(isend_){}
		/**
		 * TODO iter++
		 */

		iterator operator++(int) {
            iterator p = *this;
            if(isend) throw invalid_iterator();
            if(point->rs!= nullptr){
                point = point->rs;
                while(point->ls!= nullptr)point = point->ls;
            }
            else{
                while(point->pa != nullptr && point != point->pa->ls)point = point->pa;
                if(point->pa != nullptr) point = point->pa;
                else {//找不到后继
                    isend = true;//到达end()
                    point = nullptr;
                }
            }
            return p;
        }
		/**
		 * TODO ++iter
		 */
		iterator & operator++() {
            if(isend) throw invalid_iterator();
            if(point->rs!= nullptr){
                point = point->rs;
                while(point->ls!= nullptr)point = point->ls;
            }
            else{
                while(point->pa != nullptr && point != point->pa->ls)point = point->pa;
                if(point->pa != nullptr) point = point->pa;
                else {//找不到后继
                    isend = true;//到达end()
                    point = nullptr;
                }
            }
            return *this;
        }
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {
            iterator p = *this;//todo 对end()--?
            if(isend ){
                if(me->empty()) throw invalid_iterator();
                isend = false;
                point = me->Isend();
                return p;
            }
            if(point->ls != nullptr){
                point = point->ls;
                while(point->rs != nullptr) point = point->rs;//左儿子的右子树里最大的
                return p;
            }
            else{//父节点，回溯 第一个作为右儿子出现
                while(point->pa!= nullptr&&point->pa->ls == point)point = point->pa;
                if(point->pa!= nullptr) point = point->pa;
                else throw invalid_iterator();

            }
            return p;
        }
		/**
		 * TODO --iter
		 */
		iterator & operator--() {
            if(isend){
                if(me->empty()) throw invalid_iterator();
                isend = false;
                point = me->Isend();
                return *this;
            }
            if(point->ls != nullptr){
                point = point->ls;
                while(point->rs != nullptr) point = point->rs;
                return *this;
            }
            else{//没有左儿子
                while(point->pa!= nullptr&&point->pa->ls == point)point = point->pa;
                if(point->pa!= nullptr) point = point->pa;
                else throw invalid_iterator();
            }
            return *this;
        }
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		map::value_type & operator*() const {
            return *(point->data);
        }
		bool operator==(const iterator &rhs) const {
            if(me==rhs.me)return (point==rhs.point);
            return false;
        }
		bool operator==(const const_iterator &rhs) const {
            if(me==rhs.me)return (point==rhs.point_c);
            return false;
        }
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
            if(me==rhs.me)return (point!=rhs.point);
            return true;
        }
		bool operator!=(const const_iterator &rhs) const {
            if(me==rhs.me)return (point!=rhs.point_c);
            return true;
        }

		/**
		 * for the support of it->first. 
		 * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
		 */
		map::value_type* operator->() const noexcept {
              return point->data;
        }
	};
	class const_iterator {
        friend class iterator;
		// it should has similar member method as iterator.
		//  and it should be able to construct from an iterator.
		private:
			const Node*point_c = nullptr;
            const map*me = nullptr;
            bool isend = 0;
		public:
			const_iterator() {
				point_c = nullptr;
			}
			const_iterator(const const_iterator &other) {
				point_c = other.point_c;
                me = other.me;
                isend = other.isend;
			}
			const_iterator(const iterator &other) {
				point_c = other.point;
                me = other.me;
                isend = other.isend;
			}
            const_iterator(Node* p, const map*m,int isend_ = 0):point_c(p),me(m),isend(isend_){}
            const_iterator operator++(int) {
                const_iterator p = *this;
                if(isend) throw invalid_iterator();
                if(point_c->rs!= nullptr){
                    point_c = point_c->rs;
                    while(point_c->ls!= nullptr)point_c = point_c->ls;
                }
                else{
                    while(point_c->pa != nullptr && point_c != point_c->pa->ls)point_c = point_c->pa;
                    if(point_c->pa != nullptr) point_c = point_c->pa;
                    else {//找不到后继
                        isend = true;//到达end()
                        point_c = nullptr;
                    }
                }
                return p;
            }
        /**
         * TODO ++iter
         */
        const_iterator & operator++() {
            if(isend) throw invalid_iterator();
            if(point_c->rs!= nullptr){
                point_c = point_c->rs;
                while(point_c->ls!= nullptr)point_c = point_c->ls;
            }
            else{
                while(point_c->pa != nullptr && point_c != point_c->pa->ls)point_c = point_c->pa;
                if(point_c->pa != nullptr) point_c = point_c->pa;
                else {//找不到后继
                    isend = true;//到达end()
                    point_c = nullptr;
                }
            }
            return *this;
        }
        /**
         * TODO iter--
         */
        const_iterator operator--(int) {
//            if(*this==me->begin())throw invalid_iterator();
            const_iterator p = *this;
            if(isend){
                if(me->empty()) throw invalid_iterator();
                isend = false;
                point_c = me->Isend();
                return p;
            }
            if(point_c->ls != nullptr){
                point_c = point_c->ls;
                while(point_c->rs != nullptr) point_c = point_c->rs;
                return p;
            }
            else{//没有左儿子
                while(point_c->pa!= nullptr&&point_c->pa->ls == point_c)point_c = point_c->pa;
                if(point_c->pa!= nullptr) point_c = point_c->pa;
                else throw invalid_iterator();
//                else std::cout << "here_wrong" << endl;

            }
            return p;
        }
        /**
         * TODO --iter
         */
        const_iterator & operator--() {
//            if(*this==me->begin())throw invalid_iterator();
            if(isend){
                if(me->empty()) throw invalid_iterator();
                isend = false;
                point_c = me->Isend();
                return *this;
            }
            if(point_c->ls != nullptr){
                point_c = point_c->ls;
                while(point_c->rs != nullptr) point_c = point_c->rs;
            }
            else{//没有左儿子
                while(point_c->pa!= nullptr&&point_c->pa->ls == point_c)point_c = point_c->pa;
                if(point_c->pa!= nullptr) point_c = point_c->pa;
                else throw invalid_iterator();
//                else std::cout << "here_wrong" << endl;
            }
            return *this;
        }
        /**
         * a operator to check whether two iterators are same (pointing to the same memory).
         */
        const map::value_type & operator*() const {
            return *(point_c->data);
        }
        bool operator==(const iterator &rhs) const {
            if(me==rhs.me)return (point_c==rhs.point);
            return false;
        }
        bool operator==(const const_iterator &rhs) const {
            if(me==rhs.me)return (point_c==rhs.point_c);
            return false;
        }
        /**
         * some other operator for iterator.
         */
        bool operator!=(const iterator &rhs) const {
            if(me==rhs.me)return (point_c!=rhs.point);
            return true;
        }
        bool operator!=(const const_iterator &rhs) const {
            if(me==rhs.me)return (point_c!=rhs.point_c);
            return true;
        }

        /**
         * for the support of it->first.
         * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
         */
        const map::value_type* operator->() const noexcept {
              return point_c->data;
        }
	};
	/**
	 * TODO two constructors
	 */
	map() {
        root = nullptr;
        current_size = 0;
    }
	map(const map &other) {
        root = Creat(other.root, nullptr);
        current_size = other.current_size;
    }
	/**
	 * TODO assignment operator
	 */
	map & operator=(const map &other) {
        if(&other == this) return *this;
        Destroy(root);
        root = Creat(other.root, nullptr);
        current_size = other.current_size;
        return *this;
    }
	/**
	 * TODO Destructors
	 */
	~map() {
        Destroy(root);
    }
	/**
	 * TODO
	 * access specified element with bounds checking
	 * Returns a reference to the mapped value of the element with key equivalent to key.
	 * If no such element exists, an exception of type `index_out_of_bound'
	 */
	T & at(const Key &key) {
        Node *x = find_(key,root);
        if(x == nullptr) throw index_out_of_bound();
        return x->data->second;
    }
	const T & at(const Key &key) const {
        Node *x = find_(key,root);
        if(x == nullptr) throw index_out_of_bound();
        return x->data->second;
    }
	/**
	 * TODO
	 * access specified element 
	 * Returns a reference to the value that is mapped to a key equivalent to key,
	 *   performing an insertion if such key does not already exist.
	 */
	T & operator[](const Key &key) {
        Node*x = find_(key,root);
        if(x == nullptr){
            ++current_size;
            value_type node(key,T());
            insert_(root,node, nullptr);
            Node*y = find_(key,root);
            return y->data->second;
        }
        else return x->data->second;
    }
	/**
	 * behave like at() throw index_out_of_bound if such key does not exist.
	 */
	const T & operator[](const Key &key) const {
        Node*x = find_(key,root);
        if(x == nullptr)throw index_out_of_bound();
        return x->data->second;
    }
	/**
	 * return a iterator to the beginning
	 */
	iterator begin() {
        if(!current_size){
            iterator p(nullptr,this,1);
            return p;
        }
        Node*tmp = root;
        while(tmp->ls != nullptr) tmp = tmp->ls;
        iterator p(tmp,this);
        return p;
    }
	const_iterator cbegin() const {
        if(!current_size){
            const_iterator p(nullptr,this,1);
            return p;
        }
        Node*tmp = root;
        while(tmp->ls != nullptr) tmp = tmp->ls;
        const_iterator p(tmp,this,0);
        return p;
    }
	/**
	 * return a iterator to the end
	 * in fact, it returns past-the-end.
	 */
	iterator end()const {
        iterator p(nullptr,this,1);
        return p;
    }
	const_iterator cend() const {
        const_iterator p(nullptr,this,1);
        return p;
    }
	/**
	 * checks whether the container is empty
	 * return true if empty, otherwise false.
	 */
	bool empty() const {
        return (current_size == 0);
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
        Destroy(root);
        root = nullptr;
        current_size = 0;
    }
	/**
	 * insert an element.
	 * return a pair, the first of the pair is
	 *   the iterator to the new element (or the element that prevented the insertion), 
	 *   the second one is true if insert successfully, or false.
	 */
	pair<iterator, bool> insert(const value_type &value) {
        Node*x = find_(value.first,root);
        iterator p(x,this);
        if(x== nullptr){
            ++current_size;
             insert_(root,value, nullptr);
             x = find_(value.first,root);
            iterator p0(x,this);
            pair<iterator, bool> ai(p0,true);
            return ai;
        }
        pair<iterator, bool> ai(p,false);
        return ai;
    }
	/**
	 * erase the element at pos.
	 *
	 * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
	 */
	void erase(iterator pos) {
        if(pos.isend || pos.me != this) throw invalid_iterator();
        remove(root,pos.point->data->first);
        --current_size;
    }
	/**
	 * Returns the number of elements with key 
	 *   that compares equivalent to the specified argument,
	 *   which is either 1 or 0 
	 *     since this container does not allow duplicates.
	 * The default method of check the equivalence is !(a < b || b > a)
	 */
	size_t count(const Key &key) const {
        Node *x = find_(key,root);
        return (x== nullptr)?0:1;
    }
	/**
	 * Finds an element with key equivalent to key.
	 * key value of the element to search for.
	 * Iterator to an element with key equivalent to key.
	 *   If no such element is found, past-the-end (see end()) iterator is returned.
	 */
	iterator find(const Key &key) {
        Node*x = find_(key,root);
        if(x == nullptr) {
            iterator p(nullptr,this,1);
            return p;
        }
        iterator p(x,this);
        return p;
    }
	const_iterator find(const Key &key) const {
        Node*x = find_(key,root);
        if(x == nullptr) {
            const_iterator p(nullptr,this,1);
            return p;
        }
        const_iterator p(x,this);
        return p;
    }
private:
    friend class iterator;
    friend class const_iterator;
    Node *root = nullptr;
    Compare compare;
    size_t current_size = 0;
    Node* Isend()const{
        Node *tmp = this->root;
        while(tmp->rs != nullptr)tmp = tmp->rs;
        return tmp;
    }
    Node* find_  (const Key &key,Node* const x)const{//return nullptr if not found
        if(x == nullptr) return nullptr;
        if(!compare(key,x->data->first)&&!compare(x->data->first,key)) return x;
        if(compare(key,x->data->first))return find_(key,x->ls);
        else return find_(key,x->rs);
    }
    Node* Creat(Node *t,Node*p){
        Node *tmp = nullptr;
        if(t == nullptr) return tmp;
        tmp = new Node(*(t->data),p,t->height);
        if(t->ls!= nullptr) tmp->ls = Creat(t->ls,tmp);
        if(t->rs != nullptr) tmp->rs = Creat(t->rs,tmp);
        return tmp;
    }
    void Destroy(Node *t){
        if(t == nullptr) return ;
        Destroy(t->rs);
        Destroy(t->ls);
        delete t;
        t = nullptr;
    }
    int getHigh(Node*x){
        if(x == nullptr) return 0;
        return x->height;
    }

    int max(int x,int y){return (x > y)?x:y;}
    inline void RightPlus(Node *p) {//RR情况
        bool isroot = false;
        Node *tmp = p->rs;
        tmp->pa = p->pa;
        if(p->pa!= nullptr){
            if(p==p->pa->ls) p->pa->ls = tmp;
            else p->pa->rs = tmp;//根节点
        }
        else isroot = true;
        p->pa = tmp;
        p->rs = tmp->ls;
        if(tmp->ls!= nullptr)tmp->ls->pa = p;
        tmp->ls = p;
        p->height = max(getHigh(p->ls), getHigh(p->rs)) + 1;
        tmp->height = max(getHigh(tmp->ls),getHigh(tmp->rs)) + 1;
        if(isroot) root = tmp;
    }
        inline void LeftPlus(Node *p) {//LL情况
        bool isroot = false;
            Node *tmp = p->ls;
            tmp->pa = p->pa;
            if(p->pa!= nullptr){
                if(p==p->pa->ls) p->pa->ls = tmp;
                else p->pa->rs = tmp;//根节点
            }
            else isroot = true;
            p->pa = tmp;
            p->ls = tmp->rs;
            if(tmp->rs!= nullptr)tmp->rs->pa= p;
            tmp->rs = p;
            p->height = max(getHigh(p->ls), getHigh(p->rs)) + 1;
            tmp->height = max(getHigh(tmp->ls),getHigh(tmp->rs)) + 1;
            if(isroot) root = tmp;
        }
    inline void LeftRight(Node *p) {
        RightPlus(p->ls);
        LeftPlus(p);
    }

    inline void RightLeft(Node *p) {
        LeftPlus(p->rs);
        RightPlus(p);
    }
    void insert_(Node *&p, map::value_type x, Node* pa_) {
        if (p == nullptr) {
            p = new Node(x,pa_);
        }
        else if (compare(p->data->first,x.first)) {//插入右子树
            insert_(p->rs,x,p);
            if (abs(getHigh(p->rs) - getHigh(p->ls)) == 2) {
                if (compare(p->rs->data->first,x.first)) {//RR
                    RightPlus(p);
                } else {
                    RightLeft(p);
                }
            }
        } else {//插入左子树
            insert_(p->ls, x,p);
            if (abs(getHigh(p->rs) - getHigh(p->ls)) == 2) {
                if (compare(p->ls->data->first,x.first)) {//LR
                    LeftRight(p);
                } else {
                    LeftPlus(p);
                }
            }
        }
        p->height = max(getHigh(p->ls), getHigh(p->rs)) + 1;
    }
    bool remove(Node *p, const Key x) {//返回值代表删除后高度是否变矮(false)
        bool isroot = false;
        if(!compare(x,p->data->first)&&!compare(p->data->first,x)){
            if(p->ls== nullptr||p->rs== nullptr){//叶节点或有一个孩子
                Node*tmp = p;
                if(p->pa!= nullptr){
                    if(p->pa->ls==p)p->pa->ls = (p->ls== nullptr)?p->rs:p->ls;
                    else p->pa->rs = (p->ls== nullptr)?p->rs:p->ls;
                }
                else isroot = true;
                p = (p->ls== nullptr)?p->rs:p->ls;
                if(p!= nullptr)p->pa = tmp->pa;//父节点
                delete tmp;
                if(isroot) root = p;
                return false;//todo 高度改变的话？
            }
            else{//有两个儿子，找左子树最大，如果子树高度变化，则有可能导致平衡因子改变，需要adjust
                Node*tmp = p->ls;
                while(tmp->rs != nullptr)tmp = tmp->rs;
                delete p->data;
                p->data = new value_type (*(tmp->data));
                tmp->height = p->height;
                p->height = 1;
                if(tmp->pa == p){
                    tmp->pa = p->pa;
                    Node*left = tmp->ls;
                    tmp->ls = p;
                    tmp->rs = p->rs;
                    if(p->pa!= nullptr){//p为根节点
                        if(p->pa->ls==p)p->pa->ls = tmp;
                        else p->pa->rs = tmp;
                    }
                    else isroot = true;
                    p->pa = tmp;
                    p->rs->pa = tmp;
                    p->ls = left;
                    p->rs = nullptr;
                }
                else{
                    tmp->pa->rs = p;
                    Node *p0 = tmp->pa;
                    tmp->pa = p->pa;
                    Node*left = tmp->ls;
                    tmp->ls = p->ls;
                    tmp->rs = p->rs;
                    if(p->pa != nullptr){
                        if(p->pa->ls == p)p->pa->ls = tmp;
                        else p->pa->rs = tmp;
                    }
                    else isroot = true;
                    p->pa = p0;
                    p->ls->pa = tmp;
                    p->rs->pa = tmp;
                    p->ls = left;
                    p->rs = nullptr;
                }
                if(remove(tmp->ls,tmp->data->first)){
                    if(isroot) root = tmp;
                    return true;
                }//删除了节点以后左子树没有变矮-不用调整
                return   adjust(tmp,0,isroot);
            }
        }
        else if(compare(p->data->first,x)){
            if(remove(p->rs,x))return true;//删除了节点以后右子树没有变矮-不用调整
            return adjust(p,1,isroot);
        }
        else {
            if(remove(p->ls,x))return true;//删除了节点以后左子树没有变矮-不用调整
            return adjust(p,0,isroot);
        }
    }
    bool adjust(Node*p,int rs,bool isroot){
        if(!rs){//在左子树上删除
            if(abs(getHigh(p->rs) - getHigh(p->ls)==1)){if(isroot)root = p;return true;}
            if(getHigh(p->rs) == getHigh(p->ls)){--p->height;if(isroot)root = p;return false;}
            if(getHigh(p->rs->rs) < getHigh(p->rs->ls)){
                RightLeft(p);
                return false;
            }
            RightPlus(p);
            if(getHigh(p->ls)== getHigh(p->rs))return false;
            else return true;
        }
        else {
            if(abs(getHigh(p->ls) - getHigh(p->rs))==1){if(isroot)root = p;return true;}
            if(getHigh(p->rs) == getHigh(p->ls)){--p->height;if(isroot)root = p;return false;}
            if(getHigh(p->ls->ls) < getHigh(p->ls->rs)){
                LeftRight(p);
                return false;
            }
            LeftPlus(p);
            if(getHigh(p->ls)== getHigh(p->rs))return false;
            else return true;
        }
    }



};

}

#endif
