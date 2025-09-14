#pragma once

#define REPOSITORY_VERSION QString("https://mods.minwix.net/")

#include <functional>

class Application;
class MainWindow;
class Task;

void onimaiLoadMainWidget(MainWindow* mainWindow);
void onimaiLoadLauncher(Application* application);