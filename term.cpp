#include "term.h"

Term::Term(){
	id = count;
	count++;
}

void Term::display(){
	ImGui::BeginChild(("Term#" + std::to_string(id)).c_str(), ImVec2(0, 0), ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeY);
	ImGui::SetWindowFontScale((fontScale) ? *fontScale : 1.0f);
	ImGui::Text(term.c_str());
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
