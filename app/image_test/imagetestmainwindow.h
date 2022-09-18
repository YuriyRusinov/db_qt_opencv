#pragma once

#include <QMainWindow>

namespace Ui {
    class Image_test_main_window;
};

class ImageTestMainWindow : public QMainWindow {
public:
    ImageTestMainWindow(QWidget* parent=nullptr, Qt::WindowFlags flags=Qt::WindowFlags());
    virtual ~ImageTestMainWindow();

private slots:
    void dbConnect();
    void close();

private:
    Ui::Image_test_main_window* _UI;
private:
    Q_OBJECT
};
