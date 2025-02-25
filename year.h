#pragma once

#include <vector>
#include <imgui.h>

#include "term.h"

class Year {
public:
	std::vector<Term> terms = {};
	std::string year = "Default";

	bool tabOpen = true;
	//float* fontScale = nullptr;

	Year(std::string y = "Default");
	int getColumns();

	void display();

	void addTerm(Term& t);

	void calculatePercentages();

	float getOverallPercentage();

private:
	static inline int count = 0;
	int id = -1;
};