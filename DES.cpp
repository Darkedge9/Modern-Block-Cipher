#include <iostream>
#include <string>
#include <unordered_map>
#include <bitset>
#include <vector>
using namespace std;

string hexToBin(string &hex)

{
    string binary;
    for (char hexChar : hex)
    {
        // cout<<hexChar<<" ";
        binary += bitset<4>(stoi(string(1, hexChar), nullptr, 16)).to_string();
    }
    return binary;
}

string BinToHex(string &bin)
{

    unordered_map<string, char> binToHexMap = {
        {"0000", '0'}, {"0001", '1'}, {"0010", '2'}, {"0011", '3'}, {"0100", '4'}, {"0101", '5'}, {"0110", '6'}, {"0111", '7'}, {"1000", '8'}, {"1001", '9'}, {"1010", 'A'}, {"1011", 'B'}, {"1100", 'C'}, {"1101", 'D'}, {"1110", 'E'}, {"1111", 'F'}};

    string hex;
    for (int i = 0; i < bin.length(); i += 4)
    {
        string chotaString = bin.substr(i, 4);

        hex += binToHexMap[chotaString];
    }

    return hex;
}

string planetextPermutation(string st)
{
    // Fixed Initial Permutation table
    int IPTable[8 * 8] = {58, 50, 42, 34, 26, 18, 10, 2,
                          60, 52, 44, 36, 28, 20, 12, 4,
                          62, 54, 46, 38, 30, 22, 14, 6,
                          64, 56, 48, 40, 32, 24, 16, 8,
                          57, 49, 41, 33, 25, 17, 9, 1,
                          59, 51, 43, 35, 27, 19, 11, 3,
                          61, 53, 45, 37, 29, 21, 13, 5,
                          63, 55, 47, 39, 31, 23, 15, 7};

    // convert Hex code to Binary
    string bin = hexToBin(st);
    cout << "converted to Binary form:";
    for (int i = 0; i < 64; i += 4)
    {
        cout << bin.substr(i, 4) << " ";
    }
    cout << endl;
    // permute the resultant binary
    string pBinary(64, '0');
    for (int i = 0; i < 64; i++)
    {
        pBinary[i] = bin[IPTable[i] - 1];
    }
    cout << "Permute the Binary form:";
    for (int i = 0; i < 64; i += 4)
    {
        cout <<pBinary.substr(i, 4) << " ";
    }
    cout << endl;

    string phex = BinToHex(pBinary);
    cout << "Converted the Permuted binary to hexadecimal :" << phex;
    return pBinary;
}

string circularLeftShift(string input, int shifts)
{
    int len = input.length();
    shifts = shifts % len;
    return input.substr(shifts) + input.substr(0, shifts);
}

string circularRightShift(string input, int shifts)
{
    int len = input.length();
    shifts = shifts % len;
    return input.substr(len - shifts) + input.substr(0, len - shifts);
}

vector<string> keyGeneration(string key)
{
    vector<string> roundKeys;
    string binKey = hexToBin(key);
    cout << "Key in Binary form:";
    for (int i = 0; i < 64; i += 8)
    {
        cout << binKey.substr(i, 8) << "  ";
    }
    cout << endl;

    string result = "";
    for (int i = 0; i < 64; i++)
    {

        if ((i + 1) % 8 != 0)
        {
            result += binKey[i];
        }
    }

    cout << "Key after dropping the parity bits:";
    for (int i = 0; i < 56; i += 7)
    {
        cout << result.substr(i, 7) << "  ";
    }
    cout << endl;

    int KP1[56] = {57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18,
                   10, 2, 59, 51, 43, 35, 27, 19, 11, 3, 60, 52, 44, 36,
                   63, 55, 47, 39, 31, 23, 15, 7, 62, 54, 46, 38, 30, 22,
                   14, 6, 61, 53, 45, 37, 29, 21, 13, 5, 28, 20, 12, 4};

    int KP2[48] = {14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10, 23, 19, 12, 4, 26, 8,
                   16, 7, 27, 20, 13, 2, 41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48,
                   44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32};

    // Apply KP1 to the 56-bit key
    string pKey(56, '0');
    for (int i = 0; i < 56; i++)
    {
        pKey[i] = binKey[KP1[i] - 1];
    }

    cout << "56 bits permutation of the key in Binary form:";
    for (int i = 0; i < 56; i += 7)
    {
        cout << " " << pKey.substr(i, 7) << "  ";
    }
    cout << endl;

    string leftHalf = pKey.substr(0, 28);
    string rightHalf = pKey.substr(28, 28);

    for (int i = 0; i < 16; i++)
    {
        cout << "******************************** ROUND : " << (i + 1) << " ********************************" << endl;

        int shift = (i == 0 || i == 1 || i == 8 || i == 15) ? 1 : 2;
        leftHalf = circularLeftShift(leftHalf, shift);
        rightHalf = circularRightShift(rightHalf, shift);

        pKey = leftHalf + rightHalf;

        string op(48, '0');
        for (int i = 0; i < 48; i++)
        {
            op[i] = pKey[KP2[i] - 1];
        }
         
        roundKeys.push_back(op);
        cout << "Permuted the Binary form:" << endl;
        for (int i = 0; i < 48; i += 6)
        {
            cout << " " << op.substr(i, 6) << "  ";
        }
        cout << endl;                    

        cout << "Generated Round " << (i + 1) << " Key : " << op << endl;
        cout<<endl<<endl;
    }

    return roundKeys;
}

