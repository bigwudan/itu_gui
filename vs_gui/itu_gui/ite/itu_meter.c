#include <assert.h>
#include <math.h>
#include <string.h>
#include "itu.h"
#include "itu_cfg.h"
#include "itu_math.h"

static const char meterName[] = "ITUMeter";

static void ituMeterExit(ITUWidget* widget)
{
    ITUMeter* meter = (ITUMeter*) widget;
    assert(widget);
    ITU_ASSERT_THREAD();
}

bool ituMeterUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3)
{
    bool result = false;
    ITUMeter* meter = (ITUMeter*) widget;
    assert(meter);

    if ((ev == ITU_EVENT_MOUSEDOWN) || (ev == ITU_EVENT_MOUSEMOVE && meter->pressed))
    {
        if ((ev == ITU_EVENT_MOUSEDOWN) || (ev == ITU_EVENT_MOUSEMOVE && --meter->delayCount <= 0))
        {
            if (ituWidgetIsEnabled(widget))
            {
                int x = arg2 - widget->rect.x;
                int y = arg3 - widget->rect.y;

                if (ituWidgetIsInside(widget, x, y))
                {
                    int orgX, orgY, vx1, vy1, vx2, vy2, dot, det, value;
                    float angle;

                    orgX = widget->rect.width / 2;
                    orgY = widget->rect.height / 2;

                    vx2 = x - orgX;
                    vy2 = y - orgY;

                    if (vx2 * vx2 + vy2 * vy2 >= meter->minRadius * meter->minRadius)
                    {
                        vx1 = 0;
                        vy1 = -orgY;

                        dot = vx1 * vx2 + vy1 *vy2;
                        det = vx1 * vy2 - vy1 * vx2;
                        angle = atan2f(det, dot) * (float)(180.0f / M_PI);

                        if ((angle < meter->startAngle && meter->startAngle < meter->endAngle) || angle < 0)
                            angle += 360.0f;

                        //printf("(%d, %d) (%d, %d) angle=%f\n", vx1, vy1, vx2, vy2, angle);

                        if (meter->startAngle < meter->endAngle)
                        {
                            if (meter->startAngle <= angle && angle <= meter->endAngle)
                            {
                                int range = meter->endAngle - meter->startAngle;
                                value = (int)roundf((angle - meter->startAngle) * meter->maxValue / range);

                                ituMeterSetValue(meter, value);

                                ituExecActions((ITUWidget*)meter, meter->actions, ITU_EVENT_CHANGED, value);
                                result = widget->dirty = true;
                            }
                        }
                        else if (meter->startAngle > meter->endAngle)
                        {
                            if (meter->startAngle >= 360 && angle < meter->endAngle)
                                angle += 360;

                            if (meter->startAngle >= angle && angle >= meter->endAngle)
                            {
                                int range = meter->startAngle - meter->endAngle;
                                value = (int)roundf((meter->startAngle - angle) * meter->maxValue / range);

                                ituMeterSetValue(meter, value);

                                ituExecActions((ITUWidget*)meter, meter->actions, ITU_EVENT_CHANGED, value);
                                result = widget->dirty = true;
                            }
                        }
                        meter->pressed = true;
                        meter->delayCount = meter->delay;
                    }
                }
            }

        }
    }
    else if (ev == ITU_EVENT_MOUSEUP)
    {
        if (ituWidgetIsEnabled(widget))
        {
            if (meter->pressed)
            {
                meter->pressed = false;
                widget->dirty = true;

                result |= widget->dirty;
                return result;
            }
        }
    }
    else if (ev == ITU_EVENT_LAYOUT)
    {
        if (!meter->pointerIcon && (meter->pointerName[0] != '\0'))
        {
            meter->pointerIcon = (ITUIcon*) ituSceneFindWidget(ituScene, meter->pointerName);
        }
    }
    result |= ituIconUpdate(widget, ev, arg1, arg2, arg3);

    return widget->visible ? result : false;
}

