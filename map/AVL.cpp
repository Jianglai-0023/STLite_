#include<iostream>
#include<algorithm>
using namespace std;
//struct AVLnode;
//typedef AVLnode *AVLtree;
class AVLtree {
    struct AVLnode {
        int data;
        int height = 1;//权值，树高
        int size = 1;//root本身大小,求排名
        int freq = 1;//data出现频率
        AVLnode *ls = nullptr;
        AVLnode *rs = nullptr;
        AVLnode *pa = nullptr;
        AVLnode() : data(0), height(1),size(1), ls(nullptr), rs(nullptr) {}
        AVLnode(int a,AVLnode* pa_) : data(a),pa(pa_){};
//        AVLnode(int data,int height,)
    };
public:
    void Insert(int data_){
        insert(root,data_, nullptr);
    }

    int DtoR(int data_,AVLnode* p){//按值找排名
        if(p == nullptr) return 0;
        if(p->data == data_) return getSize(p->ls) + 1;
        if(data_ < p->data) return DtoR(data_,p->ls);
        else return getSize(p->ls) + p->freq + DtoR(data_,p->rs);
    }
    int RtoD(int rank_,AVLnode *p){//按排名找值
        if(p == nullptr) return 0;
        if(rank_ >= getSize(p->ls) +1 && rank_ <= getSize(p->ls) + p->freq){
            return p->data;
        }
        else if(rank_ < getSize(p->ls) +1){
            return RtoD(rank_,p->ls);
        }
        else{
            return RtoD(rank_ - getSize(p->ls)-p->freq,p->rs);
        }
    }
    AVLnode* findp(int data_,AVLnode*&pa,AVLnode*&FRpa,AVLnode*p){
        if(p== nullptr) return nullptr;
        if(p->data==data_)return p;//考虑找不到的情况
        pa = p;
        if(data_ < p->data) return findp(data_,pa,FRpa,p->ls);
        else{
            FRpa = p;
            return findp(data_,pa,FRpa,p->rs);
        }
    }
    AVLnode* findPre(int data_){
        AVLnode*parent = nullptr;
        AVLnode*FRparent = nullptr;
        AVLnode*ans = findp(data_,parent,FRparent,root);
        if(ans== nullptr) return FRparent;
        if(ans->ls!= nullptr){//左子树不为空
            AVLnode*tmp = ans->ls;
            while(tmp->rs != nullptr)tmp = tmp->rs;
            return tmp;
        }
        else{//左子树为空
            if(parent->rs == ans) return parent;
            return FRparent;
        }
    }
    AVLnode* findb(int data_,AVLnode*&pa,AVLnode*&FLpa,AVLnode*p){
//        Intraverse(p);
//        cout << endl;
//        cout << p->data << "qwq" << endl;
        if(p == nullptr) return nullptr;
        if(p->data==data_)return p;
        pa = p;
        if(data_<p->data){
            FLpa = p;
            return findb(data_,pa,FLpa,p->ls);
        }
        else{
            return findb(data_,pa,FLpa,p->rs);
        }

    }
    AVLnode* findBac(int data_){
        AVLnode*parent = nullptr;
        AVLnode*FLparent = nullptr;
        AVLnode*ans = findb(data_,parent,FLparent,root);
        if(ans == nullptr) return FLparent;
        if(ans->rs!= nullptr){//右子树不为空
            AVLnode*tmp = ans->rs;
            while(tmp->ls != nullptr)tmp = tmp->ls;
            return tmp;
        }
        else{//左子树为空
            if(parent->ls == ans) return parent;
            return FLparent;
        }
    }
private:

    int getHigh(AVLnode *&p) {
        if (p == nullptr) return 0;
        else return p->height;
    }


    int getSize(AVLnode *&p) {
        if (p == nullptr) return 0;
        else return p->size;
    }

