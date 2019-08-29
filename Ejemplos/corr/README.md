# Correlation Coefficient Program
This project consists in determining the correlation coefficient of a mxn matrix. First, we need to explain this term, so we can have a clear understanding of the problem to solve. The correlation coefficient measures the strength of the relationship between the relative movements of two variables. This measurement, if it is applied correctly, will be in a range between -1.0 and 1.0. Also, a measurement greater than 1.0 or less than -1.0 means that there was an error in the correlation. Person correlation is the most commonly used in statistics. Thus, for any user that has a CSV file with data that can be compared, the program will be able to determine the correlation coefficient of the data the user provided. 
### Main objective
The main objective of the program is to process the data  that the user provides, analyze the data, apply the corellation coefficient, reduce the CSV file and provide the CSV file with the data that has a correlation with eachother.
### Inputs
The input of the program will be a CSV file that will be transformed into a matrix. In our project the rows of the matrix will be different types of genes and the columns of the matrix will be different types of cancer. Also, the user will input a range that he desires to make calculations. 
### Output
The output of the program will be the same CSV file that the user inserted, but it will be reduced, depending on the corrrelation found. Also, the user will have the opportunity to select a specific type of cancer and look into the other types of cancer that have a correlation with it. Another function of the output will be an option to transpose the matrix, if desired.
### User manual
The way that the user will interact with the program is with the command line. Here is how it will look and its uses:
* This command will be for doing the transpose of the matrix: -t
* This command will be for the correlation coefficient, and it will have two parameters, they will be numbers, that specify the range: -cc
* This command will be for the anticorrelation coefficient, and it will have two parameters, they will be numbers, that specify the range: -ac
* This command will be to calculate the correlation from -1.0 to 1.0, including both edges: -ir
* This command will be to calculate the correlation from -1.0 to 1.0, excluding the edges: -er
* This command will be for the output of the file: -o
* This commando will be for the correlation matrix: -m
* This command will display the user manual appear whenever the user needs it: --help
### Building requisites
* A POSIX Unix operating system.
* A GCC-compatible compiler.
### Authors
* Jean Carlo Delgado
  * jeankalo21@gmail.com
* Filip Sobejko Bogusz
  * filip.sobejko@gmail.com

Heat propagation simulation is a proyect from an Computer Science workshop.

MIT License
Copyright 2019 (c)

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
