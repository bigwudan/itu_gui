#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "itu.h"
#include "itu_cfg.h"
#include "itu_private.h"

static const char textName[] = "ITUText";

void ituTextExit(ITUWidget* widget)
{
    ITUText* text = (ITUText*) widget;
    assert(widget);
    ITU_ASSERT_THREAD();

    if (text->string)
    {
        free(text->string);
        text->string = NULL;
    }
    ituWidgetExitImpl(widget);
}

bool ituTextClone(ITUWidget* widget, ITUWidget** cloned)
{
    ITUText* text = (ITUText*)widget;
    assert(widget);
    assert(cloned);
    ITU_ASSERT_THREAD();

    if (*cloned == NULL)
    {
        ITUWidget* newWidget = malloc(sizeof(ITUText));
        if (newWidget == NULL)
            return false;

        memcpy(newWidget, widget, sizeof(ITUText));
        newWidget->tree.child = newWidget->tree.parent = newWidget->tree.sibling = NULL;
        *cloned = newWidget;
    }

    if (text->string)
    {
        ITUText* newText = (ITUText*)*cloned;
        newText->string =strdup(text->string);
    }
    return ituWidgetCloneImpl(widget, cloned);
}

bool ituTextUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3)
{
    bool result = false;


    return result;
}

void ituTextDraw(ITUWidget* widget, ITUSurface* dest, int x, int y, uint8_t alpha)
{
    
}

void ituTextInit(ITUText* text)
{
   
}

void ituTextLoad(ITUText* text, uint32_t base)
{
   
}

void ituTextSetFontWidth(ITUText* text, int height)
{
    
}

void ituTextSetFontHeight(ITUText* text, int height)
{
  
}

void ituTextSetFontSize(ITUText* text, int size)
{
   
}

void ituTextSetLanguage(ITUText* text, int lang)
{
  
}

void ituTextSetStringImpl(ITUText* text, char* string)
{
   
}

char* ituTextGetStringImpl(ITUText* text)
{
   
}

void ituTextSetBackColor(ITUText* text, uint8_t alpha, uint8_t red, uint8_t green, uint8_t blue)
{
    
}

void ituTextResize(ITUText* text)
{
 
}
