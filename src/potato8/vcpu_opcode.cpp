#include <cstring>

#include "vcpu.hpp"

const unsigned int FONT_START_ADDRESS = 0x50;

void vcpu::OP_00E0()
{
    memset(&gfx, 0u, 2048);
    drawflag = true;
}

void vcpu::OP_00EE()
{
    if (sp < 1)
        printf("CPU: Faulty instruction\nCPU: Ignoring\n");
        return;
    pc = stack[--sp];
}

void vcpu::OP_1nnn()
{
    uint16_t address = (opcode & 0x0FFFu);
    pc = address;
}

void vcpu::OP_2nnn()
{
    uint16_t address = (opcode & 0x0FFFu);

    stack[sp] = pc;
    ++sp;
    pc = address;
}

void vcpu::OP_3xnn()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t byte = opcode & 0x00FFu;
    
    if (V[Vx] == byte)
        pc += 2;
}

void vcpu::OP_4xnn()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t byte = opcode & 0x00FFu;
    if (V[Vx] != byte)
        pc += 2;
}

void vcpu::OP_5xy0()
{
    uint8_t Vx = (opcode & 0x0F00) >> 8;
    uint8_t Vy = (opcode & 0x00F0) >> 4;
    if (V[Vx] == V[Vy]){
        pc += 2;
    }
}

void vcpu::OP_6xnn()
{
    uint8_t Vx = (opcode & 0x0F00) >> 8;
    uint8_t byte = (opcode & 0x00FF);
    //printf("Vx = V%X", Vx);
    V[Vx] = byte;
}

void vcpu::OP_7xnn()
{
    uint8_t Vx = (opcode & 0x0F00) >> 8;
    uint8_t byte = (opcode & 0x00FF);
    V[Vx] += byte;
}

void vcpu::OP_8xy0()
{
    uint8_t Vx = (opcode & 0x0F00) >> 8;
    uint8_t Vy = (opcode & 0x00F0) >> 4;

    V[Vx] = V[Vy];
}

void vcpu::OP_8xy1()
{
    uint8_t Vx = (opcode & 0x0F00) >> 8;
    uint8_t Vy = (opcode & 0x00F0) >> 4;

    V[Vx] |= V[Vy];
}

void vcpu::OP_8xy2()
{
    uint8_t Vx = (opcode & 0x0F00) >> 8;
    uint8_t Vy = (opcode & 0x00F0) >> 4;

    V[Vx] &= V[Vy];
}

void vcpu::OP_8xy3()
{
    uint8_t Vx = (opcode & 0x0F00) >> 8;
    uint8_t Vy = (opcode & 0x00F0) >> 4;

    V[Vx] ^= V[Vy];
}

void vcpu::OP_8xy4()
{
    uint8_t Vx = (opcode & 0x0F00) >> 8;
    uint8_t Vy = (opcode & 0x00F0) >> 4;

    uint64_t sum = V[Vx] + V[Vy];

    V[0xF] = sum > 255u ? 1 : 0;

    V[Vx] = sum & 0xFFu;
}

void vcpu::OP_8xy5() // Vx -= Vy
{
    uint8_t Vx = (opcode & 0x0F00) >> 8;
    uint8_t Vy = (opcode & 0x00F0) >> 4;

    V[0xF] = V[Vy] < V[Vx] ? 1 : 0;

    V[Vx] -= V[Vy];
}

void vcpu::OP_8xy6() // SHR Vx = Vy >> 1
{
    uint8_t Vx = (opcode & 0x0F00) >> 8;
    uint8_t Vy = (opcode & 0x00F0) >> 4;

    V[0xF] = V[Vx] & 1u;

    V[Vx] = V[Vy] >> 1;
}

void vcpu::OP_8xy7()
{
    uint8_t Vx = (opcode & 0x0F00) >> 8;
    uint8_t Vy = (opcode & 0x00F0) >> 4;

    V[0xF] = V[Vy] > V[Vx] ? 1 : 0;

    V[Vx] = V[Vy] - V[Vx];
}

void vcpu::OP_8xyE()
{
    uint8_t Vx = (opcode & 0x0F00) >> 8;
    uint8_t Vy = (opcode & 0x00F0) >> 4;

    V[0xF] = V[Vy] > V[Vx] ? 1 : 0;

    V[Vx] = V[Vx] << 1;

    // V[Vx] = V[Vy] - V[Vx];
}

void vcpu::OP_9xy0()
{
    uint8_t Vx = (opcode & 0x0F00) >> 8;
    uint8_t Vy = (opcode & 0x00F0) >> 4;

    if (V[Vx] != V[Vy])
        pc += 2;
}

void vcpu::OP_Annn()
{
    uint16_t address = (opcode & 0x0FFFu);
    I = address;
}

void vcpu::OP_Bnnn()
{
    uint16_t address = (opcode & 0x0FFFu);
    pc = address + V[0];
}

void vcpu::OP_Cxnn()
{
    uint8_t mask = opcode & 0x00FFu;
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    V[Vx] = rand() % mask;
}

void vcpu::OP_Dxyn()
{
    drawflag = true;

    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;
    uint8_t height = opcode & 0x000Fu;

    uint8_t xpos = V[Vx] % VIDEO_WIDTH;
    uint8_t ypos = V[Vy] % VIDEO_HEIGHT;

    V[0xF] = 0;
    for (unsigned int row = 0; row < height; row++)
    {
        uint8_t sprByte = memory[I + row];
        for (unsigned int col = 0; col < 8; col++)
        {
            uint8_t sprPixel = sprByte & (0x80u >> col);
            uint32_t* scrPixel = &gfx[(ypos + row) * VIDEO_WIDTH + (xpos + col)];
            if (sprPixel)
            {
                if (*scrPixel == 0xFFFFFFFF)
                    V[0xF] = 1;

                *scrPixel ^= 0xFFFFFFFF;
            }

        }
    }
}

void vcpu::OP_Ex9E()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t key = V[Vx];

    if (keys[key])
        pc += 2;
}

void vcpu::OP_ExA1()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t key = V[Vx];

    if (!keys[key])
        pc += 2;
}

void vcpu::OP_Fx07()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    V[Vx] = delay_timer;
}

void vcpu::OP_Fx0A()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    
    bool flag = true; // True if a key has been pressed, false if none
    for(int i = 0; i < 16; i++)
    {
        if (keys[i]){
            V[Vx] = i;
            flag = true;
            break;
        }
    }

    if (flag)
    {
        pc -= 2;
    }
}

void vcpu::OP_Fx15()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    delay_timer = V[Vx];
}

void vcpu::OP_Fx18()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    delay_timer = V[Vx];
}

void vcpu::OP_Fx1E()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    I += V[Vx];
}

void vcpu::OP_Fx29()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t digit = V[Vx];

	I = FONT_START_ADDRESS + (5 * digit);
}

void vcpu::OP_Fx33()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t value = V[Vx];

    memory[I + 2] = value % 10;
	value /= 10;

	// Tens-place
	memory[I + 1] = value % 10;
	value /= 10;

	// Hundreds-place
	memory[I] = value % 10;
}

void vcpu::OP_Fx55()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    for (uint8_t i = 0; i < Vx; i++)
    {
        memory[I + 1] = V[i];
    }
}

void vcpu::OP_Fx65()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    for (uint8_t i = 0; i < Vx; i++)
    {
        V[i] = memory[I + 1];
    }
}

void vcpu::OP_NULL(){}