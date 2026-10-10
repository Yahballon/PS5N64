# Updating your GitHub source (AI-assisted)

1. Extract `PS5N64-source-v7-by-YAHBALLON.zip`.
2. Copy the contents of its `PS5N64` folder into the root of your existing source repository. Do not create a nested PS5N64 folder.
3. Review the changes, then commit and push using your normal Git workflow.

This ZIP contains a complete source snapshot, not a patch. Git history, builds,
downloaded SDKs, old source snapshots and game/user data are excluded. Existing
files in your repository that are absent from this ZIP are not automatically
deleted. The separate runnable release ZIP belongs in GitHub Releases.

Dependencies must be prepared separately as described in BUILDING.md. This source
snapshot includes the vendored N64 core and its pinned revision/license notices.
The source manifest records SHA-256 hashes for all files in the snapshot.

After editing source, run `python3 tools/source_release.py --write --zip` from
the repository root to regenerate the manifest and verified source archive.
Run `python3 tools/source_release.py` to check the manifest without changing it.
Inherited PCSX2 GitHub automation is archived in `docs/upstream-github/`; keep
it there when updating this repository so those upstream release jobs stay inactive.
