#pragma once

#include <vector>
#include <iostream>

#include "module.h"

class Module;

class Term {
public:
	std::vector<Module> modules = {};
	std::string term = "Default Term";

	//float* fontScale = nullptr;

	Term();

	void display();

	void addModule(Module& m);

	void calculatePercentages();

	void deleteModule(const int i);

	float getOverallPercentage();

private:
	static inline int count = 0;
	int id = -1;

};