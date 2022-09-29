#pragma once

#include <QMainWindow>

class QMdiArea;

namespace Ui {
    class Image_test_main_window;
};

class dbOpenCvS;

class ImageTestMainWindow : public QMainWindow {
public:
    ImageTestMainWindow(QWidget* parent=nullptr, Qt::WindowFlags flags=Qt::WindowFlags());
    virtual ~ImageTestMainWindow();

private slots:
    void dbConnect();
    void close();
    void viewImages();

private:
    Ui::Image_test_main_window* _UI;
    QMdiArea* _mMdiArea;

    dbOpenCvS* _mDbOpenCv;
private:
    Q_OBJECT
};
