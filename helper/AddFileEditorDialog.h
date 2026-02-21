#pragma once
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include "CustomNodeEditor.h"

class AddFileEditorDialog : public QDialog {
    Q_OBJECT
public:
    AddFileEditorDialog(const QString& customPath, QWidget* parent = nullptr)
        : QDialog(parent), m_customPath(customPath)
    {
        setWindowTitle("Create New Text File");
        resize(800, 600);

        QVBoxLayout* mainLayout = new QVBoxLayout(this);

        // ----- Top: file name -----
        nameEdit = new QLineEdit(this);
        nameEdit->setPlaceholderText("Enter file name (without extension)");
        mainLayout->addWidget(nameEdit);

        // ----- Middle: your existing editor -----
        editor = new CustomNodeEditor(code, this); // still uses your CustomNodeEditor
        mainLayout->addWidget(editor);

        // ----- Bottom: buttons -----
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
        if (name.isEmpty()) return;

        if (!name.endsWith(".txt", Qt::CaseInsensitive))
            name += ".txt";

        QString fullPath = m_customPath + "/" + name;

        QFile file(fullPath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::critical(this, "Error", "Failed to create file.");
            return;
        }

        QTextStream out(&file);
        out << code; // updated live by CustomNodeEditor
        file.close();

        accept(); // closes dialog
    }

public:
    QString getFileName() const { return nameEdit->text().trimmed(); }
    QString getFileContent() const { return code; }

private:
    QLineEdit* nameEdit;
    QPushButton* confirmButton;
    QPushButton* cancelButton;

    CustomNodeEditor* editor; // your existing editor
    QString code;             // linked to editor
    QString m_customPath;
};