string desFunction(string rightHalf, string roundKeys)
{
    // Expansion Table (E) - Expands 32-bit to 48-bit
    int expansionTable[48] = {32, 1, 2, 3, 4, 5,
                              4, 5, 6, 7, 8, 9,
                              8, 9, 10, 11, 12, 13,
                              12, 13, 14, 15, 16, 17,
                              16, 17, 18, 19, 20, 21,
                              20, 21, 22, 23, 24, 25,
                              24, 25, 26, 27, 28, 29,
                              28, 29, 30, 31, 32, 1};

    int S[8][4][16] = {
        // S1
        {{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
         {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
         {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
         {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}},
        // S2
        {{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
         {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
         {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
         {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}},
        // S3
        {{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
         {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
         {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
         {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}},
        // S4
        {{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
         {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
         {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
         {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}},
        // S5
        {{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
         {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
         {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
         {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}},
        // S6
        {{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
         {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
         {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
         {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}},
        // S7
        {{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
         {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
         {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
         {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}},
        // S8
        {{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
         {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
         {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
         {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}}};

    // Straight P-Box Permutation
    int P[32] = {16, 7, 20, 21, 29, 12, 28, 17,
                 1, 15, 23, 26, 5, 18, 31, 10,
                 2, 8, 24, 14, 32, 27, 3, 9,
                 19, 13, 30, 6, 22, 11, 4, 25};

    // Step 1: Expand the 32-bit rightHalf to 48-bit using the Expansion Table (E)
    string expandedRight(48, '0');
    for (int i = 0; i < 48; i++)
    {
        expandedRight[i] = rightHalf[expansionTable[i] - 1];
    }

    // Step 2: XOR the expanded right half with the round key
    string xored(48, '0');
    for (int i = 0; i < 48; i++)
    {
        xored[i] = (expandedRight[i] == roundKeys[i]) ? '0' : '1';
    }

    // Step 3: Apply the S-box substitution (8 S-boxes of 6 bits each)
    string substituted(32, '0');
    for (int i = 0; i < 8; i++)
    {
        string sixBits = xored.substr(i * 6, 6);
        int row = stoi(sixBits.substr(0, 1) + sixBits.substr(5, 1), nullptr, 2); // first and last bit form the row
        int col = stoi(sixBits.substr(1, 4), nullptr, 2);                        // middle 4 bits form the column
        int sValue = S[i][row][col];
        substituted.replace(i * 4, 4, bitset<4>(sValue).to_string());
    }

    // Step 4: Apply the Straight P-box permutation to the substituted output
    string permuted(32, '0');
    for (int i = 0; i < 32; i++)
    {
        permuted[i] = substituted[P[i] - 1];
    }

    return permuted;
}

string XOR(string leftHalf, string functionOutput)
{

 string xored(32, '0');
    for (int i = 0; i < 48; i++)
    {
        xored[i] = (leftHalf[i] == functionOutput[i]) ? '0' : '1';
    }

    return xored;
}

string feistelStructure(string permutedPlaintext, vector<string> roundKeys) {
    // Split the 64-bit permuted plaintext into two 32-bit halves
    string leftHalf = permutedPlaintext.substr(0, 32);
    string rightHalf = permutedPlaintext.substr(32, 32);

    cout << "Initial Left Half (L0): " << leftHalf << endl;
    cout << "Initial Right Half (R0): " << rightHalf << endl;

    // Perform 16 rounds of DES
    for (int round = 0; round < 16; round++) {
        cout << "********** ROUND " << (round + 1) << " **********" << endl;

        // Save the old right half (for use in the next left half)
        string oldRightHalf = rightHalf;

        // Apply the DES function on the right half with the current round key
        string functionOutput = desFunction(rightHalf, roundKeys[round]);

        // XOR the left half with the output of the DES function
        rightHalf = XOR(leftHalf, functionOutput);
        

        // The new left half is the old right half
        leftHalf = oldRightHalf;

        cout << "After Round " << (round + 1) << " Left Half (L" << round + 1 << "): " << leftHalf << endl;
        cout << "After Round " << (round + 1) << " Right Half (R" << round + 1 << "): " << rightHalf << endl;
    }

    // Combine the final halves (note: for final permutation, the halves are swapped)
    string finalBlock = rightHalf + leftHalf;  // Swap the final halves
    cout << "Final combined (R16 + L16): " << finalBlock << endl;

    return finalBlock;
}

//Read the code from here
int main()
{

cout<<"\nDESDESDESDESDESDESDESDESDESDESDESDESDESDESDESDESDESDESDESDESDESDESDESDESDESDESDESDESDESDESDESDESDESDESDESDESDESDESDESDESDESDESDESDESDESDESDESDESDESDESDESDESD\n\n\n";
    string st;
    cout << "Enter the 16 word bit Plaintext for Encryption :";
    cin >> st;
    cout<<endl;
    string permutedPlaintext = planetextPermutation(st);

    cout<<"\n\nBinary:"<<permutedPlaintext;
    cout<<"\n\n\n";

    string key;
    cout << "Enter the key (16 words) for key generation :";
    cin >> key;
    
    cout<<"\n\n\n";
    vector<string> roundKeys = keyGeneration(key);  // Now this will work

    feistelStructure(permutedPlaintext,roundKeys);
    return 0;
}
