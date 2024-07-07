#pragma once

// Define OpenGL loader: GLEW, GLAD, etc.
#define GL_SILENCE_DEPRECATION
#include <glad/glad.h>

// glfw for window
#include <GLFW/glfw3.h>
// imgui for gui widgets
#include <imgui.h>

// imgui implementations for glfw and opengl
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <iostream>

#include "WindowIcon.h"


class Window {
public:
    GLFWwindow* window = nullptr;

    static void init_glfw() {
        // glfw window creation and imgui initialisation
        // Initialize GLFW
        if (!glfwInit()) {
            glfwTerminate();
            return;
        }
    }

    Window(const std::string window_title, int width, int height) {
        // Create a windowed mode window and its OpenGL context
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        //constexpr size_t WIDTH = 1920;
        //constexpr size_t HEIGHT = 1080;

        // Create window
        window = glfwCreateWindow(width, height, window_title.c_str(), nullptr, nullptr);
        if (!window) {
            glfwTerminate();
            std::cerr << "UNABLE TO CREATE WINDOW\n";
            return;
        }

        glfwSetWindowSizeLimits(window, 900, 500, GLFW_DONT_CARE, GLFW_DONT_CARE);

        // Make the window's context current
        glfwMakeContextCurrent(window);
        glfwSwapInterval(1); // Enable vsync

        // Initialize OpenGL loader (glad)
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cerr << "UNABLE TO LOAD OPENGL\n";
            return;
        }

        // Initialize ImGui
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsLight();

        // Initialize ImGui backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");

        glfwSetFramebufferSizeCallback(window, (GLFWframebuffersizefun)framebuffer_resize_callback);

        // Create a GLFWimage and set the icon
        GLFWimage icon;
        icon.width = icon_width;
        icon.height = icon_height;
        icon.pixels = icon_data;

        glfwSetWindowIcon(window, 1, &icon);
    }

    ~Window() {
        // Cleanup imgui
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(window);
        glfwTerminate();
    }

    bool is_open() {
        return !glfwWindowShouldClose(window);
    }

    void poll_events() {
        // Poll and handle events
        glfwPollEvents();
    }

    template <typename Function>
    void set_key_callback(Function func) {
        // Handle key press
        glfwSetKeyCallback(window, func);
    }

    std::pair<int, int> get_window_size() {
        // Get window size
        glfwGetFramebufferSize(window, &window_width, &window_height);
        return { window_width, window_height };
    }

    void begin_frame() {
        // Start the ImGui frame
        ImGui_ImplOpenGL3_NewFrame();

        ImGui_ImplGlfw_NewFrame();

        ImGui::NewFrame();
    }

    void end_frame() {
        // Render ImGui
        ImGui::Render();
        glViewport(0, 0, window_width, window_height);

        //Set window background colour
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap frame buffers
        glfwSwapBuffers(window);
    }

private:
    int window_width, window_height;

    static void framebuffer_resize_callback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    }
};