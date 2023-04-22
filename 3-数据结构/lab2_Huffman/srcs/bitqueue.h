#pragma once
#include <deque>
#include <string>
#define uint unsigned

using namespace std;

class BitQueue : public deque<bool> {
public:
    uint def_byte = 8;
    /* BitQueue是一个01串队列，其提供两种接口
     * push_b_s(string s)将一个01 string类型字符串压入队列中
     * pop_b_s()从队列中弹出一个“字节”并返回这个字节string类型字符串，如果不足一个“字节”，则返回的字符串长度不足一个“字”长
     */
    BitQueue(uint def_of_byte){
        this->def_byte = def_of_byte;
    }
    void push_b_s(string s) {
        uint n = s.size();
        for (uint i = 0; i < n; i++) {
            if (s[i] == '1') {
                this->push_back(true);
            }
            else {
                this->push_back(false);
            }
        }
    }
    string pop_b_s() {
        //弹出一个字节，如果
        string ans;
        uint n = this->size();
        if (n >= 8) {
            for (uint j = 0; j < 8; j++) {
                ans += (this->front()) ? '1' : '0';
                this->pop_front();
            }
        }
        //不足一个字节，把不足一个字节的留在这个队列中
        return ans;
    }
    string pop_b_s(uint len) {
        //弹出len bit的一个字符串，如果
        string ans;
        uint n = this->size();
        if (n >= len) {
            for (uint j = 0; j < len; j++) {
                ans += (this->front()) ? '1' : '0';
                this->pop_front();
            }
        }
        //不足一个字节，把不足一个字节的留在这个队列中
        return ans;
    }
    //弹出一位
    char pop_bit() {
        char ans = '0';
        if (this->front()) ans = '1';
        this->pop_front();
        return ans;
    }
};

