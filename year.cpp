#include "year.h"

Year::Year(std::string y)
	:
	year(y)
{
	id = count;
	count++;
}

int Year::getColumns(){
	int num = 0;
	for (auto& t : terms) {
		num += t.modules.size();
	}
	return num;
}

void Year::display(){

	ImGui::BeginChild(("Year#"+std::to_string(id)).c_str(), ImVec2(0, 0), ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeY);
	//ImGui::SetWindowFontScale((fontScale) ? *fontScale : 1.0f);
	ImGui::Text("Number Of Terms: %d", terms.size());
	for (auto& t : terms) {
		t.display();
		//ImGui::Separator();
	}


	ImGui::EndChild();
}

void Year::addTerm(Term& t){
	terms.emplace_back(t);
}

void Year::calculatePercentages(){
	for (auto& t : terms) {
		t.calculatePercentages();
	}
}

float Year::getOverallPercentage(){
	float total = 0.0f;
	for (auto& t : terms) {
		total += t.getOverallPercentage();
	}
	return total;
}

int Year::getTotalNumModules(){
	return std::accumulate(terms.begin(), terms.end(), 0, [](int total, Term& t) { return total + t.modules.size(); });
}

std::vector<const char*> Year::getModuleNames(){
	std::vector<const char*> names;
	for (auto& t : terms) {
		for (auto& m : t.modules) {
			names.push_back(m.data.moduleName.c_str());
		}
	}
	return names;
}

std::vector<float> Year::getModulePercentages(){
	std::vector<float> percentages;
	for (auto& t : terms) {
		for (auto& m : t.modules) {
			percentages.push_back(m.data.modulePercent);
		}
	}
	return percentages;
}
