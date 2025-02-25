#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include <iostream>

#include <imgui.h>
#include <imnodes.h>
#include <ctre.hpp>

#include "nlohmann/json.hpp"

#include "keyInformation.h"

enum class ConditionType {
	PassCredits,
	WeightedAverage,
	CreditsGTPercent,
	True,
	False,
	Unknown
};

struct Node{
	int id;
	std::string text;
	std::string condition;
	int yesid = -1;
	int noid = -1;
	int blankid = -1;

	// Function to determine the identifier based on the content of the string
	ConditionType determineIdentifier(const std::string str) const {
		if (ctre::match<"pass credits.*">(str)) {
			return ConditionType::PassCredits;
		}
		else if (ctre::match<"weighted.*">(str)) {
			return ConditionType::WeightedAverage;
		}
		else if (ctre::match<".*credits >.*">(str)) {
			return ConditionType::CreditsGTPercent;
		}
		else if (ctre::match<"true">(str)) {
			return ConditionType::True;
		}
		else {
			return ConditionType::Unknown;
		}
	}

	// Function to process different strings and extract numbers
	std::vector<int> processString(const std::string& str) const {
		// Determine the identifier
		//ConditionType identifier = determineIdentifier(str);

		// Regular expression to match numbers
		constexpr auto number_pattern = ctll::fixed_string{ "(\\d+)" };

		//std::cout << "Processing " << identifier << ":\n";
		//std::cout << "Processing: " << str << '\n';

		std::vector<int> nums;

		// Using CTRE to find all matches
		for (auto match : ctre::range<number_pattern>(str)) {
			std::string num = std::string(match.to_view());
			//std::cout << num << '\n';
			int number = std::stoi(num);

			// Different actions based on the identifier
			/*if (identifier == ConditionType::PassCredits) {
				std::cout << "Pass credits: " << number << "\n";
			}
			else if (identifier == ConditionType::WeightedAverage) {
				std::cout << "Weighted average: " << number << "%\n";
			}
			else if (identifier == ConditionType::CreditsGTPercent) {
				std::cout << "Credits gt percent: " << number << '\n';;
			}
			else if (identifier == ConditionType::True) {
				std::cout << "True: " << number << '\n';
			}
			else {
				std::cout << "Number: " << number << "\n";
			}*/
			nums.emplace_back(number);
		}

		return nums;
	}

	bool checkCondition(KeyInformation ki) const {
		std::vector<int> nums = processString(condition);
		ConditionType identifier = determineIdentifier(condition);
		switch (identifier) {
		case ConditionType::PassCredits:
			return ki.passedCredits >= nums[0];
		case ConditionType::WeightedAverage:
			return ki.weightedAverage >= nums[0];
		case ConditionType::CreditsGTPercent: {
			int nc = 0;
			for (std::pair<int, float>& p : ki.creditsAtPercent) {
				if (p.second >= (float)nums[1]) {
					nc += p.first;
				}
			}
			return nc >= nums[0];
		};
		case ConditionType::True:
			return true;
		case ConditionType::False:
			return false;
		case ConditionType::Unknown:
			return false;
		}
	}
};

class ProgressionTree {
public:
	ProgressionTree();
	ProgressionTree(const std::string& path, const std::string uni = "UON");

	void drawTree(int widgetWidth, int widgetHeight, KeyInformation keyInformation);

	std::vector<Node> nodes;
	std::vector<std::pair<int, int>> links;

	std::vector<std::vector<int>> layout;

private:
	int width = -1;
	int height = -1;
	
};