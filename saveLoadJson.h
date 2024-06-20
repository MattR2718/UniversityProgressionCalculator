#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include "year.h"

#include "nlohmann/json.hpp"

std::vector<Year> loadYearsFromJson(const std::string& path) {
	std::ifstream file(path);
	nlohmann::json data = nlohmann::json::parse(file);

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
                    m.data.moduleName = course.key();
                    m.data.credits = course.value()["Credits"];
                    m.data.examRatio = course.value()["examRatio"];
                    m.data.courseworkRatio = course.value()["courseworkRatio"];
                    m.data.examPercent = course.value()["examPercent"];
                    m.data.courseworkPercent = course.value()["courseworkPercent"];

                    t.modules.emplace_back(m);
                }
                y.terms.emplace_back(t);
            }
            years.emplace_back(y);
        }
    }
    catch (nlohmann::json::exception& e) {
        std::cerr << "JSON error: " << e.what() << '\n';
    }

	return years;
}

// Function to convert a Module object to a JSON object
void toJson(nlohmann::json& j, const Module& m) {
    j = nlohmann::json{
        {"Credits", m.data.credits},
        {"examRatio", m.data.examRatio},
        {"courseworkRatio", m.data.courseworkRatio},
        {"examPercent", m.data.examPercent},
        {"courseworkPercent", m.data.courseworkPercent}
    };
}

// Function to convert a Term object to a JSON object
void toJson(nlohmann::json& j, const Term& t) {
    for (const auto& module : t.modules) {
        nlohmann::json moduleJson;
        toJson(moduleJson, module);
        j[t.term][module.data.moduleName] = moduleJson;
    }
}

// Function to convert a Year object to a JSON object
void toJson(nlohmann::json& j, const Year& y) {
    for (const auto& term : y.terms) {
        nlohmann::json termJson;
        toJson(termJson, term);
        j[y.year].update(termJson);
    }
}

// Save function
void saveYearsToJson(const std::string& path, const std::vector<Year>& years) {
    nlohmann::json j;
    for (const auto& year : years) {
        toJson(j, year);
    }

    //Print out the json
    //std::cout << j.dump(4) << '\n';

    std::ofstream file(path);
    file << j.dump(4); // Indent with 4 spaces
}