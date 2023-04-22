#pragma once
#include "huffmannode.h"
#include <iostream>
#include <QDebug>

#define uint unsigned

class Heap{
public:
    HuffmanNode** element_address = nullptr;
    uint capacity = 0;//载量
    uint space_used = 0;//已用空间

    //注意堆初始化有载量
    Heap(uint capacity) {
        this->capacity = capacity;
        this->element_address = new HuffmanNode * [capacity + 1];//从1开始储存数据比较好
        for (uint i = 0; i < capacity + 1; i++) {
            element_address[i] = nullptr;
        }
        this->space_used = 0;
    }
    //交换函数，已经封装好
    void swap(HuffmanNode* a, HuffmanNode* b) {
        uint tmp_weight = a->weight;
        uint tmp_name = a->name;
        std::vector<HuffmanNode*> tmp_child = a->child;
        uint tmp_fork_num = a->fork_num;
        uint tmp_child_num = a->child_num;
        bool tmp_is_virtual = a->is_virtual;

        a->weight = b->weight;
        b->weight = tmp_weight;
        a->name = b->name;
        b->name = tmp_name;
        a->child_num = b->child_num;
        b->child_num = tmp_child_num;
        a->is_virtual = b->is_virtual;
        b->is_virtual = tmp_is_virtual;

        for (int i = 0; i < tmp_fork_num; i++) {
            a->child[i] = b->child[i];
            b->child[i] = tmp_child[i];
        }
    }
    //插入函数，已经封装好
    bool insert(HuffmanNode* node) {
        if (space_used < capacity) {
            this->element_address[space_used + 1] = node;
            for (uint i = space_used + 1; i / 2 > 0 && element_address[i]->weight < element_address[i / 2]->weight; i >>= 1) {
                swap(element_address[i], element_address[i / 2]);
            }
            this->space_used++;
            return true;
        }
        return false;
    }
    //获取堆顶元素并从堆中删除
    HuffmanNode* pop() {
        HuffmanNode* returnvalue = element_address[1];
        element_address[1] = element_address[space_used];
        element_address[space_used] = nullptr;
        space_used--;
        heapify();
        return returnvalue;
    }
    //堆化
    void heapify() {
        for (uint i = space_used / 2; i >= 1; i--) {
            while (1) {
                uint maxPos = i;
                if (i * 2 <= space_used && element_address[i]->weight > element_address[i * 2]->weight)maxPos = i * 2;
                if (i * 2 + 1 <= space_used && element_address[maxPos]->weight > element_address[i * 2 + 1]->weight)maxPos = i * 2 + 1;
                if (maxPos == i)break;
                swap(element_address[i], element_address[maxPos]);
                i = maxPos;
            }
        }
    }
    //是否为空
    bool is_empty() {
        if (space_used)return false;
        return true;
    }
    //展示堆
    void display() {
        string s = "[";
        for (uint i = 1; i <= space_used; i++) {
            s += to_string(element_address[i]->weight);
            if (i != space_used)s += ",";
        }
        s += "]";
        qDebug() << s.c_str() << endl;
    }
};

