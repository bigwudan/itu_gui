#include <assert.h>
#include <malloc.h>

#include <string.h>
#include <math.h>
#include "itu.h"
#include "itu_cfg.h"
#include "itu_private.h"


static const char iconName[] = "ITUIcon";

void ituIconExit(ITUWidget* widget)
{
    ITUIcon* icon = (ITUIcon*) widget;
    assert(widget);
    ITU_ASSERT_THREAD();

    if (icon->filePath)
    {
        free(icon->filePath);
        icon->filePath = NULL;
    }

    if (widget->flags & ITU_LOADED)
    {
        if (icon->loadedSurf)
        {
            ituSurfaceRelease(icon->loadedSurf);
            icon->loadedSurf = NULL;
        }
        widget->flags &= ~ITU_LOADED;
    }

    if (icon->surf)
    {
        ituSurfaceRelease(icon->surf);
        icon->surf = NULL;
    }
    ituWidgetExitImpl(widget);
}

bool ituIconClone(ITUWidget* widget, ITUWidget** cloned)
{
    ITUIcon* icon = (ITUIcon*)widget;
    assert(widget);
    assert(cloned);
    ITU_ASSERT_THREAD();

    if (*cloned == NULL)
    {
        ITUWidget* newWidget = malloc(sizeof(ITUIcon));
        if (newWidget == NULL)
            return false;

        memcpy(newWidget, widget, sizeof(ITUIcon));
        newWidget->tree.child = newWidget->tree.parent = newWidget->tree.sibling = NULL;
        *cloned = newWidget;
    }

    if (!(icon->widget.flags & ITU_EXTERNAL) && icon->staticSurf)
    {
        ITUIcon* newIcon = (ITUIcon*)*cloned;
        ITUSurface* surf = icon->staticSurf;
        if (surf->flags & ITU_COMPRESSED)
            newIcon->surf = NULL;
        else
            newIcon->surf = ituCreateSurface(surf->width, surf->height, surf->pitch, surf->format, (const uint8_t*)surf->addr, surf->flags);

        ituWidgetUpdate(newIcon, ITU_EVENT_LOAD, 0, 0, 0);
    }
    return ituWidgetCloneImpl(widget, cloned);
}

static void IconLoadExternalData(ITUIcon* icon, ITULayer* layer)
{
    ITUWidget* widget = (ITUWidget*)icon;
    ITUSurface* surf;

    assert(widget);

    if (!icon->staticSurf || icon->surf || !(widget->flags & ITU_EXTERNAL))
        return;

    if (!layer)
        layer = ituGetLayer(widget);

    surf = ituLayerLoadExternalSurface(layer, (uint32_t)icon->staticSurf);
    if (surf)
    {
        if (surf->flags & ITU_COMPRESSED)
            icon->surf = ituSurfaceDecompress(surf);
        else
            icon->surf = ituCreateSurface(surf->width, surf->height, surf->pitch, surf->format, (const uint8_t*)surf->addr, surf->flags);
    }
}

static void IconLoadImage(ITUIcon* icon, char* path)
{
   
}

bool ituIconUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3)
{
    bool result = false;
    return result;
}

void ituIconDraw(ITUWidget* widget, ITUSurface* dest, int x, int y, uint8_t alpha)
{

}

void ituIconInit(ITUIcon* icon)
{
    assert(icon);
    ITU_ASSERT_THREAD();
    memset(icon, 0, sizeof (ITUIcon));
    ituWidgetInit(&icon->widget);
    ituWidgetSetType(icon, ITU_ICON);
    ituWidgetSetName(icon, iconName);
    ituWidgetSetExit(icon, ituIconExit);
    ituWidgetSetClone(icon, ituIconClone);
    ituWidgetSetUpdate(icon, ituIconUpdate);
    ituWidgetSetDraw(icon, ituIconDraw);
}

void ituIconLoad(ITUIcon* icon, uint32_t base)
{
   
}

bool ituIconLoadJpegData(ITUIcon* icon, uint8_t* data, int size)
{
    ITUWidget* widget = (ITUWidget*)icon;
    ITURectangle* rect = &widget->rect;
    ITUSurface* surf;
    bool result = false;


    return result;
}

static void* IconLoadJpegFileTask(void* arg)
{
  
}

void ituIconLoadJpegFileSync(ITUIcon* icon, char* filepath)
{
    
}

bool ituIconLoadPngData(ITUIcon* icon, uint8_t* data, int size)
{
    ITUWidget* widget = (ITUWidget*)icon;
    ITURectangle* rect = &widget->rect;
    ITUSurface* surf;
    bool result = false;


    return result;
}

static void* IconLoadPngFileTask(void* arg)
{
 
    return NULL;
}

void ituIconLoadPngFile(ITUIcon* icon, char* filepath)
{
    
}

void ituIconLoadPngFileSync(ITUIcon* icon, char* filepath)
{
    
}

void ituIconLoadStaticData(ITUIcon* icon)
{
   
}

void ituIconReleaseSurface(ITUIcon* icon)
{
  
}

void ituIconLinkSurface(ITUIcon* icon, ITUIcon* src)
{
 
}

void ituIconRmSetEnable(ITUIcon* icon, bool enable)
{
	
}

bool ituIconRmIsEnable(ITUIcon* icon)
{
	return false;
}

bool ituIconRmSetAngle(ITUIcon* icon, int start_angle, int end_angle)
{
	return false;
}