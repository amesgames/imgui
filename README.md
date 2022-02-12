This amesgames fork of Dear ImGui provides an artist/designer with the ability to generate source code that initializes a
default font and color style of their choosing.

These source files can then be checked in and used in downstream projects where ImGui is used.

This addon tool works on Windows only, using DirectX12.

Run the amesgames-style-tool.bat file from a Developer Command Prompt for VS 19 to use the tool.

Passing the following argument to the tool will cause the tool to load a TTF font and render with a particular pixel size.

```-f <path-to-ttf-font-file> <size-in-pixels>```

Multiple fonts can be provided.

The user can then use the **Tools | Style Editor** of the Dear ImGui Demo window to change the default font and the style Colors.

The **Export to Source** button can then be pressed to save the configuration to the source files.
