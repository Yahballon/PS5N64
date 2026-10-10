"""Prepare and verify the PS5N64 source release (AI-assisted)."""

import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import zipfile

ROOT = Path(__file__).resolve().parents[1]
MANIFEST = ROOT / "SOURCE-MANIFEST.json"


def source_files():
    # Include both existing tracked files and new source files, honoring ignores.
    result = subprocess.run(
        ["git", "-c", f"safe.directory={ROOT.as_posix()}", "ls-files",
         "--cached", "--others", "--exclude-standard", "-z"],
        cwd=ROOT, check=True, stdout=subprocess.PIPE,
    )
    names = sorted(set(result.stdout.decode("utf-8").split("\0")) - {"", MANIFEST.name})
    files = {}
    for name in names:
        path = ROOT / name
        if path.is_symlink() or not path.resolve().is_relative_to(ROOT):
            raise ValueError(f"Source release cannot include a link outside the tree: {name}")
        if not path.is_file():
            continue  # A tracked file may have been removed in the working tree.
        if ".git" in Path(name).parts:
            raise ValueError(f"Git metadata must not be included: {name}")
        if path.suffix.lower() in {".o", ".obj", ".elf", ".prx", ".exe", ".z64", ".v64", ".n64", ".nes", ".sfc", ".smc"}:
            raise ValueError(f"Build output or game image must not be included: {name}")
        files[name] = hashlib.sha256(path.read_bytes()).hexdigest()
    return files


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--write", action="store_true", help="Refresh the SHA-256 source manifest")
    parser.add_argument("--zip", action="store_true", help="Create the source ZIP under release/")
    args = parser.parse_args()
    files = source_files()
    manifest = {
        "project": "PS5N64", "version": "v7 by YAHBALLON", "ai_assisted": True,
        "core": "ParaLLEl N64 / Angrylion, cached interpreter", "files": files,
    }
    if args.write:
        MANIFEST.write_text(json.dumps(manifest, indent=2) + "\n", encoding="utf-8")
    saved = json.loads(MANIFEST.read_text(encoding="utf-8"))
    if saved != manifest:
        old = saved.get("files", {})
        changed = sorted(name for name in set(files) | set(old) if files.get(name) != old.get(name))
        for name in changed:
            print(f"Manifest mismatch: {name}")
        raise SystemExit("Source manifest is stale. Review changes, then run with --write.")
    print(f"Verified {len(files)} source file hashes (AI-assisted).")
    if args.zip:
        output = ROOT / "release" / "PS5N64-source-v7-by-YAHBALLON.zip"
        output.parent.mkdir(exist_ok=True)
        with zipfile.ZipFile(output, "w", zipfile.ZIP_DEFLATED) as archive:
            for name in sorted([*files, MANIFEST.name]):
                archive.write(ROOT / name, "PS5N64/" + name)
        with zipfile.ZipFile(output) as archive:
            if archive.testzip() is not None:
                raise SystemExit("Source ZIP integrity check failed.")
            for name, digest in files.items():
                if hashlib.sha256(archive.read("PS5N64/" + name)).hexdigest() != digest:
                    raise SystemExit(f"Source ZIP hash mismatch: {name}")
        print(f"Created and verified {output}")


if __name__ == "__main__":
    main()
