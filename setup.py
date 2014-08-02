#! /usr/bin/python
from distutils.core import setup, Extension

setup(
    ext_modules=[Extension("_scoring_board", ["_scoring_board.c", "scoring_board.c"])],
)
