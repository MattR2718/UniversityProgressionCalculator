#include "module.h"


Module::Module(std::string mn, int cr, float er, float crat, float ep, float cp) 
	:
	moduleName(mn),
	credits(cr),
	examRatio(er),
	courseworkRatio(crat),
	examPercent(ep),
	courseworkPercent(cp)
{
	id = count;
	count++;
}

void Module::display(){
	ImGui::BeginChild((moduleName + "#" + std::to_string(id)).c_str(), ImVec2(0, 0), ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeY);
	ImGui::SetWindowFontScale((fontScale) ? *fontScale : 1.0f);
	ImGui::Text(("Module Name: " + moduleName).c_str());
	ImGui::Text(("Credits: " + std::to_string(credits)).c_str());
	ImGui::Text(("Exam Weight: " + std::to_string(examRatio) + "%%").c_str());
	ImGui::Text(("Coursework Weight: " + std::to_string(courseworkPercent) + "%%").c_str());
	ImGui::Text(("Exam Percentage: " + std::to_string(examPercent) + "%%").c_str());
	ImGui::Text(("Coursework Percent: " + std::to_string(courseworkPercent) + "%%").c_str());
	ImGui::Text(("Pass Or Fail Module?....." + std::string(((pass) ? "Pass" : "Fail"))).c_str());
	ImGui::EndChild();
}
