#include "emuwindow.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char **argv) {
  QApplication app(argc, argv);

  QTranslator translator;
  if (translator.load(QLocale::system(), "", "", ":/i18n"))
    app.installTranslator(&translator);

  EmuWindow window;
  window.show();
  return app.exec();
}
