#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class EmuWindow;
}
QT_END_NAMESPACE

class EmuWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit EmuWindow(QWidget *parent = nullptr);
  ~EmuWindow();

public slots:
  void openROM();

private:
  Ui::EmuWindow *ui;
};
