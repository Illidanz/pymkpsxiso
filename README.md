# pymkpsxiso
Python interface for mkpsxiso.

## Installation
`pip install pymkpsxiso`

## Usage
```
import pymkpsxiso
import pydumpsxiso

pydumpsxiso.run("img.bin", "extract", "extract.xml")
pymkpsxiso.run("img_new.bin", "img_new.cue", "extract.xml")
```
