//Filename: main.cpp
//Assignment: Project 1: Normalized Crosscorrelation
//Name: Osh Ong
//Grouped with Raffy Colobong, Nathan Ocampo
// Section: ENGG 151.01 - C

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

bool readSignal(string filename, double*& data, int& start, int& duration)
{
    ifstream file(filename);

    if (!file)
        return false;

    vector<double> values;
    string line;

    if (!getline(file, line))
        return false;

    stringstream first(line);

    string a, b;
    first >> a;

    if (first >> b)
    {
        start = stoi(a);
        values.push_back(stod(b));
    }
    else
    {
        start = 0;
        values.push_back(stod(a));
    }

    while (getline(file, line))
    {
        stringstream ss(line);
        double value;

        if (!(ss >> value))
            break;

        values.push_back(value);
    }

    duration = values.size();

    data = new double[duration];

    for (int i = 0; i < duration; i++)
        data[i] = values[i];

    return true;
}

double getAverage(double data[], int duration)
{
    double sum = 0;

    for (int i = 0; i < duration; i++)
        sum += data[i];

    return sum / duration;
}

void removeAverage(double data[], int duration, double average)
{
    for (int i = 0; i < duration; i++)
        data[i] = data[i] - average;
}

int main(int argc, char* argv[])
{
    if (argc != 4)
    {
        cout << "Usage: xcorr [xdata] [ydata] [output file]" << endl;
        return 1;
    }

    string xFile = argv[1];
    string yFile = argv[2];
    string outputFile = argv[3];

    double* xData;
    double* yData;

    int xStart, yStart;
    int xDuration, yDuration;

    if (!readSignal(xFile, xData, xStart, xDuration))
    {
        cout << "Unable to read " << xFile << endl;
        return 1;
    }

    if (!readSignal(yFile, yData, yStart, yDuration))
    {
        cout << "Unable to read " << yFile << endl;
        delete[] xData;
        return 1;
    }

    cout << "Signal with start index "
         << xStart << ", duration "
         << xDuration << ", imported from "
         << xFile << endl;

    cout << "Signal with start index "
         << yStart << ", duration "
         << yDuration << ", imported from "
         << yFile << endl;


    // Display raw x signal

    cout << endl;
    cout << "x_raw(n):" << endl;

    for (int i = 0; i < xDuration; i++)
    {
        cout << xStart + i << "   "
             << xData[i] << endl;
    }


    // Display raw y signal

    cout << endl;
    cout << "y_raw(n):" << endl;

    for (int i = 0; i < yDuration; i++)
    {
        cout << yStart + i << "   "
             << yData[i] << endl;
    }


    // Calculate averages

    double xAve = getAverage(xData, xDuration);
    double yAve = getAverage(yData, yDuration);

    cout << endl;
    cout << "x average: " << xAve << endl;
    cout << "y average: " << yAve << endl;


    // Remove the averages

    removeAverage(xData, xDuration, xAve);
    removeAverage(yData, yDuration, yAve);


    // Display zero-average x signal

    cout << endl;
    cout << "x(n):" << endl;

    for (int i = 0; i < xDuration; i++)
    {
        cout << xStart + i << "   "
             << xData[i] << endl;
    }


    // Display zero-average y signal

    cout << endl;
    cout << "y(n):" << endl;

    for (int i = 0; i < yDuration; i++)
    {
        cout << yStart + i << "   "
             << yData[i] << endl;
    }


    delete[] xData;
    delete[] yData;

    return 0;
}
