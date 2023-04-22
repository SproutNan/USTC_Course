/*
 * @Author       : Chivier Humber
 * @Date         : 2021-08-30 14:36:39
 * @LastEditors  : Chivier Humber
 * @LastEditTime : 2021-09-13 15:08:48
 * @Description  : header file for small assembler
 */

#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

const int kLC3LineLength = 16;

extern bool gIsDebugMode;
extern bool gIsErrorLogMode;
extern bool gIsHexMode;

// LC3标准操作码集
const std::vector<std::string> kLC3Commands({
    "ADD",   // 00: "0001" + reg(line[1]) + reg(line[2]) + op(line[3])
    "AND",   // 01: "0101" + reg(line[1]) + reg(line[2]) + op(line[3])
    "BR",    // 02: "0000000" + pcoffset(line[1],9)
    "BRN",   // 03: "0000100" + pcoffset(line[1],9)
    "BRZ",   // 04: "0000010" + pcoffset(line[1],9)
    "BRP",   // 05: "0000001" + pcoffset(line[1],9)
    "BRNZ",  // 06: "0000110" + pcoffset(line[1],9)
    "BRNP",  // 07: "0000101" + pcoffset(line[1],9)
    "BRZP",  // 08: "0000011" + pcoffset(line[1],9)
    "BRNZP", // 09: "0000111" + pcoffset(line[1],9)
    "JMP",   // 10: "1100000" + reg(line[1]) + "000000"
    "JSR",   // 11: "01001" + pcoffset(line[1],11)
    "JSRR",  // 12: "0100000"+reg(line[1])+"000000"
    "LD",    // 13: "0010" + reg(line[1]) + pcoffset(line[2],9)
    "LDI",   // 14: "1010" + reg(line[1]) + pcoffset(line[2],9)
    "LDR",   // 15: "0110" + reg(line[1]) + reg(line[2]) + offset(line[3])
    "LEA",   // 16: "1110" + reg(line[1]) + pcoffset(line[2],9)
    "NOT",   // 17: "1001" + reg(line[1]) + reg(line[2]) + "111111"
    "RET",   // 18: "1100000111000000"
    "RTI",   // 19: "1000000000000000"
    "ST",    // 20: "0011" + reg(line[1]) + pcoffset(line[2],9)
    "STI",   // 21: "1011" + reg(line[1]) + pcoffset(line[2],9)
    "STR",   // 22: "0111" + reg(line[1]) + reg(line[2]) + offset(line[3])
    "TRAP"   // 23: "11110000" + h2b(line[1],8)
});

// LC3标准TRAP指令集
const std::vector<std::string> kLC3TrapRoutine({
    "GETC",  // x20
    "OUT",   // x21
    "PUTS",  // x22
    "IN",    // x23
    "PUTSP", // x24
    "HALT"   // x25
});

// 字符串类型：注释、标记、值、操作码、操作数、错误
enum StringType { sComment, sLabel, sValue, sOpcode, sOprand, sError };

// 值类型：地址、立即数
enum ValueType { vAddress, vValue };

// 一行代码类型：未定，注释，指令，伪指令
enum LineStatusType { lPending = -1, lComment, lOperation, lPseudo };

// token_tp是一个二元组，前一个是str，后一个是str的type，即字符串类型：注释、标记、值、操作码、操作数、错误
typedef std::pair<std::string, StringType> token_tp;
inline void SetDebugMode(bool debug) {
    gIsDebugMode = debug;
}
inline void SetErrorLogMode(bool error) {
    gIsErrorLogMode = error;
}
inline void SetHexMode(bool hex) {
    gIsHexMode = hex;
}

// value_tp是一个类，保存一个值（val_）和其类型（type_），里面有一些设置之类的函数
class value_tp {
    private:
    ValueType type_;
    int val_;

    public:
    value_tp(ValueType type, int val) : type_(type), val_(val) {}
    value_tp(int val) : type_(vValue), val_(val) {}
    value_tp() : type_(ValueType::vValue), val_(0) {}
    ~value_tp() {}

    ValueType getType() const { return type_; }
    int getVal() const { return val_; }
    void setType(ValueType type) { type_ = type; }
    void setVal(int val) { val_ = val; }
    friend std::ostream &operator<<(std::ostream &os, const value_tp &value);
};

// label_map_tp是用于记录label和其对应的指令（value_tp）的映射（map），用addLabel等函数进行操作
class label_map_tp {
    private:
    std::map<std::string, value_tp> labels_;

    public:
    ~label_map_tp() {}
    void AddLabel(const std::string &str, const value_tp &val);
    // void AddLabel(std::string str, std::string val);
    value_tp GetValue(const std::string &str) const;
    friend std::ostream &operator<<(std::ostream &os, const label_map_tp &label_map);
    void display() {
        for (auto i = labels_.begin(); i != labels_.end(); i++) {
            std::cout << i->first << "|" << i->second << std::endl;
        }
    }
};

// trim from left 删除左侧空格
inline std::string &LeftTrim(std::string &s, const char *t = " \t\n\r\f\v") {
    auto begin = s.find_first_not_of(t);
    if (begin == s.npos) {
        s.clear();
    }
    else {
        auto size = s.size();
        s = s.substr(begin, size - begin);
    }
    return s;
}

// trim from right 删除右侧空格
inline std::string &RightTrim(std::string &s, const char *t = " \t\n\r\f\v") {
    auto end = s.find_last_not_of(t);
    if (end == s.npos) {
        s.clear();
    }
    else {
        s = s.substr(0, end + 1);
    }
    return s;
}

// trim from left & right 删除两侧空格
inline std::string &Trim(std::string &s, const char *t = " \t\n\r\f\v") {
    return LeftTrim(RightTrim(s, t), t);
}

// 查看str是否是一个LC3操作码，返回在kLC3Command中的编号，不是返回-1
inline int IsLC3Command(const std::string &str) {
    int index = 0;
    for (auto command : kLC3Commands) {
        if (str == command) {
            return index;
        }
        ++index;
    }
    return -1;
}

// 查看str是否是一个TRAP正规操作，返回在kLC3TrapRoutine中的编号，不是返回-1
inline int IsLC3TrapRoutine(const std::string &str) {
    int index = 0;
    for (auto trap : kLC3TrapRoutine) {
        if (str == trap) {
            return index;
        }
        ++index;
    }
    return -1;
}

// 将一个十六进制字符转换成其对应的十进制数
inline int CharToDec(const char &ch) {
    if (ch >= '0' && ch <= '9') {
        return ch - '0';
    }
    if (ch >= 'A' && ch <= 'F') {
        return ch - 'A' + 10;
    }
    return -1;
}

//将十进制数转换为十六进制字符，接受的值是0~F
inline char DecToChar(const int &num) {
    if (num <= 9) {
        return num + '0';
    } else {
        return num - 10 + 'A';
    }
    return -1;
}

//把str转换成十进制数
int RecognizeNumberValue(std::string str);

//把number转换成16位二进制01串
std::string NumberToAssemble(const int &number);
std::string NumberToAssemble(const std::string &number);

//把二进制01串转换成十六进制字符串
std::string ConvertBin2Hex(std::string bin);

//汇编器
class assembler {
private:
    label_map_tp label_map;
    std::string TranslateOprand(int current_address, std::string str, int opcode_length = 3);

public:
    int assemble(std::string input_filename, std::string output_filename);
};