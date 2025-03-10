import Editor.Editor;

QtEditor gEditor;

IEditor& Editor::Get()
{
	return gEditor;
}
