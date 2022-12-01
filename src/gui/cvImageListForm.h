#pragma once

#include <QWidget>

namespace Ui {
    class cv_image_list_form;
};

class QAbstractItemModel;
class QAbstractItemDelegate;

class cvImageListForm : public QWidget {
public:
    cvImageListForm( QWidget* parent=nullptr, Qt::WindowFlags flags=Qt::WindowFlags() );
    virtual ~cvImageListForm();

    void setImagesModel( QAbstractItemModel* imModel );
    void setImagesItemDelegate( QAbstractItemDelegate* imDelegate );

private slots:
    void addImage( );
    void updImage( );
    void delImage( );
    void viewImage( );

    void refreshImages( );

signals:
    void insertImage( );
    void updateImage( qlonglong id );
    void deleteImage( qlonglong id );

    void refreshModel( );
    void viewDbImage( qlonglong id );
private:
    Ui::cv_image_list_form* _UI;

private:
    Q_OBJECT
};
