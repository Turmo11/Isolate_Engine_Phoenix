
#include "E_TextEditor.h"

E_TextEditor::E_TextEditor() : EditorPanel("Console")
{
}

E_TextEditor::~E_TextEditor()
{

}
void E_TextEditor::InitializeTextEditor()
{
	//TEXT EDITOR Initialized
	TextEditor::LanguageDefinition lang = TextEditor::LanguageDefinition::Wren();

	text_editor_file = "alphascript.txt";

	std::ifstream t(text_editor_file.c_str());
	if (t.good())
	{
		std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
		txt_editor.SetText(str);
	}
	txt_editor.SetPalette(TextEditor::GetDarkPalette());
}

bool E_TextEditor::Draw(ImGuiIO& io)
{
	bool ret = true;

	ImGui::Begin("Text Editor", nullptr, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);

	auto cpos = txt_editor.GetCursorPosition();
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save"))
			{
				std::string textToSave = txt_editor.GetText();

				/*App->fileSystem->Remove("");

				App->fileSystem->Save("", textToSave.c_str(), text_editor.GetText().size());*/
			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "Ctrl+Z", nullptr, txt_editor.CanUndo()))
				txt_editor.Undo();
			if (ImGui::MenuItem("Redo", "Ctrl+Y", nullptr, txt_editor.CanRedo()))
				txt_editor.Redo();

			ImGui::Separator();

			if (ImGui::MenuItem("Copy", "Ctrl+C", nullptr, txt_editor.HasSelection()))
				txt_editor.Copy();
			if (ImGui::MenuItem("Cut", "Ctrl+X", nullptr, txt_editor.HasSelection()))
				txt_editor.Cut();
			if (ImGui::MenuItem("Delete", "Del", nullptr, txt_editor.HasSelection()))
				txt_editor.Delete();
			if (ImGui::MenuItem("Paste", "Ctrl+V", nullptr, ImGui::GetClipboardText() != nullptr))
				txt_editor.Paste();

			ImGui::Separator();

			if (ImGui::MenuItem("Select all", nullptr, nullptr))
				txt_editor.SetSelection(TextEditor::Coordinates(), TextEditor::Coordinates(txt_editor.GetTotalLines(), 0));

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	ImGui::Text("%6d/%-6d Total lines: %d | %s | %s | Cursor: %s                       File name: %s | Language: %s",
		cpos.mLine + 1,
		cpos.mColumn + 1,
		txt_editor.GetTotalLines(),
		txt_editor.CanUndo() ? "<-" : "-",
		txt_editor.CanRedo() ? "->" : "-",
		txt_editor.IsOverwrite() ? "Ovr" : "Ins",
		text_editor_file.c_str(),
		txt_editor.GetLanguageDefinition().mName.c_str());

	txt_editor.Render("TextEditor");
	ImGui::End();

	return ret;
}

bool E_TextEditor::CleanUp()
{
	return true;
}


