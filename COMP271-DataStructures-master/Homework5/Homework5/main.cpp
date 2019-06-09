// Mary B. Makarious 04.19.2016
// Homework 5: Sorting COMP 271
// Help from: Google. C++ Textbook. Fahed al Rafati. YouTube.

// Required includes
#include <iostream> // for streams
#include <fstream> // for files
#include <vector>
#include <cstring>
#include <cstdlib>
#include <string>
#include <algorithm>

// Using namespace

using namespace std;

// Functions to use
void bubble_Sort(int arr[], int n);

void insertion_Sort(int arr[], int length);

void quick_Sort(int arr[], int left, int right);

void shell_Sort(int A[], int n);

void merge(int A[], int low, int high, int mid);
void merge_Sort(int A[], int low, int high);

bool array_read(string line, int *& pos);

int num_of_comp = 0;
int num_of_exch = 0;




// Bubble Sort: Algorithm
/* Info: O(n^2). Inefficient for sorting large data volumes.
 * Compare each pair of adjacent elements from the beginning of an array and, if they are in reversed order, swap them.
 * If at least one swap has been done, repeat step 1.
 */

void bubble_Sort(int A[], int n)
{
    int i, j, temp;
    for(i = 1; i < n; i++)
    {
        for(j = 0; j < n - 1; j++)      // j varies from 0 to n - 1
        {
            num_of_comp++;         // Comparison within the for loop 
            if(A[j] > A[j + 1])     //compare two successive numbers
            {
                temp = A[j];        // Swap A[j] with A[j + 1]
                A[j] = A[j + 1];
                A[j + 1] = temp;
                num_of_exch++;        // Increment exchanges
            }
        }
    }
    //cout << "Algorithm: Insertion Sort. Exchanges: " << num_of_exch << " Comparisons: " << num_of_comp << endl;
    //cout << endl;
}

// Insertion Sort: Algorithm
/* Info: O(n^2). Used for small arrays
 * Array is imaginary divided into two parts - sorted one and unsorted one.
 * At the beginning, sorted part contains first element of the array and unsorted one contains the rest.
 * At every step, algorithm takes first element in the unsorted part and
 * inserts it to the right place of the sorted one.
 * When unsorted part becomes empty, algorithm stops.
 */

void insertion_Sort(int arr[], int length)
{
    int i, j, temp, num_of_comp, num_of_exch;
    for (i = 1; i < length; i++)
    {
        j = i;
        while (j > 0 && arr[j - 1] > arr[j])  // Compare A[j-1] to A[i]
        {
            temp = arr[j];
            arr[j] = arr[j - 1]; // Shifts the element
            arr[j - 1] = temp;
            j--;
            num_of_exch++;    // Increment exchanges counter
        }
        num_of_comp++; // Increment comparison counter
    }
    //cout << "Algorithm: Insertion Sort. Exchanges: " << num_of_exch << " Comparisons: " << num_of_comp << endl;
   // cout << endl;
}

// Quick Sort: Algorithm
/* Info: O(n log n). Suitable for big data.
 * Choose a pivot value. We take the value of the middle element as pivot value, but it can be any value,
 * which is in range of sorted values, even if it doesn't present in the array.
 * Partition. Rearrange elements in such a way, that all elements which are lesser than the pivot go to the
 * left part of the array and all elements greater than the pivot, go to the right part of the array.
 * Values equal to the pivot can stay in any part of the array. Notice, that array may be divided in non-equal parts.
 * Sort both parts. Apply quicksort algorithm recursively to the left and the right parts.
 */
void quick_Sort(int arr[], int left, int right)
{
    {
        int i = left;
        int j = right;
        int temp, num_of_comp, num_of_exch;
        
        // Chosing a pivot value
        int pivot = arr[(left + right) / 2];
        
        // Partitioning
        while (i <= j)
        {
            num_of_comp++;
            while (arr[i] < pivot)
                i++;
            while (arr[j] > pivot)
                j--;
            if (i <= j)
            {
                temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
                i++;
                j--;
                num_of_exch++;
            }
        };
        
        // Recursion Steps
        if (left < j)
            quick_Sort(arr, left, j);
        if (i < right)
            quick_Sort(arr, i, right);
       // cout << "Algorithm: Quick Sort. Exchanges: " << num_of_exch << " Comparisons: " << num_of_comp << endl;
        //cout << endl;
    }
    
}

