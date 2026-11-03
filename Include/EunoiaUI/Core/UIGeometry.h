#pragma once
#include <cmath>
#include <string>
#include <algorithm>

namespace EunoiaUI {

class UIVector {
public:
    float Horizontal = 0.0f;
    float Vertical = 0.0f;

    UIVector() = default;
    UIVector(float across, float down) : Horizontal(across), Vertical(down) {}

    // Convenience aliases
    float& x() { return Horizontal; }
    float& y() { return Vertical; }
    float x() const { return Horizontal; }
    float y() const { return Vertical; }

    UIVector operator+(const UIVector& other) const {
        return UIVector(Horizontal + other.Horizontal, Vertical + other.Vertical);
    }
    UIVector operator-(const UIVector& other) const {
        return UIVector(Horizontal - other.Horizontal, Vertical - other.Vertical);
    }
    UIVector operator*(float scalar) const {
        return UIVector(Horizontal * scalar, Vertical * scalar);
    }
    UIVector operator/(float scalar) const {
        return (scalar != 0.0f) ? UIVector(Horizontal / scalar, Vertical / scalar) : UIVector(0.0f, 0.0f);
    }
    UIVector& operator+=(const UIVector& other) {
        Horizontal += other.Horizontal;
        Vertical += other.Vertical;
        return *this;
    }
    UIVector& operator-=(const UIVector& other) {
        Horizontal -= other.Horizontal;
        Vertical -= other.Vertical;
        return *this;
    }

    bool operator==(const UIVector& other) const {
        return std::abs(Horizontal - other.Horizontal) < 1e-4f && std::abs(Vertical - other.Vertical) < 1e-4f;
    }
    bool operator!=(const UIVector& other) const {
        return !(*this == other);
    }

    float Length() const {
        return std::sqrt(Horizontal * Horizontal + Vertical * Vertical);
    }
};

class UIColor {
public:
    unsigned char Red = 255;
    unsigned char Green = 255;
    unsigned char Blue = 255;
    unsigned char Alpha = 255;

    UIColor() = default;
    UIColor(int r, int g, int b, int a = 255)
        : Red((unsigned char)std::clamp(r, 0, 255)),
          Green((unsigned char)std::clamp(g, 0, 255)),
          Blue((unsigned char)std::clamp(b, 0, 255)),
          Alpha((unsigned char)std::clamp(a, 0, 255)) {}

    UIColor(float r, float g, float b, float a = 1.0f)
        : Red((unsigned char)std::clamp((int)(r * 255.0f), 0, 255)),
          Green((unsigned char)std::clamp((int)(g * 255.0f), 0, 255)),
          Blue((unsigned char)std::clamp((int)(b * 255.0f), 0, 255)),
          Alpha((unsigned char)std::clamp((int)(a * 255.0f), 0, 255)) {}

    unsigned int Pack() const {
        return (unsigned int)Red | ((unsigned int)Green << 8) | ((unsigned int)Blue << 16) | ((unsigned int)Alpha << 24);
    }

    UIColor Blend(const UIColor& other, float amount) const {
        float t = std::clamp(amount, 0.0f, 1.0f);
        float inv = 1.0f - t;
        return UIColor(
            (int)(Red * inv + other.Red * t),
            (int)(Green * inv + other.Green * t),
            (int)(Blue * inv + other.Blue * t),
            (int)(Alpha * inv + other.Alpha * t)
        );
    }

    UIColor Fade(float amount) const {
        float t = std::clamp(amount, 0.0f, 1.0f);
        return UIColor(Red, Green, Blue, (int)(Alpha * t));
    }

    float AsFloatR() const { return Red / 255.0f; }
    float AsFloatG() const { return Green / 255.0f; }
    float AsFloatB() const { return Blue / 255.0f; }
    float AsFloatA() const { return Alpha / 255.0f; }

    static UIColor FromHex(const std::string& hex);

