#ifndef STARTUPWINDOW_H
#define STARTUPWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

class StartupWindow : public QWidget {
    Q_OBJECT

public:
    explicit StartupWindow(QWidget *parent = nullptr);

signals:
    void projectCreated(const QString &name, const QString &path);

private slots:
    void browseFolder();
    void createProject();
    void validateInputs();

private:
    QLineEdit *nameEdit;
    QLineEdit *pathEdit;
    QPushButton *browseButton;
    QPushButton *createButton;
};

#endif //STARTUPWINDOW_H 