    void update ( AVLnode *p) {
        if(p == nullptr) return ;
        p->size = getSize(p->ls) + getSize(p->rs) + p->freq;//p——root大小为p->ls.size + p->rs.size + freq;
        p->height = max(getHigh(p->ls), getHigh(p->rs)) + 1;
    }
//不用引用
    inline void RightPlus(AVLnode *p) {//RR情况，单旋转，左旋
        bool isroot = false;
        AVLnode *tmp = p->rs;
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
        p->size = getSize(p->ls) + getSize(p->rs) + p->freq;
        tmp->height = max(getHigh(tmp->ls),getHigh(tmp->rs)) + 1;
        tmp->size = getSize(tmp->ls) + getSize(tmp->rs) + tmp->freq;
        if(isroot) root = tmp;
//        p = tmp;
    }

    inline void LeftPlus(AVLnode *p) {//LL情况
        bool isroot = false;
        AVLnode *tmp = p->ls;
        tmp->pa = p->pa;
        if(p->pa!= nullptr){
            if(p==p->pa->ls) p->pa->ls = tmp;
            else p->pa->rs = tmp;
        }
        else isroot = true;
        p->pa = tmp;
        p->ls = tmp->rs;
        if(tmp->rs!= nullptr)tmp->rs->pa= p;
        tmp->rs = p;
        p->height = max(getHigh(p->ls), getHigh(p->rs)) + 1;
        p->size = getSize(p->ls) + getSize(p->rs) + p->freq;
        tmp->height = max(getHigh(tmp->ls),getHigh(tmp->rs)) + 1;
        tmp->size = getSize(tmp->ls) + getSize(tmp->rs) + tmp->freq;
        if(isroot) root = tmp;
//        p = tmp;
    }

    inline void LeftRight(AVLnode *p) {
        RightPlus(p->ls);
        LeftPlus(p);
    }

    inline void RightLeft(AVLnode *p) {
        LeftPlus(p->rs);
        RightPlus(p);
    }
    AVLnode* find(int x){
        AVLnode* tmp = root;
        while(tmp != nullptr && tmp->data != x){
            if(x > tmp->data){
                tmp = tmp->rs;
            }
            else tmp = tmp->ls;
        }
        return tmp;
    }

    void insert(AVLnode *&p, int x,AVLnode*pa) {//insert 可以用引用--直接在某个父节点的ls/rs指针上构造一个new Node；记得新节点要连回去
        if (p == nullptr) {
            p = new AVLnode(x,pa);
        } else if (x == p->data) {
            p->freq++;
        } else if (x > p->data) {//插入右子树
            insert(p->rs, x,p);
            if (abs(getHigh(p->rs) - getHigh(p->ls)) == 2) {
                if (x > p->rs->data) {//RR
                    RightPlus(p);
                } else {
                    RightLeft(p);
                }
            }
        } else {//插入左子树
            insert(p->ls, x,p);
            if (abs(getHigh(p->rs) - getHigh(p->ls)) == 2) {
                if (x > p->ls->data) {//LR
                    LeftRight(p);
                } else {
                    LeftPlus(p);
                }
            }
        }
        update(p);
    }

