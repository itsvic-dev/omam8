#include "emuwindow.h"
#include <QApplication>

int main(int argc, char **argv) {
  QApplication app(argc, argv);
  EmuWindow window;
  window.show();
  return app.exec();
}
