# Game Project : A gift to my girlfriend Ellie

## How to run

```bat
xmake f --mode=release
xmake
xmake run
```

## ONLY AVAILABLE ON WINDOWS.

## The plan

- Refactor to D3D12
    - Staging buffers/textures
    - Loading textures
    - Loading floating point textures
- Start rendering
    - Render graph
    - Skybox renderer
    - Deferred renderer
    - Opaque pass
    - Shadows
    - Anti-aliasing (MSAA/FXAA)
    - Bloom
    - Color correction
    - Accessibility (color correction for daltonism)
    - Blit
    - UI
- Map system
- Physics
- Scripting
- Polish, publish on windows
- Reimplement engine backend on different platforms if needed
