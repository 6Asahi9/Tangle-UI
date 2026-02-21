#include "MainWindow.h"
#include <QFile>
#include <QTextStream>
#include <QMenu>
#include <QProcess>
#include <QShortcut>
#include <QKeySequence>
#include <QJsonDocument>
#include <QJsonObject>
#include <QGraphicsItem>
#include <QDir>
#include <QFileInfoList>
#include "CanvasNode.h"
#include <algorithm>
#include <QList>
#include <QMessageBox>
#include "CanvasNode.h"
#include "PythonEditor.h"
#include "ProjectManager.h"
#include "MenuManager.h"
#include "StartupWindow.h"
// ----------------------------------------------------

MainWindow::MainWindow(const QString &projectName,
                       const QString &projectPath,
                       QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    currentProjectPath = projectPath;
    setWindowTitle("TangleML - " + projectName);
    
    ui.plainTextEdit->hide();
    PythonEditor* editor = new PythonEditor(ui.tab_3);
    editor->setGeometry(ui.plainTextEdit->geometry());
    editor->show();

    highlighter = new PythonHighlighter(editor->document());
    ui.plainTextEdit = editor; 

    canvas = new GraphicsViewCanvas(ui.tab_1);
    canvas->setGeometry(ui.graphicsViewCanvas->geometry());
    canvas->show();
    ui.graphicsViewCanvas->hide();

    // menu ------------------------
    MenuManager* menuMgr = new MenuManager(ui.menubar, this);
    menuMgr->setupMenus();
    connect(menuMgr, &MenuManager::saveTriggered, this, [this]() {savePyViewToModel();reloadModelFile();});

    // engine menu ----------------
    QMenu *engineMenu = new QMenu(this);
    engineMenu->addAction("PyTorch");
    engineMenu->addAction("TangleML");
    ui.btnEngine->setMenu(engineMenu);

    connect(engineMenu, &QMenu::triggered, this, [this](QAction *action){
        ui.btnEngine->setText(action->text());
    });

    //  toolbox loading -------------
    ui.treeWidget->clear();

    QString toolboxPath = QDir::currentPath() + "/toolbox";

    if (QDir(toolboxPath).exists())
        loadToolbox(toolboxPath, nullptr);

    ui.treeWidget->expandAll();

    for (int i = 0; i < ui.treeWidget->topLevelItemCount(); ++i)
        disableParentDragging(ui.treeWidget->topLevelItem(i));

    // run button ---------------
    connect(ui.btnRun, &QPushButton::clicked, this, [this]() {

        QString pythonPath = "python";
        QString modelFile = currentProjectPath + "/.tangle/model.py";

        QFile file(modelFile);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;

        QTextStream out(&file);
        out << ui.plainTextEdit->toPlainText();
        file.close();

        ui.txtConsole->clear();
        appendConsole("Running model.py...\n");

        QProcess *process = new QProcess(this);

        connect(process, &QProcess::readyReadStandardOutput, this, [=]() {
            appendConsole(process->readAllStandardOutput());
        });

        connect(process, &QProcess::readyReadStandardError, this, [=]() {
            appendConsole(process->readAllStandardError());
        });

        process->start(pythonPath, QStringList() << modelFile);
    });


    // model loading ---------------------
    loadModelFile(currentProjectPath + "/.tangle/model.py");

    ui.btnRun->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Return));

    new QShortcut(QKeySequence("Ctrl+s"), this, [this]() {
        savePyViewToModel();
        reloadModelFile(); 
    });

    // zoom shortcuts -------------------------
    QShortcut *zoomIn1 = new QShortcut(QKeySequence("Ctrl+="), this);
    QShortcut *zoomIn2 = new QShortcut(QKeySequence("Ctrl++"), this);
    QShortcut *zoomOut = new QShortcut(QKeySequence("Ctrl+-"), this);

    connect(zoomIn1, &QShortcut::activated, this, &MainWindow::zoomIn);
    connect(zoomIn2, &QShortcut::activated, this, &MainWindow::zoomIn);
    connect(zoomOut, &QShortcut::activated, this, &MainWindow::zoomOut);

    // delete selected nodes --------------
    QShortcut* deleteShortcut = new QShortcut(QKeySequence(Qt::Key_Delete), this);

    connect(deleteShortcut, &QShortcut::activated, this, [this]() {
    if (!canvas) return;
    QList<QGraphicsItem*> selected = canvas->scene()->selectedItems();
    for (QGraphicsItem* item : selected)
    {
        CanvasNode* node = dynamic_cast<CanvasNode*>(item);

        if (node)
        {
            if (node->leftConnection)
                node->leftConnection->rightConnection = nullptr;

            if (node->rightConnection)
                node->rightConnection->leftConnection = nullptr;

            if (canvas->pendingConnection == node)
                canvas->pendingConnection = nullptr;
        }

        canvas->scene()->removeItem(item);
        delete item;
    }

    canvas->scene()->update();   
});
    loadViewSettings();
    
    QString projectsJson = QCoreApplication::applicationDirPath() + "/projects.json";
    projectManager = new ProjectManager(ui.treeWidget_2, projectsJson, this);
    if (!projectName.isEmpty() && !currentProjectPath.isEmpty()) {
        projectManager->addProject(projectName, currentProjectPath);
    }
}
// ----------------------------------------------------
void MainWindow::zoomIn()
{
    int tab = ui.tabWidget->currentIndex();

    if (tab == 0 && canvas) {
        canvasZoom *= 1.1;
        QPointF center(canvas->viewport()->width()/2.0,
                       canvas->viewport()->height()/2.0);
        canvas->zoom(1.1, center);
    }
    else if (tab == 2) {
        pyZoom++;
        QFont f = ui.plainTextEdit->font();
        f.setPointSize(f.pointSize() + 1);
        ui.plainTextEdit->setFont(f);
    }

    saveViewSettings();
}

