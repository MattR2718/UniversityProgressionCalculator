#pragma once

#include <vector>

#include "module.h"

class Term {
public:
	std::vector<Module> modules = {};
	std::string term = "Default Term";

	float* fontScale = nullptr;

	Term();

	void display();

	void addModule(Module& m);

	void calculatePercentages();

private:
	static inline int count = 0;
	int id = -1;

};