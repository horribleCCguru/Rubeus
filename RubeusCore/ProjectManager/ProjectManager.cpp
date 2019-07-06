#include <fstream>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>
#include <experimental/filesystem>
#include <array>
#include <map>
#include <functional>
#include <utility>
#include <cstring>

namespace fs = std::experimental::filesystem;

class ProjectManager {
	GLFWwindow* window;

	std::map<std::string, std::string> GameFiles;

	std::array< std::function<void(void)>, 3> viewFunctions;
	//void (ProjectManager::* viewFunctions[2])();
	bool ProjectWindow = true;
	int CurrentView;
	char RootPath[1024];
	std::string GamePath;
	std::vector<fs::path> ProjectList;
	fs::path CurrentProject;
	std::vector<std::pair<std::string, fs::path>> CurrentProjectObjects;
	std::vector<std::pair<std::string, fs::path>> CurrentProjectLevels;
	bool show_demo_window = true;
	bool show_another_window = false;
public:
	ProjectManager() :CurrentView(1)
	{
		//ImGuiStyle& style = ImGui::GetStyle();


		memset(RootPath, 0, sizeof(RootPath));

		//REMOVE THIS
		strcpy(RootPath, "D:/Rubeus");

		GameFiles.insert({ "UserInitH", R"V0G0N("#pragma once
			// Include Game levels here as
			// #include "game_name/engine_files/level.level_name.h"
		)V0G0N" });

		GameFiles.insert({ "UserInitCpp", R"V0G0N("#include "user_init.h"
#include "../RubeusCore.h"
// Initialise levels here as
// SampleLevel * sample_level = new SampleLevel("sample_level");
// Add startup level as
// std::string startupLevel = "sample_level";
// Initialise game objects as
// SampleObject * sample_object = new SampleObject(
// 	   "sample_object",
// 	   "sample_level",
// 	   6.0f, 3.0f,
// 	   3.0f, 3.0f,
// 	   "Assets/debug.png",
// 	   false
// );
		)V0G0N" });

		GameFiles.insert({ "LevelH", R"V0G0N("#pragma once
#include "../RubeusCore.h"
class %s : public Rubeus::RLevel
{
protected:
// User defined members
// Don't look down
public:
%s(std::string name)
: RLevel(name)
{
}
~%s()
{
}
void begin() override;
void onEnd() override;
};
		)V0G0N" });

		GameFiles.insert({ "LevelCpp", R"V0G0N("#include "%s.h"
void %s::begin()
{
}
void %s::onEnd()
{
}
		)V0G0N" });

		GameFiles.insert({ "ObjectH", R"V0G0N("#pragma once
#include "../RubeusCore.h"
class %s : public Rubeus::RGameObject
{
// User members
// Do not look below
public:
%s(std::string name, std::string levelName, float x, float y, float deltaX, float deltaY, const char * imageFilePath, bool enablePhysics = false, const Rubeus::Awerere::EColliderType & type = Rubeus::Awerere::EColliderType::NO_COLLIDER, Rubeus::Awerere::ACollider * collider = NULL, bool generatesHit = false, const Rubeus::Awerere::APhysicsMaterial & physicsMat = Rubeus::Awerere::APhysicsMaterial())
: RGameObject(name, levelName, x, y, deltaX, deltaY, imageFilePath, enablePhysics, type, collider, generatesHit, physicsMat)
{
}
// TODO: Add default paramters
%s(std::string name, std::string levelName, float x, float y, float deltaX, float deltaY, float & r, float & g, float & b, bool enablePhysics = false, const Rubeus::Awerere::APhysicsMaterial & material = Rubeus::Awerere::APhysicsMaterial(), const Rubeus::Awerere::EColliderType & type = Rubeus::Awerere::EColliderType::NO_COLLIDER, Rubeus::Awerere::ACollider * collider = NULL, bool generatesHit = false)
: RGameObject(name, levelName, x, y, deltaX, deltaY, r, g, b, enablePhysics, material, type, collider, generatesHit)
{
}
void begin() override;
void onHit(RGameObject * hammer, RGameObject * nail, const Rubeus::Awerere::ACollideData & collisionData) override;
void onMessage(Rubeus::Message * msg) override;
void tick() override;
};
		)V0G0N" });

		GameFiles.insert({ "ObjectCpp", R"V0G0N("#include "%s.h"
void %s::begin()
{
}
void %s::onHit(RGameObject * hammer, RGameObject * nail, const Rubeus::Awerere::ACollideData & collisionData)
{
}
void %s::tick()
{
}
void %s::onMessage(Rubeus::Message * msg)
{
}
		)V0G0N" });

		viewFunctions = { { std::bind(&ProjectManager::landingPage, this), std::bind(&ProjectManager::allProjectsPage, this), std::bind(&ProjectManager::currentProjectPage, this) } };
		showWindow();
	}
	~ProjectManager()
	{
		glfwTerminate();
	}
	void initImGuiStyle()
	{
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowPadding = ImVec2(20, 20);
		style.WindowRounding = 0.0f;
		style.WindowBorderSize = 0.0f;
		style.WindowMinSize = ImVec2(100, 200);
		style.WindowTitleAlign = ImVec2(0.2f, 0.5f);
	}

	int showWindow()
	{
		if (!glfwInit())
			return -1;

		/* Create a windowed mode window and its OpenGL context */
		window = glfwCreateWindow(640, 480, "Rubeus Project Manager", NULL, NULL);
		if (!window)
		{
			glfwTerminate();
			return -1;
		}

		/* Make the window's context current */
		glfwMakeContextCurrent(window);

		if (glewInit() != GLEW_OK)
		{
			std::cout << "GLEW initialisation failed";

			return false;
		}

		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 130");
		ImGui::StyleColorsDark();
		bool show_demo_window = true;
		bool show_another_window = false;
		bool show_test_window = true;
		bool show_project_window = false;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
		initImGuiStyle();

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{



			/* Render here */
			glClear(GL_COLOR_BUFFER_BIT);

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

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
			if (ProjectWindow)
			{
				static bool WindowLocationInit = false;
				ImGui::Begin("Rubeus Project Manager", &ProjectWindow, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar);
				ImGui::SetWindowSize(ImVec2((float)640, (float)480));
				if (!WindowLocationInit)
				{
					ImGui::SetWindowPos(ImVec2(0, 0));
					WindowLocationInit = true;
				}
				viewFunctions[CurrentView]();
				ImGui::End();
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();

		}

	}

	//HELPER FUNCTIONS

	void updateProjectList()
	{
		ProjectList.clear();
		for (const auto & entry : fs::directory_iterator(GamePath))
			ProjectList.push_back(entry.path());
	}
	
	void updateEngineFilesLists(std::string& Warning)
	{
		for (const auto& dirEntry : fs::directory_iterator(fs::path(CurrentProject / "engine_files")))
			if (dirEntry.path().extension() == fs::path(".cpp"))
				if (fs::exists((dirEntry.path().parent_path() / dirEntry.path().stem()).string() + ".h"))
				{
					if (dirEntry.path().stem().string().substr(0, 6) == "level.")
						CurrentProjectLevels.emplace_back( dirEntry.path().stem().string().substr(6, dirEntry.path().string().length()-1), dirEntry.path() );
					else if(dirEntry.path().stem().string().substr(0, 7) == "object.")
						CurrentProjectObjects.emplace_back( dirEntry.path().stem().string().substr(7, dirEntry.path().string().length() - 1), dirEntry.path() );
				}
				else
					Warning += dirEntry.path().filename().string() + " ";
	}
	void updateEngineFilesLists()
	{
		CurrentProjectLevels.clear();
		CurrentProjectObjects.clear();
		for (const auto& dirEntry : fs::directory_iterator(fs::path(CurrentProject / "engine_files")))
			if (dirEntry.path().extension() == fs::path(".cpp"))
				if (fs::exists((dirEntry.path().parent_path() / dirEntry.path().stem()).string() + ".h"))
				{
					if (dirEntry.path().stem().string().substr(0, 6) == "level.")
						CurrentProjectLevels.emplace_back(dirEntry.path().stem().string().substr(6, dirEntry.path().string().length() - 1), dirEntry.path());
					else if (dirEntry.path().stem().string().substr(0, 7) == "object.")
						CurrentProjectObjects.emplace_back(dirEntry.path().stem().string().substr(7, dirEntry.path().string().length() - 1), dirEntry.path());
				}
	}
	void displayProjectFiles(std::vector<std::pair<std::string, fs::path>> &test, char* childName)
	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
		ImGui::BeginChild(childName, ImVec2(ImGui::GetWindowContentRegionWidth(), 100), false, window_flags);

		ImVec2 button_sz(180, 80);
		ImGuiStyle& style = ImGui::GetStyle();
		float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

		for (unsigned i = 0; i < test.size(); i++)
		{
			ImGui::PushID(i);
			if (ImGui::Button(test[i].first.c_str(), button_sz))
			{
				CurrentProject = ProjectList[i];
				CurrentView = 2;
			}
			if (ImGui::BeginPopupContextItem())
			{
				if (ImGui::Button("Delete"))
				{
					fs::remove(test[i].second);
					fs::remove((test[i].second.parent_path()/test[i].second.stem()).string()+std::string(".h"));
					updateEngineFilesLists();
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
			float last_button_x2 = ImGui::GetItemRectMax().x;
			float next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x;
			if (i + 1 < test.size() && next_button_x2 < window_visible_x2)
				ImGui::SameLine();
			ImGui::PopID();
		}
		ImGui::EndChild();
	}
	bool createFiles(std::string Type, std::string Name = "")
	{
		std::transform(Name.begin(), Name.end(), Name.begin(), ::tolower);
		std::string Name_h,Name_cpp;
		std::string BasePath = CurrentProject.string();
		if (Type == "Level")
		{
			Name_h = BasePath + "/engine_files/level." + Name + ".h";
			Name_cpp = BasePath + "/engine_files/level." + Name + ".cpp";
		}
		else if (Type == "Object")
		{
			Name_h = BasePath + "/engine_files/object." + Name + ".h";
			Name_cpp = BasePath + "/engine_files/object." + Name + ".cpp";
		}
		else if (Type == "UserInit")
		{
			Name_h = BasePath + "/user_init.h";
			Name_cpp = BasePath + "/user_init.cpp";
		}
		std::ofstream NewFile(Name_h);
		if (NewFile.fail())
			return 1;
		NewFile << GameFiles[Type + "H"];
		NewFile.close();
		NewFile.open(Name_cpp);
		if (NewFile.fail())
		{
			fs::remove(fs::path(Name_h));
			return 1;
		}
		NewFile << GameFiles[Type + "Cpp"];
		updateEngineFilesLists();
		return 0;
	}
	//VIEW FUNCTIONS WHICH ARE BEING STORED IN viewFunctions ARRAY
	void landingPage()
	{
		ImGui::Text("Enter path of root directory of Rubeus");
		ImGui::InputText("##source", RootPath, IM_ARRAYSIZE(RootPath));
		if (ImGui::Button("OK"))
			if (fs::exists(RootPath))
				CurrentView = 1;
		ImGui::Text("exists- %d", fs::exists(RootPath));
	}
	void allProjectsPage()
	{
		if (GamePath.empty())
		{
			GamePath = std::string(RootPath) + std::string("/RubeusCore/Game");
			updateProjectList();
		}
		ImGui::Dummy(ImVec2(0.0f, 15.0f));
		ImGui::SameLine(20.0f);
		ImGui::Text("Your projects-");
		ImGui::Dummy(ImVec2(0.0f, 40.0f));
		ImGui::Separator();
		ImGui::BeginChild("Child1", ImVec2(ImGui::GetWindowContentRegionWidth(), 250), false);


		ImVec2 button_sz(160, 80);
		ImGuiStyle& style = ImGui::GetStyle();
		float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

		for (unsigned i = 0; i < ProjectList.size(); i++)
		{
			ImGui::PushID(i);
			if (ImGui::Button(ProjectList[i].filename().string().c_str(), button_sz))
			{
				CurrentProject = ProjectList[i];
				CurrentView = 2;
			}
			if (ImGui::BeginPopupContextItem())
			{
				if (ImGui::Button("Delete")) {
					fs::remove_all(ProjectList[i]);
					updateProjectList();
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
			float last_button_x2 = ImGui::GetItemRectMax().x;
			float next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x;
			if (i + 1 < ProjectList.size() && next_button_x2 < window_visible_x2)
				ImGui::SameLine();
			ImGui::PopID();
		}
		ImGui::EndChild();
		ImGui::Separator();
		ImGui::Dummy(ImVec2(0.0f, 20.0f));
		if (ImGui::Button("New Project", ImVec2(160, 80)))
			ImGui::OpenPopup("New Project");

		if (ImGui::BeginPopupModal("New Project", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			static char NewProjectName[1024];
			static char ErrorMsg[100];
			ImGui::Text("Enter Name of Project-");
			ImGui::InputText("##source", NewProjectName, IM_ARRAYSIZE(NewProjectName));
			ImGui::Separator();

			if (ErrorMsg[0] != 0)
			{
				ImGui::Text(ErrorMsg);
			}

			if (ImGui::Button("Create", ImVec2(120, 0)))
			{
				CurrentProject = GamePath + std::string("/") + std::string(NewProjectName);
				if (fs::create_directory(CurrentProject))
				{
					updateProjectList();
					fs::create_directory(CurrentProject / fs::path("engine_files"));
					createFiles("UserInit");
					memset(NewProjectName, 0, sizeof(NewProjectName));
					memset(ErrorMsg, 0, sizeof(ErrorMsg));
					ImGui::CloseCurrentPopup();
				}
				else
				{
					strcpy(ErrorMsg, "ERROR: project files creation failed, try changing project name");
				}
			}
			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}
	void currentProjectPage()
	{
		//Not relevant outside this so though it was better to declare these here
		static bool NewObjectError;
		static bool NewLevelError;
		static std::string Warning;
		static bool EngineFilesScanned = false;
		static char NewObjectName[1024];
		static char NewLevelName[1024];
		if (EngineFilesScanned == false) {
			updateEngineFilesLists(Warning);
			EngineFilesScanned = true;
		}
		if (!Warning.empty()) {
			if (ImGui::BeginPopupModal("Warning", NULL, ImGuiWindowFlags_AlwaysAutoResize))
			{

				ImGui::Text("WARNING\n These files don't have corresponding .h files-");
				ImGui::Text(Warning.c_str());
				if (ImGui::Button("OK", ImVec2(120, 0)))
				{
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
		}
		if (ImGui::Button("<-"))
		{
			CurrentView = 1;
			NewObjectError = 0;
			NewLevelError = 0;
			Warning = "";
			EngineFilesScanned = false;
			memset(NewObjectName, 0, sizeof(NewObjectName));
			memset(NewLevelName, 0, sizeof(NewLevelName));
		}
		ImGui::SameLine();
		ImGui::Text("Current Project- ");
		ImGui::SameLine();
		ImGui::Text(CurrentProject.string().c_str());
		ImGui::Dummy(ImVec2(0.0f, 20.0f));
		ImGui::Text("Levels-");
		ImGui::Dummy(ImVec2(0.0f, 5.0f));
		ImGui::Separator();
		ImGui::Dummy(ImVec2(0.0f, 5.0f));
		displayProjectFiles(CurrentProjectLevels, "Child1");
		ImGui::Dummy(ImVec2(0.0f, 5.0f));
		ImGui::InputText("##source1", NewLevelName, IM_ARRAYSIZE(NewLevelName));
		ImGui::SameLine();
		if (ImGui::Button("Create New Level") && NewLevelName[0] != 0)
		{
			NewLevelError = createFiles("Level", NewLevelName);
			if (!NewLevelError)
				memset(NewLevelName, 0, sizeof(NewLevelName));
		}
		if (NewLevelError)
			ImGui::Text("ERROR: level files creation failed, try changing level name");
		ImGui::Dummy(ImVec2(0.0f, 10.0f));
		ImGui::Text("Objects-");
		ImGui::Dummy(ImVec2(0.0f, 5.0f));
		ImGui::Separator();
		ImGui::Dummy(ImVec2(0.0f, 5.0f));
		displayProjectFiles(CurrentProjectObjects, "Child2");
		ImGui::Dummy(ImVec2(0.0f, 5.0f));
		ImGui::InputText("##source2", NewObjectName, IM_ARRAYSIZE(NewObjectName));
		ImGui::SameLine();
		if (ImGui::Button("Create New Object") && NewObjectName[0] != 0)
		{
			NewObjectError = createFiles("Object", NewObjectName);
			if (!NewObjectError)
				memset(NewObjectName, 0, sizeof(NewObjectName));
		}
		if (NewObjectError)
			ImGui::Text("ERROR: Object files creation failed, try changing object name");
	}
};

int main(void)
{
	ProjectManager PM;
	return 0;
}