// Shell Sort: Algorithm
/* Info: O(n logV2 n). Suitable for big data.
 * The running time of Shellsort is heavily dependent on the gap sequence it uses. For many practical variants,
 * determining their time complexity remains an open problem.
 * The idea is to arrange the list of elements so that, starting anywhere,
 * considering every hth element gives a sorted list.
 */
void shell_Sort(int A[], int n)
{
    int temp, gap, i, num_of_exch, num_of_comp;
    int swapped;
    gap = n/2;
    do
    {
        do
        {
            swapped = 0;
            for(i = 0; i < n - gap; i++)
            {
                num_of_comp++;      // Comparing with for loop.
                if(A[i] > A[i + gap])
                {
                    temp = A[i];
                    A[i] = A[i + gap];
                    A[i + gap] = temp;
                    swapped = 1;
                    num_of_exch++;    // Increment exchange counter.
                }
            }
        }
        while(swapped == 1);    //set swap equal to 1
    }
    while((gap = gap/2) >= 1);
   // cout << "Algorithm: Shell Sort. Exchanges: " << num_of_exch << " Comparisons: " << num_of_comp << endl;
   // cout << endl;
}

// Merge Sort: Algorithm
/* Info: O(n+m).
 * Introduce read-indices i, j to traverse arrays A and B, accordingly.
 * Introduce write-index k to store position of the first free cell in the resulting array.
 * By default i = j = k = 0.
 * At each step: if both indices are in range (i < m and j < n),
 * choose minimum of (A[i], B[j]) and write it to C[k].
 * Increase k and index of the array, algorithm located minimal value at, by one.
 * Repeat step 2.
 * Copy the rest values from the array, which index is still in range, to the resulting array.
 */

void merge(int A[], int low, int high, int mid)
{
    int i, j, k, C[10000], num_of_comp,num_of_exch;
    i = low;          // First part index
    j = mid + 1;      // Second part index
    k = 0;            // Merge the first to arrays into array C
    while ((i <= mid) && (j <= high))
    {
        num_of_comp++;      // Increment comparison counter.
        if (A[i] < A[j])
        {
            C[k] = A[i++];
            num_of_exch++;        // Increment exchange counter.
        }
        else
        {
            C[k] = A[j++];
            num_of_exch++;        // Increment exchange counter.
        }
        k++;
    }
    while (i <= mid)
    {
        C[k++] = A[i++];
        num_of_exch++;        // Increment exchange counter.
    }
    while (j <= high)
    {
        C[k++] = A[j++];
        num_of_exch++;        // Increment exchange counter.
    }
    for (i = low, j = 0; i < high; i++, j++)
        // Copy the contents in Array C back into Array A
    {
        A[i] = C[j];
    }
}

void merge_Sort(int A[], int low, int high)  // merge_Sort
{
    int mid;
    if (low < high)
    {
        mid=(low + high)/2;
        merge_Sort(A, low, mid);
        merge_Sort(A, mid + 1, high);
        merge(A, low, high, mid);       // call the merge function
        // cout << "Algorithm: Merge Sort. Exchanges: " << num_of_exch << " Comparisons: " << num_of_comp << endl;
        // cout << endl;
    }
    
    return;
}

