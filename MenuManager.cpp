#include "MenuManager.h"
#include <QMessageBox>
#include "StartupWindow.h"
#include "MainWindow.h"

MenuManager::MenuManager(QMenuBar* menubar, QObject* parent)
    : QObject(parent), m_menubar(menubar)
{
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
    QAction* openAction = new QAction("Open File", this);
    QAction* saveAction = new QAction("Save", this);
    QAction* exitAction = new QAction("Exit", this);

    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    connect(newAction, &QAction::triggered, this, &MenuManager::onNewProject);
    connect(openAction, &QAction::triggered, this, &MenuManager::AddFile);
    connect(saveAction, &QAction::triggered, this, &MenuManager::onSave);
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
void MenuManager::AddFile()   { emit AddFileTriggered(); }

void MenuManager::onSave(){
    emit saveTriggered();
    if (parent())
    {
        QMetaObject::invokeMethod(parent(), "savePyViewToModel");
        QMetaObject::invokeMethod(parent(), "reloadModelFile");
    }
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