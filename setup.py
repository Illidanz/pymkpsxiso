import os
import platform
import subprocess
from setuptools import setup, Extension

with open("README.md", "r") as fh:
    long_description = fh.read()

MACROS = [
    ("MKPSXISO_NO_LIBFLAC", "1"),
]

def is_musl_system():
    if "musl" in platform.libc_ver()[0].lower():
        return True
    try:
        result = subprocess.run(['ldd', '/bin/ls'], stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        if 'musl' in result.stdout.lower() or 'musl' in result.stderr.lower():
            return True
    except (FileNotFoundError, OSError):
        pass
    return False

if is_musl_system():
    print("Musl libc detected: Aliasing stat64 to stat")
    MACROS.append(("stat64", "stat"))
    MACROS.append(("fstat64", "fstat"))
    MACROS.append(("lstat64", "lstat"))
    MACROS.append(("fseeko64", "fseeko"))
    MACROS.append(("ftello64", "ftello"))

if not os.name == 'nt':
    EXTRA_COMPILE_ARGS = ["-std=c++17"]
else:
    EXTRA_COMPILE_ARGS = ["/std:c++17"]

INCLUDES = ["mkpsxiso/src/shared", "mkpsxiso/src/dumpsxiso", "mkpsxiso/src/mkpsxiso", "mkpsxiso/ThreadPool", "mkpsxiso/tinyxml2", "mkpsxiso/miniaudio"]

sources = ["pymkpsxiso.cpp"]
for dir in INCLUDES:
    for file in os.listdir(dir):
        if file.endswith(".cpp") and "xmltest" not in file and "example" not in file and "main.cpp" not in file:
            sources.append(dir + "/" + file)

def main():
    setup(name="pymkpsxiso",
          version="0.1.8",
          author="Illidan",
          description="Python interface for mkpsxiso.",
          long_description=long_description,
          long_description_content_type="text/markdown",
          url="https://github.com/Illidanz/pymkpsxiso",
          license="MIT",
          classifiers=[
              "Programming Language :: Python :: 3",
          ],
          ext_modules=[Extension("pymkpsxiso", sources, include_dirs=INCLUDES, define_macros=MACROS, extra_compile_args=EXTRA_COMPILE_ARGS)]
        )

if __name__ == "__main__":
    main()
