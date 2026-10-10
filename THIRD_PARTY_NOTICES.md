# Third-party components

(AI-assisted) Original copyright/license notices are retained in the source tree and application license directory. The following identifies the main components; it does not replace their license texts.

| Component | Source / notices |
| --- | --- |
| PS5SX2/PCSX2 frontend and scaffold | Retained source headers and COPYING.GPLv3 |
| ParaLLEl N64 / Mupen64Plus and Angrylion | n64/third_party/parallel-n64 source headers, component COPYING/LICENSE files, and n64/licenses |
| PS5_Vulkan | https://github.com/mihawk-99/PS5_Vulkan; its LICENSE and dependency notices |
| OpenGL-derived shader compiler toolchain | https://github.com/blackbearreloaded/ps5-opengl; retained source notices |
| PS5 payload SDK fork | https://github.com/mihawk-99/PS5_PayloadSDK |
| Native app boilerplate | https://github.com/blackbearreloaded/ps5-native-app-boilerplate; GPL-3.0-or-later source headers |
| Restricted Lapy helper | Pinned build manifest and included Lapy-MIT.txt |
| Libnfs | https://github.com/sahlberg/libnfs; included COPYING notice |
| libcurl | curl notice in n64/licenses/ and application licenses/ |
| OpenSSL | OpenSSL notice in n64/licenses/ and application licenses/ |
| zlib / zstd / libpsl | Included individual notices |
| Mozilla CA bundle | MPL-2.0 notice and bundle provenance in source |
| Roboto, PromptFont, Font Awesome and stb/qrcode code | Retained font/library notices and source headers |
| Launcher logo | Supplied by YAHBALLON; original image retained in n64/assets/ |

The main application ZIP includes a licenses/ directory inside PPSA99206. The vendored N64 core revision is recorded in n64/third_party/parallel-n64/UPSTREAM_REVISION.txt. BUILDING.md describes the required prepared dependencies. Additional dependency source snapshots are supplied separately; prepared SDK and PacBrew binary caches are not embedded in the project source ZIP.
