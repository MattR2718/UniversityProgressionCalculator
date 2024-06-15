#pragma once

#include <string>

#include <imgui-SFML.h>
#include <imgui.h>

class Module {

public:

	std::string moduleName = "Default Module";
	int credits = 0;
	float examRatio = 50.0f;
	float courseworkRatio = 50.0f;
	float examPercent = 0.0f;
	float courseworkPercent = 0.0f;
	float modulePercent = 0.0f;
	bool pass = false;

	float* fontScale = nullptr;

	Module(std::string mn = "Default Module", int cr = 0, float er = 50.0f, float crat = 50.0f, float ep = 50.0f, float cp = 50.0f);

	void display();

	void calculateModulePercent();


private:

	static inline int count = 0;
	int id = -1;

};