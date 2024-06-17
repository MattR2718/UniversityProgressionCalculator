// UniversityProgressionCalculator.cpp : Defines the entry point for the application.
//

#include "UniversityProgressionCalculator.h"

#include <limits> // for std::numeric_limits

namespace ed = ax::NodeEditor;

struct Node {
    ed::NodeId id;
    std::string name;
    ImVec2 position;
    std::vector<ed::PinId> inputPins;
    std::vector<ed::PinId> outputPins;
};

struct Link {
    ed::LinkId id;
    ed::PinId startPinId;
    ed::PinId endPinId;
};

std::vector<Node> nodes;
std::vector<Link> links;

void InitializeNodes() {
    static int nextId = 1;

    // Create first node
    Node node1;
    node1.id = ed::NodeId(nextId++);
    node1.name = "Node 1";
    node1.position = ImVec2(100, 100);
    node1.inputPins.push_back(ed::PinId(nextId++));
    node1.outputPins.push_back(ed::PinId(nextId++));
    nodes.push_back(node1);

    // Create second node
    Node node2;
    node2.id = ed::NodeId(nextId++);
    node2.name = "Node 2";
    node2.position = ImVec2(300, 100);
    node2.inputPins.push_back(ed::PinId(nextId++));
    node2.outputPins.push_back(ed::PinId(nextId++));
    nodes.push_back(node2);

    Node node00;
    node00.id = ed::NodeId(nextId++);
    node00.position = ImVec2(0, 0);
    nodes.push_back(node00);

    Node node1000010000;
    node1000010000.id = ed::NodeId(nextId++);
    node1000010000.position = ImVec2(10000, 10000);
    nodes.push_back(node1000010000);

    // Create link
    Link link;
    link.id = ed::LinkId(nextId++);
    link.startPinId = node1.outputPins[0];
    link.endPinId = node2.inputPins[0];
    links.push_back(link);
}

ImVec2 GetNodeBoundsMin() {
    ImVec2 minPos(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());

    for (const auto& node : nodes) {
        if (node.position.x < minPos.x) minPos.x = node.position.x;
        if (node.position.y < minPos.y) minPos.y = node.position.y;
    }

    return minPos;
}

ImVec2 GetNodeBoundsMax() {
    ImVec2 maxPos(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());

    for (const auto& node : nodes) {
        if (node.position.x > maxPos.x) maxPos.x = node.position.x;
        if (node.position.y > maxPos.y) maxPos.y = node.position.y;
    }

    return maxPos;
}

void ZoomToFitNodes() {
    ImVec2 minPos = GetNodeBoundsMin();
    ImVec2 maxPos = GetNodeBoundsMax();

    // Calculate the bounding box that fits all nodes
    ImVec2 nodesSize = ImVec2(maxPos.x - minPos.x, maxPos.y - minPos.y);
    float zoomLevel = 1.0f;

    // Calculate zoom level to fit nodes within the viewport
    if (nodesSize.x > 0 && nodesSize.y > 0) {
        ImVec2 availableSize = ImGui::GetContentRegionAvail();
        zoomLevel = std::min(availableSize.x / nodesSize.x, availableSize.y / nodesSize.y);
    }

    // Set ImGui Node Editor zoom level (adjust according to your Node Editor's API)
    // ed::SetNodeEditorZoomLevel(zoomLevel); // Example hypothetical function
}

void DrawNodes() {
    for (auto& node : nodes) {
        ed::BeginNode(node.id);
        ImGui::Text("%s", node.name.c_str());

        ImGui::Dummy(ImVec2(0, 20));

        ImGui::Text("Inputs");
        for (auto pinId : node.inputPins) {
            ed::BeginPin(pinId, ed::PinKind::Input);
            ImGui::Text("-> Input");
            ed::EndPin();
        }

        ImGui::Dummy(ImVec2(0, 20));

        ImGui::Text("Outputs");
        for (auto pinId : node.outputPins) {
            ed::BeginPin(pinId, ed::PinKind::Output);
            ImGui::Text("Output ->");
            ed::EndPin();
        }

        ed::EndNode();
    }

    for (auto& link : links) {
        ed::Link(link.id, link.startPinId, link.endPinId);
    }
}

