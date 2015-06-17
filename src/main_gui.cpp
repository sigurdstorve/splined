#include <stdexcept>
#include <iostream>
#include <QDesktopWidget>
#include <QApplication>
#include <QObject>
#include "SplineEditorApp.hpp"

void startGuiApplication(int argc, char** argv) {
    try {
        QLocale::setDefault(QLocale::c()); // always use dot as decimal separator

        QApplication app(argc, argv);
        SplineEditorApp mainWin;
        
        // Auto-size and center
        QDesktopWidget* desktop = QApplication::desktop();
        int screenWidth = desktop->width();
        int screenHeight = desktop->height();
        std::cout << "Desktop width: " << screenWidth << std::endl;
        std::cout << "Desktop height: " << screenHeight << std::endl;
        
        mainWin.show();
        app.exec();
    } catch (std::exception& e) {
        // Using GUI stuff here resulted in strange seg.fault.
        std::cout << "Application terminated by unhandled exception: " << e.what() << std::endl;
    }
}

int main(int argc, char** argv) {
    startGuiApplication(argc, argv);
    exit(0);
}
