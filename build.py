"""
Compilation script
"""

import os
import sys
import subprocess


SRC_DIR = "src"

EXE_NAME = "main.exe"

FILES = [os.path.join(SRC_DIR, file) for file in os.listdir(SRC_DIR)]


if "-w" in sys.argv:
    subprocess.run(
        [
            "gcc",
            *FILES,
            "-Wall",
            "-Wextra",
            "-o",
            EXE_NAME,
            "-Iinclude"
        ]
    )
else:
    subprocess.run(
        [
            "gcc",
            *FILES,
            "-o",
            EXE_NAME,
            "-Iinclude"
        ]
    )


if "-r" in sys.argv:
    subprocess.run([EXE_NAME])
