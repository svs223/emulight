#pragma once

#include <string>
#include <mutex>
#include <queue>

#include <SDL3/SDL.h>
#include <GL/glew.h>

#include "tsq.hpp"
#include "msg.hpp"

using std::string;

class vgpu {
    public:
        void gpumain(thread_queue<Message>& gpuq, thread_queue<Message>& cpuq, int pitch);
        void draw(void const* buffer, int pitch);

        static void start(vgpu& gpu, thread_queue<Message>& gpuq, thread_queue<Message>& cpuq, int pitch)
        {
            gpu.gpumain(gpuq, cpuq, pitch);
        }

        vgpu(string title, unsigned int winwidth, unsigned int winheight, int texwidth, int texheight);
        ~vgpu();

    private:
        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Texture* texture;
};