This program encodes the user input into a file. It can encode itself as spaces after the line or into the variables as binary data where 0 corresponds to a camelCasedVariable and 1 corresponds to an underscored_variable. Preprocessor directives must be manually moved because pycparser does not support these.

Run "make" to compile. Then run "./encoder". Write "Variable" or "Space" when prompted. The message that you write when prompted will be incoded into "test.c" and output into "output.c".
