#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include "year.h"

#include "nlohmann/json.hpp"

std::vector<Year> loadFromJson(const std::string& path) {
	std::ifstream file(path);
	nlohmann::json data = nlohmann::json::parse(file);

    //std::string linetxt;
    //while (std::getline(file, linetxt)) {
    //    std::cout << linetxt << '\n';
    //}

    //nlohmann::json data;
    //file >> data;

    std::vector<Year> years;

    // Access the data
    try {
        for (const auto& year : data.items()) {
            //std::cout << year.key() << ":\n";
            Year y;
            y.year = year.key();
            for (const auto& term : year.value().items()) {
                //std::cout << "  " << term.key() << ":\n";
                Term t;
                t.term = term.key();
                for (const auto& course : term.value().items()) {
                    Module m;
                    //std::cout << "    " << course.key() << ":\n";
                    //std::cout << "      Credits: " << course.value()["Credits"] << "\n";
                    //std::cout << "      Exam Ratio: " << course.value()["examRatio"] << "\n";
                    //std::cout << "      Coursework Ratio: " << course.value()["courseworkRatio"] << "\n";
                    //std::cout << "      Exam Percent: " << course.value()["examPercent"] << "\n";
                    //std::cout << "      Coursework Percent: " << course.value()["courseworkPercent"] << "\n";
                    m.moduleName = course.key();
                    m.credits = course.value()["Credits"];
                    m.examRatio = course.value()["examRatio"];
                    m.courseworkRatio = course.value()["courseworkRatio"];
                    m.examPercent = course.value()["examPercent"];
                    m.courseworkPercent = course.value()["courseworkPercent"];

                    t.modules.emplace_back(m);
                }
                y.terms.emplace_back(t);
            }
            years.emplace_back(y);
        }
    }
    catch (nlohmann::json::exception& e) {
        std::cerr << "JSON error: " << e.what() << std::endl;
    }

	return years;
}