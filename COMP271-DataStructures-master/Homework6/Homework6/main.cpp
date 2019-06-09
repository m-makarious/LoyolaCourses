// Mary B. Makarious 04.26.2016
// Homework 6: Hashing and Collisions
// Help from: YouTube, Google

// Required includes
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

// Using the standard namespace
using namespace std;

// Extraction used to take out certain digits when the file is read
int extractionFunction(int numberOfElement) //Extract the certain degits when read the file
{
    string initialSSN = "000000000"; // Social Security Numbers have 9 digits
    char *valueC= new char[100];
    sprintf(valueC, "%i", numberOfElement); // Take the data and convert it into a string
    string value = (string) valueC; // string casting
    
    int j=0; // counter
    
    // Converting...
    for(int i=0; i<value.size(); i++)
    {
        initialSSN[i] = value[j];
        j++;
    }
    
    // Extract the 3, 5, 7, 8th digits from the SSN
    // Final hashcode will be from the 3, 5, 7, and 8th digits
    string finalHashCode = "0000"; // Final hashed address will be only 4 digits
    finalHashCode[0] = initialSSN[2];
    finalHashCode[1] = initialSSN[4];
    finalHashCode[2] = initialSSN[6];
    finalHashCode[3] = initialSSN[7];
    
    // Convert the strong back into an integer
    return atoi(finalHashCode.c_str());
    
}

int quadraticProbing(int hash_Table[], int indexNumber, int userInput) // User input would be the max value since it makes the size of the array
{
    int position;
    
    // Position is determined by making sure it doesn't exceed the max/userInput value
    position = indexNumber % userInput;
    
    // Checking for collisions... if there is a collision, use quadraticProbing
    for(int i=1; i<100000; i++)
    {
        if(hash_Table[position+(userInput+i)*(userInput+i)]==0) // ==0 or ==NULL?
        {
            return indexNumber;
        }
    }
    
    return -1;  // Index position already full
}

// Begin main function
int main()
{
    // Initializing variables and the hashtable to use
    string dataFromFile;
    string numbers;
    ifstream data_in;   // Declares the in stream
    int userNumber = 0;
    int * nHash_Table;  // Creates a hashtable
    nHash_Table = new int[100000]; // Size of 100,000
    
    // Going through the hashtable
    for (int i = 0; i < 100000; i++)
    {
        nHash_Table[i] = 0;
    }
    
    // Opens the file where all the socials are found
    data_in.open("everybodys_socials.txt");
    
    // ERROR: If the file isn't open, then the file was not found
    if(!data_in.is_open())
    {
        cout << "File not found. \n";
        
    }
    
    vector<int> allNumbers; // Store all numbers in a vector because vectors are dynamic
    
    while(data_in.peek()!=EOF) // Run until the end of the file
    {
        {
            getline(data_in, dataFromFile); // getting the information
            istringstream stringNumbers(dataFromFile); //istringsteam: input string stream
            
            while(getline(stringNumbers, numbers,',')) // comma-delimited file
            {
                // Converting strings back to integers in the vector
                allNumbers.push_back(atoi(numbers.c_str()));
            }
        }
        
        // Prompt user for a number
        cout << "Enter a number between 1 and 450,000,000: ";
        cin >> userNumber;
        
        userNumber= userNumber%100000; // Makes sure it doesnt go over 450,000,000
        
        // ERROR: Check that the user input is not the range
        if (userNumber < 1 || userNumber > 450000000)
        {
            cout << "The number must be between 1 and 450,000,000. \n";
            cout << "Please enter another number: ";
            cin >> userNumber;
        }
        
        // User input was in the range
        else
        {
            cout << "The number you entered was: " << userNumber << endl;
        }
        
        int indexNum;
        for (int i = 0; i < allNumbers.size(); i++)
        {
            // Hash all the numbers
            indexNum = extractionFunction(allNumbers[i]); // Stores the extracted address of numbers
            
            // If the address is empty, then store the value
            if (nHash_Table[indexNum] == 0) // or NULL?
            {
                nHash_Table[indexNum] = allNumbers[i];
            }
            
            else
            {
                // if not avaiable then use quadraticProbing for next space
                indexNum = quadraticProbing(nHash_Table, indexNum, userNumber);
                if (indexNum == -1) // -1 indicates index position is already full
                {
                    cout << "Hash Table is already filled up." << endl;
                }
                
                else
                {
                    // After going through quadraticProbing, the element is put in the next avaliable index value
                    nHash_Table[indexNum] = allNumbers[i];
                }
            }
        }
        
        // Writes the results out to "hashed_socials.txt"
        ofstream data_out; // Opens the out stream
        data_out.open("hashed_socials.txt");
        
        // Writing hashtable files
        // Ask: Is there a way to find the size of the hashtable instead of declaring i<1000000 each time? (ex. hashtable.size()? )
        for(int i=0; i<100000; i++)
        {
            data_out << nHash_Table[i];
            if(i<99999)
                data_out << ',';
            
        }
        
        // Cleaning up!
        data_in.clear();
        data_in.close();
        data_out.clear();
        data_out.close();
    }
    return 0;
}