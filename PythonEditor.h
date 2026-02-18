#pragma once
#include <QPlainTextEdit>

class PythonEditor : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit PythonEditor(QWidget* parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent* event) override;

private:
    QString indentationForNewLine();
};
