#pragma once

#include <string>
#include <chrono>
#include <cstring>

#include <imgui-SFML.h>
#include <imgui.h>

struct ModuleData {
	std::string moduleName = "Default Module";
	int credits = 0;
	float examRatio = 50.0f;
	float courseworkRatio = 50.0f;
	float examPercent = 0.0f;
	float courseworkPercent = 0.0f;
	float modulePercent = 0.0f;
	bool pass = false;

	int examRatioArr[2]{ 0, 0 };
	int courseworkRatioArr[2]{ 0, 0 };
	int examPercentArr[2]{ 0, 0 };
	int courseworkPercentArr[2]{ 0, 0 };

	char mNameBuf[256] = "";

	static inline int uniqueIdent = 0;


	void set(const ModuleData& d, bool calcFromInts = false) {
		moduleName = std::string(d.mNameBuf);
		if (moduleName == "") { moduleName = "Default Name" + std::to_string(uniqueIdent++); }
		credits = d.credits;

		if (!calcFromInts) {
			examRatio = d.examRatio;
			courseworkRatio = d.courseworkRatio;
			examPercent = d.examPercent;
			courseworkPercent = d.courseworkPercent;
		}
		else {
			examRatio = (float)d.examRatioArr[0] + (float)d.examRatioArr[1] / 100;
			courseworkRatio = (float)d.courseworkRatioArr[0] + (float)d.courseworkRatioArr[1] / 100;
			examPercent = (float)d.examPercentArr[0] + (float)d.examPercentArr[1] / 100;
			courseworkPercent = (float)d.courseworkPercentArr[0] + (float)d.courseworkPercentArr[1] / 100;
		}
	}

	std::string makeFloatArrString(int arr[2]) {
		return std::to_string(arr[0]) + "." + std::to_string(arr[1]) + "%";
	}
};


class Module {

public:
	float* fontScale = nullptr;

	ModuleData data;

	Module(std::string mn = "Default Module", int cr = 0, float er = 50.0f, float crat = 50.0f, float ep = 50.0f, float cp = 50.0f);

	void display();

	void calculateModulePercent();


private:

	static inline int count = 0;
	int id = -1;

	bool editingModule = false;
	bool setData = true;
	ModuleData tempData;
};