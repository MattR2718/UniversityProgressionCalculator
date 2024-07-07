#include "term.h"

Term::Term(){
	id = count;
	count++;
}

void Term::display(){
	ImGui::BeginChild(("Term#" + std::to_string(id)).c_str(), ImVec2(0, 0), ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeY);
	ImGui::SetWindowFontScale((fontScale) ? *fontScale : 1.0f);
	ImGui::Text(term.c_str());
	ImGui::SameLine();
	if (ImGui::Button("Add Module")) {
		this->modules.emplace_back(Module());
	}

	ImGui::SameLine();
	if (ImGui::Button("Edit Term")) {
		ImGui::OpenPopup(("Edit Term##Popup" + term).c_str());
	}
	if (ImGui::BeginPopupModal(("Edit Term##Popup" + term).c_str())) {
		ImGui::Text(term.c_str());

		//click ok when finished adjusting
		if (ImGui::Button("Exit")) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	for (auto& m : modules) {

		m.display();
		//ImGui::Separator();
	}

	ImGui::EndChild();
}

void Term::addModule(Module& m){
	modules.emplace_back(m);
}

void Term::calculatePercentages(){
	for (auto& m : modules) {
		m.calculateModulePercent();
	}
}