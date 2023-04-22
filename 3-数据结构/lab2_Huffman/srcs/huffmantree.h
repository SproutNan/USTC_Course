#pragma once
#include <map>
#include <string>
#include <cmath>
#include <QDebug>
#include <fstream>
#include <stack>
#include "huffmannode.h"
#include "heap.h"

using namespace std;

#define CS_BYTE_BITS 8
#define uint unsigned

class HuffmanTree{
public:
    uint fork_num = 2;//叉数

    map<uint, string> code_map;
    //map, first->字符代表的数字，second->字符被编的码
    
    HuffmanNode* root = nullptr;
    
    //[MARK]下面这个函数还未重构！！
    HuffmanTree(uint Scan_bits, uint fork_num) {
        this->fork_num = fork_num;
        this->header_bytes = 6;
        this->byte_type = Scan_bits;
        this->root = new HuffmanNode(0, 9999, fork_num);//huffman树的根节点，不储存任何信息
        if (byte_type % CS_BYTE_BITS) {
            extra_bits = CS_BYTE_BITS / 2;
        }
        else {
            extra_bits = 0;
        }
        //如果是.5个字节作为一个字，则末尾多半个字节未使用
    }

    //将此树全部编码，存于code[i]中
    void encode_helper(HuffmanNode* node) {
        uint fork_num = node->fork_num;
        for (uint i = 0; i < fork_num; i++) {
            if (node->child[i]) {
                if (fork_num <= 10) {
                    node->child[i]->code = node->code + to_string(i);
                }
                else {
                    if (i < 10) {
                        node->child[i]->code = node->code + to_string(i);
                    }
                    else {
                        node->child[i]->code = node->code + (char)(i - 10 + 'A');
                    }
                }
                encode_helper(node->child[i]);
            }
        }
        return;
    }
    void insert_encoded_leaf_to_map(HuffmanNode* node) {
        if (node) {
            if (node->child_num == 0) {
                if (!node->is_virtual) {
                    node->Regularize();
                    code_map.insert(pair<uint, string>(node->name, node->code));
                }
                return;
            }
            else {
                for (uint i = 0; i < node->fork_num; i++) {
                    insert_encoded_leaf_to_map(node->child[i]);
                }
            }
        }
        return;
    }
    void encode(HuffmanNode* node) {
        encode_helper(node);
        insert_encoded_leaf_to_map(node);
    }
    //根据堆h来建n叉树
    void build(Heap* h, uint n) {
        //首先，要根据huffman树的特点来补节点
        uint num_need_to_comple_to_heap = h->space_used % (n - 1);
        if (num_need_to_comple_to_heap) {
            num_need_to_comple_to_heap = n - num_need_to_comple_to_heap;
        }
        while (num_need_to_comple_to_heap--) {
            h->insert(new HuffmanNode(0, 9898 + num_need_to_comple_to_heap, n, true));//插入虚拟节点
        }
        int tmp_cnt = 1280;
        //如果h中还有n个以上，则可以弄一个完整的节点出来
        while (h->space_used > n) {
            auto new_node = new HuffmanNode(0, tmp_cnt++, n);
            for (uint i = 0; i < n; i++) {
                new_node->InsertChild(h->pop());
            }
            new_node->CaculateWeight();//把当前节点的权重计算为所有孩子的权重之和
            h->insert(new_node);
        }
        //现在堆中只有n个节点
        if (h->space_used == n) {
            for (uint i = 0; i < n; i++) {
                root->InsertChild(h->pop());
            }
            root->CaculateWeight();
        }
        else {
            exit(-1);
        }
        return;
    }
    //展示编码
    void display_encode() {
        for (auto i : code_map) {
            if (i.second.length()) {
                qDebug() << "the[" << i.first << "]is encoded as[" << i.second.c_str() << "]." << endl;
            }
        }
    }
    //[MARK]下面这个函数还未重构！！！统计存这个huffman树需要多少额外的字节
    void stat_extra_bytes() {
        //先统计有多少字符被编码了
        header_bytes += code_map.size() * 9;
        //一个编码需要2n个字节存信息：这个字符序号，和这个字符的码长，n是所认为的“字节”数
        uint bits = 0;
        for (auto i : code_map) {
            bits += i.second.length();
        }
        header_bytes += bits / CS_BYTE_BITS;
        extra_bits += bits % CS_BYTE_BITS;//不要忘了写入文件的时候还是一个字节一个字节的……
    }
    //将树保存成html代码以供展示
    void save_tree(string new_file_name) {
        int size = new_file_name.size();
        for (int i = size - 1; i >= 0; i--) {
            if (new_file_name[i] == '\\') {
                size = i;
                break;
            }
        }
        new_file_name = new_file_name.substr(0, size + 1) + "HuffmanTree.html";
        ofstream new_file(new_file_name.c_str(), ios::binary | ios::out);

        fstream head("head.txt", ios::binary | ios::in);
        while (1) {
            char ch = head.get();
            if (ch != EOF)new_file.put(ch);
            else break;
        }
        stack<HuffmanNode*> stk;
        stk.push(root);
        while (stk.size()) {
            auto p = stk.top();
            stk.pop();
            for (uint i = 0; i < fork_num; i++) {
                if (p->child[i]) {
                    string ss = "{ source: '" + to_string(p->name) + "', target : '" + to_string(p->child[i]->name) + "',  'rela' : '" + to_string(i) + "', type : 'resolved' },\n";
                    stk.push(p->child[i]);
                    for (int j = 0; j < ss.size(); j++) new_file.put(ss[j]);
                }
            }
        }
        fstream tail("tail.txt", ios::binary | ios::in);
        while (1) {
            char ch = tail.get();
            if (ch != EOF)new_file.put(ch);
            else break;
        }
    }
    //下面这些不知道是干什么的，看不懂了呜呜呜呜
    uint ceiling_type() {
        if (byte_type == 4 || byte_type == 8)return 1;
        if (byte_type == 12 || byte_type == 16)return 2;
        if (byte_type == 20 || byte_type == 24)return 3;
        if (byte_type == 28 || byte_type == 32)return 4;
    }
    //计算存序号时需要的字节数
    uint header_bytes = 8;//文件头增加的字节数
    uint extra_bits = 0;//文件尾部多加的0bit数
    uint byte_type = 8;//读取的时候一个字是多少位
};