bool array_read(string line, int *& pos)   //reading in the text files into arrays
{
    ifstream thisfile;      //input file stream
    thisfile.open(line.c_str());     //c-style string, open the input file
    
    if(!thisfile.is_open())     //if the file fails to open
    {
        cout << "Can't find the file yo!" << line << endl;
        cout << "Wrong! check the location \n";
        cout << endl; //space
        return false;
    }
    string y;       //create a random string
    string x = " ";     //create a random string
    while(thisfile.peek()!= EOF)
        //peek reads and returns the next character without extracting
        //while this is not equal to end of the file
    {
        thisfile >> y;      //read in string y
        x += y + " ";
    }
    thisfile.clear();
    thisfile.close();      // Clean and close
    
    vector<string> stringVector;
    y = "";
    for( int i = 0; i < x.length();i++)
    {
        if (x[i] == ' ')
        {
            stringVector.push_back(y);
            y = "";
        }
        else
        {
            y += x[i];
        }
    }
    if(pos != NULL)        //making sure the memory is free
    {
        delete pos;
    }
    pos = new int[stringVector.size()];
    for(int i = 0; i < stringVector.size(); i++)
    {
        pos[i]= atoi(stringVector[i].c_str());
    }
    return true;
}

int main(int argc, const char * argv[])
{
    int * allNumbers;
    string namesofFiles[4];
    namesofFiles[0] = "FewUnique.txt";
    cout << "FewUnique.txt is the first set of data" << endl;   // print
    namesofFiles[1] = "NearlySorted.txt";
    cout << "NearlySorted.txt is the second set of data" << endl;     // print
    namesofFiles[2] = "Random.txt";
    cout << "Random.txt is the thirs set of data" << endl;       // print
    namesofFiles[3] = "Reversed.txt";
    cout << "Reversed.txt is the fourth set of data" << endl;     // print
    cout << endl;
    cout << "****************" << endl;
    
    for(int i = 0; i < 4; i++)
    {
        
        // Go through Bubble Sort
        array_read(namesofFiles[i], allNumbers);
        bubble_Sort(allNumbers, 10000);
        cout << "Bubble Sort Comparisons: " << num_of_comp << endl;
        cout << "Bubble Sort Exchanges: " << num_of_exch << endl;
        cout << endl;
        
        // Go through Insertion Sort
        array_read(namesofFiles[i], allNumbers);
        insertion_Sort(allNumbers, 10000);
        cout << "Insertion Sort Comparisons: " << num_of_comp << endl;
        cout << "Insertion Sort Exchanges: " << num_of_exch << endl;
        cout << endl;
        
        // Go through Quick Sort
        array_read(namesofFiles[i], allNumbers);
        quick_Sort(allNumbers, 0, 9999);
        cout << "Quick Sort Comparisons: " << num_of_comp << endl;
        cout << "Quick Sort Exchanges: " << num_of_exch << endl;
        cout << endl;
        
        // Go through Shell Sort
        array_read(namesofFiles[i], allNumbers);
        shell_Sort(allNumbers, 10000);
        cout << "Shell Sort Comparisons: " << num_of_comp << endl;
        cout << "Shell Sort Exchanges: " << num_of_exch << endl;
        cout << endl;
        
        // Go through Merge Sort
        array_read(namesofFiles[i], allNumbers);
        merge_Sort(allNumbers, 0, 9999);
        cout << "Merge Sort Comparisons: " << num_of_comp << endl;
        cout << "Merge Sort Exchanges: " << num_of_exch << endl;
        cout << endl;
        
        
        cout << endl;
        cout << "****************" << endl;
        cout << endl;
        
        
        // Debugging
        //Test on each algorithm with smaller array
        /*int nums[] = {2,77,3,6,1,83,4,31,71,10};
         bubble_Sort(nums, 10);*/
        
        /*int nums[] = {2,77,3,6,1,83,4,31,71,10};
         insertion_Sort(nums, 10);*/
        
        /*int nums[] = {2,77,3,6,1,83,4,31,71,10};
         quick_Sort(nums, 1, 10);*/
        
        /*int nums[] = {2,77,3,6,1,83,4,31,71,10};
         shell_Sort(nums, 10);*/
        
        /*int nums[] = {2,77,3,6,1,83,4,31,71,10};
         merge_Sort(nums, 1, 10);*/
    }
    cout << "Done." << endl;
    return 0;
}