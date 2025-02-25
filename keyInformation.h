#pragma once

#include <vector>

#include <imgui.h>

#include "year.h"
#include "term.h"
#include "module.h"

struct KeyInformation {
	int passedCredits = 0;
	float weightedAverage = 0.0f;
	std::vector<std::pair<int, float>> creditsAtPercent;

	void calcData(const Year& year) {
		passedCredits = 0;
		weightedAverage = 0.0f;
		creditsAtPercent.clear();
		int numCredits = 0;

		int nm = 0;
		for (auto& term : year.terms) {
			for (auto& module : term.modules) {
				nm++;
				numCredits += module.data.credits;
				if (module.data.modulePercent >= 40.0) {
					passedCredits += module.data.credits;
				}
				weightedAverage += ((float)module.data.credits / 10.0) * (module.data.courseworkPercent * module.data.courseworkRatio / 100.0f + module.data.examPercent * module.data.examRatio / 100.0f);
				creditsAtPercent.emplace_back(std::make_pair(module.data.credits, module.data.modulePercent));
			}
		}
		weightedAverage /= (float)numCredits / 10.0;
	}
	
	void display(bool& show) {
		ImGui::Begin("Key Information", NULL, ImGuiWindowFlags_AlwaysAutoResize);
		//ImGui::SetWindowFontScale(textScale);
		ImGui::Text("Passed Credits: %d", passedCredits);
		ImGui::Text("Weighted Average: %.2f%%", weightedAverage);
		ImGui::Separator();
		for (const std::pair<int, float>& p : creditsAtPercent) {
			ImGui::Text("%d Credits at %.2f%%", p.first, p.second);
		}

		if (!ImGui::IsWindowFocused()) {
			show = false;
		}

		if (ImGui::Button("Close")) {
			show = false;
		}

		ImGui::End();
	}
};