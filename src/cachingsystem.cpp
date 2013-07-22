#include "cachingsystem.h"

#define MAX_COST 100

QCache<QString, CachedImageObject> *CachingSystem::imageCache;
CachedImageObject *CachingSystem::currObj;

void CachingSystem::init()
{
    imageCache = new QCache<QString, CachedImageObject>;
    imageCache->setMaxCost(MAX_COST);
}

void CachingSystem::insert(QString id, QImage thumbnail, int orig_w, int orig_h, double orig_dens_x, double orig_dens_y)
{
    CachedImageObject *c = new CachedImageObject;
    c->id = id;
    c->thumbnail = thumbnail;
    c->orig_w = orig_w;
    c->orig_h = orig_h;
    c->orig_dens_x = orig_dens_x;
    c->orig_dens_y = orig_dens_y;

    imageCache->insert(id, c);
}

bool CachingSystem::find(QString key)
{
    if (imageCache->contains(key)) {
        currObj = imageCache->object(key);

        return true;
    }

    return false;
}

void CachingSystem::clear()
{
    imageCache->clear();
}

void CachingSystem::remove(QString key)
{
    imageCache->remove(key);
}

QImage CachingSystem::thumbnail()
{
    return currObj->thumbnail;
}

int CachingSystem::originalWidth()
{
    return currObj->orig_w;
}

int CachingSystem::originalHeight()
{
    return currObj->orig_h;
}

double CachingSystem::originalDensityX()
{
    return currObj->orig_dens_x;
}

double CachingSystem::originalDensityY()
{
    return currObj->orig_dens_y;
}
