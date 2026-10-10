#!/usr/bin/env bash
# (AI-assisted) Build the separate PS5N64 app using existing, public PS5 dependencies.
set -euo pipefail
here=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
: "${PS5N64_DEPS:?Set PS5N64_DEPS to an absolute path containing prepared dependencies; see BUILDING.md}"
export PS5_PAYLOAD_SDK=${PS5_PAYLOAD_SDK:-$PS5N64_DEPS/PS5_Vulkan/.deps/native/ps5-payload-sdk}
export LIBNFS_PREFIX=${LIBNFS_PREFIX:-$PS5N64_DEPS/libnfs-install}
export LIBZ_PREFIX=${LIBZ_PREFIX:-$PS5N64_DEPS/filesystem-helper/.deps/pacbrew/v0.40.2/sysroot/user/homebrew}
python3 "$here/build_shelf.py"
