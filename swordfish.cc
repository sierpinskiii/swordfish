/*
 * Swordfish: Realtime LogFile Monitoring Software
 * Copyright (C) 2022 Collodi Choi
 * All rights reserved.
 * 2022-Nov-19 Created by Collodi Choi
 */

#include "crow.h"
#include "config.h"
#include "ttyreader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <vector>
#include <signal.h>
#include <pty.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>

using namespace std;
// cout << endl << "Initiating: " << PIPE_LOC << endl;
TTYreader reader = TTYreader(LINK_LOC, MODE);

crow::json::wvalue ftoj(std::string loc) {
    std::stringstream buffer;
    std::ifstream ifs(loc);
    buffer << ifs.rdbuf();

    auto x = crow::json::load(buffer.str());
    crow::json::wvalue y(x);

    return y;
}

int main(int argc, char *argv[])
{
    crow::SimpleApp app;
    //crow::mustache::set_global_base("WebGui");

    CROW_ROUTE(app, "/")
    ([]{
        auto page = crow::mustache::load_text("imgui.html");
        return page;
    });

    CROW_ROUTE(app, "/imgui.data") // Symlink in the templates. Should fix this dirty hack
    ([]{
        auto page = crow::mustache::load_text("imgui.data");
        return page;
    });

    CROW_ROUTE(app, "/term/tty<int>")
    ([](int tty_id){
        auto page = crow::mustache::load("tty.html");
        cout << "reader.str: " << reader.str << endl;
        crow::mustache::context ctx({{"tty", reader.str}});
        return page.render(ctx);
    });

    CROW_ROUTE(app, "/json")
    ([]{ 
        return ftoj(LINK_LOC);
    });

    CROW_ROUTE(app, "/json/better")
    ([]{ 
        std::stringstream buffer;
        std::ifstream ifs(LINK_LOC);
        buffer << ifs.rdbuf();

        auto page = crow::mustache::load("json.html");
        cout << "\nbuffer.str:\n" << buffer.str() << endl;
        crow::mustache::context ctx({{"json", buffer.str()}});
        return page.render(ctx);
    });

    CROW_ROUTE(app, "/json/<string>")([](std::string key){
        return ftoj(LINK_LOC)[key];
    });

    app.port(18080).multithreaded().run();

    return 0;
}

