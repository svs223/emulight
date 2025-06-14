#include <iostream>
#include <thread>
#include <memory>
#include <cstdint>

#include "vcpu.hpp"
#include "vgpu.hpp"
#include "tsq.hpp"
#include "msg.hpp"

int main(int argc, char **argv)
{
    if (argc < 2){
        printf("Invalid arguments\n");
        return 1;
    }
    printf("Creating CPU and GPU Objects\n");

    vcpu cpu;
    vgpu gpu("Potato8", cpu.VIDEO_WIDTH * 10, cpu.VIDEO_HEIGHT * 10, cpu.VIDEO_WIDTH, cpu.VIDEO_HEIGHT);
    thread_queue<Message> gpuq;
    thread_queue<Message> cpuq;

    printf("Initalizing CPU\n");

    cpu.initalize();

    printf("Loading game\n");

    cpu.loadGame(argv[1]);

    printf("Game loaded, starting emulation\n");

    // for(int i = 0; i < 4096; i++)
    // {
    //     uint16_t opc1 = (cpu.memory[i] << 8u);
    //     ++i;
    //     uint16_t opc2 = cpu.memory[i];
    //     printf("0x%04X : 0x%04X\n", i - 1, opc1 + opc2);
    // }

    // return 0;
    // std::terminate();

    int pitch = cpu.calcPitch();

    std::thread process_thread(vcpu::start, std::ref(cpu), std::ref(gpuq), std::ref(cpuq));

    vgpu::start(std::ref(gpu), std::ref(gpuq), std::ref(cpuq), pitch);

    process_thread.detach();

    return 0;
}