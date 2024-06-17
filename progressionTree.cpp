#include "progressionTree.h"

ProgressionTree::ProgressionTree(const std::string& path, const std::string uni){
	if (!std::filesystem::exists(path)) {
		return;
	}

	std::ifstream file(path);

	//std::string linetxt;
	//while (std::getline(file, linetxt)) {
	//	std::cout << linetxt << '\n';
	//}

	//file.seekg(0, std::ios::beg);

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
					//if (nodesData["blank"] != -1) {
					//	this->links.emplace_back(std::pair<int, int>{ nodesData["id"], nodesData["blank"] });
					//}
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


void ProgressionTree::drawTree(int widgetWidth, int widgetHeight) {
	this->unique = 0;

	// Define padding between nodes
	const int xPadding = 20;  // Horizontal gap
	const int yPadding = 20;  // Vertical gap

	// Calculate the gaps considering the padding
	int xGap = (widgetWidth - (layout[0].size() - 1) * xPadding) / layout[0].size();
	int yGap = (widgetHeight - (layout.size() - 1) * yPadding) / layout.size();

	int n = 0;
	for (int j = 0; j < layout.size(); j++) {
		for (int i = 0; i < layout[j].size(); i++) {
			if (n < nodes.size() && layout[j][i] != -1) {
				Node node = nodes[n++];

				ImNodes::BeginNode(node.id);

				float xPos = i * (xGap + xPadding);
				float yPos = j * (yGap + yPadding);
				ImNodes::SetNodeEditorSpacePos(node.id, ImVec2(xPos, yPos));

				ImNodes::BeginNodeTitleBar();
				ImGui::TextUnformatted("Title");
				ImNodes::EndNodeTitleBar();

				std::vector<std::string> lines = wrapText(node.text, 10.0f, ImGui::GetFont());

				for (const std::string& line : lines) {
					ImGui::TextWrapped("%s", line.c_str());
				}

				// You can define your pins here
				ImNodes::BeginInputAttribute(node.yesid);
				ImGui::Text("Input");
				ImNodes::EndInputAttribute();

				ImNodes::BeginOutputAttribute(node.noid);
				ImGui::Text("Output");
				ImNodes::EndOutputAttribute();

				for (auto& l : this->links) {
					if (l.first == node.id) {
						if (l.second == node.yesid) {
							ImNodes::BeginOutputAttribute(l.second);
							ImGui::Text("YES");
							ImNodes::EndOutputAttribute();
						}
						if (l.second == node.noid) {
							ImNodes::BeginOutputAttribute(l.second);
							ImGui::Text("NO");
							ImNodes::EndOutputAttribute();
						}
					}

					if (l.second == node.id) {
						for (auto& nod : this->nodes) {
							if (l.first == nod.id) {
								ImNodes::BeginInputAttribute(nod.id);
								ImGui::Text("YESI");
								ImNodes::EndInputAttribute();
							}
							if (l.first == nod.id) {
								ImNodes::BeginInputAttribute(nod.id);
								ImGui::Text("NOI");
								ImNodes::EndInputAttribute();
							}
						}
					}
				}

				ImNodes::EndNode();

			}
		}
	}

	for (int i = 0; i < links.size(); ++i)
	{
		const std::pair<int, int> p = links[i];
		// in this case, we just use the array index of the link
		// as the unique identifier
		ImNodes::Link(i, p.first, p.second);
	}
}