int main() {

    //const int WIDTH = 800;
    //const int HEIGHT = 800;
    const int WIDTH = floor(static_cast<int>(0.8 * sf::VideoMode::getDesktopMode().width) / 100) * 100;
    const int HEIGHT = floor(static_cast<int>(0.8 * sf::VideoMode::getDesktopMode().height) / 100) * 100;

    // float bounds = 1.0;
    // noise::utils::NoiseMap heightMap = generateHeightMap(WIDTH, HEIGHT, bounds);

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "PerlinTerrain");
    window.setVerticalSyncEnabled(true);
    auto _ = ImGui::SFML::Init(window);
    ImGuiIO& io = ImGui::GetIO();
    //io.FontAllowUserScaling = true;
    
    float textScale = (sf::VideoMode::getDesktopMode().width < 1350) ? 1.4 : 1.8;

    std::vector<Year> years;

    bool appearancePopup = false;

    std::string path = "../../../data/data.json";
    if (std::filesystem::exists(path)) {
        years = loadFromJson(path);
    }

    for (auto& y : years) {
        y.calculatePercentages();
    }
    
    sf::Clock refreshClock;
    refreshClock.restart();

    ed::EditorContext* context = ed::CreateEditor();
    ed::SetCurrentEditor(context);

    InitializeNodes();

    sf::Clock deltaClock;
    while (window.isOpen())
    {
        sf::Event event;
        ImGui::SFML::ProcessEvent(window, event);
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        ImGui::SFML::Update(window, deltaClock.restart());
        window.clear(sf::Color::Black);


        // Create a menu bar at the top of the screen
        if (ImGui::BeginMainMenuBar()) {
            // Set font scale for menu bar
            ImGui::SetWindowFontScale(textScale);
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Create")) {
                    // Handle Create action
                }
                if (ImGui::MenuItem("Open", "Ctrl+O")) {
                    // Handle Open action
                }
                if (ImGui::MenuItem("Save", "Ctrl+S")) {
                    // Handle Save action
                }
                if (ImGui::MenuItem("Save as..")) {
                    // Handle Save as action
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Settings")) {
                if (ImGui::MenuItem("Appearance")) {
                    // Open the appearance popup
                    appearancePopup = true;
                }
                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }

        // Get the height of the main menu bar
        float menuBarHeight = ImGui::GetFrameHeightWithSpacing() * textScale;

        //
        // Main Screen
        ImGui::Begin("Frame", nullptr, ImGuiWindowFlags_NoDecoration);
        auto windowSize = window.getSize();

        // Make window fill screen but positioned below the menu bar
        ImGui::SetWindowPos(ImVec2(0.0f, menuBarHeight));
        ImGui::SetWindowSize(ImVec2(windowSize.x / 2, windowSize.y - menuBarHeight));

        // Apply font scaling for the content
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
        ImGui::GetIO().Fonts->Fonts[0]->Scale = textScale;

        // Create tabs for years
        if (years.size()) {
            ImGuiTabBarFlags tabBarFlags = ImGuiTabBarFlags_None;
            if (ImGui::BeginTabBar("Years Tab Bar", tabBarFlags)) {
                ImGuiTabItemFlags tabItemFlags = ImGuiTabItemFlags_NoAssumedClosure | ImGuiTabItemFlags_NoReorder;
                for (auto& year : years) {
                    if (year.tabOpen && ImGui::BeginTabItem(year.year.c_str(), &year.tabOpen, tabItemFlags)) {
                        // Set Number Of Columns, for each term
                        ImGui::Columns(year.terms.size());
                        //year.display();

                        for (auto& t : year.terms) {
                            t.display();
                            t.fontScale = &textScale;
                            ImGui::NextColumn();
                        }

                        ImGui::EndTabItem();
                    }
                }
                ImGui::EndTabBar();
            }
        }
        ImGui::PopFont();

        ImGui::End();

        //
        // Popup code
        if (appearancePopup && ImGui::Begin("AppearanceSettings", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
            ImGui::GetIO().Fonts->Fonts[0]->Scale = textScale;

            ImGui::Text("Appearance Settings");
            ImGui::Separator();

            ImGui::InputFloat("Text Scale", &textScale, 0.1f, 0.25f, "%.3f");

            if (!ImGui::IsWindowFocused()) {
                appearancePopup = false;
            }

            if (ImGui::Button("Close")) {
                appearancePopup = false;
            }
            ImGui::PopFont();
            ImGui::End();
        }


        //ImGui::Begin("Tree Diagram");

        //// Make window fill screen but positioned below the menu bar
        //ImGui::SetWindowPos(ImVec2(windowSize.x / 2, menuBarHeight));
        //ImGui::SetWindowSize(ImVec2(windowSize.x / 2, windowSize.y - menuBarHeight));

        //ed::NavigateToContent();

        //

        //DrawNodes();

        //ImGui::End();

        // ImGui Node Editor window
        ImGui::Begin("Node Editor");

        // Call zoom to fit function before drawing nodes
        ZoomToFitNodes();

        // Begin ImGui Node Editor content
        ed::Begin("My Node Editor");

        //// Make window fill screen but positioned below the menu bar
        ImGui::SetWindowPos(ImVec2(windowSize.x / 2, menuBarHeight));
        ImGui::SetWindowSize(ImVec2(windowSize.x / 2, windowSize.y - menuBarHeight));

        // Draw nodes and links
        DrawNodes();

        ed::NavigateToContent();

        ed::End();
        // End ImGui Node Editor content

        ImGui::End();


        //ImGui::ShowDemoWindow();


        //End of Main Screen
        //
        //

        ImGui::SFML::Render(window);
        window.display();
    }
    ed::DestroyEditor(context);
    ImGui::SFML::Shutdown();
}