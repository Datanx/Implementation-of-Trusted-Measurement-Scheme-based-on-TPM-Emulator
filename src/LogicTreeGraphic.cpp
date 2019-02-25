/* 
 * File:   LogicTreeGraphic.cpp
 * Author: root
 * 
 * Created on February 28, 2010, 3:49 PM
 */

#include "LogicTreeGraphic.h"

LogicTreeGraphic::LogicTreeGraphic(cairo_t* cr, LogicTree* lt) {
    if(this->m_CR == NULL)
        return;

    this->m_lt = lt;
    this->m_CR = cr;
}

LogicTreeGraphic::LogicTreeGraphic(const LogicTreeGraphic& orig) {
}

LogicTreeGraphic::~LogicTreeGraphic() {
}

void LogicTreeGraphic::drawCircle(POINT center, int radius)
{
    if(this->m_CR == NULL)
    {
        printf("The m_CR is NULL\n");
        return;
    }

    cairo_set_line_width(this->m_CR, 3);
    cairo_set_source_rgb(this->m_CR, 1.0, 0, 0);
    cairo_arc(this->m_CR, center.x, center.y, radius, 0, 2*PI);
    cairo_stroke_preserve(this->m_CR);
    cairo_set_source_rgb (this->m_CR, 0.3, 0.4, 0.6);
    cairo_fill (this->m_CR);

    return;
}

void LogicTreeGraphic::drawLine(POINT from, POINT to)
{
    if(this->m_CR == NULL)
        return;

    cairo_set_source_rgb(this->m_CR, 0, 0, 0);
    cairo_set_line_width (this->m_CR, 2);
    cairo_move_to(this->m_CR, from.x, from.y);
    cairo_line_to(this->m_CR, to.x, to.y);

    cairo_stroke(this->m_CR);

    return;
}

void LogicTreeGraphic::drawText(POINT pos, const char* text)
{
    if(this->m_CR == NULL)
        return;

    cairo_set_source_rgb(this->m_CR, 1,1,1);
    cairo_select_font_face(this->m_CR, "Arial", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(this->m_CR, 13);
    cairo_move_to(this->m_CR, pos.x, pos.y);
    cairo_show_text(this->m_CR, text);
    cairo_stroke(this->m_CR);

    return;
}

void LogicTreeGraphic::DrawTree(LogicTreeNode* subroot, POINT cur_draw_pos, POINT last_draw_pos, int deep)
{
    if(subroot == NULL)
        return;

    //Draw Circle
    this->drawCircle(cur_draw_pos, 15);
    //Draw Line
    this->drawLine(cur_draw_pos, last_draw_pos);

    //Draw left subtree
    POINT left;
    int len = pow(2.0, (double)deep);
    left.x = cur_draw_pos.x - DIST_WIDTH * len;
    left.y = cur_draw_pos.y + DIST_HEIGHT;
    this->DrawTree(subroot->m_LChild, left, cur_draw_pos, deep - 1);

    //Draw right subtree
    POINT right;
    right.x = cur_draw_pos.x + DIST_WIDTH * len;
    right.y = cur_draw_pos.y + DIST_HEIGHT;
    this->DrawTree(subroot->m_RChild, right, cur_draw_pos, deep - 1);

    //Draw Text
    POINT text_pos;
    text_pos.x = cur_draw_pos.x - 14;
    text_pos.y = cur_draw_pos.y + 5;
    std::string val;
    if(subroot->m_NodeType == TNT_SYM || subroot->m_NodeType == TNT_EMPTY)
    {
        val = LogicTreeParser::m_opArray[(int)subroot->m_Operator].c_str();
    }
    else
    {
        val = subroot->m_NodeValue;
    }
    this->drawText(text_pos, val.c_str());
}

void LogicTreeGraphic::DoPaint(POINT draw_pos)
{
    if(this->m_CR == NULL)
        return;

    int deep = this->m_lt->get_height(this->m_lt->m_root);
    this->DrawTree(this->m_lt->m_root, draw_pos, draw_pos, deep - 2);
}
