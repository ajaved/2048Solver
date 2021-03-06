#! /usr/bin/python
from distutils.core import setup, Extension
import numpy.distutils.misc_util

setup(
    ext_modules=[Extension("_scoring_board", 
                           ["_scoring_board.c", "scoring_board.c"], 
                           extra_compile_args=['-O3'])],
    include_dirs=numpy.distutils.misc_util.get_numpy_include_dirs(),
)

# extra_compile_args=['-fopenmp'],
# extra_link_args=['-lgomp']
