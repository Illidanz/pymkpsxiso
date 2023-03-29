# pymkpsxiso
Python interface for mkpsxiso.

## Installation
`pip install pymkpsxiso`

## Usage
```
import pymkpsxiso

pymkpsxiso.dump("img.bin", "extract", "extract.xml")
pymkpsxiso.make("img_new.bin", "img_new.cue", "extract.xml")
```
