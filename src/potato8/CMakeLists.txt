add_executable(p8 main.cpp vcpu.cpp vcpu_opcode.cpp vcpu_table.cpp vgpu.cpp msg.cpp include/msg.hpp include/packet/key_press_packet.hpp include/vgpu.hpp include/vcpu.hpp include/tsq.hpp)
target_include_directories(p8 PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(p8 PRIVATE SDL3::SDL3)