import sys
from pycparser import parse_file, c_parser, c_generator

filename = sys.argv[1]
oldvar = sys.argv[2]
newvar = sys.argv[3]
ast = parse_file(filename, use_cpp=True)
ast.changevar(oldvar, newvar)
generator = c_generator.CGenerator()
print(generator.visit(ast))
