#include "CustomNodeEditor.h"
#include "../PythonEditor.h"
#include "../PythonHighlighter.h"
#include <QVBoxLayout>

CustomNodeEditor::CustomNodeEditor(QString& codeRef, QWidget* parent)
    : QDialog(parent), code(codeRef)
{
    setWindowTitle("Custom Python Node");
    resize(600, 400);

    QVBoxLayout* layout = new QVBoxLayout(this);

    editor = new PythonEditor(this);
    layout->addWidget(editor);

    highlighter = new PythonHighlighter(editor->document());

    editor->setPlainText(code);

    connect(editor, &QPlainTextEdit::textChanged, this, [this]() {
        code = editor->toPlainText();
    });
}
