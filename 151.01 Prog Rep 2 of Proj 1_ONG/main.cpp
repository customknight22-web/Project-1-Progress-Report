#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;


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


bool readSignal(string filename, double*& data, int& start, int& duration)
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


    // First token is an integer
    if (isValidInteger(firstToken, possibleStart))
    {
        string secondToken;

        if (firstLine >> secondToken)
        {
            double secondValue;

            if (isValidDouble(secondToken, secondValue))
            {
                start = possibleStart;
                values.push_back(secondValue);
            }
            else
            {
                start = 0;
                values.push_back(static_cast<double>(possibleStart));
            }
        }
        else
        {
            start = 0;
            values.push_back(static_cast<double>(possibleStart));
        }
    }

    // First token is a floating point value
    else if (isValidDouble(firstToken, possibleValue))
    {
        start = 0;
        values.push_back(possibleValue);
    }

    else
    {
        return false;
    }


    // Read remaining lines
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


int computeNormalizedCrosscorrelation(
    double xRaw[], int xStartIndex, int xDuration,
    double yRaw[], int yStartIndex, int yDuration,
    double** correlation, int* correlationStartIndex)
{
    // Calculate averages
    double xSum = 0;
    double ySum = 0;

    for (int i = 0; i < xDuration; i++)
        xSum += xRaw[i];

    for (int i = 0; i < yDuration; i++)
        ySum += yRaw[i];

    double xAverage = xSum / xDuration;
    double yAverage = ySum / yDuration;


    // Create zero-average signals
    double* x = new double[xDuration];
    double* y = new double[yDuration];

    for (int i = 0; i < xDuration; i++)
        x[i] = xRaw[i] - xAverage;

    for (int i = 0; i < yDuration; i++)
        y[i] = yRaw[i] - yAverage;


    // Calculate rxx(0) and ryy(0)
    double rxx = 0;
    double ryy = 0;

    for (int i = 0; i < xDuration; i++)
        rxx += x[i] * x[i];

    for (int i = 0; i < yDuration; i++)
        ryy += y[i] * y[i];

    double normalization =
        sqrt(rxx * ryy);


    // Cannot normalize if one signal has zero energy
    if (normalization == 0)
    {
        delete[] x;
        delete[] y;

        *correlation = nullptr;

        return 0;
    }


    // Determine correlation size and starting index
    int correlationDuration =
        xDuration + yDuration - 1;

    *correlationStartIndex =
        xStartIndex - (yStartIndex + yDuration - 1);

    *correlation =
        new double[correlationDuration];


    // Calculate normalized crosscorrelation
    for (int k = 0; k < correlationDuration; k++)
    {
        int lag =
            *correlationStartIndex + k;

        double rxy = 0;

        for (int i = 0; i < xDuration; i++)
        {
            int n =
                xStartIndex + i;

            int yIndex =
                (n - lag) - yStartIndex;

            if (yIndex >= 0 &&
                yIndex < yDuration)
            {
                rxy += x[i] * y[yIndex];
            }
        }

        (*correlation)[k] =
            rxy / normalization;
    }


    delete[] x;
    delete[] y;

    return correlationDuration;
}


bool writeSignal(
    string filename,
    double data[],
    int start,
    int duration)
{
    ofstream file(filename);

    if (!file)
        return false;

    file << setprecision(10);

    file << start
         << " "
         << data[0]
         << endl;

    for (int i = 1; i < duration; i++)
    {
        file << data[i] << endl;
    }

    return true;
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


    double* xData = nullptr;
    double* yData = nullptr;

    int xStart;
    int yStart;

    int xDuration;
    int yDuration;


    if (!readSignal(
            xFile,
            xData,
            xStart,
            xDuration))
    {
        cout << "Unable to import a valid signal from "
             << xFile
             << endl;

        return 1;
    }


    cout << "Signal with start index "
         << xStart
         << ", duration "
         << xDuration
         << ", imported from "
         << xFile
         << endl;


    if (!readSignal(
            yFile,
            yData,
            yStart,
            yDuration))
    {
        cout << "Unable to import a valid signal from "
             << yFile
             << endl;

        delete[] xData;

        return 1;
    }


    cout << "Signal with start index "
         << yStart
         << ", duration "
         << yDuration
         << ", imported from "
         << yFile
         << endl;


    double* correlation = nullptr;

    int correlationStart;

    int correlationDuration =
        computeNormalizedCrosscorrelation(
            xData,
            xStart,
            xDuration,
            yData,
            yStart,
            yDuration,
            &correlation,
            &correlationStart);


    // Check if normalized crosscorrelation could be calculated
    if (correlationDuration == 0)
    {
        cout << endl;
        cout << "Unable to compute normalized crosscorrelation."
             << endl;

        cout << "One or both signals have zero energy after average removal."
             << endl;

        delete[] xData;
        delete[] yData;

        return 1;
    }


    // Show signal on console if duration is less than 20
    if (correlationDuration < 20)
    {
        cout << endl;
        cout << "Normalized Crosscorrelation:" << endl;

        cout << setprecision(10);

        for (int i = 0;
             i < correlationDuration;
             i++)
        {
            cout << correlationStart + i
                 << "   "
                 << correlation[i]
                 << endl;
        }
    }


    if (!writeSignal(
            outputFile,
            correlation,
            correlationStart,
            correlationDuration))
    {
        cout << "Unable to export signal with start index "
             << correlationStart
             << ", duration "
             << correlationDuration
             << ", to "
             << outputFile
             << endl;

        delete[] correlation;
        delete[] xData;
        delete[] yData;

        return 1;
    }


    cout << endl;

    cout << "Crosscorrelation signal with start index "
         << correlationStart
         << ", duration "
         << correlationDuration
         << ", exported to "
         << outputFile
         << endl;


    delete[] correlation;
    delete[] xData;
    delete[] yData;

    return 0;
}
