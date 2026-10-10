# PS5N64 v7 by YAHBALLON (AI-assisted)

This source update contains the current PS5N64 frontend and software N64 backend.
It includes local/USB cache fixes, bounded offline-NAS detection, correct empty
library paths, an English UI, cache-reset/NFS-clear settings actions, and the
latest stereo audio buffering changes. The original shelf UI and save/controller
features are retained.

Host validation covers local/USB additions/removals with cached NFS, offline
fallback, NFS configuration deletion and settings reload, cache-only reset,
audio buffering/fades/recovery and streaming sample-rate duration. Native builds,
memory parameters and signed executable integrity passed. The v7 audio changes
still need PS5 testing across games. No ParaLLEl-RDP or Gopher64 port is included.

## GitHub source cleanup — 2026-10-10 (AI-assisted)

- Archived inherited PCSX2 GitHub automation and the leftover PS5SNES readme.
- Redirected the old NES-based build entry point to the supported N64 shelf build.
- Updated both install guides to v7 and matched the documented release ZIP name
  to the build output. Removed a preview link whose image was absent.
- Added source-manifest verification and verified ZIP packaging; retained
  upstream source, assembly, audio resources and license notices.
- Passed the existing stereo audio buffer and streaming resampler checks on
  Windows. Verified source hashes and ZIP contents, Python syntax and doc paths.

The PS5 native build and Linux core integration tests were not rerun during this
source cleanup: the required prepared PS5 dependencies were unavailable here,
and WSL access was denied. No new console compatibility claims are made.
