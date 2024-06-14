#pragma once

#include <vector>

#include "module.h"

class Term {
public:
	std::vector<Module> modules = {Module("MODULE 1"), Module("MODULE 2"), Module("MODULE 3"), Module("MODULE 4")};

	float* fontScale = nullptr;

	Term();

	void display();

private:
	static inline int count = 0;
	int id = -1;

};