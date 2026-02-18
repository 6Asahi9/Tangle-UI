#include "PythonEditor.h"
#include <QKeyEvent>
#include <QTextCursor>

PythonEditor::PythonEditor(QWidget* parent)
    : QPlainTextEdit(parent){}

QString PythonEditor::indentationForNewLine()
{
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::Up);
    cursor.movePosition(QTextCursor::StartOfLine);
    cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);

    QString prevLine = cursor.selectedText();
    int spaces = 0;
    while (spaces < prevLine.size() && prevLine[spaces] == ' ')
        spaces++;

    QString indent(spaces, ' ');
    if (prevLine.trimmed().endsWith(":"))
        indent += "    ";

    return indent;
}

void PythonEditor::keyPressEvent(QKeyEvent* event)
{
    QString t = event->text();
    if (t == "(") { insertPlainText("()"); moveCursor(QTextCursor::Left); return; }
    if (t == "{") { insertPlainText("{}"); moveCursor(QTextCursor::Left); return; }
    if (t == "[") { insertPlainText("[]"); moveCursor(QTextCursor::Left); return; }

    if (event->key() == Qt::Key_Return)
    {
        QPlainTextEdit::keyPressEvent(event);
        insertPlainText(indentationForNewLine());
        return;
    }
    QPlainTextEdit::keyPressEvent(event);
}

