/* 
 * File:   LogicTreeGraphic.h
 * Author: root
 *
 * Created on February 28, 2010, 3:49 PM
 */

#ifndef _LOGICTREEGRAPHIC_H
#define	_LOGICTREEGRAPHIC_H

#include <math.h>
#include <gtk/gtk.h>
#include <cairo/cairo.h>
#include "LogicTreeNode.h"
#include "LogicTree.h"

#define PI 3.1415926
#define DIST_HEIGHT     40
#define DIST_WIDTH	10

typedef struct _POINT POINT;
struct _POINT
{
    int x;
    int y;
};

class LogicTreeGraphic {
public:
    LogicTreeGraphic(cairo_t* cr, LogicTree* lt);
    LogicTreeGraphic(const LogicTreeGraphic& orig);
    virtual ~LogicTreeGraphic();
    //Drawing functions
    void drawCircle(POINT center, int radius);
    void drawText(POINT pos, const char* text);
    void drawLine(POINT from, POINT to);
    void DrawTree(LogicTreeNode* subroot, POINT cur_draw_pos, POINT last_draw_pos, int deep);
    void DoPaint(POINT draw_pos);
private:
    cairo_t* m_CR;
    LogicTree* m_lt;
};

#endif	/* _LOGICTREEGRAPHIC_H */

