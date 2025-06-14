#include "vcpu.hpp"

void vcpu::Table0()
{
    ((*this).*(table0[opcode & 0x000Fu]))();
}

void vcpu::Table8()
{
    ((*this).*(table0[opcode & 0x000Fu]))();
}

void vcpu::TableE()
{
    ((*this).*(table0[opcode & 0x000Fu]))();
}

void vcpu::TableF()
{
    ((*this).*(table0[opcode & 0x00FFu]))();
}