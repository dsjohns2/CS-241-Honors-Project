import sys
from pycparser import parse_file, c_parser, c_generator

filename = sys.argv[1]
ast = parse_file(filename, use_cpp=True)
ast.getvars()