void ituMeterDraw(ITUWidget* widget, ITUSurface* dest, int x, int y, uint8_t alpha)
{
	ITUMeter* meter = (ITUMeter*)widget;
	ITURectangle prevClip = { 0, 0, 0, 0 };

	ITCTree* child = widget->tree.child;
	widget->tree.child = NULL;
	ituBackgroundDraw(widget, dest, x, y, alpha);
	widget->tree.child = child;

	ituWidgetSetClipping(widget, dest, x, y, &prevClip);

	x += widget->rect.x;
	y += widget->rect.y;
	alpha = alpha * widget->alpha / 255;

	if (meter->pointerIcon && meter->pointerIcon->surf)
	{
		float diff = 0.0;
		float angle = 0.0;
		float last_angle = 0.0;

		if (meter->endAngle > meter->startAngle)
		{
			angle = meter->startAngle + (meter->endAngle - meter->startAngle) * meter->value / (float)meter->maxValue;
			last_angle = meter->startAngle + (meter->endAngle - meter->startAngle) * meter->lastvalue / (float)meter->maxValue;
		}
		else if (meter->endAngle < meter->startAngle)
		{
			angle = meter->startAngle - (meter->startAngle - meter->endAngle) * meter->value / (float)meter->maxValue;
			last_angle = meter->startAngle - (meter->startAngle - meter->endAngle) * meter->lastvalue / (float)meter->maxValue;
		}

		diff = (angle - last_angle);

		if (meter->anifitcount <= 1)
		{
			ituWidgetSetCustomData(meter, 0);
		}

		//do animation fit when shift value small
		if (meter->anifitcount > 1)
		{
			int total_draw_loop = 1;
			int shadowAlpha = 100;
			int count = (int)ituWidgetGetCustomData(meter);
			bool print_debug = false;
			float step = diff;
			float target = last_angle;
			ITUSurface* surf = ituCreateSurface(widget->rect.width, widget->rect.height, 0, dest->format, NULL, 0);
			ituBitBlt(surf, 0, 0, widget->rect.width, widget->rect.height, dest, x, y);


			//type B
			//real target
			//second shadow |---S---|
			if (1)
			{
				//int av = ((count == 1) ? (255) : (200));
				target = angle;

				if (1)
				{
#ifndef CFG_WIN32_SIMULATOR
					ituRotate(dest, x + meter->pointerIcon->widget.rect.x, y + meter->pointerIcon->widget.rect.y, meter->pointerIcon->surf, meter->pointerX, meter->pointerY, angle, 1.0f, 1.0f);
#else
					ituRotate(dest, x + meter->pointerIcon->widget.rect.x + meter->pointerX, y + meter->pointerIcon->widget.rect.y + meter->pointerY, meter->pointerIcon->surf, meter->pointerX, meter->pointerY, angle, 1.0f, 1.0f);
#endif
				}

				if (0)
				{
					target = last_angle + (step / 2);
#ifdef CFG_WIN32_SIMULATOR
					ituRotate(surf, meter->pointerIcon->widget.rect.x + meter->pointerX, meter->pointerIcon->widget.rect.y + meter->pointerY, meter->pointerIcon->surf, meter->pointerX, meter->pointerY, target, 1.0f, 1.0f);
#else
					ituRotate(surf, meter->pointerIcon->widget.rect.x, meter->pointerIcon->widget.rect.y, meter->pointerIcon->surf, meter->pointerX, meter->pointerY, target, 1.0f, 1.0f);
#endif
					ituAlphaBlend(dest, x, y, widget->rect.width, widget->rect.height, surf, 0, 0, 10);

				}

				if (meter->lastvalue != meter->value) //do not fit when finger moving
				{
					target = last_angle + (step / 2);
#ifdef CFG_WIN32_SIMULATOR
					ituRotate(surf, meter->pointerIcon->widget.rect.x + meter->pointerX, meter->pointerIcon->widget.rect.y + meter->pointerY, meter->pointerIcon->surf, meter->pointerX, meter->pointerY, target, 1.0f, 1.0f);
#else
					ituRotate(surf, meter->pointerIcon->widget.rect.x, meter->pointerIcon->widget.rect.y, meter->pointerIcon->surf, meter->pointerX, meter->pointerY, target, 1.0f, 1.0f);
#endif
					ituAlphaBlend(dest, x, y, widget->rect.width, widget->rect.height, surf, 0, 0, shadowAlpha);

				}

				if (0)
				{
					printf("[%d]target %.2f \n", count, angle);
				}

			}

			/* for debug usage
			if (count == 0)
				count = 0;
			else if (count == 1)
				count = 1;
			else if (count == 2)
				count = 2;
			*/
			
			/* bad performance
			ituTransform(
			dest, x + meter->pointerIcon->widget.rect.x, y + meter->pointerIcon->widget.rect.y, widget->rect.width, widget->rect.height,
			meter->pointerIcon->surf, 0, 0, meter->pointerIcon->surf->width, meter->pointerIcon->surf->height,
			0, 0, 1.0f, 1.0f, target, 0, true, true, initAlpha);
			*/

			ituDestroySurface(surf);
			surf = NULL;

			count++;
			
			if (count < total_draw_loop)
			{
				ituWidgetSetCustomData(widget, count);
				ituWidgetSetDirty(meter->pointerIcon, false);
				ituSurfaceSetClipping(dest, prevClip.x, prevClip.y, prevClip.width, prevClip.height);
				return;
			}
			else
			{
				ituWidgetSetCustomData(widget, 0);
				ituWidgetSetDirty(meter->pointerIcon, false);
				ituSurfaceSetClipping(dest, prevClip.x, prevClip.y, prevClip.width, prevClip.height);
				meter->lastvalue = meter->value;
				return;
			}

		}
		else //reset last value
		{
			//meter->lastvalue = meter->value;
		}

		// default drawing without any shadow fit
		if (meter->anifitcount <= 1)
		{
			if (meter->pointerIcon->widget.alpha == 255)
			{
#ifndef CFG_WIN32_SIMULATOR
				ituRotate(dest, x + meter->pointerIcon->widget.rect.x, y + meter->pointerIcon->widget.rect.y, meter->pointerIcon->surf, meter->pointerX, meter->pointerY, angle, 1.0f, 1.0f);
#else
				ituRotate(dest, x + meter->pointerIcon->widget.rect.x + meter->pointerX, y + meter->pointerIcon->widget.rect.y + meter->pointerY, meter->pointerIcon->surf, meter->pointerX, meter->pointerY, angle, 1.0f, 1.0f);
#endif
			}
			else
			{
				ITUSurface* surf = ituCreateSurface(widget->rect.width, widget->rect.height, 0, dest->format, NULL, 0);
				ituBitBlt(surf, 0, 0, widget->rect.width, widget->rect.height, dest, x, y);
#ifdef CFG_WIN32_SIMULATOR
				ituRotate(surf, meter->pointerIcon->widget.rect.x + meter->pointerX, meter->pointerIcon->widget.rect.y + meter->pointerY, meter->pointerIcon->surf, meter->pointerX, meter->pointerY, angle, 1.0f, 1.0f);
#else
				ituRotate(surf, meter->pointerIcon->widget.rect.x, meter->pointerIcon->widget.rect.y, meter->pointerIcon->surf, meter->pointerX, meter->pointerY, angle, 1.0f, 1.0f);
#endif
				ituAlphaBlend(dest, x, y, widget->rect.width, widget->rect.height, surf, 0, 0, meter->pointerIcon->widget.alpha);
				ituDestroySurface(surf);
				surf = NULL;
			}
			meter->lastvalue = meter->value;
		}
		ituWidgetSetDirty(meter->pointerIcon, false);

	}
	ituSurfaceSetClipping(dest, prevClip.x, prevClip.y, prevClip.width, prevClip.height);
}

