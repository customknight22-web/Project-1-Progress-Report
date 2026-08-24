Step 1: Open main.cpp in Code::Blocks.
Step 2: Make sure x.txt and y.txt are in the same working folder as the program.
Step 3: In Code::Blocks, go to Project → Set programs' arguments.
Step 4: Enter:
x.txt y.txt output.txt
Step 5: Build and run the program.
Step 6: Check if the program successfully reads both files. It should show:
x.txt: start index = -4, duration = 7
y.txt: start index = -4, duration = 8
Step 7: Check the displayed raw signal values.

For x:
2, -1, 3, 7, 1, 2, -3

For y:
1, -1, 2, -2, 4, 1, -2, 4

Step 8: Check the calculated averages. They should be approximately:
x average = 1.57143
y average = 0.875
Step 9: Check the zero-average x values:
0.428571, -2.57143, 1.42857, 5.42857, -0.571429, 0.428571, -4.57143
Step 10: Check the zero-average y values:
0.125, -1.875, 1.125, -2.875, 3.125, 0.125, -2.875, 3.125


