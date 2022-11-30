// ttyr.h

#include <iostream>
#include <thread>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
//using namespace nlohmann::literals;
using namespace std;

string ctos(char *a)
{
    string s(a);
    return s;
}

class TTYreader
{
    public:
        TTYreader(){}
        TTYreader(std::string x, std::string y);
        int tty_id;
        std::thread reader_thread;
        string loc, mode;
        int fd;
        string str;
        vector<string> tty;
        char temp[TEMP_SIZE];
        // string pipe;
        void update_tty(std::string loc, std::string mode);
};

TTYreader::TTYreader(std::string x, std::string y)
{
    loc = x;
    mode = y;
    cout << endl << "Reading " << loc << endl;
    mkfifo(loc.c_str(), 0666);
    // reader_thread = std::thread(update_tty);
    reader_thread = 
        std::thread(&TTYreader::update_tty, this, loc, mode);
}

void TTYreader::update_tty(std::string loc, std::string mode)
{
    cout << endl << "Process started: " << loc << endl;


    if (mode == "json")
    {
        while (1)
        {
            /*
            std::ifstream ifs(LINK_LOC);
            std::stringstream buffer;

            buffer << ifs.rdbuf();
            str = buffer.str();

            json data = json::parse(ifs);

            ifs.close();
            */
        }
    }

    else if (mode == "pipe")
    {
        while(1) {
            // How would I optimize the performance leak 
            // regarding these open/close events?
            // std::cout << std::endl << 
            //      "Reading " << pipe << std::endl;
            fd = open(loc.c_str(), O_RDONLY, O_NONBLOCK);
            read(fd, temp, TEMP_SIZE);
            str = ctos(temp);
            cout << loc << ": ACK: " << str << endl;
            close(fd);

            sleep(0.1);
        }
    }

    else
    {
        cout << "Not a valid MODE" << endl;
    }
}

