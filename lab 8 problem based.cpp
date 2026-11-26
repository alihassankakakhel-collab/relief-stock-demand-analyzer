// Lab08.cpp - Relief Stock Demand Analyzer
// CS-102 Programming Fundamentals, UET Peshawar
#include <iostream>
#include <limits>
using namespace std;

const int SIZE = 10;

// ---------- Function prototypes ----------
int readInteger(const char prompt[]);
int readCode(int itemNumber, const int codes[], int filled);
int readDemand(int itemNumber);
void displayItems(const int codes[], const int demand[], int size);
int calculateTotal(const int demand[], int size);
double calculateAverage(int total, int size);
int findLargestIndex(const int demand[], int size);
int findSmallestIndex(const int demand[], int size);
int countAboveAverage(const int demand[], int size, double average);
int findCode(const int codes[], int size, int target);
int collectUrgent(const int demand[], int size, int threshold, int urgentIndexes[]);
void displayUrgent(const int codes[], const int demand[], const int urgentIndexes[], int used);
void displayReverse(const int codes[], const int demand[], int size);

int main()
{
    int codes[SIZE];
    int demand[SIZE];
    int urgentIndexes[SIZE];

    cout << "===== Relief Stock Demand Analyzer =====" << endl;

    // Input with validation
    for (int i = 0; i < SIZE; i++)
    {
        codes[i] = readCode(i + 1, codes, i);
        demand[i] = readDemand(i + 1);
    }

    int threshold = -1;
    while (threshold < 0)
    {
        threshold = readInteger("Enter urgent threshold (0 or more): ");
        if (threshold < 0)
            cout << "Threshold cannot be negative." << endl;
    }

    cout << endl << "--- All items ---" << endl;
    displayItems(codes, demand, SIZE);

    int total = calculateTotal(demand, SIZE);
    double average = calculateAverage(total, SIZE);
    cout << endl << "Total demand   : " << total << endl;
    cout << "Average demand : " << average << endl;

    int high = findLargestIndex(demand, SIZE);
    int low = findSmallestIndex(demand, SIZE);
    if (high != -1 && low != -1)
    {
        cout << "Highest demand : " << demand[high] << " (code " << codes[high] << ")" << endl;
        cout << "Lowest demand  : " << demand[low] << " (code " << codes[low] << ")" << endl;
    }

    cout << "Items above average: " << countAboveAverage(demand, SIZE, average) << endl;

    int target = readInteger("\nEnter an item code to search: ");
    int position = findCode(codes, SIZE, target);
    if (position == -1)
        cout << "Item code " << target << " was not found." << endl;
    else
        cout << "Item code " << target << " found at index " << position
             << " with demand " << demand[position] << "." << endl;

    int used = collectUrgent(demand, SIZE, threshold, urgentIndexes);
    cout << endl << "--- Urgent items (demand >= " << threshold << ") ---" << endl;
    displayUrgent(codes, demand, urgentIndexes, used);

    cout << endl << "--- Reverse review list ---" << endl;
    displayReverse(codes, demand, SIZE);

    return 0;
}

// ---------- Function definitions ----------

// Reads one integer and recovers if the user types something that is not a number.
int readInteger(const char prompt[])
{
    int value;
    cout << prompt;
    while (!(cin >> value))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a whole number: ";
    }
    return value;
}

// Reads a positive item code that is not already used in codes[0..filled-1].
int readCode(int itemNumber, const int codes[], int filled)
{
    while (true)
    {
        cout << "Item " << itemNumber << " - ";
        int code = readInteger("enter item code (positive): ");
        if (code <= 0)
        {
            cout << "Item code must be positive." << endl;
            continue;
        }
        if (findCode(codes, filled, code) != -1)
        {
            cout << "That item code is already used." << endl;
            continue;
        }
        return code;
    }
}

int readDemand(int itemNumber)
{
    int value = -1;
    while (value < 0)
    {
        cout << "Item " << itemNumber << " - ";
        value = readInteger("enter demand (0 or more): ");
        if (value < 0)
            cout << "Demand cannot be negative." << endl;
    }
    return value;
}

void displayItems(const int codes[], const int demand[], int size)
{
    for (int i = 0; i < size; i++)
        cout << "Index " << i << "  Code " << codes[i] << "  Demand " << demand[i] << endl;
}

int calculateTotal(const int demand[], int size)
{
    int total = 0;
    for (int i = 0; i < size; i++)
        total += demand[i];
    return total;
}

// Divides only when size is positive.
double calculateAverage(int total, int size)
{
    if (size <= 0)
        return 0.0;
    return static_cast<double>(total) / size;
}

// Returns -1 for an empty array. The strict > keeps the first of repeated highest values.
int findLargestIndex(const int demand[], int size)
{
    if (size <= 0)
        return -1;
    int largestIndex = 0;
    for (int i = 1; i < size; i++)
        if (demand[i] > demand[largestIndex])
            largestIndex = i;
    return largestIndex;
}

int findSmallestIndex(const int demand[], int size)
{
    if (size <= 0)
        return -1;
    int smallestIndex = 0;
    for (int i = 1; i < size; i++)
        if (demand[i] < demand[smallestIndex])
            smallestIndex = i;
    return smallestIndex;
}

int countAboveAverage(const int demand[], int size, double average)
{
    int count = 0;
    for (int i = 0; i < size; i++)
        if (demand[i] > average)
            count++;
    return count;
}

int findCode(const int codes[], int size, int target)
{
    for (int i = 0; i < size; i++)
        if (codes[i] == target)
            return i;
    return -1;
}

// Fills urgentIndexes from index 0 and returns how many entries are meaningful.
int collectUrgent(const int demand[], int size, int threshold, int urgentIndexes[])
{
    int used = 0;
    for (int i = 0; i < size; i++)
    {
        if (demand[i] >= threshold)
        {
            urgentIndexes[used] = i;
            used++;
        }
    }
    return used;
}

void displayUrgent(const int codes[], const int demand[], const int urgentIndexes[], int used)
{
    if (used == 0)
    {
        cout << "No urgent items." << endl;
        return;
    }
    cout << "Urgent indexes:";
    for (int k = 0; k < used; k++)
        cout << " " << urgentIndexes[k];
    cout << endl;
    for (int k = 0; k < used; k++)
    {
        int idx = urgentIndexes[k];
        cout << "Code " << codes[idx] << "  Demand " << demand[idx] << endl;
    }
}

void displayReverse(const int codes[], const int demand[], int size)
{
    for (int i = size - 1; i >= 0; i--)
        cout << "Index " << i << "  Code " << codes[i] << "  Demand " << demand[i] << endl;
}
