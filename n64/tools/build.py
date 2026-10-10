"""Compatibility entry point for the PS5N64 shelf build (AI-assisted)."""
from pathlib import Path
import runpy

if __name__ == "__main__":
    runpy.run_path(str(Path(__file__).with_name("build_shelf.py")), run_name="__main__")
