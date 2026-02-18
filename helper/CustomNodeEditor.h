#pragma once
#include <QDialog>

class PythonEditor;
class PythonHighlighter;

class CustomNodeEditor : public QDialog
{
    Q_OBJECT
public:
    explicit CustomNodeEditor(QString& codeRef, QWidget* parent = nullptr);

private:
    PythonEditor* editor;
    PythonHighlighter* highlighter;
    QString& code; 
};
