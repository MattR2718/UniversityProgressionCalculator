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


void ProgressionTree::drawTree(int widgetWidth, int widgetHeight) {
	this->unique = 0;

	// Define padding between nodes
	const int xPadding = 20;  // Horizontal gap
	const int yPadding = 20;  // Vertical gap
	const int margin = 50;    // Margin from the edges

	// Calculate the gaps considering the padding
	int xGap = (widgetWidth - 2 * margin - (layout[0].size() - 1) * xPadding) / layout[0].size();
	int yGap = (widgetHeight - 2 * margin - (layout.size() - 1) * yPadding) / layout.size();

	// Set up node color style with transparency
	ImNodesStyle& style = ImNodes::GetStyle();
	
	style.Colors[ImNodesCol_NodeBackground] = IM_COL32(50, 50, 50, 90);   // Node background color
	style.Colors[ImNodesCol_NodeBackgroundHovered] = IM_COL32(75, 75, 75, 180);  // Node background color when hovered
	style.Colors[ImNodesCol_NodeBackgroundSelected] = IM_COL32(75, 75, 75, 180);  // Node background color when selected
	style.Colors[ImNodesCol_NodeOutline] = IM_COL32(100, 100, 100, 180);   // Node outline color

	int n = 0;
	std::vector<ImVec2> nodePositions(nodes.size());
	std::vector<ImVec2> nodeSizes(nodes.size());

	for (int j = 0; j < layout.size(); j++) {
		for (int i = 0; i < layout[j].size(); i++) {
			if (n < nodes.size() && layout[j][i] != -1) {
				Node node = nodes[n];
				ImNodes::BeginNode(node.id);

				float xPos = margin + i * (xGap + xPadding);
				float yPos = margin + j * (yGap + yPadding);
				ImNodes::SetNodeEditorSpacePos(node.id, ImVec2(xPos, yPos));
				nodePositions[n] = ImVec2(xPos, yPos);

				ImNodes::BeginNodeTitleBar();
				ImGui::TextUnformatted("Title");
				ImNodes::EndNodeTitleBar();

				std::vector<std::string> lines = wrapText(node.text, 10.0f, ImGui::GetFont());

				for (const std::string& line : lines) {
					ImGui::TextWrapped("%s", line.c_str());
				}

				ImNodes::BeginInputAttribute(node.id * 1000);
				ImGui::Text("IN");
				ImNodes::EndInputAttribute();

				ImNodes::BeginOutputAttribute(node.id * 1001);
				ImGui::Text("OUT");
				ImNodes::EndOutputAttribute();

				ImNodes::EndNode();

				n++;
			}
		}
	}

	// Draw the links after nodes have been positioned
	for (int i = 0; i < links.size(); ++i) {
		const std::pair<int, int> p = links[i];
		ImNodes::Link(i, p.first * 1001, p.second * 1000);
	}
}

