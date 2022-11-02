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
    void dbDisconnect();
    void close();

    void viewTypes();
    void viewImages();
    void insertImage();
    void importFromDir();
    void importFromCamera();
    void importFromURL();

    void setActionsEnable( bool enable );
    void addSubWindow( QWidget* w );
private:
    void init();

    Ui::Image_test_main_window* _UI;
    QMdiArea* _mMdiArea;

    dbOpenCvS* _mDbOpenCv;
    bool _isDbConnected;
private:
    Q_OBJECT
};
