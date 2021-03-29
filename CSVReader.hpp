#pragma once
#include <bits/stdc++.h>

class CSVReader {
private:
    std::string file;
    std::string delimeter;
    int size;

public:
    CSVReader(std::string file, std::string delm, int size) : file(file), delimeter(delm), size(size) {}
    std::vector<std::string> split(std::string);
    std::vector<std::vector<std::string>> read();
};

std::vector<std::vector<std::string> > CSVReader::read() {
    std::ifstream file(file);
    std::vector<std::vector<std::string>> sheet;
    std::string buf_line = "";
    
   
    bool first_line = true;
    int idx = 0;
    while (getline(file, buf_line)) {
        if (first_line) {
            first_line = false;
            continue;
        }
        sheet.push_back(split(buf_line));
        std::cout << idx++ << std::endl;
        //if (++idx > 1000) break;
    }
    file.close();

    std::vector<int> perm(sheet.size());
    std::iota(perm.begin(), perm.end(), 0);
    std::random_shuffle(perm.begin(), perm.end());
    std::vector<std::vector<std::string>> sel_sheet;
    for (int i = 0; i < size; i++) sel_sheet.push_back(sheet[perm[i]]);

    return sel_sheet;
}

std::vector<std::string> CSVReader::split(std::string str) {
    char delm = this->delimeter[0];
    std::vector<std::string> elements;
    std::stringstream ss(str);
    std::string buf;

    while (getline(ss, buf, delm)) {
        if (!buf.empty() && buf.front() == '\"') buf = buf.substr(1, buf.length() - 2);
        elements.push_back(buf);
    }

    return elements;
}
