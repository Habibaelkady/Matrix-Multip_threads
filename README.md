Matrix Multiplication using threads

Using POSIX thread library to implement matrix multiplication utilizing threads <br />
First file matmult.c (used for the computation of matrix multiplication) <br />
Three versions are implemented inside matmult.c file <br />
matmult : normal matrix multiplication without using threads<br />
matmult_v1 : where the computation of each element of the matrix multiplication happens in thread<br />
matmult_v2 : where computation of each row of the output matrix happens in thread<br />

The second file project.c that generates two random matrices each time and calls the multiplication functions<br />
To run the code all you need to do is running the makefile using the following commands in your shell<br />
% make
% ./lab6
