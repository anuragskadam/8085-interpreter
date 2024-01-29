#include <bits/stdc++.h>
#include <fstream>

using namespace std;

const int MEM_SIZE = 65536;

class Intel8085
{
private:
    // Registers
    uint8_t A;
    uint8_t B;
    uint8_t C;
    uint8_t D;
    uint8_t E;
    uint8_t H;
    uint8_t L;

    uint16_t mem[65536]; // Memory

    uint16_t pc;  // Program Counter
    uint16_t ccc; // Clock Cycle Counter
    uint16_t ic;  // Instruction Counter

    bool zero_flag;
    bool carry_flag;

    bool haltEnc; // Halt Encountered
    bool haltExecuted;
    bool errorEnc;

    void MOV(char r1, char r2)
    {
        uint8_t temp;

        if (r2 == 'A')
            temp = A;
        else if (r2 == 'B')
            temp = B;
        else if (r2 == 'C')
            temp = C;
        else if (r2 == 'D')
            temp = D;
        else if (r2 == 'E')
            temp = E;
        else if (r2 == 'H')
            temp = H;
        else if (r2 == 'L')
            temp = L;

        if (r1 == 'A')
            A = temp;
        else if (r1 == 'B')
            B = temp;
        else if (r1 == 'C')
            C = temp;
        else if (r1 == 'D')
            D = temp;
        else if (r1 == 'E')
            E = temp;
        else if (r1 == 'H')
            H = temp;
        else if (r1 == 'L')
            L = temp;

        pc += 1;
        ccc += 4;
        ic += 1;
    }

    void MVI(char r, int num)
    {
        if (r == 'A')
            A = (num);
        else if (r == 'B')
            B = (num);
        else if (r == 'C')
            C = (num);
        else if (r == 'D')
            D = (num);
        else if (r == 'E')
            E = (num);
        else if (r == 'H')
            H = (num);
        else if (r == 'L')
            L = (num);
        pc += 2;
        ccc += 7;
        ic += 1;
    }

    void DCR(char r)
    {
        if (r == 'A')
            zero_flag = --A ? false : true;
        else if (r == 'B')
            zero_flag = --B ? false : true;
        else if (r == 'C')
            zero_flag = --C ? false : true;
        else if (r == 'D')
            zero_flag = --D ? false : true;
        else if (r == 'E')
            zero_flag = --E ? false : true;
        else if (r == 'H')
            zero_flag = --H ? false : true;
        else if (r == 'L')
            zero_flag = --L ? false : true;
        pc += 1;
        ccc += 4;
        ic += 1;
    }

    void INR(char r)
    {
        if (r == 'A')
            zero_flag = ++A ? false : true;
        else if (r == 'B')
            zero_flag = ++B ? false : true;
        else if (r == 'C')
            zero_flag = ++C ? false : true;
        else if (r == 'D')
            zero_flag = ++D ? false : true;
        else if (r == 'E')
            zero_flag = ++E ? false : true;
        else if (r == 'H')
            zero_flag = ++H ? false : true;
        else if (r == 'L')
            zero_flag = ++L ? false : true;
        pc += 1;
        ccc += 4;
        ic += 1;
    }
    void JNZ(string labelIn)
    {
        if (!zero_flag)
            runLine(labelLineMap[labelIn]);
    }

    void JZ(string labelIn)
    {
        if (zero_flag)
            runLine(labelLineMap[labelIn]);
    }

    void HLT()
    {
        haltEnc = true;
    }

    int hexToDecimal(const string &hexString)
    {
        stringstream ss;
        ss << hex << hexString;

        int decimalValue;
        ss >> decimalValue;

        return decimalValue;
    }

    vector<string> code;
    unordered_map<string, int> labelLineMap;

