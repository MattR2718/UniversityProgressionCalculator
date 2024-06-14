#pragma once

#include <vector>
#include <imgui.h>

#include "term.h"

class Year {
public:
	std::vector<Term> terms = { Term(), Term() };
	std::string year = "Default";

	bool tabOpen = true;
	float* fontScale = nullptr;

	Year(std::string y = "Default");
	int getColumns();

	void display();

private:
	static inline int count = 0;
	int id = -1;
};