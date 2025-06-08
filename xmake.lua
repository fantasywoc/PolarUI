add_rules("mode.debug", "mode.release")
set_project("LiteUI")
set_languages("c++17")
set_version("1.0.0")

add_requires("glfw 3.3.8", {configs = {shared = true}})
add_requires("nanovg", {configs = {shared = true}})
add_requires("glew", {configs = {shared = true}})

target("ui")
    set_kind("static")
    add_files("src/UIWindow.cpp")
    add_files("src/component/*.cpp")
    add_files("src/animation/*.cpp")  -- 添加动画系统源文件
    add_includedirs("src", "src/component", "src/animation")  -- 添加动画系统头文件路径
    add_packages("glfw", "nanovg", "glew")

target("button-demo")
    set_kind("binary")
    add_files("src/main.cpp")
    add_deps("ui")
    add_packages("glfw", "nanovg", "glew")
    
    -- 添加头文件搜索路径
    add_includedirs("src", "src/component", "src/widget", "src/animation")  -- 添加动画系统头文件路径
    
    -- 添加编码选项
    if is_plat("windows") then
        add_cxflags("/utf-8")
        add_links("opengl32", "gdi32", "user32", "kernel32")
    else
        add_links("GL", "X11", "pthread", "Xrandr", "Xi", "dl")
    end
    
    -- 设置运行目录
    set_rundir("$(projectdir)")
    
    -- 添加调试信息
    if is_mode("debug") then
        set_symbols("debug")
        set_optimize("none")
        add_defines("DEBUG")
    else
        set_optimize("fast")
    end
    
    add_cxxflags("/EHsc")

-- 剪切板演示程序
target("clipboard-demo")
    set_kind("binary")
    add_files("src/clipboard_demo.cpp")
    add_deps("ui")
    add_packages("glfw", "nanovg", "glew")
    
    -- 添加头文件搜索路径
    add_includedirs("src", "src/component", "src/widget", "src/animation")  -- 添加动画系统头文件路径
    
    -- 添加编码选项
    if is_plat("windows") then
        add_cxflags("/utf-8")
        add_links("opengl32", "gdi32", "user32", "kernel32")
    else
        add_links("GL", "X11", "pthread", "Xrandr", "Xi", "dl")
    end
    
    -- 设置运行目录
    set_rundir("$(projectdir)")
    
    -- 添加调试信息
    if is_mode("debug") then
        set_symbols("debug")
        set_optimize("none")
        add_defines("DEBUG")
    else
        set_optimize("fast")
    end
    
    add_cxxflags("/EHsc")