    void instructionRunner(int lineNumZeroInd, string label = "DEFAULT", string one = "DEFAULT", string two = "DEFAULT", string three = "DEFAULT")
    {

        if (label != "DEFAULT")
            labelLineMap.insert(make_pair(label, lineNumZeroInd));

        if (one == "MVI")
        {
            char r = two[0];
            two.erase(0, 2);
            MVI(r, hexToDecimal(two));
            runLine(lineNumZeroInd + 1);
        }
        else if (one == "MOV")
        {
            char r1 = two[0];
            char r2 = two[2];

            MOV(r1, r2);
            runLine(lineNumZeroInd + 1);
        }
        else if (one == "DCR")
        {
            DCR(two[0]);
            runLine(lineNumZeroInd + 1);
        }

        else if (one == "INR")
        {
            INR(two[0]);
            runLine(lineNumZeroInd + 1);
        }

        else if (one == "JNZ")
        {
            JNZ(two);
        }
        else if (one == "JZ")
        {
            JZ(two);
        }

        else if (one == "HLT")
            HLT();
        else if (one != "DEFAULT")
        {
            cerr << "UnidentifiedCommandError: Line " << lineNumZeroInd + 1 << "\n\t" << code[lineNumZeroInd] << endl;
            errorEnc = true;
            HLT();
        }
    }

    void runLine(int lineNo = 0)
    {
        string sentence = code[lineNo];
        istringstream iss(sentence);

        vector<string> words;

        string word;
        while (iss >> word)
        {
            words.push_back(word);
        }

        vector<string> args = {"DEFAULT", "DEFAULT", "DEFAULT", "DEFAULT"};

        for (int i = 0, j = 0; i < words.size(); i++)
        {
            if (i == 0 && words[i][words[i].length() - 1] == ':')
            {
                words[i].erase(words[i].length() - 1);
                args[args.size() - 1] = words[0];
                continue;
            }
            args[j] = words[i];
            j++;
        }

        instructionRunner(lineNo, args[3], args[0], args[1], args[2]);
        if (errorEnc | haltExecuted)
            return;

        if (!haltEnc)
        {
            runLine(lineNo + 1);
        }
        else
        {
            int dispMem = 0;
            cout << "A | " << hex << static_cast<unsigned>(A) << 'H' << endl;
            cout << "B | " << hex << static_cast<unsigned>(B) << 'H' << endl;
            cout << "C | " << hex << static_cast<unsigned>(C) << 'H' << endl;
            cout << "D | " << hex << static_cast<unsigned>(D) << 'H' << endl;
            cout << "E | " << hex << static_cast<unsigned>(E) << 'H' << endl;
            cout << "H | " << hex << static_cast<unsigned>(H) << 'H' << endl;
            cout << "L | " << hex << static_cast<unsigned>(L) << 'H' << endl;

            // cout << "\nEnter memory address in 0x to view the value stored there." << endl;
            // cout << "To escape, enter -1." << endl;
            // cin >> dispMem;
            // while (dispMem > 0)
            // {
            //     cout << static_cast<unsigned>(mem[dispMem]) << endl
            //          << endl;
            //     cin >> dispMem;
            // }

            haltExecuted = true;
        }
    }

public:
    void reset()
    {

        A = B = C = D = E = H = L = 0;

        pc = 0;
        ccc = 0;
        ic = 0;

        zero_flag = false;
        carry_flag = false;
        haltEnc = false;
        haltExecuted = false;
        errorEnc = false;

        for (int i = 0; i++; i < MEM_SIZE)
        {
            mem[i] = 0;
        }
    }

    void runCode()
    {
        runLine();
    }

    Intel8085()
    {
        reset();
    }

    void setCode(vector<string> codeIn)
    {
        code = codeIn;
    }
};

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        cerr << "Usage: " << argv[0] << " <filename>" << endl;
        return 1;
    }

    const char *filename = argv[1];

    // cout << filename.size() << endl;

    ifstream inputFile(filename);

    if (!inputFile.is_open())
    {
        cerr << "Error opening f ile: " << filename << endl;
        return 1;
    }

    vector<string> vec;
    string sentence;

    while (getline(inputFile, sentence))
    {
        vec.push_back(sentence);
    }
    inputFile.close();

    Intel8085 myCPU;
    myCPU.setCode(vec);
    myCPU.runCode();

    return 0;
}