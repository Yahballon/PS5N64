// (AI-assisted) Original generated N64 test program, no copyrighted game/boot ROM.
#include "core_session.h"
#include <cstdio>
#include <cassert>
#include <filesystem>
#include <vector>
#include <cstring>
static void be(std::vector<uint8_t>&r,size_t at,uint32_t v){r[at]=v>>24;r[at+1]=v>>16;r[at+2]=v>>8;r[at+3]=v;}
extern "C" uint32_t** plugin_get_vi_registers(); extern "C" uint32_t egcvip_get_input(void*); extern "C" int g_real_stop; extern "C" int frame_break;
int main(int argc,char**argv){assert(argc==2);std::string save_dir=argv[1];
 std::filesystem::create_directories(save_dir);
 std::vector<uint8_t> rom(1<<20);be(rom,0,0x80371240);be(rom,8,0x80000400);rom[0x3e]='E';std::memcpy(rom.data()+0x20,"PS5N64 ORIGINAL TEST ",20);
 std::vector<uint32_t> code;
 auto emit=[&](uint32_t v){code.push_back(v);};
 auto li=[&](unsigned reg,uint32_t v){emit(0x3c000000|(reg<<16)|(v>>16));emit(0x34000000|(reg<<21)|(reg<<16)|(v&65535));};
 li(8,0xa0100000);li(9,0xf801f801);li(10,0xa0125800);
 emit(0xad090000);emit(0x25080004);emit(0x150afffd);emit(0);
 li(8,0xa410000c);li(9,0x15);emit(0xad090000);
 li(8,0xa4400000);
 uint32_t vi[]={0x302,0x100000,320,2,0,0x03e52239,525,0x0c15,0x0c150c15,0x00800300,0x002501ff,0x000e0204,0x200,0x400};
 for(unsigned i=0;i<14;++i){li(9,vi[i]);emit(0xad090000|(i*4));}
 li(8,0xa0200000);li(9,0x40004000);li(10,0xa0201000);
 emit(0xad090000);emit(0x25080004);emit(0x150afffd);emit(0);
 li(8,0xa4500000);li(9,1);emit(0xad090008);li(9,1520);emit(0xad090010);li(9,15);emit(0xad090014);li(9,0x200000);emit(0xad090000);li(9,4096);
 unsigned pc=0xa4000040+unsigned(code.size()*4);emit(0xad090004);emit(0x08000000|((pc>>2)&0x03ffffff));emit(0);
 for(size_t i=0;i<code.size();++i)be(rom,0x40+i*4,code[i]);
 // HLE PIF starts executing our own IPL3 at A4000040; it only draws a red framebuffer.
 n64::Session core(save_dir);
 core.SetOption("parallel-n64-cpucore","cached_interpreter");core.SetOption("parallel-n64-gfxplugin","angrylion");core.SetOption("parallel-n64-rspplugin","hle");core.SetOption("parallel-n64-angrylion-multithread","2");core.SetOption("parallel-n64-alt-map","enabled");
 if(!core.Load("original-test.z64",rom)){printf("LOAD ERROR %s\n",core.error.c_str());return 1;}
 bool audible=false;for(int i=0;i<12;++i){core.Run();for(auto sample:core.audio)audible|=sample!=0;printf("STOP %d BREAK %d ",g_real_stop,frame_break);printf("FRAME %d %ux%u pixels=%zu audio=%zu error=%s\n",i,core.width,core.height,core.pixels.size(),core.audio.size(),core.error.c_str());fflush(stdout);}

 auto vi_regs=plugin_get_vi_registers();for(int i=0;i<14;++i)printf("VI%d=%08x ",i,*vi_regs[i]);printf("\n");
 size_t nonblack=0;uint32_t first=0;for(auto p:core.pixels)if(p&0xffffff){++nonblack;first=p;}printf("NONBLACK %zu first=%08x\n",nonblack,first);
 size_t red=0;for(auto p:core.pixels)if((p&255)>150&&((p>>8)&255)<80)++red;
 auto* ram=static_cast<uint32_t*>(retro_get_memory_data(RETRO_MEMORY_SYSTEM_RAM));printf("RAM %08x %08x sample=%08x\n",ram[0x100000/4],ram[0x110000/4],core.pixels[core.pixels.size()/2]);printf("RED %zu/%zu\n",red,core.pixels.size());fflush(stdout);assert(red>core.pixels.size()/2&&!core.pixels.empty());
 assert(audible);printf("AUDIO %.6f Hz produced nonzero samples\n",core.sample_rate);
 int channel=0;core.buttons[0]=1;core.axes[0][0][0]=32767;core.axes[0][0][1]=-32767;core.axes[0][1][0]=32767;uint32_t keys=egcvip_get_input(&channel);assert(keys&0x80);assert(keys&0x100);assert((int8_t)(keys>>16)>0);assert((int8_t)(keys>>24)>0);core.buttons[0]=0;core.axes[0][0][0]=core.axes[0][0][1]=core.axes[0][1][0]=0;
 assert(core.SaveState());uint32_t before=ram[0x100000/4];ram[0x100000/4]=0;assert(core.LoadState());assert(ram[0x100000/4]==before);
 auto* battery=static_cast<uint8_t*>(retro_get_memory_data(RETRO_MEMORY_SAVE_RAM));assert(battery);battery[0]=0x5a;assert(core.SaveBattery());assert(core.Unload());assert(core.Load("original-test.z64",rom));assert(static_cast<uint8_t*>(retro_get_memory_data(RETRO_MEMORY_SAVE_RAM))[0]==0x5a);core.Run();assert(core.Unload());
 be(rom,0x48,0x3c0907c1);be(rom,0x4c,0x352907c1);assert(core.Load("original-green.z64",rom));for(int i=0;i<12;++i)core.Run();size_t green=0;for(auto p:core.pixels)if(((p>>8)&255)>150&&(p&255)<80)++green;printf("GREEN after reload %zu/%zu\n",green,core.pixels.size());fflush(stdout);assert(green>core.pixels.size()/2);assert(core.Unload());
 printf("N64 generated MIPS framebuffer and save-state roundtrip passed (AI-assisted)\n");
}
