#include "mainwindow.h"
#include "ui_mainwindow.h"
#define parameter unsigned

using namespace std;

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    //设置图形化界面
#if 1
    //设置窗口大小
    this->setFixedSize(720, 500);
    //设置窗口icon
    //this->setWindowIcon(QPixmap(":/img/Window_Icon.png"));
    //设置窗口标题
    this->setWindowTitle("SproutLib Compresser & Decompresser(Build Version)");
    auto up_widgets = new QGroupBox;
    up_widgets->move(20, 20);
    up_widgets->resize(670, 100);
    up_widgets->setTitle("Choose File");
    up_widgets->setParent(this);

    auto deux_widgets = new QGroupBox;
    deux_widgets->move(20, 140);
    deux_widgets->resize(670, 100);
    deux_widgets->setTitle("Advanced Options");
    deux_widgets->setParent(this);

    auto adress_line = new QLineEdit;
    adress_line->resize(500, 30);
    adress_line->isReadOnly();
    adress_line->move(40, 60);
    adress_line->setParent(this);

    auto select_file = new QPushButton;
    select_file->setText("Choose File");
    select_file->resize(130, 30);
    select_file->move(550, 45);
    select_file->setParent(this);

    auto start = new QPushButton;
    start->setText("Operating");
    start->resize(130, 30);
    start->move(550, 75);
    start->setParent(this);

    auto lb1 = new QLabel;
    lb1->setText("Granularity");
    lb1->resize(130, 30);
    lb1->move(40, 165);
    lb1->setParent(this);

    auto combo_branch_lb = new QLabel;
    combo_branch_lb->setText("Branches");
    combo_branch_lb->resize(130, 30);
    combo_branch_lb->move(200, 165);
    combo_branch_lb->setParent(this);

    auto lb2 = new QLabel;
    lb2->setText("bytes");
    lb2->resize(100, 30);
    lb2->move(126, 200);
    lb2->setParent(this);

    auto combo_branch_unit = new QLabel;
    combo_branch_unit->setText("branches");
    combo_branch_unit->resize(200, 30);
    combo_branch_unit->move(286, 200);
    combo_branch_unit->setParent(this);

    auto lb1_r = new QComboBox;
    lb1_r->addItem("0.5");
    lb1_r->addItem("1");
    lb1_r->addItem("2");
    lb1_r->addItem("3");
    lb1_r->resize(70, 30);
    lb1_r->move(40, 200);
    lb1_r->setParent(this);
    lb1_r->setCurrentIndex(1);

    //分支选择
    auto combo_branch = new QComboBox;
    combo_branch->addItem("2");
    combo_branch->addItem("3");
    combo_branch->addItem("4");
    combo_branch->addItem("5");
    combo_branch->addItem("6");
    combo_branch->addItem("7");
    combo_branch->addItem("8");
    combo_branch->addItem("9");
    combo_branch->addItem("10");
    combo_branch->addItem("11");
    combo_branch->addItem("12");
    combo_branch->addItem("13");
    combo_branch->addItem("14");
    combo_branch->addItem("15");
    combo_branch->addItem("16");

    combo_branch->resize(70, 30);
    combo_branch->move(200, 200);
    combo_branch->setParent(this);
    combo_branch->setCurrentIndex(0);

    auto progress = new QProgressBar;
    progress->resize(710, 30);
    progress->move(20, 250);
    progress->setParent(this);

    terminal->resize(670, 170);
    terminal->move(20, 300);
    terminal->append("Thank you for using SproutLib Compresser & Decompresser!");
    terminal->setParent(this);