void MainWindow::zoomOut()
{
    int tab = ui.tabWidget->currentIndex();

    if (tab == 0 && canvas) {
        canvasZoom /= 1.1;
        QPointF center(canvas->viewport()->width()/2.0,
                       canvas->viewport()->height()/2.0);
        canvas->zoom(1.0/1.1, center);
    }
    else if (tab == 2) {
        pyZoom--;
        QFont f = ui.plainTextEdit->font();
        f.setPointSize(f.pointSize() - 1);
        ui.plainTextEdit->setFont(f);
    }

    saveViewSettings();
}

// ----------------------------------------------------

void MainWindow::loadViewSettings()
{
    QFile file(currentProjectPath + "/.tangle/view.json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonObject obj = doc.object();
    file.close();

    canvasZoom = obj.value("canvasZoom").toDouble(1.0);
    pyZoom = obj.value("pyZoom").toInt(0);

    if (canvas) {
        QPointF center(canvas->viewport()->width()/2.0,
                       canvas->viewport()->height()/2.0);
        canvas->zoom(canvasZoom, center);
    }

    QFont f = ui.plainTextEdit->font();
    f.setPointSize(f.pointSize() + pyZoom);
    ui.plainTextEdit->setFont(f);
}

void MainWindow::saveViewSettings()
{
    QFile file(currentProjectPath + "/.tangle/view.json");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QJsonObject obj;
    obj["canvasZoom"] = canvasZoom;
    obj["pyZoom"] = pyZoom;

    QJsonDocument doc(obj);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
}

// ----------------------------------------------------

void MainWindow::loadModelFile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QTextStream in(&file);
    ui.plainTextEdit->setPlainText(in.readAll());
    file.close();
}

void MainWindow::appendConsole(const QString &text)
{
    ui.txtConsole->append(text);
}

void MainWindow::reloadModelFile()
{
    loadModelFile(currentProjectPath + "/.tangle/model.py");
}

void MainWindow::savePyViewToModel()
{
    QList<CanvasNode*> nodes;

    for (QGraphicsItem* item : canvas->scene()->items())
        if (CanvasNode* n = dynamic_cast<CanvasNode*>(item))
            nodes.append(n);

    for (CanvasNode* n : nodes)
    {
        if (!n->leftConnection && !n->rightConnection)
        {
            QMessageBox::warning(this,
                "Warning",
                "Some nodes are not connected. Connect wires first.");
            return;
        }
    }

    canvas->saveNodePtr();
    QList<CanvasNode*> order = collectExecutionOrder();

    QFile file(currentProjectPath + "/.tangle/model.py");

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);

    for (CanvasNode* node : order){
        if (node->isCustom){
            out << node->customCode << "\n";
            continue;
        }

        QString path = QDir::currentPath()+ "/../toolbox/" + node->getText();

        QFile snippet(path);
        if (snippet.open(QIODevice::ReadOnly))
        {
            out << snippet.readAll() << "\n";
            snippet.close();
        }
    }

    file.close();
    reloadModelFile();
}

// ----------------------------------------------------

void MainWindow::disableParentDragging(QTreeWidgetItem *item)
{
    if (!item) return;

    if (item->childCount() > 0)
        item->setFlags(item->flags() & ~Qt::ItemIsDragEnabled);

    for (int i = 0; i < item->childCount(); ++i)
        disableParentDragging(item->child(i));
}

// ----------------------------------------------------

void MainWindow::loadToolbox(const QString& path, QTreeWidgetItem* parentItem)
{
    QDir dir(path);

    QFileInfoList entries = dir.entryInfoList(
        QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot,
        QDir::DirsFirst | QDir::Name
    );

    for (const QFileInfo& info : entries)
    {
        // -------- folders --------
        if (info.isDir())
        {
            QTreeWidgetItem* folderItem;

            if (parentItem)
                folderItem = new QTreeWidgetItem(parentItem);
            else
                folderItem = new QTreeWidgetItem(ui.treeWidget);

            folderItem->setText(0, info.fileName());

            loadToolbox(info.absoluteFilePath(), folderItem);
        }

        // -------- .cpp files --------
        else if (info.suffix().toLower() == "txt")
        {
            QString name = info.completeBaseName();

            QTreeWidgetItem* fileItem;

            if (parentItem)
                fileItem = new QTreeWidgetItem(parentItem);
            else
                fileItem = new QTreeWidgetItem(ui.treeWidget);

            QString relativePath =
                QDir(QDir::currentPath() + "/../toolbox")
                .relativeFilePath(info.absoluteFilePath());

            fileItem->setText(0, name);
            fileItem->setData(0, Qt::UserRole, relativePath);

        }

    }
}

QList<CanvasNode*> MainWindow::collectExecutionOrder()
{
    QList<CanvasNode*> nodes;

    for (QGraphicsItem* item : canvas->scene()->items())
        if (CanvasNode* n = dynamic_cast<CanvasNode*>(item))
            nodes.append(n);

    // find start nodes (no parent)
    QList<CanvasNode*> starts;
    for (CanvasNode* n : nodes)
        if (!n->leftConnection)
            starts.append(n);

    // sort chains by Y position
    std::sort(starts.begin(), starts.end(),
              [](CanvasNode* a, CanvasNode* b)
              {
                  return a->scenePos().y() < b->scenePos().y();
              });

    QList<CanvasNode*> result;

    for (CanvasNode* start : starts)
    {
        CanvasNode* current = start;

        while (current)
        {
            result.append(current);
            current = current->rightConnection;
        }
    }

    return result;
}