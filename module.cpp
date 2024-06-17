#include "module.h"


Module::Module(std::string mn, int cr, float er, float crat, float ep, float cp) {
	data.moduleName = mn;
	data.credits = cr;
	data.examRatio = er;
	data.courseworkRatio = crat;
	data.examPercent = ep;
	data.courseworkPercent = cp;
	id = count;
	count++;
}

void Module::display(){
	ImGui::BeginChild((data.moduleName + "#" + std::to_string(id)).c_str(), ImVec2(0, 0), ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeY);
	ImGui::SetWindowFontScale((fontScale) ? *fontScale : 1.0f);
	ImGui::Text(("Module Name: " + data.moduleName).c_str());

	ImGui::SameLine();
	if (ImGui::Button("EDIT")) {
		this->editingModule = true;
	}

	ImGui::Text(("Credits: " + std::to_string(data.credits)).c_str());
	ImGui::Text(("Exam Weight: " + std::to_string(data.examRatio) + "%%").c_str());
	ImGui::Text(("Coursework Weight: " + std::to_string(data.courseworkPercent) + "%%").c_str());
	ImGui::Text(("Exam Percentage: " + std::to_string(data.examPercent) + "%%").c_str());
	ImGui::Text(("Coursework Percent: " + std::to_string(data.courseworkPercent) + "%%").c_str());
	ImGui::Separator();
	ImGui::Text(("Module Percent: " + std::to_string(data.modulePercent) + "%%").c_str());
	ImGui::Text(("Pass Or Fail Module?....." + std::string(((data.pass) ? "Pass" : "Fail"))).c_str());
	ImGui::EndChild();

	if (this->editingModule) {

		this->tempData.moduleName = data.moduleName;
		this->tempData.credits = data.credits;
		this->tempData.examRatio = data.examRatio;
		this->tempData.courseworkRatio = data.courseworkRatio;
		this->tempData.examPercent = data.examPercent;
		this->tempData.courseworkPercent = data.courseworkPercent;


		ImGui::Begin(("EDIT: " + this->data.moduleName).c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::InputText(("Module Name: " + data.moduleName).c_str(), this->moduleNameBuf, 256);
		ImGui::InputInt(("Credits: " + std::to_string(data.credits)).c_str(), &tempData.credits);
		ImGui::InputFloat(("Exam Weight: " + std::to_string(data.examRatio) + "%%").c_str(), &data.examRatio);
		ImGui::InputFloat(("Coursework Weight: " + std::to_string(data.courseworkPercent) + "%%").c_str(), &data.courseworkRatio);
		ImGui::InputFloat(("Exam Percentage: " + std::to_string(data.examPercent) + "%%").c_str(), &data.examPercent);
		ImGui::InputFloat(("Coursework Percent: " + std::to_string(data.courseworkPercent) + "%%").c_str(), &data.courseworkPercent);

		if (ImGui::Button("Save")) {
			data = tempData;
			data.moduleName = std::string(moduleNameBuf);
			this->editingModule = false;
		}
		if (ImGui::Button("Close")) {
			this->editingModule = false;
		}

		ImGui::End();
	}
}

void Module::calculateModulePercent(){
	data.modulePercent = ((data.examRatio / 100) * (data.examPercent / 100) + (data.courseworkRatio / 100) * (data.courseworkPercent / 100)) * 100;
	data.pass = (data.modulePercent >= 40.0) ? true : false;
}
