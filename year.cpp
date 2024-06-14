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
	ImGui::SetWindowFontScale((fontScale) ? *fontScale : 1.0f);
	ImGui::Text("Number Of Terms: %d", terms.size());
	for (auto& t : terms) {
		ImGui::Text("START OF TERM");
		t.display();
		//ImGui::Separator();
	}


	ImGui::EndChild();
}
