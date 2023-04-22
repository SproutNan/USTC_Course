/*
 * @Author       : Chivier Humber
 * @Date         : 2021-09-15 21:15:24
 * @LastEditors  : Chivier Humber
 * @LastEditTime : 2021-11-23 16:08:51
 * @Description  : file content
 */
#include "common.h"
#include "memory.h"

namespace virtual_machine_nsp {
    void memory_tp::ReadMemoryFromFile(std::string filename, int beginning_address) {
        // Read from the file
        std::fstream file(filename);
        int address_ptr = beginning_address;
        std::string line;
        while (getline(file, line)) {
            for (int i = 0; i < 16; i++) {
                memory[address_ptr] |= (line[i] == '1') ? (1 << (15 - i)) : 0;
            }
            address_ptr++;
        }
    }

    int16_t memory_tp::GetContent(int address) const {
        // get the content
        return memory[address];
    }

    int16_t& memory_tp::operator[](int address) {
        // get the content
        return memory[address];
    }    
}; // virtual machine namespace
