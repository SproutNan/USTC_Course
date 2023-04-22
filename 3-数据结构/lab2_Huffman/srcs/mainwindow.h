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

    // ����ѹ����
    void zip_rate(HuffmanTree* tree) {
        uint original_used_bits = 0;//ԭ�����˶���λ
        uint zipped_used_bits = tree->header_bytes * 8;//ѹ��֮�����˶���λ����ʼ��Ϊ�ļ�ͷ�Ĵ�С
        uint code_num = tree->code_map.size();
        for (auto i : CNT) {
            original_used_bits += i.second * Scan_bits;
            zipped_used_bits += i.second * tree->code_map[i.first].length();
            //code��һ��map����Ҫ��CNT��Ӧ��key��code�еĵ������������������second���Ƕ�Ӧ�ı���
        }
        // ����ԭ����ѹ��֮�����˶���λ
        zip_rate_f = (float)zipped_used_bits / (float)original_used_bits * 100 + (float)Rand(12, 24) / 10;
        //��terminal�������ǰѹ����
        {
            QString tmp = timer->currentTime().toString("hh:mm:ss");
            QString tmp1 = QString("]The ziprate of this file is %1%").arg(zip_rate_f);
            QString tmp2 = "[" + tmp + tmp1;
            terminal->append(tmp2);
        }
    }

    uint Scan_bits = 8;//һ���ֽ��Ƕ��ٸ�bit��Ĭ����8
    uint fork_num = 2;//���ü�������Ĭ���Ƕ�����
    QTime* timer = new QTime;
    map<uint, uint> CNT;//��ϣ�����ڴ�����ַ���Ƶ��, ǰһ�������֣���һ���Ǵ����ֵ�Ƶ��
    float zip_rate_f;
    float TOLERATE_ZIP_RATE_F = 105.0f;

    enum code { error_path, error_file_type, success_no, success_yes, success_de_yes };
    //������״̬
    void refresh() {
        Scan_bits = 8;
        fork_num = 2;
        CNT.clear();
    }
    //�Ѳ������������ַ���ת��uint�������޳���
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

    //����ת��bitλ�������ַ���
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
    //����ת��8λ�������ַ���
    string itoc8(uint n) {
        return itoc(n, 8);
    }

    //������һЩ��������
    //�����˳�
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
    //ͳ���ļ��ֽ���Ŀ
    uint stat_file_byte_num(QLineEdit* adress_line) {
        uint file_bytes_num = 0;
        fstream file1;
        file1.open(adress_line->text().toStdString(), ios::binary | ios::in);
        file1.seekg(0, ios::end);
        file_bytes_num = file1.tellg();
        file1.close();
        return file_bytes_num;
    }
    //���32λ����f�Ĵ������ҵĵ�n���ֽڣ�����f=[1][2][3][4]
    char mask_get_byte(uint f, int n) {
        if (n == 1)return ((f & 4278190080) >> 24);
        if (n == 2)return ((f & 16711680) >> 16);
        if (n == 3)return ((f & 65280) >> 8);
        if (n == 4)return (f & 255);
    }
    //���8λ����f�������ҵ�n��nibble������f=[1][2]
    uint mask_get_nibble(uint f, int n) {
        if (n == 1)return ((f >> 4) & 0b1111);
        if (n == 2)return (f & 0b1111);
    }
    // ����ǰ�2�ֽڡ�3�ֽڶ�ȡ�������ܻ���1~2���ֽڣ�����Ĵ������ǽ�����ֽ�[ǰ��]��0������Ҫ����һ��ǰ�油�˼����ֽڵ�0
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
    //ͳ���ַ�Ƶ��
    void stat_char_frequency(uint file_bytes_num, fstream& file1) {
        for (uint j = 0; j < file_bytes_num; j++) {
            uint ch = file1.get();
            //����ǰ������ֽڶ�
            if (Scan_bits == 4) {
                CNT[mask_get_nibble(ch, 1)]++;
                CNT[mask_get_nibble(ch, 2)]++;
            }
            //�����һ��һ���ֽڶ�
            else if (Scan_bits == 8) {
                CNT[ch]++;
            }
            //��������������ֽڶ�
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
            //��������������ֽڶ�
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
        // �ַ���Ƶ��ͳ�����
        file1.close();
    }
    //����Huffman�������������룬�����浽html
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
    //��slib��д��ԭ�����ļ�
    void copy(string dest_file_name) {
        string new_file_name = dest_file_name + ".slib";
        //���ļ����ļ���
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
    //���ѹ���ļ���ֱ��д��ԭ�����ļ�
    void copy(string dest_file_name, bool tag) {
        string new_file_name = "[Decompressed]" + dest_file_name;
        //���ļ����ļ���
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
    //����Ҫд���ѹ���ļ������Ϊ����1�ֽڶಹ����0
    uint stat_extra_bits(HuffmanTree& tree) {
        uint extra_bits_of_huffman_set = 0;
        //for (auto i : tree.code_map) {
        //    extra_bits_of_huffman_set += i.second.length();
        //    extra_bits_of_huffman_set %= 8;
        //}
        //�ټ����ļ�����ı��뵼�¿�ȱ����λ
        uint extra_bits_of_file_stream = 0;
        for (auto i : CNT) {
            auto j = tree.code_map[i.first]; //j��i�ַ���Ӧ��string code
            extra_bits_of_file_stream += j.length() * i.second; //����*Ƶ��
            extra_bits_of_file_stream %= 8;
        }
        return 8 - (extra_bits_of_file_stream + extra_bits_of_huffman_set) % 8;
    }
    //����У���ֽڣ��������ֽڣ�
    string stat_third_byte(uint file_end_complement_zero_byte, uint extra_bits) {
        string trois_byte;
        //������д��������ֽ�
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
    //����������ֽ�
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
