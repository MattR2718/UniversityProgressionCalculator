#pragma once

#include <vector>

#include "module.h"

class Module;

class Term {
public:
	std::vector<Module> modules = {};
	std::string term = "Default Term";

	float* fontScale = nullptr;

	Term();

	void display();

	void addModule(Module& m);

	void calculatePercentages();

	void deleteModule(const std::string_view mn);

private:
	static inline int count = 0;
	int id = -1;

};