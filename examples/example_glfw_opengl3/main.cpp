// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include "imgui.h"
#include "imgui_amesgames.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <stdio.h>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <string.h>

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int argc, char** argv)
{
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    ImFont* fonts[11];
    const int font_count = IM_ARRAYSIZE(fonts);
    assert(font_count & 2); // expect an odd number of fonts
    const int middle = font_count / 2;
    const float middle_size = ImGui::Amesgames::GetDefaultFontSize();

    // Load middle font using default size
    fonts[middle] = ImGui::Amesgames::LoadFont(middle_size);

    // Create an equal number of font sizes above and below the middle size, in increments of 2px.
    for (int i = 1; i <= middle; i++)
    {
        const float step_size = i * 2.0f;
        fonts[middle - i] = ImGui::Amesgames::LoadFont(middle_size - step_size);
        fonts[middle + i] = ImGui::Amesgames::LoadFont(middle_size + step_size);
    }

    io.FontDefault = fonts[middle];
    int font_idx = middle;

    ImGui::Amesgames::SetupStyle();

    // Load TTF Fonts from the command-line and set up source file generation data.
    if (argc > 1)
    {
        enum class CmdLineState { NONE, SOURCE_NAMESPACE, SOURCE_PATH, FONT_PATH, FONT_SIZE_PIXELS, ICON_PATH, ICON_RANGES };
        CmdLineState state = CmdLineState::NONE;
        bool namespaceOnce = false;
        bool sourcePathOnce = false;
        char fontPath[1024] = "";
        float fontSizePixels;
        ImFont* lastFont = NULL;
        char iconPath[1024] = "";
        ImVector<ImWchar> icon_ranges;
        for (int i = 1; i < argc; i++) {
            switch (state)
            {
                case CmdLineState::NONE:
                {
                    if(0 == strcmp(argv[i], "--source-namespace") || 0 == strcmp(argv[i], "-n"))
                    {
                        if (namespaceOnce)
                            fprintf(stderr, "Source namespace argument (--source-namespace, -n) is expected only once; overridding with the next occurrence\n");
                        namespaceOnce = true;
                        state = CmdLineState::SOURCE_NAMESPACE;
                    }
                    else if (0 == strcmp(argv[i], "--source-path") || 0 == strcmp(argv[i], "-p"))
                    {
                        if (sourcePathOnce)
                            fprintf(stderr, "Source path argument (--source-path, -s) is expected only once; overridding with the next occurrence\n");
                        sourcePathOnce = true;
                        state = CmdLineState::SOURCE_PATH;
                    }
                    else if (0 == strcmp(argv[i], "--font") || 0 == strcmp(argv[i], "-f"))
                        state = CmdLineState::FONT_PATH;
                    else if (0 == strcmp(argv[i], "--icon") || 0 == strcmp(argv[i], "-i"))
                        state = CmdLineState::ICON_PATH;
                    else if (0 == strcmp(argv[i], "--help") || 0 == strcmp(argv[i], "-h"))
                    {
                        fprintf(stderr, "\n");
                        fprintf(stderr, "Usage: %s [options]\n", argv[0]);
                        fprintf(stderr, "\n");
                        fprintf(stderr, "Example:\n");
                        fprintf(stderr, "\t%s -n TestName -p imgui_testpath -f misc/fonts/Roboto-Medium.ttf 16\n", argv[0]);
                        fprintf(stderr, "\n");
                        fprintf(stderr, "options:\n");
                        fprintf(stderr, "--source-namespace, -n <namespace>:\tThe C++ source code namespace for the font and style config.\n");
                        fprintf(stderr, "--source-path, -p <path>:\t\tThe C++ source code file path for the font and style config.A pair of paths is created with extensions .h and .cpp.\n");
                        fprintf(stderr, "--font, -f <path> <pixel size>:\t\tThe TTF file path for a font to be loaded, plus the pixel size it should be configured and rendered at.\n");
                        fprintf(stderr, "--icon, -i <path>:\t\tAn optional TTF file path for an icon font to be loaded and merged with the previously specified base font. Must be specified after each font where it is to be rendered.\n");
                        fprintf(stderr, "\n");
                        return 0;
                    }
                    else
                        fprintf(stderr, "Unknown argument \"%s\".\n", argv[i]);
                }
                break;

                case CmdLineState::SOURCE_NAMESPACE:
                {
                    fprintf(stderr, "Using \"%s\" for source namespace.\n", argv[i]);
                    ImGui::MemFree(io.srcNamespace);
                    io.srcNamespace = (char*)ImGui::MemAlloc(strlen(argv[i]) + 1);
                    strncpy(io.srcNamespace, argv[i], strlen(argv[i]) + 1);
                    state = CmdLineState::NONE;
                }
                break;

                case CmdLineState::SOURCE_PATH:
                {
                    fprintf(stderr, "Using \"%s\" for source path.\n", argv[i]);
                    ImGui::MemFree(io.srcPath);
                    io.srcPath = (char*)ImGui::MemAlloc(strlen(argv[i]) + 1);
                    strncpy(io.srcPath, argv[i], strlen(argv[i]) + 1);
                    state = CmdLineState::NONE;
                }
                break;

                case CmdLineState::FONT_PATH:
                {
                    strncpy(fontPath, argv[i], sizeof(fontPath) - 1);
                    state = CmdLineState::FONT_SIZE_PIXELS;
                }
                break;

                case CmdLineState::FONT_SIZE_PIXELS:
                {
                    fontSizePixels = (float)std::atof(argv[i]);
                    if (std::isnan(fontSizePixels) || std::isinf(fontSizePixels) || fontSizePixels < 1.0f)
                    {
                        fprintf(stderr, "Invalid font size in pixels \"%f\" for \"%s\". Using default value.\n", fontSizePixels, fontPath);
                        fontSizePixels = 13.0f;
                    }
                    fprintf(stderr, "Attempting to load Font from TTF file \"%s\" with size in pixels \"%f\"\n", fontPath, fontSizePixels);
                    lastFont = io.Fonts->AddFontFromFileTTF(fontPath, fontSizePixels);
                    state = CmdLineState::NONE;
                }
                break;

                case CmdLineState::ICON_PATH:
                {
                    if(!lastFont) {
                        fprintf(stderr, "Attempting to load an icon Font before any base font was loaded, aborting \"%s\"\n", argv[i]);
                        return -1;
                    }
                    strncpy(iconPath, argv[i], sizeof(iconPath) - 1);
                    state = CmdLineState::ICON_RANGES;
                }
                break;

                case CmdLineState::ICON_RANGES:
                {
                    icon_ranges.clear();
                    while(i < argc && argv[i][0] != '-')
                        icon_ranges.push_back((unsigned short)strtol(argv[i++], NULL, 0));
                    if(0 != icon_ranges.size() % 2) {
                        fprintf(stderr, "Glyph ranges has an odd number of values, they must come in pairs. Skipping last value. \"%04x\"\n", icon_ranges.back());
                        icon_ranges.back() = 0;
                    }
                    else
                        icon_ranges.push_back(0);
                    fprintf(stderr, "Attempting to load and merge icon Font from TTF file \"%s\"\n", iconPath);
                    ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
                    io.Fonts->AddFontFromFileTTF(iconPath, fontSizePixels, &icons_config, icon_ranges.Data);
                    state = CmdLineState::NONE;
                }
                break;
            }
        }
    }

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl)) {
            font_idx += (int)io.MouseWheel;
            if (font_idx < 0)
                font_idx = 0;
            else if (font_idx >= IM_ARRAYSIZE(fonts))
                font_idx = IM_ARRAYSIZE(fonts) - 1;
        }

        ImGui::PushFont(fonts[font_idx]);

        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        ImGui::PopFont();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    	
        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
