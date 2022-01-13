# complex_simplifier
Simplifying complex number arithmetic expressions symbolically using template expressions

I had a certain expression involving many constant std::complex<double> variables which were zero in either the real or imaginary slot.
This code allows systematic removal of zero entries from complex arithmetic expressions involving multiplications and additions.
It works using expression templates to build a tree representing the given complex expression. An example of this can be seen in main(),
where I have many variables called "aa0", "aa1", etc. The constructor for ComplexVariable takes three arguments: the variable name, wether
real part is zero, and whether the imaginary part is zero.

You can then build expressions involving these variables using standard C++ syntax with parenthesis, plus, and times. To access
either the real or imaginary part of the expression with zero values simplified out in string form, you can use the methods
.real() or .imag() and print to cout. The intention here is to paste the resulting code into another C++ program where the
real and imaginary parts of the variables respectively include postfixes _re or _im.

I have found that the expressions thus obtained from removal of zero coefficients of complex numbers tend to beat gcc's optimizations
even when using -O2 and -O3. However, it seems that -ffast-math is able to obtain this type of optimization, so this code is
pretty much pointless if you plan on using that compiler flag.
