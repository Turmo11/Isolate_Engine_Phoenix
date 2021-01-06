#include "Application.h"
#include "M_Editor.h"
#include "E_Toolbar.h"

E_Toolbar::E_Toolbar() : EditorPanel("Toolbar")
{

}

E_Toolbar::~E_Toolbar()
{

}

bool E_Toolbar::Draw(ImGuiIO& io)
{
	bool ret = true;

	ImGui::Begin("Toolbar");

	ShowTimeState(); // Show play, stop buttons

	ImGui::SameLine();

	ShowTimeDisplay(); // Show timer display

	ImGui::End();

	return ret;
}

bool E_Toolbar::CleanUp()
{
	bool ret = true;



	return ret;
}

void E_Toolbar::ShowTimeState()
{
	if (ImGui::Button("Play"))
	{
		App->editor->SaveSceneThroughEditor("PlayAutosave");
		App->play = true;
	}

	ImGui::SameLine();

	if (ImGui::Button("Stop"))
	{
		App->editor->LoadFileThroughEditor("Assets/Scenes/PlayAutosave.json");
		App->play = false;
	}
}

void E_Toolbar::ShowTimeDisplay()
{
	ImGui::Text("Real Time: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Put Engine Clock Here.");
	ImGui::SameLine();
	ImGui::Text("Game Time: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Put Game Clock Here.");
}