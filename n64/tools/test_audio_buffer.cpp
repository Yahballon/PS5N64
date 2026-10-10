#include "audio_buffer.h"
#include <cstdio>
#include <cassert>
#include <vector>
#include <array>
#include <thread>
static std::vector<int16_t> samples(size_t frames,int16_t l,int16_t r){std::vector<int16_t> out(frames*2);for(size_t i=0;i<frames;++i){out[2*i]=l;out[2*i+1]=r;}return out;}
int main(){n64::AudioBuffer queue;std::array<int16_t,2048> block{};queue.Push(samples(1024,1000,-1000));queue.Read(block.data(),1024);for(auto s:block)assert(s==0);
 queue.Push(samples(2048,1000,-1000));queue.Read(block.data(),1024);assert(block[0]==0&&block[128]==1000&&block[129]==-1000);
 queue.Read(block.data(),1024);assert(block[0]==1000&&block[1]==-1000);queue.Read(block.data(),1024);queue.Read(block.data(),1024);assert(block[0]>0&&block[1]<0&&block[128]==0);
 queue.Push(samples(40,2000,-2000));queue.Read(block.data(),1024);for(auto s:block)assert(s==0);
 queue.Push(samples(4000,2000,-2000));queue.Read(block.data(),1024);assert(block[128]==2000&&block[129]==-2000);
 n64::AudioBuffer overflow;overflow.Push(samples(7000,1000,-1000));overflow.Push(samples(6144,3000,-3000));overflow.Read(block.data(),1024);assert(block[128]==3000&&block[129]==-3000);
 n64::AudioBuffer concurrent;std::thread writer([&]{for(int i=0;i<300;++i)concurrent.Push(samples(800,200,-200));});for(int i=0;i<300;++i)concurrent.Read(block.data(),1024);writer.join();
 puts("Stereo prebuffer, signed fades, underrun/rebuffer, bounded backlog and concurrent producer/consumer passed (AI-assisted)");}
