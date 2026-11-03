# EunoiaUI Framework

A fast, modern, hardware-accelerated **C++20 UI Framework System** designed for the **Eunoia Engine**, powering both **In-Game UI Systems** (HUDs, health bars, player crosshairs, menus, dialogues) and **Engine UI Systems** (Editor panels, docking, layout tables, inspectors).

## Features
- **Dual-Paradigm Architecture**:
  - **Immediate-Mode API**: Fast, zero-allocation UI for tools, debug telemetry, and immediate widgets (`Frames`, `Widgets`, `Layout`).
  - **Retained-Mode In-Game Tree**: Responsive canvas with anchors, pivots, auto-scaling, and prebuilt game elements (`InGameHealthBar`, `InGameCrosshair`, `InGamePanel`, `InGameButton`, `InGameLabel`).
- **High-Performance Draw Pipeline**:
  - Batched 2D canvas with anti-aliased rounded rectangles, gradients, drop shadows, borders, circles, lines, and textures.
  - Built-in embedded crisp font rasterizer with zero external dependencies.
- **Renderer Backends**:
  - Native hardware DirectX 12 renderer.
  - Zero-overhead Engine Viewport Bridge.

## Directory Structure
- `Include/EunoiaUI/Core`: Vectors, Rectangles, Colors, Padding, Anchors, Arena Allocator, Context, Input, Styles.
- `Include/EunoiaUI/Draw`: Canvas, Draw Instances, Batches, Channels, Draw Data, Graphics Interface.
- `Include/EunoiaUI/Text`: Font metrics, glyph rasterization, and texture sheet atlas.
- `Include/EunoiaUI/Layout`: Flexible layout cursor, SameLine, child regions, resizable tables.
- `Include/EunoiaUI/Widgets`: Labels, buttons, checkboxes, toggles, sliders, fields, progress bars, tooltips.
- `Include/EunoiaUI/Desktop`: Movable, collapsible, and dockable frames/windows.
- `Include/EunoiaUI/InGame`: Responsive in-game HUD canvas and prebuilt gameplay elements.
- `Include/EunoiaUI/Backend`: DirectX 12 and Engine Viewport Bridge.
