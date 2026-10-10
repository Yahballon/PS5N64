# PS5N64

**v7 modded by YAHBALLON — AI-assisted development**

Nintendo 64 homebrew emulator for PS5. Uses the original PS5SX2 cover-flow interface and the ParaLLEl N64 core.

## Features

- N64 cartridges: `.z64`, `.v64`, `.n64`, and a ZIP containing exactly one supported ROM (Stored/Deflate).
- Internal storage, USB and directly configured NFSv3 shares. Local and USB folders are scanned on startup; the NFS library is cached.
- Original animated game shelf with a green N64 theme, settings and local web settings.
- Analog controller stick and C buttons, configurable digital bindings, save/load states and battery/Controller Pak saves.
- Native stereo audio resampled to the PS5 audio device at 48 kHz.

## Installation

Extract `PS5N64-v7-Audio-Fix-modded-by-YAHBALLON.zip`, install the included `PPSA99206` app with your homebrew installer, and restart the app after configuring game locations.

ROM locations:

- `/data/PS5N64/games/`
- `/mnt/usb0/homebrew/PS5N64/games/` (USB0 through USB7)
- `/mnt/usb0/homebrew/PS5N64/roms/`
- `/mnt/usb0/PS5N64/games/` or `/mnt/usb0/N64/`

NFS: set your export URL in All games settings, for example `nfs://192.168.0.87/N64`. This is a configuration example, not a tested export. Configuration, saves, cache and logs live under `/data/PS5N64/`; `/download0/PS5N64/` is a writable fallback.

## Default controls

| N64 | DualSense |
|---|---|
| Analog stick | Left stick |
| C directions | Right stick |
| A / B | Cross / Square |
| C Up / C Down | Triangle / Circle |
| Z | L2 |
| L / R | L1 / R1 |
| Start | Options |
| D-pad | D-pad |
| Save state | L3 + R3 + Up |
| Load state | L3 + R3 + Down |
| Return to shelf | L1 + R1 + Touchpad click |

Digital buttons and emulator shortcuts can be changed globally or per game. The exit shortcut also has a two-second emergency hold. One active controller is exposed in the PS5 app.

## Build and validation

Build: `PS5N64_DEPS=/path/to/prepared/deps bash n64/tools/build.sh`. Uses the existing public PS5 SDK, PS5_Vulkan, libnfs and static HTTPS dependencies. Core objects and the frontend are built outside their source directories.

This build uses a cached interpreter, HLE RSP and Angrylion software rendering at 1x. No executable-memory JIT is enabled. Performance and commercial-game compatibility need PS5 testing; full speed is not guaranteed. 64DD, Transfer Pak, VRU, rumble and multiplayer are not exposed by this frontend.

The supplied host test generates original MIPS cartridge code and checks framebuffer output, audio DMA, analog/C-button input, battery saves, save-state roundtrip and loading a second ROM. It does not contain commercial games or Nintendo boot firmware.

## Credits and licensing

YAHBALLON — customization. Spyros / PCSX2 contributors — original PS5SX2 frontend. ParaLLEl N64 / Mupen64Plus, Angrylion, libretro, PS5_Vulkan and their contributors — emulator and supporting libraries. Preserve upstream notices and the included GPL and component licenses when redistributing. Source modifications were AI-assisted. Not affiliated with Nintendo, Sony or the upstream projects.

Cover downloads use the filename index from https://github.com/Globaleliteee/Gaming_Console_Covers_Art_Repository/tree/main/covers/nintendo/N64 (801 front covers), with title/region matching and libretro-thumbnails fallback. Additional ROM revision/language tags do not prevent a match. Existing failed lookups are retried after this update. Artwork stays cached locally. Development is AI-assisted.
