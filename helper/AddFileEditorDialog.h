#pragma once
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include "../PythonEditor.h"
#include "../PythonHighlighter.h"

class AddFileEditorDialog : public QDialog {
    Q_OBJECT
public:
    AddFileEditorDialog(const QString& customPath, QWidget* parent = nullptr)
        : QDialog(parent), m_customPath(customPath)
    {
        setWindowTitle("Create New Text File");
        resize(800, 600);

        QVBoxLayout* mainLayout = new QVBoxLayout(this);

        // ---- Top: File name input ----
        nameEdit = new QLineEdit(this);
        nameEdit->setPlaceholderText("Enter file name (without extension)");
        mainLayout->addWidget(nameEdit);

        // ---- Middle: Python editor with highlighter ----
        editor = new PythonEditor(this);
        highlighter = new PythonHighlighter(editor->document());
        mainLayout->addWidget(editor);

        // ---- Bottom: Confirm/Cancel buttons ----
        QHBoxLayout* buttonLayout = new QHBoxLayout();
        buttonLayout->addStretch(); // push buttons to right
        confirmButton = new QPushButton("Confirm", this);
        cancelButton  = new QPushButton("Cancel", this);
        buttonLayout->addWidget(confirmButton);
        buttonLayout->addWidget(cancelButton);
        mainLayout->addLayout(buttonLayout);

        // Connect buttons
        connect(confirmButton, &QPushButton::clicked, this, &AddFileEditorDialog::onConfirm);
        connect(cancelButton, &QPushButton::clicked, this, &AddFileEditorDialog::reject);
    }

private slots:
    void onConfirm() {
        QString name = nameEdit->text().trimmed();
        if (name.isEmpty()) {
            QMessageBox::warning(this, "Warning", "File name cannot be empty!");
            return;
        }

        if (!name.endsWith(".txt", Qt::CaseInsensitive))
            name += ".txt";

        QString fullPath = m_customPath + "/" + name;

        QFile file(fullPath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::critical(this, "Error", "Failed to create file at:\n" + fullPath);
            return;
        }

        QTextStream out(&file);
        out << editor->toPlainText(); // write current editor text
        file.close();

        accept(); // close dialog
    }

private:
    QLineEdit* nameEdit;
    PythonEditor* editor;
    PythonHighlighter* highlighter;
    QPushButton* confirmButton;
    QPushButton* cancelButton;
    QString m_customPath;
};