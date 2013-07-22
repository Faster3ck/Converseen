#ifndef CACHINGSYSTEM_H
#define CACHINGSYSTEM_H

#include <QCache>
#include <QImage>

class CachedImageObject
{
public:
    QImage thumbnail;
    QString id;
    int orig_w;
    int orig_h;
    double orig_dens_x;
    double orig_dens_y;
};

class CachingSystem
{
public:
    static void init();
    static void insert(QString id, QImage thumbnail, int orig_w, int orig_h, double orig_dens_x, double orig_dens_y);
    static bool find(QString key);
    static void clear();
    static void remove(QString key);

    static QImage thumbnail();
    static int originalWidth();
    static int originalHeight();
    static double originalDensityX();
    static double originalDensityY();

    static QCache<QString, CachedImageObject> *imageCache;
private:
    static CachedImageObject *currObj;
    static int m_orig_w;
};

#endif // CACHINGSYSTEM_H
