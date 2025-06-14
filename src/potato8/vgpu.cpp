#include <string>
#include <optional>
#include <mutex>
#include <variant>

#include <SDL3/SDL.h>

#include "vgpu.hpp"
// #include "vcpu.hpp"
#include "tsq.hpp"
#include "msg.hpp"


using std::string;

vgpu::vgpu(string title, unsigned int winwidth, unsigned int winheight, int texwidth, int texheight)
{
   SDL_Init(SDL_INIT_VIDEO);
   window = SDL_CreateWindow(title.c_str(), winwidth, winheight, SDL_WINDOW_RESIZABLE);
   renderer = SDL_CreateRenderer(window, nullptr);
   texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, texwidth, texheight);
}

vgpu::~vgpu()
{
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void vgpu::draw(void const *buffer, int pitch)
{
    SDL_UpdateTexture(texture, nullptr, buffer, pitch);
    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

void vgpu::gpumain(thread_queue<Message>& gpuqueue, thread_queue<Message>& cpuqueue, int pitch)
{
    SDL_Event ev;
    bool run = true;
    bool ready2draw = false;
    bool pressed[0xF];
    uint32_t *gfxv;

    for(int i = 0xF; i > 0; i--)
    {
        pressed[i] = false;
    }

    printf("GPU: Initalized\n");
    while(run)
    {
        while(SDL_PollEvent(&ev) != 0)
        {
            switch(ev.type)
            {
                case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                case SDL_EVENT_QUIT:
                    run = false;
                    break;
                /*
                case SDL_EVENT_KEY_DOWN:
                    switch(ev.key.key)
                    {
                        case SDLK_ESCAPE:
                            run = false;
                            break;
                        case SDLK_1:
                            KeyStatusPacket packet;
                            packet.key = 0;
                            packet.status = true;
                            eventvar cpuev = packet;
                            Message msg(cpuev, MsgType::KeyStatusUpdate,  MsgTarget::CpuEvent);
                            cpuqueue.push(msg);
                    } */
                default:
                    break;   
            }
        }

        while (!gpuqueue.empty())
        {
            // printf("GPU: Recieved an event\n");

            Message msg = gpuqueue.peek();
            gpuqueue.pop();
            
            if (msg.target != MsgTarget::GpuEvent)
                continue;

            if (std::holds_alternative<FramebufferUpdatePacket>(msg.data))
            {
                // if (ready2draw)
                //     delete[] gfxv;
                gfxv = std::get<FramebufferUpdatePacket>(msg.data).fb;
                ready2draw = true;
            }
        }
        if (ready2draw)
            draw(gfxv, pitch);
    }
}