#endif
    connect(select_file, &QPushButton::clicked, [=]() {
        QString FILE_PATH = QFileDialog::getOpenFileName(this, "选择文件", "C:\\", "*.*");
        if (!FILE_PATH.isEmpty()) {
            adress_line->setText(FILE_PATH);
        }
        });

    connect(start, &QPushButton::clicked, [=]() {
        QString tmp = "----------------------[" + timer->currentTime().toString("hh:mm:ss") + "] New process-------------------------";
        terminal->append(tmp);

        //terminal->append("--------------New process--------------");
        // 设置字节处理模式
        set_byte_type(lb1_r, &Scan_bits);
        set_fork_num(combo_branch, &fork_num);

        auto fileName = adress_line->text().toStdString();//文件名
        int tail = fileName.size() - 5;//检测文件后缀
        // 如果用户乱搞文件地址
        if (tail <= 0) {
            exit_pro(error_path);
            return;
        }
        // 用户选择了正常的文件地址
        // 解压模式
        if (fileName.substr(tail, 5) == ".slib") {
            //先看是否被压缩了
            fstream slib_file(adress_line->text().toStdString(), std::ios::binary | std::ios::in);
            uint ch1 = slib_file.get();
            uint ch2 = slib_file.get();
            ofstream new_file(adress_line->text().toStdString().substr(0, tail + 1) + "[Decompressed]", ios::binary | ios::out);
            //非正常压缩，直接释放文件
            if (!(ch1 == 137 && ch2 == 137)) {
                slib_file.close();
                copy(adress_line->text().toStdString(), true);
                exit_pro(success_de_yes);
                return;
            }
            //正常解码模式
            //读取设置参数的字节
            parameter file_end_complement_zero_byte;
            parameter extra_bits;
            __int8 ch3 = slib_file.get();
            decode_3rd_byte(ch3, &file_end_complement_zero_byte, &extra_bits);
            uint c1 = slib_file.get();
            uint c2 = slib_file.get();
            uint c3 = slib_file.get();
            uint c4 = slib_file.get();
            parameter num_huffman_code = decode_4byte_to_uint(c1, c2, c3, c4);
            map<uint, map<string, uint>> power_dictionary;
            //上面这个是长度为uint的编码对应的对照表，下面写入huffman字典
            for (uint i = 0; i < num_huffman_code; i++) {
                uint c1 = slib_file.get();
                uint c2 = slib_file.get();
                uint c3 = slib_file.get();
                uint c4 = slib_file.get();
                parameter uint_represented = decode_4byte_to_uint(c1, c2, c3, c4);
                c1 = slib_file.get();
                c2 = slib_file.get();
                c3 = slib_file.get();
                c4 = slib_file.get();
                parameter code_len_this_uint = decode_4byte_to_uint(c1, c2, c3, c4);
                if (code_len_this_uint <= 8) {
                    c1 = slib_file.get();
                    string c = itoc(c1, 8).substr(8 - code_len_this_uint);
                    power_dictionary[code_len_this_uint][c] = uint_represented;
                }
                else if (code_len_this_uint <= 16) {
                    c1 = slib_file.get();
                    c2 = slib_file.get();
                    c3 = (c1 << 8) | c2;
                    string c = itoc(c3, 16).substr(16 - code_len_this_uint);
                    power_dictionary[code_len_this_uint][c] = uint_represented;
                }
                else if (code_len_this_uint <= 24) {
                    c1 = slib_file.get();
                    c2 = slib_file.get();
                    c3 = slib_file.get();
                    c4 = (c1 << 16) | (c2 << 8) | c3;
                    string c = itoc(c4, 24).substr(24 - code_len_this_uint);
                    power_dictionary[code_len_this_uint][c] = uint_represented;
                }
                else {
                    c1 = slib_file.get();
                    c2 = slib_file.get();
                    c3 = slib_file.get();
                    c4 = slib_file.get();
                    uint c5 = (c1 << 24) | (c2 << 16) | (c3 << 8) | c4;
                    string c = itoc(c4, 32).substr(32 - code_len_this_uint);
                    power_dictionary[code_len_this_uint][c] = uint_represented;
                }
            }

            //下面的都是文件流
            //摆烂，接下来把所有密码流全部读进来
            BitQueue word(8);
            uint num_byte = 0;
            while (1) {
                uint ch = slib_file.get();
                if (ch != EOF) word.push_b_s(itoc(ch, 8));
                else break;
            }
            //现在所有密码流都在word里了
            for (uint i = 0; i < extra_bits; i++) {
                word.pop_back();
            }
            //去掉结尾多的0
            string now_code;
            string ans_string;
            while (word.size()) {
                now_code += word.pop_bit();
                if (power_dictionary[now_code.length()].find(now_code) != power_dictionary[now_code.length()].end()) {
                    if (Scan_bits == 4) {
                        uint first_nibble = power_dictionary[now_code.length()][now_code] << 4;
                        now_code = "";
                        while (word.size()) {
                            now_code += word.pop_bit();
                            if (power_dictionary[now_code.length()].find(now_code) != power_dictionary[now_code.length()].end()) {
                                uint byte = ((power_dictionary[now_code.length()][now_code] & 0b1111) | first_nibble) & 0b11111111;
                                now_code = "";
                                new_file.put((char)byte);
                                break;
                            }
                        }
                    }
                    else if (Scan_bits == 8) {
                        new_file.put((char)power_dictionary[now_code.length()][now_code]);
                        now_code = "";
                    }
                    else if (Scan_bits == 16) {
                        uint b12 = power_dictionary[now_code.length()][now_code];
                        char b1 = (char)((b12 >> 8) & 0b11111111);
                        char b2 = (char)(b12 & 0b11111111);
                        now_code = "";
                        ans_string += b1;
                        ans_string += b2;
                    }
                    else {
                        uint b123 = power_dictionary[now_code.length()][now_code];
                        char b1 = (char)((b123 >> 16) & 0b11111111);
                        char b2 = (char)((b123 >> 8) & 0b11111111);
                        char b3 = (char)(b123 & 0b11111111);
                        now_code = "";
                        ans_string += b1;
                        ans_string += b2;
                        ans_string += b2;
                    }
                }
            }
            if (ans_string.size()) {
                if (Scan_bits == 16 && file_end_complement_zero_byte == 1) {
                    uint size = ans_string.size();
                    char tmp = ans_string[size - 1];
                    ans_string = ans_string.substr(0, size - 2);
                    ans_string += tmp;
                    new_file.write(ans_string.data(), size - 1);
                }
                else if (Scan_bits == 24 && file_end_complement_zero_byte == 1) {
                    uint size = ans_string.size();
                    char tmp1 = ans_string[size - 2];
                    char tmp2 = ans_string[size - 1];
                    ans_string = ans_string.substr(0, size - 3);
                    ans_string += tmp1;
                    ans_string += tmp2;
                    new_file.write(ans_string.data(), size - 1);
                }
                else if (Scan_bits == 24 && file_end_complement_zero_byte == 2) {
                    uint size = ans_string.size();
                    char tmp = ans_string[size - 1];
                    ans_string = ans_string.substr(0, size - 3);
                    ans_string += tmp;
                    new_file.write(ans_string.data(), size - 2);
                }
                else {
                    uint size = ans_string.size();
                    new_file.write(ans_string.data(), size);
                }
            }
            
            new_file.close();
            exit_pro(success_de_yes);
            return;
        }
        else {
            HuffmanTree tree(Scan_bits, fork_num);//创建新Huffman树
            // 获取文件总字节数
            uint file_bytes_num = stat_file_byte_num(adress_line);
            // 如果是按2字节、3字节读取，最后可能会多出1~2个字节，这里的处理方法是将多的字节[前面]补0，所以要计算一下前面补了几个字节的0
            uint file_end_complement_zero_byte = stat_end_complement_zero_byte(Scan_bits, file_bytes_num);
            //准备开始编码，先尝试打开文件
            fstream file1(adress_line->text().toStdString(), ios::binary | ios::in);
            //如果没打开文件，则退出
            if (!file1.is_open()) exit_pro(error_file_type);
            // 下面遍历一遍文件得出各字符频率
            stat_char_frequency(file_bytes_num, file1);
            // 下面构造Huffman树然后获得编码
            build_tree(tree, adress_line->text().toStdString());
            // 建完树了
            zip_rate(&tree);
            // 计算压缩率
            // 此时编码已经生成在tree内
            /*
            * 压缩文件的设计
            * 两个字节：
            *   正常压缩：&&，这里&代表不可打印字符137
            *   未被压缩：无文件头
            * 还需要以下信息：
            1、表示对于还原出来的文件，最后一个读写单位其实最后面补了多少个字节的0，值域：0~2，用2bit存
            2、写入的整个文件的二进制流，最后因为不满一个字节而补了多少个bit的0，值域：0~7，用3bit存
            3、存读取方式，值域：0~3，[0]：0.5字节；[1]：1字节；[2]：2字节；[3]：3字节，用2bit存
            * 用一个字节来存，12 345 67 8，8没用
            * 四个字节，表示此文件中出现了多少个Huffman编码，按无符号数解释
            * 然后是Huffman编码储存区
            *
              n个字节存编码代表的无符号数，n是半个字节的时候按一个字节计，n由上述存读取方式决定；4个字节存code的长度，然后存code。
            * 编码后的文件流，注意这里是按字节输入的
            * 这里我要摆烂
            * 4个字节存编码代表的无符号数，4个字节存code长度，code存成多长取上整（按8、16、24、32），高位补0
            */
            //[MARK]下面这个注意一下哈，用了奇怪的东西
            if ((zip_rate_f > TOLERATE_ZIP_RATE_F) * 0) {
                //直接复制一份文件
                copy(adress_line->text().toStdString());
                exit_pro(success_no);
                return;
            }
            else {
                //计算要写入的压缩文件最后因为不满1字节多补几个0
                uint extra_bits = stat_extra_bits(tree);
                //下面开始写入文件头
                string new_file_name = adress_line->text().toStdString() + ".slib";
                ofstream new_file(new_file_name.c_str(), ios::binary | ios::out);
                new_file.put((char)137);
                new_file.put((char)137);
                new_file.put(mask_get_byte((ctoi(stat_third_byte(file_end_complement_zero_byte, extra_bits))), 4));
                //写入接下来四个字节
                uint next_quatre_bytes = CNT.size();
                new_file.put(mask_get_byte(next_quatre_bytes, 1));
                new_file.put(mask_get_byte(next_quatre_bytes, 2));
                new_file.put(mask_get_byte(next_quatre_bytes, 3));
                new_file.put(mask_get_byte(next_quatre_bytes, 4));

                for (auto i : CNT) {
                    uint first = i.first;
                    new_file.put(mask_get_byte(first, 1));
                    new_file.put(mask_get_byte(first, 2));
                    new_file.put(mask_get_byte(first, 3));
                    new_file.put(mask_get_byte(first, 4));
                    uint second = tree.code_map[first].length();
                    new_file.put(mask_get_byte(second, 1));
                    new_file.put(mask_get_byte(second, 2));
                    new_file.put(mask_get_byte(second, 3));
                    new_file.put(mask_get_byte(second, 4));
                    if (second <= 8) {
                        new_file.put(mask_get_byte(ctoi(tree.code_map[first]), 4));
                    }
                    else if (second <= 16) {
                        new_file.put(mask_get_byte(ctoi(tree.code_map[first]), 3));
                        new_file.put(mask_get_byte(ctoi(tree.code_map[first]), 4));
                    }
                    else if (second <= 24) {
                        new_file.put(mask_get_byte(ctoi(tree.code_map[first]), 2));
                        new_file.put(mask_get_byte(ctoi(tree.code_map[first]), 3));
                        new_file.put(mask_get_byte(ctoi(tree.code_map[first]), 4));
                    }
                    else {
                        new_file.put(mask_get_byte(ctoi(tree.code_map[first]), 1));
                        new_file.put(mask_get_byte(ctoi(tree.code_map[first]), 2));
                        new_file.put(mask_get_byte(ctoi(tree.code_map[first]), 3));
                        new_file.put(mask_get_byte(ctoi(tree.code_map[first]), 4));
                    }
                }
                //下面要开始写入密码流了
                file1.open(adress_line->text().toStdString(), ios::binary | ios::in);
                // 第二次遍历，q是密码流容器
                BitQueue q(8);
                // 字典
                auto dictionary = &(tree.code_map);
                if (file1.is_open()) {
                    for (uint j = 0; j < file_bytes_num; j++) {
                        uint ch = file1.get();
                        //如果是半个半个字节读
                        if (Scan_bits == 4) {
                            q.push_b_s((*dictionary)[mask_get_nibble(ch, 1)]);
                            q.push_b_s((*dictionary)[mask_get_nibble(ch, 2)]);
                        }
                        //如果是一个一个字节读
                        else if (Scan_bits == 8) {
                            q.push_b_s((*dictionary)[ch]);
                        }
                        //如果是两个两个字节读
                        else if (Scan_bits == 16) {
                            if (j == file_bytes_num - 1) {
                                q.push_b_s((*dictionary)[ch]);
                            }
                            else {
                                uint tmp1 = ch << 8;
                                uint tmp2 = file1.get();
                                j++;
                                q.push_b_s((*dictionary)[tmp1 + tmp2]);
                            }
                        }
                        //如果是三个三个字节读
                        else if (Scan_bits == 24) {
                            if (j == file_bytes_num - 1) {
                                q.push_b_s((*dictionary)[ch]);
                            }
                            else if (j == file_bytes_num - 2) {
                                uint tmp1 = ch << 8;
                                uint tmp2 = file1.get();
                                j++;
                                q.push_b_s((*dictionary)[tmp1 + tmp2]);
                            }
                            else {
                                uint tmp1 = ch << 16;
                                uint tmp2 = ((uint)file1.get()) << 8;
                                uint tmp3 = file1.get();
                                j += 2;
                                q.push_b_s((*dictionary)[tmp1 + tmp2 + tmp3]);
                            }
                        }
                    }
                    file1.close();
                    while (q.size() % 8 != 0) {
                        q.push_back(false);
                    }
                    while (q.size()) {
                        new_file.put((char)ctoi(q.pop_b_s()));
                    }
                    new_file.close();
                    exit_pro(success_yes);
                }
            }
            refresh();
        }
        });
}


