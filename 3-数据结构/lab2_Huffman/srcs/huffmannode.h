#pragma once
#include <string>
#include <queue>
#include <iostream>
#include <vector>

using namespace std;

#define uint unsigned

class HuffmanNode;
class typenode {
public:
    HuffmanNode* node = nullptr;
    uint tag = 0;
    typenode(HuffmanNode* node, uint tag) {
        this->node = node;
        this->tag = tag;
    }
};

class HuffmanNode{
public:
    uint weight = 0;                      //权重
    uint name = 0;                        //表示的字符(uint序号)
    std::string code = "";                //其编码
    std::vector<HuffmanNode*> child;      //孩子链表
    uint fork_num = 2;                    //n叉树
    uint child_num = 0;                   //指示此节点孩子数量，同时也指示下一个插入的孩子的下标
    bool is_virtual = false;

    //非叶子节点初始化函数
    HuffmanNode(uint weight, uint fork_num, bool is_virtual = false) {
        this->weight = weight;
        this->fork_num = fork_num;
        if (is_virtual) this->is_virtual = true;
        for (int i = 0; i < fork_num; i++) child.push_back(nullptr);
    }
    //叶子节点初始化函数
    HuffmanNode(uint weight, uint name, uint fork_num, bool is_virtual = false) {
        this->weight = weight;
        this->name = name;
        this->fork_num = fork_num;
        if (is_virtual) this->is_virtual = true;
        for (int i = 0; i < fork_num; i++) child.push_back(nullptr);
    }
    //层序遍历
    void FloorTraverse(HuffmanNode* node) {
        queue<typenode*> Q;
        uint nowtag = 0;
        if (node) Q.push(new typenode(node, nowtag));
        while (!Q.empty()) {
            typenode* tmp = Q.front();
            Q.pop();
            if (tmp->tag != nowtag) {
                nowtag++;
                cout << endl;
            }
            cout << tmp->node->weight << "  ";
            for (int i = 0; i < tmp->node->fork_num; i++) {
                if (tmp->node->child[i]) Q.push(new typenode(tmp->node->child[i], nowtag + 1));
            }
        }
    }
    //插入孩子，返回是否成功插入
    bool InsertChild(HuffmanNode* node) {
        if (child_num < fork_num) {
            child[child_num] = node;
            child_num++;
            return true;
        }
        return false;
    }
    //把当前节点的权重计算为所有孩子的权重之和
    void CaculateWeight() {
        weight = 0;
        for (uint i = 0; i < child_num; i++) {
            weight += child[i]->weight;
        }
        return;
    }
    //将code所代表的n进制string转化为2进制string
    void Regularize() {
        //目前暂时实现将其转化为2的幂次相关的方法，这里考虑的叉数是3~16，有2，3~4，5~8，9~16四种情况
        if (fork_num == 2) return;
        else if (fork_num == 3 || fork_num == 4) {
            std::string tmp;
            uint size = code.size();
            for (uint i = 0; i < size; i++) {
                switch(code[i]) {
                case '0': tmp += "00"; break;
                case '1': tmp += "01"; break;
                case '2': tmp += "10"; break;
                case '3': tmp += "11"; break;
                }
            }
            code = tmp;
        }
        else if (fork_num >= 5 || fork_num <= 8) {
            std::string tmp;
            uint size = code.size();
            for (uint i = 0; i < size; i++) {
                switch (code[i]) {
                case '0': tmp += "000"; break;
                case '1': tmp += "001"; break;
                case '2': tmp += "010"; break;
                case '3': tmp += "011"; break;
                case '4': tmp += "100"; break;
                case '5': tmp += "101"; break;
                case '6': tmp += "110"; break;
                case '7': tmp += "111"; break;
                }
            }
            code = tmp;
        }
        else {
            std::string tmp;
            uint size = code.size();
            for (uint i = 0; i < size; i++) {
                switch (code[i]) {
                case '0': tmp += "0000"; break;
                case '1': tmp += "0001"; break;
                case '2': tmp += "0010"; break;
                case '3': tmp += "0011"; break;
                case '4': tmp += "0100"; break;
                case '5': tmp += "0101"; break;
                case '6': tmp += "0110"; break;
                case '7': tmp += "0111"; break;
                case '8': tmp += "1000"; break;
                case '9': tmp += "1001"; break;
                case 'A': tmp += "1010"; break;
                case 'B': tmp += "1011"; break;
                case 'C': tmp += "1100"; break;
                case 'D': tmp += "1101"; break;
                case 'E': tmp += "1110"; break;
                case 'F': tmp += "1111"; break;
                }
            }
            code = tmp;
        }
    }
};

