# QtUiStyle

Reusable Qt 6 Widgets components for consistently styled, frameless windows and dialogs.

## Requirements

- Qt 6.4 or newer, with the Widgets component
- CMake 3.16 or newer
- A C++17 compiler

## CMake usage

```cmake
find_package(QtUiStyle CONFIG REQUIRED)

target_link_libraries(my_app PRIVATE QtUiStyle::Core)
# Or, for the convenience dialogs as well:
target_link_libraries(my_app PRIVATE QtUiStyle::Dialogs)
```

The package can also be added directly from a pinned release:

```cmake
include(FetchContent)
FetchContent_Declare(QtUiStyle
    GIT_REPOSITORY https://github.com/5250ng/qt-ui-style.git
    GIT_TAG v0.1.0
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(QtUiStyle)
target_link_libraries(my_app PRIVATE QtUiStyle::Dialogs)
```

`add_subdirectory` is supported as well; the target names stay the same.

## Components

- `QtUiStyle::Core`: `TitleBar`, `BaseFramelessWindow`, `BaseFramelessDialog`, and application-wide title-bar styling.
- `QtUiStyle::Dialogs`: message, input, color, and file dialogs using the frameless shell.

Applications can set a shared title-bar appearance without coupling the library to their theme system:

```cpp
#include <QtUiStyle/StyleManager.h>

qt_ui_style::TitleBarStyle style;
style.background = QColor("#252526");
style.foreground = QColor("#d4d4d4");
style.buttonHover = QColor("#3c3c3c");
style.closeButtonHover = QColor("#e81123");
style.separator = QColor("#3c3c3c");
qt_ui_style::StyleManager::instance().setTitleBarStyle(style);
```

Invalid colors use the active Qt application palette, so explicit styling is optional.

## License

MIT
