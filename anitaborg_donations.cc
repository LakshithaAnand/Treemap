#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstring>
#include "treemap.h"


int main(int argc, char *argv[]) {
    std::string name, amount;

    Treemap<int, std::string> treemap;
    if (argc < 3 || argc > 4) {
        std::cerr << "Usage: " << argv[0] <<
            " <donations_file.dat> <command> [<args>]" << std::endl;
        exit(1);
    }

    if ((strcmp(argv[2], "all")) && (strcmp(argv[2], "cheap")) &&
            (strcmp(argv[2], "who")) && (strcmp(argv[2], "rich"))) {
        std::cerr << "Command '" << argv[2] << "' is invalid" << std::endl;
        std::cerr << "Possible commands are: all|cheap|rich|who" << std::endl;
        exit(1);
    }

    std::ifstream donations_file(argv[1]);

    if (!donations_file.is_open()) {
        std::cerr << "Error: cannot open file " << argv[1] << std::endl;
        exit(1);
    }

    while (getline(donations_file, name, ',') &&
            getline(donations_file, amount)) {
        treemap.Insert(stoi(amount), name);
    }

    if (strcmp(argv[2], "all") == 0) {
        while (!treemap.Empty()) {
            std::cout << treemap.Get(treemap.MinKey()) << " ("
                << treemap.MinKey() << ")" << std::endl;
            treemap.Remove(treemap.MinKey());
        }
    }

    if (strcmp(argv[2], "cheap") == 0) {
        std::cout << treemap.Get(treemap.MinKey()) << " ("
                << treemap.MinKey() << ")" << std::endl;
    }

    if (strcmp(argv[2], "rich") == 0) {
        std::cout << treemap.Get(treemap.MaxKey()) << " ("
                << treemap.MaxKey() << ")" << std::endl;
    }


    if (strcmp(argv[2], "who") == 0) {
        if (argc < 4) {
            std::cerr << "Command" << " '" << argv[2] <<
                "' " << "expects another argument: [+/-]amount" << std::endl;
            exit(1);
        }

        std::string str_num;
        str_num = argv[3];
        int num;
        num = stoi(str_num);
        if (argv[3][0] == '+' || argv[3][0] == '-') {
            str_num = str_num.substr(1, str_num.length());
            num = stoi(str_num);
            if (argv[3][0] == '+') {
                if (num > treemap.MaxKey() || num == treemap.MaxKey()) {
                    std::cout << "No match" << std::endl;
                    return 0;
                }
                int greater_value = num + 1;
                std::cout << treemap.Get(treemap.CeilKey(greater_value)) << " ("
                    << treemap.CeilKey(greater_value) << ")" << std::endl;
            }
            if (argv[3][0] == '-') {
                if (num < treemap.MinKey() || num == treemap.MinKey()) {
                    std::cout << "No match" << std::endl;
                    return 0;
                }
                int lesser_value = num - 1;
                std::cout << treemap.Get(treemap.FloorKey(lesser_value)) << " (" <<
                     treemap.FloorKey(lesser_value) << ")" << std::endl;
            }
        } else {
            num = stoi(str_num);
            if (treemap.ContainsKey(num)) {
                std::cout << treemap.Get(num) << " (" << num
                    << ")" << std::endl;
            }
        }
    }
}
