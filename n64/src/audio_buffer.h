// (AI-assisted) Bounded stereo queue with rebuffering and short click-suppression fades.
#pragma once
#include <algorithm>
#include <array>
#include <cstdint>
#include <mutex>
#include <vector>
namespace n64 {
class AudioBuffer {
    struct Stereo{int16_t l=0,r=0;};
    static constexpr size_t capacity=8192,max_buffered=6144,prebuffer=3072,fade=64;
    std::array<Stereo,capacity> ring{};size_t read=0,count=0,fade_in=0;
    bool playing=false;Stereo last{};std::mutex mutex;
public:
    static constexpr size_t grain_frames=1024;
    void Push(const std::vector<int16_t>& samples){
        std::lock_guard lock(mutex);size_t frames=samples.size()/2,offset=0;
        if(frames>max_buffered){offset=frames-max_buffered;frames=max_buffered;}
        if(count+frames>max_buffered){size_t drop=count+frames-max_buffered;read=(read+drop)%capacity;count-=drop;fade_in=fade;}
        for(size_t i=0;i<frames;++i)ring[(read+count+i)%capacity]={samples[(offset+i)*2],samples[(offset+i)*2+1]};count+=frames;
    }
    void Read(int16_t* output,size_t frames){
        std::lock_guard lock(mutex);std::fill(output,output+frames*2,0);
        if(!playing&&count>=prebuffer){playing=true;fade_in=fade;}
        if(!playing){for(size_t i=0;i<std::min(frames,fade);++i){output[2*i]=int32_t(last.l)*static_cast<int32_t>(fade-1-i)/static_cast<int32_t>(fade);output[2*i+1]=int32_t(last.r)*static_cast<int32_t>(fade-1-i)/static_cast<int32_t>(fade);}last={};return;}
        const size_t available=std::min(count,frames);
        for(size_t i=0;i<available;++i){auto value=ring[read];read=(read+1)%capacity;--count;
            if(fade_in){const size_t gain=fade-fade_in;value.l=int32_t(value.l)*static_cast<int32_t>(gain)/static_cast<int32_t>(fade);value.r=int32_t(value.r)*static_cast<int32_t>(gain)/static_cast<int32_t>(fade);--fade_in;}
            output[2*i]=value.l;output[2*i+1]=value.r;}
        if(available<frames){const size_t ramp=std::min(available,fade);
            for(size_t i=0;i<ramp;++i){const size_t at=available-ramp+i;output[2*at]=int32_t(output[2*at])*static_cast<int32_t>(ramp-1-i)/static_cast<int32_t>(ramp);output[2*at+1]=int32_t(output[2*at+1])*static_cast<int32_t>(ramp-1-i)/static_cast<int32_t>(ramp);}
            if(!available){for(size_t i=0;i<std::min(frames,fade);++i){output[2*i]=int32_t(last.l)*static_cast<int32_t>(fade-1-i)/static_cast<int32_t>(fade);output[2*i+1]=int32_t(last.r)*static_cast<int32_t>(fade-1-i)/static_cast<int32_t>(fade);}}
            playing=false;last={};
        }else if(frames)last={output[2*(frames-1)],output[2*(frames-1)+1]};
    }
};
}
