#include "itu.h"
#include "itu_cfg.h"
#include <assert.h>
void ituSurfaceSetClipping(ITUSurface *surf, int x, int y, int w, int h)
{
    if (x == 0 && y == 0 && w == 0 && h == 0)
        surf->flags &= ~ITU_CLIPPING;
    else
        surf->flags |= ITU_CLIPPING;

    surf->clipping.x      = x > 0 ? x : 0;
    surf->clipping.y      = y > 0 ? y : 0;
    surf->clipping.width  = (w <= surf->width) ? w : surf->width;
    surf->clipping.height = (h <= surf->height) ? h : surf->height;
}

void ituSurfaceRelease(ITUSurface *surf)
{
	ITUSurface *parentSurf = (ITUSurface *)surf->parent;

	if (parentSurf && (parentSurf->flags & ITU_STATIC))
	{
		assert(parentSurf->flags & ITU_COMPRESSED);
		assert(parentSurf->lockSize);

		if (--parentSurf->lockSize == 0)
		{
			ituDestroySurface(surf);
		}
	}
	else
		ituDestroySurface(surf);
}

ITUSurface *ituSurfaceDecompress(ITUSurface *surf)
{
	ITUSurface   *retSurf = NULL;
	return retSurf;
}