
#include "Application.h"
#include "M_Editor.h"
#include "M_FileSystem.h"
#include "M_Scripting.h"

#include "E_TextEditor.h"


E_TextEditor::E_TextEditor() : EditorPanel("Console")
{
	file_path = "Assets/Scripts/Move.wren";
}

E_TextEditor::~E_TextEditor()
{

}
void E_TextEditor::InitializeTextEditor()
{
	//TEXT EDITOR Initialized
	TextEditor::LanguageDefinition lang = TextEditor::LanguageDefinition::Wren();

	std::ifstream t(file_path.c_str());
	if (t.good())
	{
		std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
		script_editor.SetText(str);
	}
	script_editor.SetPalette(TextEditor::GetDarkPalette());
}

bool E_TextEditor::Draw(ImGuiIO& io)
{
	bool ret = true;

	ImGui::Begin("Text Editor", &App->editor->show_text_editor, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);

	SetIsHovered();

	editing = Editing();

	auto cpos = script_editor.GetCursorPosition();
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save", "Ctrl+S"))
			{
				SaveScript();
			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "Ctrl+Z", nullptr, script_editor.CanUndo()))
				script_editor.Undo();
			if (ImGui::MenuItem("Redo", "Ctrl+Y", nullptr, script_editor.CanRedo()))
				script_editor.Redo();

			ImGui::Separator();

			if (ImGui::MenuItem("Copy", "Ctrl+C", nullptr, script_editor.HasSelection()))
				script_editor.Copy();
			if (ImGui::MenuItem("Cut", "Ctrl+X", nullptr, script_editor.HasSelection()))
				script_editor.Cut();
			if (ImGui::MenuItem("Delete", "Del", nullptr, script_editor.HasSelection()))
				script_editor.Delete();
			if (ImGui::MenuItem("Paste", "Ctrl+V", nullptr, ImGui::GetClipboardText() != nullptr))
				script_editor.Paste();

			ImGui::Separator();

			if (ImGui::MenuItem("Select all", nullptr, nullptr))
				script_editor.SetSelection(TextEditor::Coordinates(), TextEditor::Coordinates(script_editor.GetTotalLines(), 0));

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	ImGui::Text("%6d/%-6d Total lines: %d | %s | %s | Cursor: %s                       File name: %s | Language: %s",
		cpos.mLine + 1,
		cpos.mColumn + 1,
		script_editor.GetTotalLines(),
		script_editor.CanUndo() ? "<-" : "-",
		script_editor.CanRedo() ? "->" : "-",
		script_editor.IsOverwrite() ? "Ovr" : "Ins",
		file_path.c_str(),
		script_editor.GetLanguageDefinition().mName.c_str());

	script_editor.Render("TextEditor");
	ImGui::End();

	return ret;
}

bool E_TextEditor::CleanUp()
{
	return true;
}

void E_TextEditor::SaveScript()
{
	LOG("[WREN] Saving %s script", file_path.c_str());
	std::string content = script_editor.GetText();
	App->file_system->SetFileContent(file_path.c_str(), content.c_str());
	App->scripting->LoadScript(file_path.c_str());
}

bool E_TextEditor::Editing()
{
	content_region.x = ImGui::GetWindowPos().x;
	content_region.y = ImGui::GetWindowPos().y;
	content_region.w = ImGui::GetWindowSize().x;
	content_region.h = ImGui::GetWindowSize().y;

	float2 mouse_pos = App->editor->GetWorldMousePositionThroughEditor();

	if (mouse_pos.x > content_region.x && mouse_pos.x < content_region.x + content_region.w 
		&& mouse_pos.y > content_region.y && mouse_pos.y < content_region.y + content_region.h)
	{
		return true;
	}

	return false;
}

void E_TextEditor::OpenScript(std::string script_to_open)
{
	file_path = ASSETS_SCRIPTS_PATH + script_to_open;
	InitializeTextEditor();
	App->scripting->LoadScript(file_path.c_str());
}


