#include "progressionTree.h"

ProgressionTree::ProgressionTree(){
}

ProgressionTree::ProgressionTree(const std::string& path, const std::string uni){
	if (!std::filesystem::exists(path)) {
		return;
	}

	std::ifstream file(path);

	nlohmann::json data = nlohmann::json::parse(file);

	try {
		for (const auto& uniData : data.items()) {
			if (uniData.key() == uni) {
				this->width = uniData.value()["width"];
				this->height = uniData.value()["height"];
				for (const auto& nodesData : uniData.value()["nodes"]) {
					this->nodes.emplace_back(Node{
						.id = nodesData["id"],
						.text = nodesData["text"],
						.condition = nodesData["condition"],
						.yesid = nodesData["yes"],
						.noid = nodesData["no"],
						.blankid = nodesData["blank"]
						});
					if (nodesData["yes"] != -1) {
						this->links.emplace_back(std::pair<int, int>{ nodesData["id"], nodesData["yes"] });
					}
					if (nodesData["no"] != -1) {
						this->links.emplace_back(std::pair<int, int>{ nodesData["id"], nodesData["no"] });
					}
					if (nodesData["blank"] != -1) {
						this->links.emplace_back(std::pair<int, int>{ nodesData["id"], nodesData["blank"] });
					}
				}
				this->layout = uniData.value()["layout"];
			}
		}
	}
	catch (nlohmann::json::exception& e) {
		std::cerr << "JSON error: " << e.what() << '\n';
	}
}



std::vector<std::string> wrapText(const std::string& text, float maxWidth, ImFont* font) {
	std::vector<std::string> lines;
	std::istringstream wordsStream(text);
	std::string word;
	std::string line;

	while (wordsStream >> word) {
		std::string newLine = line.empty() ? word : line + " " + word;
		//float newLineWidth = ImGui::CalcTextSize(newLine.c_str(), nullptr, false, maxWidth).x;
		float newLineWidth = newLine.length();

		if (newLineWidth <= maxWidth) {
			line = newLine;
		}
		else {
			if (!line.empty()) {
				lines.push_back(line);
			}
			line = word;
		}
	}

	if (!line.empty()) {
		lines.push_back(line);
	}

	return lines;
}




bool checkBothConditions(const std::pair<int, int>& link, const std::vector<Node>& nodes, const KeyInformation ki) {
	bool n1 = false;
	bool n2 = false;

	for (const Node& n : nodes) {
		if (n.id == link.first && n.checkCondition(ki)) { n1 = true; }
		if (n.id == link.second && n.checkCondition(ki)) { n2 = true; }
	}

	return n1 && n2;
}


void ProgressionTree::drawTree(int widgetWidth, int widgetHeight, float fontScale, KeyInformation keyInformation) {
	// Define padding between nodes
	const int xPadding = 10;  // Horizontal gap
	const int yPadding = 40;  // Vertical gap
	const int margin = 50;    // Margin from the edges

	// Calculate the gaps considering the padding
	int xGap = (widgetWidth - 2 * margin - (layout[0].size() - 1) * xPadding) / layout[0].size();
	int yGap = (widgetHeight - 2 * margin - (layout.size() - 1) * yPadding) / layout.size();

	int n = 0;
	std::vector<ImVec2> nodePositions(nodes.size());
	std::vector<ImVec2> nodeSizes(nodes.size());

	for (int j = 0; j < layout.size(); j++) {
		for (int i = 0; i < layout[j].size(); i++) {
			if (n < nodes.size() && layout[j][i] != -1) {
				Node node = nodes[n];

				bool progressionNode = node.yesid == -1 && node.noid == -1 && node.blankid == -1;
				bool greenProgression = false;
				if (progressionNode) {
					for (auto& l : links) {
						if (l.second == node.id) {
							if (nodes[l.first].checkCondition(keyInformation)) {
								greenProgression = true;
								break;
							}
						}
					}
				}

				if ((progressionNode && greenProgression) || (!progressionNode && node.checkCondition(keyInformation))) {
					ImNodes::PushColorStyle(ImNodesCol_NodeOutline, IM_COL32(0, 255, 0, 255));
				}
				ImNodes::BeginNode(node.id);

				float xPos = margin + i * (xGap + xPadding);
				float yPos = margin + j * (yGap + yPadding);
				ImNodes::SetNodeEditorSpacePos(node.id, ImVec2(xPos, yPos));
				nodePositions[n] = ImVec2(xPos, yPos);

				//ImNodes::BeginNodeTitleBar();
				//ImGui::TextUnformatted("Title");
				//ImNodes::EndNodeTitleBar();

				std::vector<std::string> lines = wrapText(node.text, 12.0f, ImGui::GetFont());

				for (const std::string& line : lines) {
					ImGui::TextWrapped("%s", line.c_str());
				}

				ImNodes::BeginInputAttribute(node.id * 100);
				ImGui::SetWindowFontScale(0.5);
				ImGui::Text("IN");
				//ImGui::Text("#IN");
				ImNodes::EndInputAttribute();

				ImNodes::BeginOutputAttribute(node.id * 7);
				ImGui::Text("OUT");
				//ImGui::Text("#OUT");
				ImGui::SetWindowFontScale(1);
				ImNodes::EndOutputAttribute();

				ImNodes::EndNode();
				if ((progressionNode && greenProgression) || (!progressionNode && node.checkCondition(keyInformation))) {
					ImNodes::PopColorStyle();
				}

				n++;
			}
		}
	}

	// Draw the links after nodes have been positioned
	// Sometimes having links causes the program to explode in memory
	// Crashes somewhere allocating memory for line segments creating bezier cuves
	// Sometimes fixes itself by removing the below code, compiling then readding for some reason
#if false
	for (int i = 0; i < links.size(); ++i) {
		const std::pair<int, int> p = links[i];
		if (checkBothConditions(p, nodes, keyInformation)) {
			ImNodes::PushColorStyle(ImNodesCol_Link, IM_COL32(0, 255, 0, 244));
			ImNodes::Link(i, p.first * 7, p.second * 100);
			ImNodes::PopColorStyle();
		}else{
			ImNodes::Link(i, p.first * 7, p.second * 100);
		}
	}
#endif
}

