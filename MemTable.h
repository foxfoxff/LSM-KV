//
// Created by huyao on 2021/4/17.
//

#ifndef LSM_MEMTABLE_H
#define LSM_MEMTABLE_H
#include <cstdint>
#include <string>
#include <vector>

const unsigned long MAXSIZE = 2097152;
using namespace std;
struct Node{
    Node *right,*down;   //向右向下足矣
    uint64_t key;
    string val;
    Node(Node*right,Node *down, uint64_t key,  string val): right(right), down(down), key(key), val(val){}
    Node(): right(nullptr), down(nullptr) {}
};

class MemTable {
private:
    uint64_t tosize;
    uint64_t numsize;
public:
    Node *head;
    MemTable() {
        head = new Node();  //初始化头结点
        tosize = 10272;
        numsize = 0;
    }

    size_t size(){
/*        Node *r_pointer=head;
        Node *d_pointer=head;

        size_t num=0;
        while (d_pointer){
            d_pointer=d_pointer->down;
            if(!d_pointer) break;
            r_pointer=d_pointer->right;
            if(!d_pointer->down){
                while(r_pointer){
                    num++;
                    r_pointer=r_pointer->right;
                }
            }

        }*/

        return numsize;
    }

    string* get(const uint64_t& key) {
        if(!head->down) return nullptr;
        Node *d_pointer=head;
        Node *r_pointer;
        bool flag=false;
        while (d_pointer)
        {
            d_pointer=d_pointer->down;
            if(!d_pointer) break;
            r_pointer=d_pointer->right;
            while(r_pointer&&r_pointer->key<=key){
                if(r_pointer->key==key) {flag=true;break;}
                r_pointer=r_pointer->right;
            }
            if(flag==true) break;
        }
        if(flag) {
            if(r_pointer->val==" ~DELETED~") return nullptr;
            string*ret =new string(r_pointer->val);
            return ret;
        }

        return nullptr;
    }

    void put(const uint64_t& key, const string& val,bool count= true) {
        bool cover=false;
        vector<Node*> pathList;    //从上至下记录搜索路径
        Node *p = head;
        while(p){
            while(p->right && p->right->key < key){
                p = p->right;
            }

            pathList.push_back(p);
            if(p&&p->right&&p->right->key==key){
                cover = true;
                break;
            }
            p = p->down;
        }
       if(cover){
           Node *change=pathList.front()->right;
           while(change){
               change->val=val;
               change=change->down;
           }
           return;
       }

        bool insertUp = true;
        Node* downNode= nullptr;
        while(insertUp && pathList.size() > 0){   //从下至上搜索路径回溯，50%概率
            Node*insert = pathList.back();
            pathList.pop_back();
            insert->right = new Node(insert->right, downNode, key, val); //add新结点
            downNode = insert->right;    //把新结点赋值为downNode
            insertUp = (rand()&1);   //50%概率
        }
        if(insertUp){  //插入新的头结点，加层
            Node * oldHead = head;
            head = new Node();
            head->right = new Node(NULL, downNode, key, val);
            head->down = oldHead;
        }
        if(count){
            numsize++;
            tosize+=val.size();
        }
    }


    bool remove(const uint64_t& key){
        bool flag =false;
        Node*p=head;
        vector<Node*> pathList;    //从上至下记录搜索路径
        while(p){
            while(p->right && p->right->key < key){
                p = p->right;
            }
            if(p&&p->right&&p->right->key==key){   //p->right是要删除的节点
                flag=true;
                break;
            }
            p = p->down;
        }
        if(!flag) {
            put(key,"~DELETED~", false);
            return false;
        }
        Node *downNode=p->right;
        Node *tmp=downNode;
        downNode=tmp->down;
        p->right=tmp->right;
        delete tmp;
        while (downNode)
        {
            p=p->down;
            while (p->right!=downNode)
            {
                p=p->right;
            }
            Node *tmp=downNode;
            downNode=tmp->down;
            p->right=tmp->right;
            delete tmp;
        }
        put(key,"~DELETED~", false);
        return true;

    }
    Node *getkeys(){
        Node *p=head;
        while(!p->down){
            p=p->down;
        }
        return p;
    }

};


#endif //LSM_MEMTABLE_H
