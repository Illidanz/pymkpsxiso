import os
import platform
import subprocess
from setuptools import setup, Extension

with open("README.md", "r") as fh:
    long_description = fh.read()

MACROS = [
    ("MKPSXISO_NO_LIBFLAC", "1"),
    ("GHC_FILESYSTEM_IMPLEMENTATION", "1"),
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
    EXTRA_COMPILE_ARGS = ["-std=c++20"]
else:
    EXTRA_COMPILE_ARGS = ["/std:c++20", "/FIstring", "/Zc:strictStrings-"]

SHARED_INCLUDES = ["mkpsxiso/src/shared", "mkpsxiso/ghc/include", "mkpsxiso/threadpool", "mkpsxiso/tinyxml2", "mkpsxiso/miniaudio"]

def get_sources(directories, exclude_mains=True):
    files = []
    for d in directories:
        if not os.path.exists(d): continue
        for f in os.listdir(d):
            if f.endswith(".cpp"):
                if exclude_mains and "main.cpp" in f.lower():
                    continue
                if "xmltest" in f or "example" in f:
                    continue
                files.append(os.path.join(d, f))
    return files

shared_cpp_files = get_sources(SHARED_INCLUDES)
build_sources = ["pymkpsxiso.cpp"] + shared_cpp_files + get_sources(["mkpsxiso/src/mkpsxiso"])
dump_sources = ["pydumpsxiso.cpp"] + shared_cpp_files + get_sources(["mkpsxiso/src/dumpsxiso"])

def main():
    setup(name="pymkpsxiso",
          version="0.2.0",
          author="Illidan",
          description="Python interface for mkpsxiso.",
          long_description=long_description,
          long_description_content_type="text/markdown",
          url="https://github.com/Illidanz/pymkpsxiso",
          license="MIT",
          classifiers=[
              "Programming Language :: Python :: 3",
          ],
          ext_modules=[
              Extension(
                    "pymkpsxiso",
                    sources=build_sources,
                    include_dirs=SHARED_INCLUDES + ["mkpsxiso/src/mkpsxiso"],
                    define_macros=MACROS,
                    extra_compile_args=EXTRA_COMPILE_ARGS
                ),
                Extension(
                    "pydumpsxiso",
                    sources=dump_sources,
                    include_dirs=SHARED_INCLUDES + ["mkpsxiso/src/dumpsxiso"],
                    define_macros=MACROS,
                    extra_compile_args=EXTRA_COMPILE_ARGS
                ),
            ]
        )

if __name__ == "__main__":
    main()
