/*
 * Swordfish: Realtime LogFile Monitoring Software
 * Copyright (C) 2022 Collodi Choi
 * All rights reserved.
 * 2022-Nov-19 Created by Collodi Choi
 */

#include "crow.h"
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <string.h>
#include <unistd.h>
#include <sqlite3.h> 
#include <vector>
#include <regex>
#include <signal.h>
#include <sys/inotify.h>

#define INOTIFY_PATH "/tmp/"

using namespace std;
using json = nlohmann::json;


volatile sig_atomic_t stop;

static void __handle_inotify_event(const struct inotify_event *event)
{
    if (event->mask & IN_MODIFY) getlog();
}

/* SQL stuff
vector<string> split(const string& s)
{
    stringstream ss(s);
    vector<string> words;
    for (string w; ss>>w; ) words.push_back(w);
    return words;
}

string quotesql(const string& s)
{
    return string("'") + s + string("'");
}

void inthand(int signum) {
    stop = 1;
}
*/

int getlog()
{
    cout << "sqlite " << sqlite3_libversion() << endl;

    char line[256];
    string sqlstatement;

    ifstream ifs("quisp.json");
    json data = json::parse(ifs);

    string logfile = "swordfish.log";
    ofstream ofs;
    ofs.open(logfile, std::ios::app);

    sqlite3 *db;
    sqlite3_stmt *stmt;

    if (ifs.good() && (sqlite3_open("./sensor.db", &db) == SQLITE_OK))
    {
        cout << "Target Log File: GOOD" << endl;
        cout << "DB opened" << endl;
        cout << "Logging initiated" << endl;



        // cin.getline(line,256);
        // auto words = split(line); // This must be something wrong
        cout << "ACK" << endl;

        /*
        sqlstatement = 
            "INSERT INTO fiber-data (sensorval) VALUES(" 
            + quotesql(words[1]) + ");";

        ofs << sqlstatement << endl;

        sqlite3_prepare(db, sqlstatement.c_str(), 
                -1, &stmt, NULL);
        sqlite3_step(stmt);
        */
    }
    else
    {
        cout << "DB failed to open" << endl;
        return 0;
    }
    
    // sqlite3_finalize(stmt);
    // sqlite3_close(db);
    cout << "DB closed" << endl;

    return 1;

}

int main()
{
    signal(SIGINT, inthand);
    logger();

    /*
    crow::SimpleApp app;

    CROW_ROUTE(app, "/<string>")([](std::string name){
        auto page = crow::mustache::load("fancypage.html");
        crow::mustache::context ctx ({{"person", name}});
        return page.render(ctx);
    });

    app.port(18080).multithreaded().run();
    */

    return 0;
}

