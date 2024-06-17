#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include <iostream>

#include <imgui.h>
#include <imnodes.h>

#include "nlohmann/json.hpp"

struct Node{
	int id;
	std::string text;
	std::string condition;
	int yesid = -1;
	int noid = -1;
	int blankid = -1;
};

class ProgressionTree {
public:
	ProgressionTree(const std::string& path, const std::string uni = "UON");

	void drawTree(int widgetWidth, int widgetHeight);

	std::vector<Node> nodes;
	std::vector<std::pair<int, int>> links;

	std::vector<std::vector<int>> layout;

private:
	int width = -1;
	int height = -1;

	int unique = 0;

};