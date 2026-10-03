//Filename: main.cpp
//Assignment: Project 2: Fourier Transform
//Name: Osh Ong
//Grouped with Raffy Colobong, Nathan Ocampo
//Section: ENGG 151.01 - C

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;

const double PI = 3.141592653589793;

// Checks if a whole string is a valid integer
bool isValidInteger(string text, int& value)
{
    stringstream ss(text);
    ss >> value;

    if (ss.fail())
        return false;

    char extra;

    if (ss >> extra)
        return false;

    return true;
}

// Checks if a whole string is a valid floating point number
bool isValidDouble(string text, double& value)
{
    stringstream ss(text);
    ss >> value;

    if (ss.fail())
        return false;

    char extra;

    if (ss >> extra)
        return false;

    return true;
}

// Reads signal from file
bool readSignal(string filename, double*& data, int& duration)
{
    ifstream file(filename);

    if (!file)
        return false;

    vector<double> values;
    string line;

    // Read first line separately
    if (!getline(file, line))
        return false;

    stringstream firstLine(line);
    string firstToken;

    if (!(firstLine >> firstToken))
        return false;

    int possibleStart;
    double possibleValue;

    if (isValidInteger(firstToken, possibleStart))
    {
        string secondToken;

        if (firstLine >> secondToken)
        {
            double secondValue;

            if (isValidDouble(secondToken, secondValue))
                values.push_back(secondValue);
            else
                values.push_back(static_cast<double>(possibleStart));
        }
        else
        {
            values.push_back(static_cast<double>(possibleStart));
        }
    }
    else if (isValidDouble(firstToken, possibleValue))
    {
        values.push_back(possibleValue);
    }
    else
    {
        return false;
    }

    // Read remaining signal values
    while (getline(file, line))
    {
        stringstream ss(line);
        string token;

        if (!(ss >> token))
            break;

        double value;

        if (!isValidDouble(token, value))
            break;

        values.push_back(value);
    }

    duration = values.size();

    data = new double[duration];

    for (int i = 0; i < duration; i++)
        data[i] = values[i];

    return true;
}

// Computes the DFT
void computeDFT(
    double* xData,
    int xDuration,
    double samplingFreq,
    double startFreq,
    double endFreq,
    int nSteps,
    double** realPart,
    double** imagPart,
    double** magnitude,
    double** phase)
{
    int numberOfPoints = nSteps + 1;

    *realPart = new double[numberOfPoints];
    *imagPart = new double[numberOfPoints];
    *magnitude = new double[numberOfPoints];
    *phase = new double[numberOfPoints];

    double freqStep =
        (endFreq - startFreq) / nSteps;

    for (int k = 0; k < numberOfPoints; k++)
    {
        double frequency =
            startFreq + k * freqStep;

        double real = 0;
        double imag = 0;

        for (int n = 0; n < xDuration; n++)
        {
            double angle =
                2.0 * PI * frequency * n
                / samplingFreq;

            real +=
                xData[n] * cos(angle);

            imag -=
                xData[n] * sin(angle);
        }

        (*realPart)[k] = real;
        (*imagPart)[k] = imag;

        (*magnitude)[k] =
            sqrt(real * real + imag * imag);

        (*phase)[k] =
            atan2(imag, real) * 180.0 / PI;
    }
}

int main(int argc, char* argv[])
{
    if (argc < 6 || argc > 7)
    {
        cout << "Usage: dft signal-file sampling-rate "
             << "start-freq end-freq nSteps (logfile)"
             << endl;

        return 1;
    }

    string signalFile = argv[1];

    double samplingFreq;
    double startFreq;
    double endFreq;

    int nSteps;

    // Validate command-line arguments
    if (!isValidDouble(argv[2], samplingFreq) ||
        !isValidDouble(argv[3], startFreq) ||
        !isValidDouble(argv[4], endFreq) ||
        !isValidInteger(argv[5], nSteps))
    {
        cout << "Invalid command-line arguments."
             << endl;

        return 1;
    }

    // Prevent invalid calculation
    if (samplingFreq <= 0 ||
        nSteps <= 0 ||
        endFreq < startFreq)
    {
        cout << "Invalid command-line arguments."
             << endl;

        return 1;
    }

    // Default log file
    string logFile = "dftlog.txt";

    if (argc == 7)
        logFile = argv[6];

    cout << "Using log file "
         << logFile
         << "."
         << endl;

    // Read signal
    double* xData = nullptr;
    int xDuration;

    if (!readSignal(
            signalFile,
            xData,
            xDuration))
    {
        cout << "Unable to extract a valid signal from "
             << signalFile
             << "."
             << endl;

        return 1;
    }

    cout << "Signal of duration "
         << xDuration
         << " extracted from "
         << signalFile
         << "."
         << endl;

    // DFT output arrays
    double* realPart = nullptr;
    double* imagPart = nullptr;
    double* magnitude = nullptr;
    double* phase = nullptr;

    computeDFT(
        xData,
        xDuration,
        samplingFreq,
        startFreq,
        endFreq,
        nSteps,
        &realPart,
        &imagPart,
        &magnitude,
        &phase);

    double freqStep =
        (endFreq - startFreq) / nSteps;

    // Open log file in append mode
    ofstream log(logFile, ios::app);

    if (!log)
    {
        cout << "Unable to open "
             << logFile
             << "."
             << endl;

        delete[] xData;
        delete[] realPart;
        delete[] imagPart;
        delete[] magnitude;
        delete[] phase;

        return 1;
    }

    // First table: frequency, real part, imaginary part

    log << endl;

    log << left
        << setw(15) << "frequency"
        << setw(20) << "real part"
        << setw(20) << "imaginary part"
        << endl;


    for (int i = 0; i <= nSteps; i++)
    {
        double frequency =
            startFreq + i * freqStep;

        log << left
            << setw(15) << frequency
            << setw(20) << realPart[i]
            << setw(20) << imagPart[i]
            << endl;
    }

    // Second table: frequency, magnitude, phase

    log << endl;

    log << left
        << setw(15) << "frequency"
        << setw(20) << "magnitude"
        << setw(20) << "phase"
        << endl;

    for (int i = 0; i <= nSteps; i++)
    {
        double frequency =
            startFreq + i * freqStep;

        log << left
            << setw(15) << frequency
            << setw(20) << magnitude[i]
            << setw(20) << phase[i]
            << endl;
    }

    log.close();

    cout << "DFT results written to "
         << logFile
         << "."
         << endl;

    // Display results on console only when nSteps < 10
    if (nSteps < 10)
    {
        cout << endl;

        cout << left
             << setw(15) << "frequency"
             << setw(20) << "real part"
             << setw(20) << "imaginary part"
             << endl;

        for (int i = 0; i <= nSteps; i++)
        {
            double frequency =
                startFreq + i * freqStep;

            cout << left
                 << setw(15) << frequency
                 << setw(20) << realPart[i]
                 << setw(20) << imagPart[i]
                 << endl;
        }

        cout << endl;

        cout << left
             << setw(15) << "frequency"
             << setw(20) << "magnitude"
             << setw(20) << "phase"
             << endl;

        for (int i = 0; i <= nSteps; i++)
        {
            double frequency =
                startFreq + i * freqStep;

            cout << left
                 << setw(15) << frequency
                 << setw(20) << magnitude[i]
                 << setw(20) << phase[i]
                 << endl;
        }
    }

    // Free dynamically allocated memory
    delete[] xData;
    delete[] realPart;
    delete[] imagPart;
    delete[] magnitude;
    delete[] phase;

    return 0;
}
