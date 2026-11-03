#pragma once
#include <memory>
#include <string>
#include <vector>
#include "EunoiaUI/Core/UIGeometry.h"

namespace EunoiaUI {

struct UIParcel {
    UIRectangle Area;
    UIVector Cursor{0.0f, 0.0f};
    float LinePeak = 0.0f;
    float Depth = 0.0f;
    float ItemWidth = -1.0f;
    bool Adjacent = false;
};

struct UITableState {
    unsigned int Identifier = 0;
    UIRectangle Area;
    int Columns = 0;
    int CurrentColumn = 0;
    int CurrentRow = 0;
    float RowTop = 0.0f;
    float RowPeak = 0.0f;
    bool Border = true;
    bool Striped = true;
    std::vector<float> Widths;
    std::vector<std::string> Titles;
};

class UILayout {
public:
    void Begin(const UIRectangle& region);
    float End();

    UIRectangle Place(const UIVector& extent);
    UIRectangle Stretch(float height);

    void Beside();
    void SameLine(float offset = 0.0f, float gap = -1.0f);
    void Skip(float amount);
    void Spacing();
    void Dummy(const UIVector& extent);

    void Indent(float amount = 0.0f);
    void Unindent(float amount = 0.0f);

    void PushWidth(float width);
    void PopWidth();

    void BeginGroup();
    UIRectangle EndGroup();

    bool BeginChild(const char* identifier, const UIVector& extent, bool framed = true);
    void EndChild();

    bool BeginTable(const char* identifier, int columns, bool border = true, bool striped = true);
    void TableSetup(const char* title, float width = 0.0f);
    void TableHeaders();
    void TableRow();
    void TableColumn();
    void EndTable();

    UIVector CurrentCursor() const;
    UIRectangle CurrentRegion() const;

private:
    std::vector<UIParcel> m_stack;
    std::vector<UITableState> m_tables;
    std::vector<float> m_widthStack;
    std::vector<UIVector> m_groupStarts;
};

inline auto Layout = std::make_unique<UILayout>();

} // namespace EunoiaUI
