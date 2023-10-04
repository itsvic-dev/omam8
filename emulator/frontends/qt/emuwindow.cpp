#include "emuwindow.h"
#include "ui_emuwindow.h"
#include <QFileDialog>
#include <QMainWindow>

EmuWindow::EmuWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::EmuWindow) {
  ui->setupUi(this);
}

EmuWindow::~EmuWindow() { delete ui; }

void EmuWindow::openROM() {
  QString romToOpen =
      QFileDialog::getOpenFileName(this, "Open ROM", "", "omam8 ROMs (*.rom)");

  qDebug() << "rom to open:" << romToOpen;
}
