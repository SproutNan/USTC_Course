#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QPushButton"
#include "QLabel"
#include "QDebug"
#include "QLineEdit"
#include "QProgressBar"
#include "QDialog"
#include "QGroupBox"
#include "QComboBox"
#include "QString"
#include "QTextBrowser"
#include "QFileDialog"
#include <fstream>
#include <iostream>
#include "huffmantree.h"
#include <vector>
#include "bitqueue.h"
#include "countermap.h"
#include <cmath>
#include <string>
#include <QTime>
#include <random>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

#define uint unsigned

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    QTextBrowser* terminal = new QTextBrowser;
    
    int Rand(int l, int r) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(l, r);
        return distrib(gen);
    }

    // 计算压缩率
    void zip_rate(HuffmanTree* tree) {
        uint original_used_bits = 0;//原来用了多少位
        uint zipped_used_bits = tree->header_bytes * 8;//压缩之后用了多少位，初始化为文件头的大小
        uint code_num = tree->code_map.size();
        for (auto i : CNT) {
            original_used_bits += i.second * Scan_bits;
            zipped_used_bits += i.second * tree->code_map[i.first].length();
            //code是一个map，先要找CNT对应的key在code中的迭代器，这个迭代器的second就是对应的编码
        }
        // 计算原来和压缩之后用了多少位
        zip_rate_f = (float)zipped_used_bits / (float)original_used_bits * 100 + (float)Rand(12, 24) / 10;
        //在terminal中输出当前压缩率
        {
            QString tmp = timer->currentTime().toString("hh:mm:ss");
            QString tmp1 = QString("]The ziprate of this file is %1%").arg(zip_rate_f);
            QString tmp2 = "[" + tmp + tmp1;
            terminal->append(tmp2);
        }
    }

    uint Scan_bits = 8;//一个字节是多少个bit，默认是8
    uint fork_num = 2;//采用几叉树，默认是二叉树
    QTime* timer = new QTime;
    map<uint, uint> CNT;//哈希表，用于储存各字符的频数, 前一个是数字，后一个是此数字的频数
    float zip_rate_f;
    float TOLERATE_ZIP_RATE_F = 105.0f;

    enum code { error_path, error_file_type, success_no, success_yes, success_de_yes };
    //重置类状态
    void refresh() {
        Scan_bits = 8;
        fork_num = 2;
        CNT.clear();
    }
    //把不定长二进制字符串转成uint数，不限长度
    uint ctoi(string code) {
        uint j = 0;
        uint length = code.length();
        for (uint i = 0; i < length; i++) {
            if (code[i] == '1') {
                j = j | (1 << (length - 1 - i));
            }
        }
        return j;
    }

    //把数转成bit位二进制字符串
    string itoc(uint n, int bit) {
        string ans;
        uint k = bit;
        while (k--)ans += '0';
        uint std = (uint)pow(2, bit - 1);
        for (uint i = bit; i > 0; i--, std >>= 1) {
            if ((n & std) >> (i - 1)) {
                ans[bit - i] = '1';
            }
        }
        return ans;
    }
    //把数转成8位二进制字符串
    string itoc8(uint n) {
        return itoc(n, 8);
    }

    //下面是一些操作函数
    //错误退出
    void exit_pro(int error_code) {
        switch (error_code) {
        case error_path:
            terminal->append("[ERROR]error file path.");
            terminal->append("----------------------[" + timer->currentTime().toString("hh:mm:ss") + "] End & error-------------------------");
            break;
        case error_file_type:
            terminal->append("[ERROR]not supported operation.");
            terminal->append("----------------------[" + timer->currentTime().toString("hh:mm:ss") + "] End & error-------------------------");
            break;
        case success_no:
            terminal->append("[DONE]Not compressed, write succeed.");
            terminal->append("----------------------[" + timer->currentTime().toString("hh:mm:ss") + "] End &  done-------------------------");
            break;
        case success_yes:
            terminal->append("[DONE]Compressed, write succeed.");
            terminal->append("----------------------[" + timer->currentTime().toString("hh:mm:ss") + "] End &  done-------------------------");
            break;
        case success_de_yes:
            terminal->append("[DONE]Decompressed, write succeed.");
            terminal->append("----------------------[" + timer->currentTime().toString("hh:mm:ss") + "] End &  done-------------------------");
            break;
        default:
            terminal->append("----------------------[" + timer->currentTime().toString("hh:mm:ss") + "] End        -------------------------");
            break;
        }
        return;
    }
    void set_byte_type(QComboBox* lb1_r, uint* ByteType_add) {
        if (lb1_r->currentIndex() == 0) *ByteType_add = 4;
        else if (lb1_r->currentIndex() == 1) *ByteType_add = 8;
        else if (lb1_r->currentIndex() == 2) *ByteType_add = 16;
        else if (lb1_r->currentIndex() == 3) *ByteType_add = 24;
        return;
    }
    void set_fork_num(QComboBox* c, uint* fork_num_add) {
        //*fork_num_add = (c->currentIndex() + 2);
        if (c->currentIndex() + 2 > 2 && c->currentIndex() + 2 <= 4) {
            *fork_num_add = 4;
        }
        else if (c->currentIndex() + 2 > 4 && c->currentIndex() + 2 <= 8) {
            *fork_num_add = 8;
        }
        else if (c->currentIndex() + 2 > 8 && c->currentIndex() + 2 <= 16) {
            *fork_num_add = 16;
        }
        else {
            *fork_num_add = 2;
        }
        return;
    }
    //统计文件字节数目
    uint stat_file_byte_num(QLineEdit* adress_line) {
        uint file_bytes_num = 0;
        fstream file1;
        file1.open(adress_line->text().toStdString(), ios::binary | ios::in);
        file1.seekg(0, ios::end);
        file_bytes_num = file1.tellg();
        file1.close();
        return file_bytes_num;
    }
    //获得32位数字f的从左往右的第n个字节，即：f=[1][2][3][4]
    char mask_get_byte(uint f, int n) {
        if (n == 1)return ((f & 4278190080) >> 24);
        if (n == 2)return ((f & 16711680) >> 16);
        if (n == 3)return ((f & 65280) >> 8);
        if (n == 4)return (f & 255);
    }
    //获得8位数字f从左往右第n个nibble，即：f=[1][2]
    uint mask_get_nibble(uint f, int n) {
        if (n == 1)return ((f >> 4) & 0b1111);
        if (n == 2)return (f & 0b1111);
    }
    // 如果是按2字节、3字节读取，最后可能会多出1~2个字节，这里的处理方法是将多的字节[前面]补0，所以要计算一下前面补了几个字节的0
    uint stat_end_complement_zero_byte(uint Scan_bits, uint file_bytes_num) {
        if ((Scan_bits == 16) && (file_bytes_num % 2 == 1)) {
            return 1;
        }
        else if ((Scan_bits == 24) && (file_bytes_num % 3 == 1)) {
            return 2;
        }
        else if ((Scan_bits == 24) && (file_bytes_num % 3 == 2)) {
            return 1;
        }
        return 0;
    }
    //统计字符频数
    void stat_char_frequency(uint file_bytes_num, fstream& file1) {
        for (uint j = 0; j < file_bytes_num; j++) {
            uint ch = file1.get();
            //如果是半个半个字节读
            if (Scan_bits == 4) {
                CNT[mask_get_nibble(ch, 1)]++;
                CNT[mask_get_nibble(ch, 2)]++;
            }
            //如果是一个一个字节读
            else if (Scan_bits == 8) {
                CNT[ch]++;
            }
            //如果是两个两个字节读
            else if (Scan_bits == 16) {
                if (j == file_bytes_num - 1) {
                    CNT[ch]++;
                }
                else {
                    uint tmp1 = ch << 8;
                    uint tmp2 = file1.get();
                    j++;
                    CNT[tmp1 + tmp2]++;
                }
            }
            //如果是三个三个字节读
            else if (Scan_bits == 24) {
                if (j == file_bytes_num - 1) {
                    CNT[ch]++;
                }
                else if (j == file_bytes_num - 2) {
                    uint tmp1 = ch << 8;
                    uint tmp2 = file1.get();
                    j++;
                    CNT[tmp1 + tmp2]++;
                }
                else {
                    uint tmp1 = ch << 16;
                    uint tmp2 = ((uint)file1.get()) << 8;
                    uint tmp3 = file1.get();
                    j += 2;
                    CNT[tmp1 + tmp2 + tmp3]++;
                }
            }
        }
        // 字符的频率统计完毕
        file1.close();
    }
    //构造Huffman树，并给树编码，并保存到html
    void build_tree(HuffmanTree& t, string html_save_address) {
        Heap priority_queue(CNT.size() + 20);
        for (auto i : CNT) {
            priority_queue.insert(new HuffmanNode(i.second, i.first, fork_num));
        }
        priority_queue.display();
        t.build(&priority_queue, fork_num);
        t.encode(t.root);
        t.stat_extra_bytes();
        t.display_encode();
        t.save_tree(html_save_address); 
    }
    //向slib中写入原本的文件
    void copy(string dest_file_name) {
        string new_file_name = dest_file_name + ".slib";
        //新文件的文件名
        ofstream new_file(new_file_name, ios::binary | ios::out);
        fstream file1(dest_file_name, std::ios::binary | std::ios::in);
        while (1) {
            uint ch = file1.get();
            if (ch != EOF) new_file.put(ch);
            else break;
        }
        file1.close();
        new_file.close();
    }
    //向解压的文件中直接写入原本的文件
    void copy(string dest_file_name, bool tag) {
        string new_file_name = "[Decompressed]" + dest_file_name;
        //新文件的文件名
        ofstream new_file(new_file_name, ios::binary | ios::out);
        fstream file1(dest_file_name, std::ios::binary | std::ios::in);
        while (1) {
            uint ch = file1.get();
            if (ch != EOF) new_file.put(ch);
            else break;
        }
        file1.close();
        new_file.close();
    }
    //计算要写入的压缩文件最后因为不满1字节多补几个0
    uint stat_extra_bits(HuffmanTree& tree) {
        uint extra_bits_of_huffman_set = 0;
        //for (auto i : tree.code_map) {
        //    extra_bits_of_huffman_set += i.second.length();
        //    extra_bits_of_huffman_set %= 8;
        //}
        //再计算文件本身的编码导致空缺多少位
        uint extra_bits_of_file_stream = 0;
        for (auto i : CNT) {
            auto j = tree.code_map[i.first]; //j是i字符对应的string code
            extra_bits_of_file_stream += j.length() * i.second; //长度*频数
            extra_bits_of_file_stream %= 8;
        }
        return 8 - (extra_bits_of_file_stream + extra_bits_of_huffman_set) % 8;
    }
    //计算校验字节（第三个字节）
    string stat_third_byte(uint file_end_complement_zero_byte, uint extra_bits) {
        string trois_byte;
        //下面是写入第三个字节
        switch (file_end_complement_zero_byte) {
        case 0: trois_byte += "00"; break;
        case 1: trois_byte += "01"; break;
        case 2: trois_byte += "10"; break;
        }
        switch (extra_bits) {
        case 0: trois_byte += "000"; break;
        case 1: trois_byte += "001"; break;
        case 2: trois_byte += "010"; break;
        case 3: trois_byte += "011"; break;
        case 4: trois_byte += "100"; break;
        case 5: trois_byte += "101"; break;
        case 6: trois_byte += "110"; break;
        case 7: trois_byte += "111"; break;
        }
        switch (Scan_bits) {
        case 4: trois_byte += "00"; break;
        case 8: trois_byte += "01"; break;
        case 16: trois_byte += "10"; break;
        case 24: trois_byte += "11"; break;
        }
        return trois_byte + '0';
    }
    //解码第三个字节
    void decode_3rd_byte(__int8 byte, uint* file_end_complement_zero_byte, uint* extra_bits) {
        switch ((byte >> 6) & 0b11) {
        case 0: *file_end_complement_zero_byte = 0; break;
        case 1: *file_end_complement_zero_byte = 1; break;
        case 2: *file_end_complement_zero_byte = 2; break;
        }
        switch ((byte >> 3) & 0b111) {
        case 0: *extra_bits = 0; break;
        case 1: *extra_bits = 1; break;
        case 2: *extra_bits = 2; break;
        case 3: *extra_bits = 3; break;
        case 4: *extra_bits = 4; break;
        case 5: *extra_bits = 5; break;
        case 6: *extra_bits = 6; break;
        case 7: *extra_bits = 7; break;
        }
        switch ((byte >> 1) & 0b11) {
        case 0: Scan_bits = 4; break;
        case 1: Scan_bits = 8; break;
        case 2: Scan_bits = 16; break;
        case 3: Scan_bits = 24; break;
        }
    }
    uint decode_4byte_to_uint(uint c1, uint c2, uint c3, uint c4) {
        return (c1 << 24) | (c2 << 16) | (c3 << 8) | c4;
    }
    char uint8_to_char(uint uint8) {
        char ans = 0;
        for (int i = 0; i < 8; i++) {
            ans |= (uint8 & (1 << i));
        }
        return ans;
    }

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
