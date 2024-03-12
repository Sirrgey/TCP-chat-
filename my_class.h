#ifndef MY_CLASS_H
#define MY_CLASS_H

#include <QAbstractItemModel>

class my_class : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit my_class(QObject *parent = nullptr);



private:
};

#endif // MY_CLASS_H
