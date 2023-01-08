#ifndef MAGICKDEFINE_H
#define MAGICKDEFINE_H

#include <QObject>

class MagickDefine
{
public:
    MagickDefine(QString magick, QString key, QString value);
    ~MagickDefine();

    QString magick();
    QString key();
    QString value();

private:
    QString m_magick;
    QString m_key;
    QString m_value;
};

#endif // MAGICKDEFINE_H
