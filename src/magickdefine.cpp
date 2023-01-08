#include "magickdefine.h"

MagickDefine::MagickDefine(QString magick, QString key, QString value)
{
    m_magick = magick;
    m_key = key;
    m_value = value;
}

MagickDefine::~MagickDefine()
{

}

QString MagickDefine::magick()
{
    return m_magick;
}

QString MagickDefine::key()
{
    return m_key;
}

QString MagickDefine::value()
{
    return m_value;
}
