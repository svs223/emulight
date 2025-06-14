#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>

#include <iostream>
#include <fstream>
#include <chrono>

#include "tsq.hpp"
#include "vcpu.hpp"
#include "msg.hpp"

unsigned char font_data[] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

const int font_sz = 80;
const unsigned int FONT_START_ADDRESS = 0x50;

vcpu::vcpu()
{
    for(int i = 0xF; i > 0; i--)
    {
        keys[i] = false;
    }
}

void vcpu::initalize(){
    for (unsigned int i = 0; i < font_sz; i++){
        memory[FONT_START_ADDRESS + i] = font_data[i];
    }
    pc = ROM_START_ADDRESS;
    sp = 0;
    // killswitch = false;
    srand(time(0));

    table[0x0] = &vcpu::Table0;
    table[0x1] = &vcpu::OP_1nnn;
    table[0x2] = &vcpu::OP_2nnn;
    table[0x3] = &vcpu::OP_3xnn;
    table[0x4] = &vcpu::OP_4xnn;
    table[0x5] = &vcpu::OP_5xy0;
    table[0x6] = &vcpu::OP_6xnn;
    table[0x7] = &vcpu::OP_7xnn;
    table[0x8] = &vcpu::Table8;
    table[0x9] = &vcpu::OP_9xy0;
    table[0xA] = &vcpu::OP_Annn;
    table[0xB] = &vcpu::OP_Bnnn;
    table[0xC] = &vcpu::OP_Cxnn;
    table[0xD] = &vcpu::OP_Dxyn;
    table[0xE] = &vcpu::TableE;
    table[0xF] = &vcpu::TableF;

    for (size_t i = 0; i <= 0xE; i++)
    {
        table0[i] = &vcpu::OP_NULL;
        table8[i] = &vcpu::OP_NULL;
        tableE[i] = &vcpu::OP_NULL;
    }

    table0[0x0] = &vcpu::OP_00E0;
    table0[0xE] = &vcpu::OP_00EE;

    table8[0x0] = &vcpu::OP_8xy0;
    table8[0x1] = &vcpu::OP_8xy1;
    table8[0x2] = &vcpu::OP_8xy2;
    table8[0x3] = &vcpu::OP_8xy3;
    table8[0x4] = &vcpu::OP_8xy4;
    table8[0x5] = &vcpu::OP_8xy5;
    table8[0x6] = &vcpu::OP_8xy6;
    table8[0x7] = &vcpu::OP_8xy7;

    tableE[0x1] = &vcpu::OP_ExA1;
    tableE[0xE] = &vcpu::OP_Ex9E;

    for (size_t i = 0; i <= 0x65; i++)
    {
        tableF[i] = &vcpu::OP_NULL;
    }

    tableF[0x07] = &vcpu::OP_Fx07;
    tableF[0x0A] = &vcpu::OP_Fx0A;
    tableF[0x15] = &vcpu::OP_Fx15;
    tableF[0x18] = &vcpu::OP_Fx18;
    tableF[0x1E] = &vcpu::OP_Fx1E;
    tableF[0x29] = &vcpu::OP_Fx29;
    tableF[0x33] = &vcpu::OP_Fx33;
    tableF[0x55] = &vcpu::OP_Fx55;
    tableF[0x65] = &vcpu::OP_Fx65;
}

void vcpu::emulateCycle(){
    opcode = (memory[pc] << 8u) | memory[pc + 1];
    
    
    if (opcode != 0x124E)
        printf("CPU: Opcode: 0x%X\n", opcode);
    


    pc += 2;
    
    // if (opcode == 0){
    //     printf("CPU: Invalid Opcode: 0x%X\n", opcode);
    //     return;
    // }
    if (opcode != 0x124E)
        printf("CPU: Decoding opcode and executing\n");

    // Decode opcode and execute
    ((*this).*(table[(opcode & 0xF000u) >> 12u]))();

    if (delay_timer > 0){
        --delay_timer;
    }
    if (sound_timer > 0){
        if (sound_timer == 1) printf("BEEP");
        --sound_timer;
    }
}

void vcpu::loadGame(std::string game){
    std::ifstream rom(game, std::ios::binary | std::ios::ate);
    if (rom.is_open()){
        std::streampos sz = rom.tellg();
        char *buf = new char[sz];
        rom.seekg(0, std::ios::beg);
        rom.read(buf, sz);
        rom.close();

        for (long i = 0; i < sz; i++){
            memory[ROM_START_ADDRESS + i] = buf[i];
        }

        delete[] buf;
    } else {
        printf("ROM Read Error\n");
    }
}

bool vcpu::displayUpdate()
{
    bool v = drawflag;
    drawflag = false;
    return v;
}

void vcpu::cpumain(thread_queue<Message> &gpuqueue, thread_queue<Message> &evqueue)
{
    auto lastCycleTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    float delta;

    while (true){
        while (!evqueue.empty())
        {
            Message ev = evqueue.peek();
            evqueue.pop();
            if (ev.target != MsgTarget::CpuEvent)
                continue;
            
            switch(ev.type)
            {
                case MsgType::KeyStatusUpdate:
                    if(std::holds_alternative<KeyStatusPacket>(ev.data))
                    {
                        KeyStatusPacket packet = std::get<KeyStatusPacket>(ev.data);
                        keys[packet.key] = packet.status;
                    }
                    break;

                default:
                    printf("VCPU: Unknown Event Type\n");
            }
        }
        currentTime = std::chrono::high_resolution_clock::now();
        delta = std::chrono::duration<float, std::chrono::milliseconds::period>(currentTime - lastCycleTime).count();

        if (delta > 3)
            emulateCycle();

        if (displayUpdate())
        {
            FramebufferUpdatePacket packet;
            // unsigned int *video = new unsigned int[64 * 32];
            memcpy(&packet.fb, &gfx, 64 * 32);
            Message msg;
            msg.target = MsgTarget::GpuEvent;
            msg.type = MsgType::FramebufferUpdate;
            msg.data = packet;
            gpuqueue.push(msg);
        }
    }
}

int vcpu::calcPitch()
{
    return sizeof(gfx[0]) * VIDEO_WIDTH;
}