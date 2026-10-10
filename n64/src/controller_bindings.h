// (AI-assisted) N64 controller mapping and edge-triggered emulator shortcuts.
#pragma once
#include <array>
#include <cstdint>
#include <map>
#include <string>
namespace n64 {
struct ButtonDefinition {const char* key;const char* label;const char* fallback;unsigned nes_id;};
inline constexpr ButtonDefinition buttons[]={
 {"N64/ButtonA","N64 A","Cross",0},{"N64/ButtonB","N64 B","Square",1},
 {"N64/ButtonCUp","C Up","Triangle",9},{"N64/ButtonCDown","C Down","Circle",8},
 {"N64/ButtonCLeft","C Left","None",10},{"N64/ButtonCRight","C Right","None",11},
 {"N64/ButtonZ","Z Trigger","L2",12},{"N64/ButtonL","L Shoulder","L1",2},
 {"N64/ButtonR","R Shoulder","R1",13},{"N64/ButtonStart","N64 Start","Options",3},
 {"N64/ButtonUp","D-pad Up","Up",4},{"N64/ButtonDown","D-pad Down","Down",5},
 {"N64/ButtonLeft","D-pad Left","Left",6},{"N64/ButtonRight","D-pad Right","Right",7}
};
struct Source {const char* name;const char* label;uint32_t mask;bool all;};
inline constexpr Source sources[]={
    {"None","None",0,false},{"Cross","Cross",0x4000,false},{"Circle","Circle",0x2000,false},
    {"Square","Square",0x8000,false},{"Triangle","Triangle",0x1000,false},
    {"Options","Options",8,false},{"Touchpad","Touchpad click",0x40100000,false},{"Create","Create",1,false},
    {"Touchpad/Create","Touchpad / Create",0x40100001,false},
    {"L1","L1",0x400,false},{"R1","R1",0x800,false},{"L2","L2",0x100,false},{"R2","R2",0x200,false},
    {"L3","L3",2,false},{"R3","R3",4,false},{"Up","D-pad Up",0x10,false},{"Down","D-pad Down",0x40,false},
    {"Left","D-pad Left",0x80,false},{"Right","D-pad Right",0x20,false},
    {"L3R3","L3 + R3",6,true},{"L1R1","L1 + R1",0xc00,true}
};
struct ShortcutDefinition {const char* name;const char* label;std::array<const char*,3> fallback;};
inline constexpr ShortcutDefinition shortcuts[]={
    {"Save","Save state",{"L3R3","Up","None"}},
    {"Load","Load state",{"L3R3","Down","None"}},
    {"Menu","Back to menu",{"L1","R1","Touchpad"}}
};
struct ControllerFrame {uint16_t joypad=0;bool save=false,load=false,menu=false;};
class ControllerBindings {
public:
    void Load(const std::map<std::string,std::string>& settings);
    ControllerFrame Update(uint32_t raw,double now);
    bool left_stick_dpad=false;
private:
    std::array<Source,14> mapping{};
    std::array<std::array<Source,3>,3> chords{};
    std::array<bool,3> previous{};
    bool initialized=false;
    double emergency_since=-1;
};
}
