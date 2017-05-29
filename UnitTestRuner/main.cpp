#include <windows.h>

#include <string>
#include <fstream>
#include <iostream>
#include <ostream>
#include <vector>
#include <sstream>

using namespace std;

typedef int(*RunAllUnitTestsPtr)(int argc, char* argv[]);


/// \brief This class is a derivate of basic_stringbuf which will output all the written data using the OutputDebugString function
template<typename TChar, typename TTraits = std::char_traits<TChar>, typename Alloc = std::allocator<TChar>>
class OutputDebugStringBuf : public std::basic_stringbuf<TChar, TTraits, Alloc> {
public:
    explicit OutputDebugStringBuf() : _buffer(256) {
        setg(nullptr, nullptr, nullptr);
        setp(_buffer.data(), _buffer.data(), _buffer.data() + _buffer.size());
    }

    ~OutputDebugStringBuf() {
    }
    typedef typename TTraits::int_type int_type;

    static_assert(std::is_same<TChar, char>::value || std::is_same<TChar, wchar_t>::value, "OutputDebugStringBuf only supports char and wchar_t types");

    int sync() try {
        MessageOutputer<TChar, TTraits>()(pbase(), pptr());
        setp(_buffer.data(), _buffer.data(), _buffer.data() + _buffer.size());
        return 0;
    }
    catch (...) {
        return -1;
    }

    int_type overflow(int_type c = TTraits::eof()) {
        auto syncRet = sync();
        if (c != TTraits::eof()) {
            _buffer[0] = c;
            setp(_buffer.data(), _buffer.data() + 1, _buffer.data() + _buffer.size());
        }
        return syncRet == -1 ? TTraits::eof() : 0;
    }


private:
    std::vector<TChar>		_buffer;

    template<typename TChar, typename TTraits>
    struct MessageOutputer;

    template<>
    struct MessageOutputer<char, std::char_traits<char>> {
        template<typename TIterator>
        void operator()(TIterator begin, TIterator end) const {
            std::string s(begin, end);
            OutputDebugStringA(s.c_str());
        }
    };

    template<>
    struct MessageOutputer<wchar_t, std::char_traits<wchar_t>> {
        template<typename TIterator>
        void operator()(TIterator begin, TIterator end) const {
            std::wstring s(begin, end);
            OutputDebugStringW(s.c_str());
        }
    };

};

bool FileExist(const string& filePath)
{
    ifstream file(filePath);

    return !file.fail();
}

int main(int argc, char* argv[])
{
    //static OutputDebugStringBuf<char> charDebugOutput;
    //std::cerr.rdbuf(&charDebugOutput);
    //std::clog.rdbuf(&charDebugOutput);
    //std::cout.rdbuf(&charDebugOutput);

    //static OutputDebugStringBuf<wchar_t> wcharDebugOutput;
    //std::wcerr.rdbuf(&wcharDebugOutput);
    //std::wclog.rdbuf(&wcharDebugOutput);
    //std::wcout.rdbuf(&wcharDebugOutput);
    std::cout << "Running Google Test!" << std::endl;
#if 1
    HMODULE hModule = GetModuleHandle(NULL);
    char runnerFilePath_c[MAX_PATH];
    GetModuleFileName(hModule, runnerFilePath_c, MAX_PATH);

    string runnerFilePath = runnerFilePath_c;
    string binDir;
    const size_t last_slash_idx = runnerFilePath.rfind('\\');
    if (std::string::npos != last_slash_idx)
    {
        binDir = runnerFilePath.substr(0, last_slash_idx);
    }

    string moduleName = "";
    if (argc > 1)
    {
        moduleName = argv[1];
        if (moduleName.find(".") == std::string::npos)
            moduleName = moduleName + ".dll";
    }

    string scanHubFilePath = binDir + "\\" + moduleName;
    if (!FileExist(scanHubFilePath))
    {
        cout << moduleName << " is not found." << endl;
        return 1;
    }

    HINSTANCE hinstLib = LoadLibrary(scanHubFilePath.c_str());
    if (hinstLib == NULL)
    {
        cout << "Failed to load " << moduleName << endl;
        return 1;
    }

    RunAllUnitTestsPtr runAllUnitTestsPtr = (RunAllUnitTestsPtr)GetProcAddress(hinstLib, "RunAllUnitTests");
    if (runAllUnitTestsPtr == NULL)
    {
        cout << "RunAllUnitTests not found in " << moduleName << " Check if " << moduleName << "is shipping version." << endl;
        return 1;
    }

    // The unit tests will be run during this query.
    return (*runAllUnitTestsPtr)(argc, argv);
#endif
}