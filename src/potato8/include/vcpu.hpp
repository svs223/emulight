#pragma once

#include <string>
#include <mutex>

#include "tsq.hpp"
#include "msg.hpp"

class vcpu {
    public:
        // Constants
        const int VIDEO_WIDTH = 64;
        const int VIDEO_HEIGHT = 32;
        const unsigned int ROM_START_ADDRESS = 0x200;

        vcpu();
        void cpumain(thread_queue<Message> &q, thread_queue<Message>&);
        void initalize();
        void loadGame(std::string);
        void emulateCycle();
        bool displayUpdate();
        // void kill();

        int calcPitch();

        static void start(vcpu& cpu, thread_queue<Message>& gpuq, thread_queue<Message>& cpuq)
        {
            cpu.cpumain(gpuq, cpuq);
        }

    private:
        // Virtual hardware
        uint16_t pc;
        uint16_t I;
        uint8_t V[16];
        uint8_t delay_timer, sound_timer;
        uint16_t opcode;
        uint32_t gfx[64 * 32];
        uint16_t stack[16];
        uint8_t sp;
        bool keys[16];
        bool drawflag;
        uint8_t memory[4096];
        // int delay;

        // Opcode Functions
        void OP_00E0();
        void OP_00EE();
        void OP_1nnn();
        void OP_2nnn();
        void OP_3xnn();
        void OP_4xnn();
        void OP_5xy0();
        void OP_6xnn();
        void OP_7xnn();
        void OP_8xy0();
        void OP_8xy1();
        void OP_8xy2();
        void OP_8xy3();
        void OP_8xy4();
        void OP_8xy5();
        void OP_8xy6();
        void OP_8xy7();
        void OP_8xyE();
        void OP_9xy0();
        void OP_Annn();
        void OP_Bnnn();
        void OP_Cxnn();
        void OP_Dxyn();
        void OP_Ex9E();
        void OP_ExA1();
        void OP_Fx07();
        void OP_Fx0A();
        void OP_Fx15();
        void OP_Fx18();
        void OP_Fx1E();
        void OP_Fx29();
        void OP_Fx33();
        void OP_Fx55();
        void OP_Fx65();
        void OP_NULL();

        typedef void (vcpu::*VcpuFunc)();
        // Tables
        VcpuFunc table[0xF + 1];
        VcpuFunc table0[0xE + 1];
        VcpuFunc table8[0xE + 1];
        VcpuFunc tableE[0xE + 1];
        VcpuFunc tableF[0x65 + 1];

        // Table functions
        void Table0();
        void Table8();
        void TableE();
        void TableF();

        // Synchronization
        // std::mutex killswitch_mtx;
        // bool killswitch;
};
