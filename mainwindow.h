#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QShortcut>
#include <QList>
#include "Wire.h"
#include "CanvasNode.h"
#include "ui_mainwindow.h"
#include "PythonHighlighter.h"
#include "GraphicsViewCanvas.h"
#include "ProjectManager.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(const QString &projectName,
                        const QString &projectPath,
                        QWidget *parent = nullptr);

    void loadModelFile(const QString &filePath);
    void appendConsole(const QString &text);
    void reloadModelFile();
    void savePyViewToModel();
    void loadToolbox(const QString& path, QTreeWidgetItem* parentItem);

private:
    Ui::MainWindow ui;
    QString currentProjectPath;
    PythonHighlighter *highlighter = nullptr;

    // Canvas
    GraphicsViewCanvas *canvas = nullptr;

    double canvasZoom = 1.0;
    int pyZoom = 0;

    void disableParentDragging(QTreeWidgetItem* item);
    void loadViewSettings();
    void saveViewSettings();
    void zoomIn();
    void zoomOut();
    QList<CanvasNode*> collectExecutionOrder();

    ProjectManager* projectManager = nullptr;
};

#endif // MAINWINDOW_H
