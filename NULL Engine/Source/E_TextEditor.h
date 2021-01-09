#ifndef __E_TEXTEDITOR_H__
#define __E_TEXTEDITOR_H__

#include "EditorPanel.h"

#include "ImGui/include/TextEditor.h"
#include <fstream>
class TextEditor;

class E_TextEditor : public EditorPanel
{
public:
	E_TextEditor();
	~E_TextEditor();

	bool Draw(ImGuiIO& io) override;
	bool CleanUp() override;

	void InitializeTextEditor();
	void SaveScript();
	bool Editing();

public:

	TextEditor script_editor;
	std::string file_path;
	bool editing;

private:

	SDL_Rect content_region;
};

#endif // !__E_TEXTEDITOR_H__
