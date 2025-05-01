#include <windows.h>
#include <iostream>
#include <string>

//Pour compiler le fichier (exemple) :
//g++ -o ResChange ResChange.cpp -luser32 -lgdi32
//cl /EHsc ResChange.cpp user32.lib gdi32.lib

//Pour utiliser le .exe généré
//ResChange.exe : Pour ouvrir l'UI
//ResChange.exe 16/9 : Pour modifier la résolution sans ouvrir l'UI
//ResChange.exe 4/3 : Pour modifier la résolution sans ouvrir l'UI

// Déclarations de fonctions
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void ChangeResolution(int width, int height);
void RestoreDefaultResolution();
void CloseApp();

// Identifiants des boutons
#define ID_BUTTON_16_9 1
#define ID_BUTTON_4_3 2
#define ID_BUTTON_CLOSE 3

// Variables globales pour stocker la résolution par défaut
int defaultWidth;
int defaultHeight;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const char CLASS_NAME[] = "Sample Window Class";

    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Obtenir la résolution actuelle (par défaut)
    DEVMODE devMode;
    memset(&devMode, 0, sizeof(devMode));
    devMode.dmSize = sizeof(devMode);
    if (EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devMode)) {
        defaultWidth = devMode.dmPelsWidth;
        defaultHeight = devMode.dmPelsHeight;
    }

    // Analyser les arguments de la ligne de commande
    std::string cmdLine(lpCmdLine);
    if (cmdLine == "16/9") {
        ChangeResolution(1920, 1080);
        return 0;
    } else if (cmdLine == "4/3") {
        ChangeResolution(1024, 768);
        return 0;
    } else if (!cmdLine.empty() && cmdLine != "16/9" && cmdLine != "4/3") {
        std::cerr << "Usage: resolution.exe 'vide ou 16/9 ou 4/3'" << std::endl;
        std::cerr.flush();
        return 0;
    }

    // Si aucun argument n'est fourni, ouvrir l'interface graphique
    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        "Screen Resolution",        // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, 270, 120,

        NULL,       // Parent window
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // Créer les boutons
    CreateWindow(
        "BUTTON",  // Predefined class
        "16/9",     // Button text
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles
        30,         // x position
        30,         // y position
        60,         // Button width
        30,         // Button height
        hwnd,       // Parent window
        (HMENU)ID_BUTTON_16_9, // No menu.
        (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
        NULL);      // Pointer not needed.

    CreateWindow(
        "BUTTON",  // Predefined class
        "4/3",     // Button text
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles
        100,        // x position
        30,         // y position
        60,         // Button width
        30,         // Button height
        hwnd,       // Parent window
        (HMENU)ID_BUTTON_4_3, // No menu.
        (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
        NULL);      // Pointer not needed.

    CreateWindow(
        "BUTTON",  // Predefined class
        "Fermer",  // Button text
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles
        170,        // x position
        30,         // y position
        60,         // Button width
        30,         // Button height
        hwnd,       // Parent window
        (HMENU)ID_BUTTON_CLOSE, // No menu.
        (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
        NULL);      // Pointer not needed.

    // Boucle de messages
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return static_cast<int>(msg.wParam); //0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_COMMAND:
        if (LOWORD(wParam) == ID_BUTTON_16_9) {
            ChangeResolution(1920, 1080);
        } else if (LOWORD(wParam) == ID_BUTTON_4_3) {
            ChangeResolution(1024, 768);
        } else if (LOWORD(wParam) == ID_BUTTON_CLOSE) {
            CloseApp();
        }
        break;
    case WM_CLOSE:
        CloseApp();
        break;
    case WM_DESTROY:
        CloseApp();
        break;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam); //0;
}

void ChangeResolution(int width, int height) {
    DEVMODE devMode;
    memset(&devMode, 0, sizeof(devMode));
    devMode.dmSize = sizeof(devMode);

    // Obtenir les paramètres actuels de l'écran
    if (EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devMode)) {
        // Définir la nouvelle résolution
        devMode.dmPelsWidth = width;
        devMode.dmPelsHeight = height;
        devMode.dmBitsPerPel = devMode.dmBitsPerPel;
        devMode.dmDisplayFrequency = 120;
        
        devMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL | DM_DISPLAYFREQUENCY;

        // Changer la résolution
        LONG result = ChangeDisplaySettings(&devMode, CDS_TEST);
        if (result == DISP_CHANGE_SUCCESSFUL) {
            result = ChangeDisplaySettings(&devMode, CDS_UPDATEREGISTRY);
            if (result == DISP_CHANGE_SUCCESSFUL) {
                std::cerr << "La résolution a été changée avec succés." << std::endl; // << width << "x" << height << "." << std::endl;
            } else {
                std::cerr << "Échec du changement de résolution." << std::endl;
            }
        } else {
            std::cerr << "La résolution n'est pas prise en charge." << std::endl;
        }
    } else {
        std::cerr << "Impossible d'obtenir les paramètres actuels de l'écran." << std::endl;
    }
    std::cerr.flush();
}

void RestoreDefaultResolution() {
    ChangeResolution(defaultWidth, defaultHeight);
}

void CloseApp() {
    RestoreDefaultResolution();
    PostQuitMessage(0);
}
