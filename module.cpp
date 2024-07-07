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
	this->calculateModulePercent();

	ImGui::BeginChild((data.moduleName + "#" + std::to_string(id)).c_str(), ImVec2(0, 0), ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeY);
	ImGui::SetWindowFontScale((fontScale) ? *fontScale : 1.0f);
	ImGui::Text(("Module Name: " + data.moduleName).c_str());

	ImGui::SameLine();
	if (ImGui::Button("EDIT")) {
		this->editingModule = !this->editingModule;
		ImGui::OpenPopup(("EDIT: " + this->data.moduleName).c_str());
	}

	if (ImGui::Button("Edit Module")) {
		ImGui::OpenPopup(("Edit Module##Popup" + data.moduleName).c_str());
	}
	if (ImGui::BeginPopupModal(("Edit Module##Popup" + data.moduleName).c_str())) {
		ImGui::Text(data.moduleName.c_str());





		//click ok when finished adjusting
		if (ImGui::Button("Exit")) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	ImGui::Text("Credits: %d", data.credits);
	ImGui::Text("Exam Weight: %.2f%%", data.examRatio);
	ImGui::Text("Coursework Weight: %.2f%%", data.courseworkRatio);
	ImGui::Text("Exam Percent: %.2f%%", data.examPercent);
	ImGui::Text("Coursework Percent: %.2f%%", data.courseworkPercent);
	
	ImGui::Separator();
	ImGui::Text(("Module Percent: " + std::to_string(data.modulePercent) + "%%").c_str());
	ImGui::Text(("Pass Or Fail Module?....." + std::string(((data.pass) ? "Pass" : "Fail"))).c_str());
	ImGui::EndChild();

	//if(ImGui::BeginPopupModal(("EDIT: " + this->data.moduleName).c_str())){
	if (this->editingModule) {
		if (this->setData) {
			this->tempData.set(this->data);
			strcpy(this->tempData.mNameBuf, this->data.moduleName.c_str());
			this->setData = false;
		}

		ImGui::Begin(("EDIT: " + this->data.moduleName).c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize);
		
		ImGui::InputText("Module Name", tempData.mNameBuf, sizeof(tempData.mNameBuf), ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_AlwaysOverwrite);
		
		ImGui::InputInt("Credits", &tempData.credits);

		ImGui::InputFloat("Exam Weight", &tempData.examRatio, 0.01f, 1.0f, "%.2f%%");
		ImGui::InputFloat("Coursework Weight", &tempData.courseworkRatio, 0.01f, 1.0f, "%.2f%%");
		ImGui::InputFloat("Exam Percentage", &tempData.examPercent, 0.01f, 1.0f, "%.2f%%");
		ImGui::InputFloat("Coursework Percentage", &tempData.courseworkPercent, 0.01f, 1.0f, "%.2f%%");

		if (ImGui::Button("Save")) {
			this->data.set(this->tempData);
			this->editingModule = false;
			this->setData = true;
			//ImGui::CloseCurrentPopup();
		}
		if (ImGui::Button("Close")) {
			this->editingModule = false;
			this->setData = true;
			//ImGui::CloseCurrentPopup();
		}

		ImGui::End();
	}
}

void Module::calculateModulePercent(){
	data.modulePercent = ((data.examRatio / 100) * (data.examPercent / 100) + (data.courseworkRatio / 100) * (data.courseworkPercent / 100)) * 100;
	data.pass = (data.modulePercent >= 40.0) ? true : false;
}
