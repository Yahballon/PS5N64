// (AI-assisted) N64 ROM byte-order, ZIP and library-cache regression checks.
#include "games.h"
#include "library_cache.h"
#include "controller_bindings.h"
#include <cassert>
#include <filesystem>
#include <cstdio>
int main(int argc,char**argv){assert(argc==2);std::string root=argv[1],error;std::vector<uint8_t> canonical,rom;assert(n64::ReadRom(root+"/test.z64",canonical,error));for(auto file:{"test.v64","test.n64","test.zip","stored.zip"}){assert(n64::ReadRom(root+"/"+file,rom,error));assert(rom==canonical);}for(auto file:{"bad.z64","multiple.zip","empty.zip","corrupt.zip"}){assert(!n64::ReadRom(root+"/"+file,rom,error));assert(rom.empty());}auto scanned=n64::Scan({root});assert(scanned.size()==9);std::filesystem::create_directories(root+"/cache");auto cache=n64::CacheFile(root,{root});assert(n64::WriteLibrary(cache,scanned));std::vector<n64::Game> restored;assert(n64::ReadLibrary(cache,{root},restored));assert(restored.size()==scanned.size());n64::ControllerBindings input;input.Load({});assert(!input.left_stick_dpad);assert(input.Update(0x4000,0).joypad&1);assert(input.Update(0x100,1).joypad&(1u<<12));assert(input.Update(0x800,2).joypad&(1u<<13));assert(input.Update(0x400,3).joypad&(1u<<2));printf("N64 z64/v64/n64 normalization, Stored/Deflate ZIP, invalid ZIP rejection, warm cache and bindings passed (AI-assisted)\n");}