void ituMeterInit(ITUMeter* meter)
{
    assert(meter);
    ITU_ASSERT_THREAD();

    memset(meter, 0, sizeof (ITUMeter));

    ituBackgroundInit(&meter->bg);

    ituWidgetSetType(meter, ITU_METER);
    ituWidgetSetName(meter, meterName);
    ituWidgetSetExit(meter, ituMeterExit);
    ituWidgetSetUpdate(meter, ituMeterUpdate);
    ituWidgetSetDraw(meter, ituMeterDraw);
}

void ituMeterLoad(ITUMeter* meter, uint32_t base)
{
    assert(meter);

    ituBackgroundLoad(&meter->bg, base);

	ituWidgetSetCustomData(meter, 0);

	if (meter->pointerIcon)
		meter->pointerIcon = (ITUIcon*)((uint32_t)meter->pointerIcon + base);

    ituWidgetSetExit(meter, ituMeterExit);
    ituWidgetSetUpdate(meter, ituMeterUpdate);
    ituWidgetSetDraw(meter, ituMeterDraw);
}

void ituMeterSetValue(ITUMeter* meter, int value)
{
    assert(meter);
    ITU_ASSERT_THREAD();

    if (meter->value == value)
        return;

    if (value < 0 || value > meter->maxValue)
    {
        LOG_WARN "incorrect value: %d\n", value LOG_END
        return;
    }

    meter->value = value;

    //ituWidgetUpdate(meter, ITU_EVENT_LAYOUT, 0, 0, 0);
    ituExecActions((ITUWidget*)meter, meter->actions, ITU_EVENT_CHANGED, value);
    ituWidgetSetDirty(meter, true);
}