    bool remove(AVLnode *p, int x) {//返回值代表删除后高度是否变矮(false)
        bool isroot = false;//不要用引用了
        if(p == nullptr){std::cout << "remove nothing"<<std::endl;return true;} //应该没有这种情况
        --p->size;
        if(x == p->data){
            if(p->freq > 1){
                --p->freq;
                return true;
            }
            else if(p->ls== nullptr||p->rs== nullptr){//叶节点或有一个孩子
                AVLnode*tmp = p;
                if(p->pa!= nullptr){
                    if(p->pa->ls==p)p->pa->ls = (p->ls== nullptr)?p->rs:p->ls;
                    else p->pa->rs = (p->ls== nullptr)?p->rs:p->ls;
                }
                else isroot = true;
                p = (p->ls== nullptr)?p->rs:p->ls;
                if(p != nullptr) p->pa = tmp->pa;
                delete tmp;
                if (isroot) root = p;
                return false;
            }
            else{//有两个儿子，找左子树最大，如果子树高度变化，则有可能导致平衡因子改变，需要adjust
                AVLnode*tmp = p->ls;//改变两个指针的位置，实际delete为p指向的那一块地址
                AVLnode* m = p->ls;
                while(tmp->rs != nullptr){
                    tmp = tmp->rs;
                }
//                if(tmp==p)cout << "1&&" << endl;
                p->data = tmp->data;
                p->freq = 1;
                int s = tmp->freq - 1;
                while(m->rs != nullptr){
                    m->size -= s;
                    m = m->rs;
                }
//                tmp->height = p->height;
                tmp->size = p->size;
//                tmp->freq = 1;
                if(tmp->pa == p){
                    tmp->pa = p->pa;
                    AVLnode*left = tmp->ls;
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
                    AVLnode *p0 = tmp->pa;
                    tmp->pa = p->pa;
                    AVLnode*left = tmp->ls;
                    tmp->ls = p->ls;
                    tmp->rs = p->rs;
//                    if(tmp==p)cout << "2&&" << endl;
                    if(p->pa != nullptr){
                        if(p->pa->ls == p)p->pa->ls = tmp;
                        else p->pa->rs = tmp;
                    }
//                    if(tmp==p)cout << "3&&" << endl;
                    else isroot = true;
                    p->pa = p0;
//                    p0->rs = p;
                    p->ls->pa = tmp;
                    p->rs->pa = tmp;
//                    if(tmp==p)cout << "&&" << endl;
                    p->ls = left;
                    p->rs = nullptr;
                }
                if(remove(tmp->ls,tmp->data)){
                    if(isroot)root = tmp;
//                    update(tmp);
                    return true;//删除了节点以后左子树没有变矮-不用调整
                }
                return adjust(tmp,0,isroot);
//                update(tmp);
//                if(isroot)root = tmp;
//                return flag;
//                if(adjust(tmp,0) && isroot){root = tmp;return true;}
//                if(isroot)root = tmp;
//                return adjust(tmp,0);
            }
        }
        else if(x > p->data){
            if(remove(p->rs,x)){
//                update(p);
                return true;//删除了节点以后左子树没有变矮-不用调整
            }
            return adjust(p,1,isroot);
        }
        else {
            if(remove(p->ls,x)){
//               update(p);
                return true;
            }//删除了节点以后右子树没有变矮-不用调整
            return adjust(p,0,isroot);
        }
//        update(p);
    }
    bool adjust(AVLnode*p,int rs,bool isroot){
        if(!rs){//在左子树上删除
            if(getHigh(p->rs) - getHigh(p->ls)==1){if(isroot)root = p;return true;}
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
            if(getHigh(p->ls) - getHigh(p->rs)==1){if(isroot)root = p;return true;}
            if(getHigh(p->rs) == getHigh(p->ls)){if(isroot)root = p;--p->height;return false;}
            if(getHigh(p->ls->ls) < getHigh(p->ls->rs)){
                LeftRight(p);
                return false;
            }
            LeftPlus(p);
            if(getHigh(p->ls)== getHigh(p->rs))return false;
            else return true;
        }
    }
public:
    AVLnode* root = nullptr;
    void Remove(int data_){
//        if(find(data_)== nullptr)
        remove(root,data_);
    }
    void Intraverse(AVLnode *p) {//中序遍历
        if(p == nullptr) return;
        Intraverse(p->ls);
        cout << p->data << ' '<<p->freq <<' ' << p->size << '|' ;
        Intraverse(p->rs);
//        cout << endl;
    }
};
int main(){
    AVLtree A;
    int x;
    int n;
    cin >> n;
    for(int i = 1; i <= n; ++i){
        int opt;
        cin >> opt;
        switch(opt){
            case 1:
                cin >> x;
                A.Insert(x);
//               A.Intraverse(A.root);
//               cout << endl;
                break;
            case 2:
                cin >> x;
                A.Remove(x);
//               A.Intraverse(A.root);
//               cout << endl;
                break;
            case 3:
                cin >> x;
                cout <<A.DtoR(x,A.root) << endl;
                break;

            case 4:
                cin >> x;
                cout << A.RtoD(x,A.root)<< endl;
                break;
            case 5:
                cin >> x;
                if(A.findPre(x)==nullptr) cout << -2147483647 << endl;
                else cout << A.findPre(x)->data << endl;
                break;
            case 6:
                cin >> x;
                if(A.findBac(x)==nullptr) cout << 2147483647 << endl;
                else cout << A.findBac(x)->data << endl;
                break;

            case 7:
                A.Intraverse(A.root);
                cout << endl;
                break;
        }
    }

}