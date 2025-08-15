#pragma once

#define REPOSITORY_VERSION QString("https://git.onimai.ru/ONIMAI-SMP/multimc-pack/raw/branch/main/")

#include <functional>

class Application;
class MainWindow;
class Task;

void onimaiLoadMainWidget(MainWindow* mainWindow);
void onimaiLoadLauncher(Application* application);