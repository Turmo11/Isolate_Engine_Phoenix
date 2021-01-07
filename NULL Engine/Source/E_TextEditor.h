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

public:
	TextEditor txt_editor;
	std::string text_editor_file;
};

#endif // !__E_TEXTEDITOR_H__
