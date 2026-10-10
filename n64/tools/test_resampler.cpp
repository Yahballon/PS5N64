#include "resampler.h"
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdio>
int main(){for(double rate:{32000.0,44100.0,48000.0}){n64::Resampler resampler;resampler.Reset(rate);size_t total=0;for(size_t at=0;at<size_t(rate);at+=333){size_t frames=std::min(size_t(333),size_t(rate)-at);std::vector<int16_t> input(frames*2);for(size_t i=0;i<frames;++i){input[2*i]=1000;input[2*i+1]=-1000;}auto output=resampler.Process(input);assert(output.size()%2==0);for(size_t i=0;i<output.size();i+=2){assert(output[i]==1000&&output[i+1]==-1000);}total+=output.size()/2;}assert(total>=47998&&total<=48001);}puts("Streaming sample-rate conversion preserves one-second duration and stereo polarity (AI-assisted)");}
