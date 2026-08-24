Step 1: Open main.cpp in Code::Blocks. <br>
Step 2: Make sure x.txt and y.txt are in the same working folder as the program. <br>
Step 3: In Code::Blocks, go to Project → Set programs' arguments. <br>
Step 4: Enter: <br>
x.txt y.txt output.txt <br><br>
Step 5: Build and run the program. <br>
Step 6: Check if the program successfully reads both files. It should show: <br>
x.txt: start index = -4, duration = 7 <br>
y.txt: start index = -4, duration = 8 <br>
Step 7: Check the displayed raw signal values. <br>
<br>
For x:<br>
2, -1, 3, 7, 1, 2, -3<br>
<br>
For y:<br>
1, -1, 2, -2, 4, 1, -2, 4<br>
<br>
Step 8: Check the calculated averages. They should be approximately:<br>
x average = 1.57143<br>
y average = 0.875<br><br>
Step 9: Check the zero-average x values:<br>
0.428571, -2.57143, 1.42857, 5.42857, -0.571429, 0.428571, -4.57143<br><br>
Step 10: Check the zero-average y values:<br>
0.125, -1.875, 1.125, -2.875, 3.125, 0.125, -2.875, 3.125<br>
<br>
<br>
PROGRESS DESCRIPTION <br>
The program can currently read the two input signal files using command-line arguments and identify the starting index and duration of each signal. The imported raw signal values are stored in dynamically allocated arrays and displayed for verification.<br>
The program can also calculate the average of each signal and remove these averages to produce the required zero-average signals. The provided Basic Test data were used to check the implementation, and the resulting averages and zero-average signal values matched the expected calculations.<br>
The normalized crosscorrelation computation, output file generation, and remaining error handling have not yet been implemented and will be completed in the succeeding stages of the project.
