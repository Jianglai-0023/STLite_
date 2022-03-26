#ifndef SJTU_PRIORITY_QUEUE_HPP
#define SJTU_PRIORITY_QUEUE_HPP

#include <cstddef>
#include <functional>
#include "exceptions.hpp"

namespace sjtu {
/**
 * a container like std::priority_queue which is a heap internal.
 */

    template<typename T, class Compare = std::less<T>>
    class priority_queue {
    public:
        struct Node {
            Node *ls_ = nullptr;
            Node *rs_ = nullptr;
            T item_;
            int npl_ = 0;//-1 if node is empty
            Node(){
            }
            Node(T item, Node*l = nullptr, Node*r = nullptr,int npl = 0):item_(item),ls_(l),rs_(r),npl_(npl){
            }
            ~Node(){
            }
        };
        void swap0(Node *&x, Node *&y){
            Node *tmp;
            tmp = x;
            x = y;
            y = tmp;
        };
        Compare cmp;
        Node *root = nullptr;
        size_t size0 = 0;//size = 0 with root = nullptr
        Node *Creat(Node *n){//建树操作
           Node *tmp = new Node(n->item_, nullptr,nullptr,n->npl_);
           if(n->rs_ != nullptr) tmp->rs_ = Creat(n->rs_);
            if(n->ls_ != nullptr) tmp->ls_ = Creat(n->ls_);
            return tmp;//todo
        }
        void Clear(Node *t){
            if(t == nullptr) return;
            Clear(t->rs_);
            Clear(t->ls_);
            delete t;
            t = nullptr;
        }
        Node *Merge(Node *r1,Node *r2){
            if(r1 == nullptr) return r2;
            else if(r2 == nullptr) return r1;
            else{
                if(cmp(r1->item_,r2->item_)){//r1 < r2
                    r2->rs_ = Merge(r1,r2->rs_);
                    if(r2->ls_== nullptr||r2->rs_->npl_ > r2->ls_->npl_)swap0(r2->ls_,r2->rs_);
                    if(r2->rs_== nullptr) r2->npl_ = 0;
                    else r2->npl_ = r2->rs_->npl_ + 1;
                    return r2;
                }
                else{
                    r1->rs_ = Merge(r2,r1->rs_);
                    if(r1->ls_== nullptr||r1->rs_->npl_ > r1->ls_->npl_)swap0(r1->ls_,r1->rs_);
                    if(r1->rs_== nullptr) r1->npl_ = 0;
                    else r1->npl_ = r1->rs_->npl_ + 1;
                    return r1;
                }
            }
        }


    public:

    /**
     * TODO constructors
     */
    priority_queue() {
//        root = new Node;

    }

    priority_queue(const priority_queue &other) {
        root = Creat(other.root);
        size0 = other.size0;
    }
    /**
     * TODO deconstructor
     */
    ~priority_queue() {
        Clear(root);
    }

    /**
     * TODO Assignment operator
     */
    priority_queue &operator=(const priority_queue &other) {
        if(&other == this) return *this;
        Clear(root);
        root = Creat(other.root);
        size0 = other.size0;
        return *this;
    }

    /**
     * get the top of the queue.
     * @return a reference of the top element.
     * throw container_is_empty if empty() returns true;
     */
    const T &top() const {
        if(empty())throw container_is_empty();
         return root->item_;
    }

    /**
     * TODO
     * push new element to the priority queue.
     */
    void push(const T &e) {
        Node*tmp = new Node(e, nullptr, nullptr,0);
        root = Merge(root,tmp);
        ++size0;
    }

    /**
     * TODO
     * delete the top element.
     * throw container_is_empty if empty() returns true;
     */
    void pop() {
        if(empty()) throw container_is_empty();
        Node *tmp = root;
        root = Merge(tmp->rs_,tmp->ls_);
        delete tmp;
        --size0;
    }

    /**
     * return the number of the elements.
     */
    size_t size() const {
        return size0;
    }

    /**
     * check if the container has at least an element.
     * @return true if it is empty, false if it has at least an element.
     */
    bool empty() const {
        if(size0 == 0) return true;
        return false;
    }

    /**
     * merge two priority_queues with at least O(logn) complexity.
     * clear the other priority_queue.
     */
     void print(Node *t){
         if(t == nullptr) return;
        std::cout << t->item_ << std::endl;
        print(t->ls_);
        print(t->rs_);
     }
    void merge(priority_queue & other) {
        root = Merge(root,other.root);
        size0 += other.size0;
        other.root = nullptr;
        other.size0 = 0;
    }
};

}

#endif
