#include "EunoiaUI/Layout/UILayout.h"
#include "EunoiaUI/Core/UIStyle.h"
#include "EunoiaUI/Draw/UICanvas.h"
#include "EunoiaUI/Text/UIFont.h"
#include <algorithm>

namespace EunoiaUI {

void UILayout::Begin(const UIRectangle& region) {
    UIParcel parcel;
    parcel.Area = region;
    parcel.Cursor = region.Origin();
    parcel.LinePeak = 0.0f;
    parcel.Depth = 0.0f;
    parcel.ItemWidth = -1.0f;
    parcel.Adjacent = false;
    m_stack.push_back(parcel);
}

float UILayout::End() {
    if (m_stack.empty()) return 0.0f;
    float totalHeight = m_stack.back().Cursor.Vertical - m_stack.back().Area.Top;
    m_stack.pop_back();
    return totalHeight;
}

UIRectangle UILayout::Place(const UIVector& extent) {
    if (m_stack.empty()) return UIRectangle(0.0f, 0.0f, extent.Horizontal, extent.Vertical);
    auto& cur = m_stack.back();

    float w = extent.Horizontal;
    if (cur.ItemWidth > 0.0f) {
        w = cur.ItemWidth;
    } else if (!m_widthStack.empty()) {
        w = m_widthStack.back();
    }

    float x = cur.Cursor.Horizontal + cur.Depth;
    float y = cur.Cursor.Vertical;

    UIRectangle slot(x, y, w, extent.Vertical);
    cur.LinePeak = std::max(cur.LinePeak, extent.Vertical);

    if (cur.Adjacent) {
        cur.Cursor.Horizontal += w + Style->Spacing;
        cur.Adjacent = false;
    } else {
        cur.Cursor.Horizontal = cur.Area.Left;
        cur.Cursor.Vertical += cur.LinePeak + Style->Spacing;
        cur.LinePeak = 0.0f;
    }

    return slot;
}

UIRectangle UILayout::Stretch(float height) {
    if (m_stack.empty()) return UIRectangle(0.0f, 0.0f, 100.0f, height);
    auto& cur = m_stack.back();
    float availableW = cur.Area.Width - cur.Depth;
    return Place(UIVector(std::max(10.0f, availableW), height));
}

void UILayout::Beside() {
    if (m_stack.empty()) return;
    m_stack.back().Adjacent = true;
}

void UILayout::SameLine(float offset, float gap) {
    if (m_stack.empty()) return;
    auto& cur = m_stack.back();
    cur.Adjacent = true;
    if (offset > 0.0f) {
        cur.Cursor.Horizontal = cur.Area.Left + offset;
    }
}

void UILayout::Skip(float amount) {
    if (m_stack.empty()) return;
    m_stack.back().Cursor.Vertical += amount;
}

void UILayout::Spacing() {
    Skip(Style->Spacing);
}

void UILayout::Dummy(const UIVector& extent) {
    Place(extent);
}

void UILayout::Indent(float amount) {
    if (m_stack.empty()) return;
    m_stack.back().Depth += (amount > 0.0f ? amount : Style->PaddingWide);
}

void UILayout::Unindent(float amount) {
    if (m_stack.empty()) return;
    m_stack.back().Depth = std::max(0.0f, m_stack.back().Depth - (amount > 0.0f ? amount : Style->PaddingWide));
}

void UILayout::PushWidth(float width) {
    m_widthStack.push_back(width);
}

void UILayout::PopWidth() {
    if (!m_widthStack.empty()) m_widthStack.pop_back();
}

void UILayout::BeginGroup() {
    if (!m_stack.empty()) {
        m_groupStarts.push_back(m_stack.back().Cursor);
    }
}

UIRectangle UILayout::EndGroup() {
    if (m_stack.empty() || m_groupStarts.empty()) return UIRectangle();
    UIVector start = m_groupStarts.back();
    m_groupStarts.pop_back();
    UIVector end = m_stack.back().Cursor;
    return UIRectangle(start, UIVector(m_stack.back().Area.Width, end.Vertical - start.Vertical));
}

bool UILayout::BeginChild(const char* identifier, const UIVector& extent, bool framed) {
    UIRectangle rect = Place(extent);
    if (framed) {
        Canvas->Rectangle(rect, Style->Surface, Style->ControlRounding);
        Canvas->Border(rect, Style->Outline, Style->ControlRounding, Style->Thickness);
    }
    Canvas->PushClip(rect.Shrink(Style->Thickness));
    Begin(rect.Pad(Style->PaddingWide * 0.5f, Style->PaddingTall * 0.5f));
    return true;
}

void UILayout::EndChild() {
    End();
    Canvas->PopClip();
}

bool UILayout::BeginTable(const char* identifier, int columns, bool border, bool striped) {
    if (columns <= 0) return false;
    UITableState tbl;
    tbl.Identifier = 0;
    tbl.Columns = columns;
    tbl.CurrentColumn = 0;
    tbl.CurrentRow = 0;
    tbl.Border = border;
    tbl.Striped = striped;
    tbl.Area = m_stack.empty() ? UIRectangle() : m_stack.back().Area;
    tbl.RowTop = m_stack.empty() ? 0.0f : m_stack.back().Cursor.Vertical;
    tbl.RowPeak = 0.0f;
    tbl.Widths.assign(columns, (tbl.Area.Width - Style->Spacing * (columns - 1)) / columns);
    m_tables.push_back(tbl);
    return true;
}

void UILayout::TableSetup(const char* title, float width) {
    if (m_tables.empty()) return;
    auto& tbl = m_tables.back();
    tbl.Titles.push_back(title ? title : "");
    if (width > 0.0f && (int)tbl.Titles.size() <= tbl.Columns) {
        tbl.Widths[tbl.Titles.size() - 1] = width;
    }
}

void UILayout::TableHeaders() {
    if (m_tables.empty()) return;
    auto& tbl = m_tables.back();

    float h = Style->ControlHeight;
    UIRectangle headerRect = Stretch(h);
    Canvas->Rectangle(headerRect, Style->Header, 0.0f);
    if (tbl.Border) Canvas->Border(headerRect, Style->Outline, 0.0f, Style->Thickness);

    float curX = headerRect.Left;
    for (int i = 0; i < (int)tbl.Titles.size() && i < tbl.Columns; ++i) {
        float w = tbl.Widths[i];
        UIRectangle cell(curX, headerRect.Top, w, h);
        Canvas->Text(Font.get(), tbl.Titles[i].c_str(), cell.Origin() + UIVector(Style->PaddingWide * 0.5f, (h - Font->LineHeight()) * 0.5f), Style->Text);
        curX += w + Style->Spacing;
    }
}

void UILayout::TableRow() {
    if (m_tables.empty()) return;
    auto& tbl = m_tables.back();
    tbl.CurrentRow++;
    tbl.CurrentColumn = 0;
}

void UILayout::TableColumn() {
    if (m_tables.empty()) return;
    auto& tbl = m_tables.back();
    if (tbl.CurrentColumn < tbl.Columns - 1) {
        tbl.CurrentColumn++;
        Beside();
    }
}

void UILayout::EndTable() {
    if (!m_tables.empty()) {
        m_tables.pop_back();
    }
}

UIVector UILayout::CurrentCursor() const {
    if (m_stack.empty()) return UIVector();
    return m_stack.back().Cursor;
}

UIRectangle UILayout::CurrentRegion() const {
    if (m_stack.empty()) return UIRectangle();
    return m_stack.back().Area;
}

} // namespace EunoiaUI
