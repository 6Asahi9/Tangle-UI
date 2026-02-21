#include "MenuManager.h"
#include <QMessageBox>
#include "StartupWindow.h"
#include "MainWindow.h"
#include <QDir>
#include <QInputDialog>
#include "helper/AddFileEditorDialog.h"
#include <QVBoxLayout>

MenuManager::MenuManager(QMenuBar* menubar, QObject* parent)
    : QObject(parent), m_menubar(menubar)
{
    toolboxPath = QDir::currentPath() + "/toolbox"; 
    customPath  = toolboxPath + "/custom";

    QDir dir(customPath);
    if (!dir.exists()) dir.mkpath(customPath); 
}

void MenuManager::setupMenus()
{
    setupFileMenu();
    setupEditMenu();
    setupViewMenu();
    setupSettingsMenu();
    setupAboutMenu();
}

// FILE  ----------------------------------
void MenuManager::setupFileMenu()
{
    QMenu* fileMenu = m_menubar->findChild<QMenu*>("menuFile");
    if (!fileMenu) return;

    QAction* newAction = new QAction("New Project", this);
    QAction* addFileAction = new QAction("Add File", this);
    QAction* saveAction = new QAction("Save", this);
    QAction* deleteFileAction = new QAction("Delete File", this);
    QAction* refreshAction = new QAction("Refresh Toolbox", this);
    QAction* exitAction = new QAction("Exit", this);

    fileMenu->addAction(newAction);
    fileMenu->addAction(addFileAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(deleteFileAction);
    fileMenu->addAction(refreshAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    connect(newAction, &QAction::triggered, this, &MenuManager::onNewProject);
    connect(addFileAction, &QAction::triggered, this, &MenuManager::AddFile);
    connect(saveAction, &QAction::triggered, this, &MenuManager::onSave);
    connect(deleteFileAction, &QAction::triggered, this, &MenuManager::DeleteFile);
    connect(refreshAction, &QAction::triggered, this, &MenuManager::RefreshToolbox);
    connect(exitAction, &QAction::triggered, this, &MenuManager::onExit);
}
// ---------------------------------------------

void MenuManager::onNewProject(){
    StartupWindow* start = new StartupWindow();
    start->setAttribute(Qt::WA_DeleteOnClose);
    QObject::connect(start, &StartupWindow::projectCreated,
                     [=](const QString &name, const QString &path) {
        MainWindow* mainWin = new MainWindow(name, path);
        mainWin->show();
        start->close();
    });

    start->show();
}

void MenuManager::AddFile() {
    AddFileEditorDialog dlg(customPath);
    if (dlg.exec() == QDialog::Accepted) {
        RefreshToolbox();
    }
}

void MenuManager::onSave(){
    emit saveTriggered();
    if (parent())
    {
        QMetaObject::invokeMethod(parent(), "savePyViewToModel");
        QMetaObject::invokeMethod(parent(), "reloadModelFile");
    }
}

void MenuManager::DeleteFile()
{
    QDir dir(customPath);
    QStringList txtFiles = dir.entryList(QStringList() << "*.txt", QDir::Files);
    if (txtFiles.isEmpty()) {
        QMessageBox::information(nullptr, "Delete File", "No files to delete in toolbox/custom.");
        return;
    }
    bool ok;
    QString fileToDelete = QInputDialog::getItem(
        nullptr,
        "Delete Text File",
        "Select file to delete:",
        txtFiles,
        0,
        false,
        &ok
    );
    if (!ok || fileToDelete.isEmpty()) return;
    QString fullPath = customPath + "/" + fileToDelete;
    QMessageBox::StandardButton reply = QMessageBox::question(
        nullptr,
        "Confirm Delete",
        "Are you sure you want to delete " + fileToDelete + "?",
        QMessageBox::Yes | QMessageBox::No
    );
    if (reply != QMessageBox::Yes) return;
    QFile::remove(fullPath);
    RefreshToolbox();
}

void MenuManager::RefreshToolbox()
{
    MainWindow* mw = qobject_cast<MainWindow*>(parent());
    if (!mw) return;

    QTreeWidget* tree = mw->findChild<QTreeWidget*>("treeWidget");
    if (!tree) return;

    tree->clear(); 
    mw->loadToolbox(toolboxPath, nullptr);
}

void MenuManager::onExit(){
    qApp->quit();
}
// EDIT -------------------------------
void MenuManager::setupEditMenu()
{
    QMenu* editMenu = m_menubar->findChild<QMenu*>("menuEdit");
    if (!editMenu) return;

    QAction* undo = new QAction("Undo", this);
    QAction* redo = new QAction("Redo", this);
    QAction* cut  = new QAction("Cut", this);
    QAction* copy = new QAction("Copy", this);
    QAction* paste= new QAction("Paste", this);

    editMenu->addAction(undo);
    editMenu->addAction(redo);
    editMenu->addSeparator();
    editMenu->addAction(cut);
    editMenu->addAction(copy);
    editMenu->addAction(paste);

    connect(undo, &QAction::triggered, this, &MenuManager::onUndo);
    connect(redo, &QAction::triggered, this, &MenuManager::onRedo);
    connect(cut, &QAction::triggered, this, &MenuManager::onCut);
    connect(copy, &QAction::triggered, this, &MenuManager::onCopy);
    connect(paste, &QAction::triggered, this, &MenuManager::onPaste);
}
// ---------------------------------------------

void MenuManager::onUndo() { emit undoTriggered(); }
void MenuManager::onRedo() { emit redoTriggered(); }
void MenuManager::onCut()  { emit cutTriggered(); }
void MenuManager::onCopy() { emit copyTriggered(); }
void MenuManager::onPaste(){ emit pasteTriggered(); }

// VIEW -------------------------------------
void MenuManager::setupViewMenu()
{
    QMenu* viewMenu = m_menubar->findChild<QMenu*>("menuView");
    if (!viewMenu) return;

    QAction* fullscreen = new QAction("Fullscreen", this);
    QAction* toolbar    = new QAction("Toggle Toolbar", this);

    viewMenu->addAction(fullscreen);
    viewMenu->addAction(toolbar);

    connect(fullscreen, &QAction::triggered, this, &MenuManager::onFullscreen);
    connect(toolbar, &QAction::triggered, this, &MenuManager::onToggleToolbar);
}
// ---------------------------------------------

void MenuManager::onFullscreen()      { emit fullscreenTriggered(); }
void MenuManager::onToggleToolbar()   { emit toggleToolbarTriggered(); }

// SETTINGS  ------------------------
void MenuManager::setupSettingsMenu()
{
    QMenu* settingsMenu = m_menubar->findChild<QMenu*>("menuSettings");
    if (!settingsMenu) return;

    QAction* preferences = new QAction("Preferences", this);
    QAction* theme       = new QAction("Change Theme", this);

    settingsMenu->addAction(preferences);
    settingsMenu->addAction(theme);

    connect(preferences, &QAction::triggered, this, &MenuManager::onPreferences);
    connect(theme, &QAction::triggered, this, &MenuManager::onChangeTheme);
}
// ---------------------------------------------

void MenuManager::onPreferences()  { emit preferencesTriggered(); }
void MenuManager::onChangeTheme()  { emit changeThemeTriggered(); }

// ABOUT  ------------------------------------------
void MenuManager::setupAboutMenu()
{
    QMenu* aboutMenu = m_menubar->findChild<QMenu*>("menuAbout");
    if (!aboutMenu) return;

    QAction* aboutApp = new QAction("About App", this);
    QAction* aboutQt  = new QAction("About Qt", this);

    aboutMenu->addAction(aboutApp);
    aboutMenu->addAction(aboutQt);

    connect(aboutApp, &QAction::triggered, this, &MenuManager::onAboutApp);
    connect(aboutQt, &QAction::triggered, this, &MenuManager::onAboutQt);
}
// ---------------------------------------------

void MenuManager::onAboutApp() { emit aboutAppTriggered(); }
void MenuManager::onAboutQt()  { emit aboutQtTriggered(); }