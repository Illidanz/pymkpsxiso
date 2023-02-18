import codecs
import os
from setuptools import setup, Extension

with open("README.md", "r") as fh:
    long_description = fh.read()

MACROS = [
    ("MKPSXISO_NO_LIBFLAC", "1"),
    ("VERSION", "\"2.02\""),
]

if not os.name == 'nt':
    EXTRA_COMPILE_ARGS = ["-std=c++17"]
else:
    EXTRA_COMPILE_ARGS = ["/std:c++17"]
    MACROS.append(("_WIN32", ""))

INCLUDES = ["mkpsxiso/src/shared", "mkpsxiso/src/dumpsxiso", "mkpsxiso/src/mkpsxiso", "mkpsxiso/ThreadPool", "mkpsxiso/tinyxml2", "mkpsxiso/miniaudio"]

sources = ["pymkpsxiso.cpp"]
for dir in INCLUDES + ["mkpsxiso/tinyxml2"]:
    for file in os.listdir(dir):
        if file.endswith(".cpp") and "xmltest" not in file and "example" not in file and "main.cpp" not in file:
            sources.append(dir + "/" + file)

def main():
    # This is kind of dirty, but the main.cpp files include a lot of code and both use a Main function, so we need to rename one
    with codecs.open("mkpsxiso/src/dumpsxiso/main.cpp", "r+", "utf-8") as f:
        all = f.read()
        f.seek(0)
        f.write(all.replace("int Main(int argc, char *argv[])", "int dumpsxiso_main(int argc, char *argv[])"))
    setup(name="pymkpsxiso",
          version="0.1.0",
          author="Illidan",
          description="Python interface for mkpsxiso.",
          long_description=long_description,
          long_description_content_type="text/markdown",
          url="https://github.com/Illidanz/pymkpsxiso",
          classifiers=[
              "Programming Language :: Python :: 3",
              "License :: OSI Approved :: MIT License",
          ],
          ext_modules=[Extension("pymkpsxiso", sources, include_dirs=INCLUDES, define_macros=MACROS, extra_compile_args=EXTRA_COMPILE_ARGS)]
        )

if __name__ == "__main__":
    main()