    // Standard Palette
    static UIColor Transparent() { return UIColor(0, 0, 0, 0); }
    static UIColor White()       { return UIColor(255, 255, 255, 255); }
    static UIColor Black()       { return UIColor(0, 0, 0, 255); }
    static UIColor Gray()        { return UIColor(128, 128, 128, 255); }
    static UIColor DarkSlate()   { return UIColor(22, 24, 29, 255); }
    static UIColor Gunmetal()    { return UIColor(30, 34, 42, 255); }
    static UIColor BlueAccent()  { return UIColor(45, 125, 245, 255); }
    static UIColor Emerald()     { return UIColor(35, 195, 120, 255); }
    static UIColor Amber()       { return UIColor(245, 160, 30, 255); }
    static UIColor Crimson()     { return UIColor(235, 55, 65, 255); }
};

class UIRectangle {
public:
    float Left = 0.0f;
    float Top = 0.0f;
    float Width = 0.0f;
    float Height = 0.0f;

    UIRectangle() = default;
    UIRectangle(float l, float t, float w, float h) : Left(l), Top(t), Width(w), Height(h) {}
    UIRectangle(const UIVector& origin, const UIVector& extent) : Left(origin.Horizontal), Top(origin.Vertical), Width(extent.Horizontal), Height(extent.Vertical) {}

    float Right() const { return Left + Width; }
    float Bottom() const { return Top + Height; }

    UIVector Origin() const { return UIVector(Left, Top); }
    UIVector Extent() const { return UIVector(Width, Height); }
    UIVector Center() const { return UIVector(Left + Width * 0.5f, Top + Height * 0.5f); }

    bool Contains(const UIVector& pt) const {
        return pt.Horizontal >= Left && pt.Horizontal < Right() &&
               pt.Vertical >= Top && pt.Vertical < Bottom();
    }

    bool Overlaps(const UIRectangle& other) const {
        return Left < other.Right() && Right() > other.Left &&
               Top < other.Bottom() && Bottom() > other.Top;
    }

    UIRectangle Shrink(float amount) const {
        return UIRectangle(Left + amount, Top + amount, std::max(0.0f, Width - amount * 2.0f), std::max(0.0f, Height - amount * 2.0f));
    }

    UIRectangle Pad(float across, float down) const {
        return UIRectangle(Left + across, Top + down, std::max(0.0f, Width - across * 2.0f), std::max(0.0f, Height - down * 2.0f));
    }

    UIRectangle Inflate(float amount) const {
        return Shrink(-amount);
    }

    UIRectangle Clip(const UIRectangle& bounds) const {
        float l = std::max(Left, bounds.Left);
        float t = std::max(Top, bounds.Top);
        float r = std::min(Right(), bounds.Right());
        float b = std::min(Bottom(), bounds.Bottom());
        return UIRectangle(l, t, std::max(0.0f, r - l), std::max(0.0f, b - t));
    }

    // Slicing operations
    UIRectangle CutTop(float amount) {
        float h = std::min(amount, Height);
        UIRectangle piece(Left, Top, Width, h);
        Top += h;
        Height -= h;
        return piece;
    }

    UIRectangle CutBottom(float amount) {
        float h = std::min(amount, Height);
        Height -= h;
        return UIRectangle(Left, Top + Height, Width, h);
    }

    UIRectangle CutLeft(float amount) {
        float w = std::min(amount, Width);
        UIRectangle piece(Left, Top, w, Height);
        Left += w;
        Width -= w;
        return piece;
    }

    UIRectangle CutRight(float amount) {
        float w = std::min(amount, Width);
        Width -= w;
        return UIRectangle(Left + Width, Top, w, Height);
    }
};

class UIPadding {
public:
    float Left = 0.0f;
    float Top = 0.0f;
    float Right = 0.0f;
    float Bottom = 0.0f;

    UIPadding() = default;
    UIPadding(float all) : Left(all), Top(all), Right(all), Bottom(all) {}
    UIPadding(float across, float down) : Left(across), Top(down), Right(across), Bottom(down) {}
    UIPadding(float l, float t, float r, float b) : Left(l), Top(t), Right(r), Bottom(b) {}
};

enum class UIAnchor {
    TopLeft,
    TopCenter,
    TopRight,
    CenterLeft,
    Center,
    CenterRight,
    BottomLeft,
    BottomCenter,
    BottomRight,
    StretchHorizontal,
    StretchVertical,
    StretchAll
};

} // namespace EunoiaUI
