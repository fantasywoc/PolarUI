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
    add_includedirs("src", "src/component", "src/widget", "src/animation", "src/TinyEXIF")  -- 添加动画系统头文件路径

    
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

-- 图片查看器演示程序
target("vimag-demo")
    set_kind("binary")
    add_files("src/Vimag.cpp","src/TinyEXIF/TinyEXIF.cpp")
    add_deps("ui")
    add_packages("glfw", "nanovg", "glew")
    
    -- 添加头文件搜索路径
    add_includedirs("src", "src/component", "src/widget", "src/animation", "src/TinyEXIF")
    
    -- 添加编码选项
    if is_plat("windows") then
        add_cxflags("/utf-8")
        add_links("opengl32", "gdi32", "user32", "kernel32")
    else
        add_links("GL", "X11", "pthread", "Xrandr", "Xi", "dl","m")
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

-- 开发阶段自动复制（保持现有的 after_build）
after_build(function (target)
    local targetdir = path.directory(target:targetfile())
    for _, pkg in ipairs(target:pkgs()) do
        local libfiles = pkg:get("libfiles")
        if libfiles then
            for _, libfile in ipairs(libfiles) do
                if libfile:endswith(".dll") then
                    os.cp(libfile, targetdir)
                end
            end
        end
    end
end)

-- 发布打包任务（新增）
-- 简化版打包任务
task("package")
    set_menu {
        usage = "xmake package",
        description = "Package application for distribution"
    }
    on_run(function()
        -- 检查可执行文件是否存在
        local exe_paths = {
            "build/windows/x64/release/vimag-demo.exe",
            "button-demo.exe",
            "a.exe"
        }
        
        local exe_file = nil
        for _, path in ipairs(exe_paths) do
            if os.isfile(path) then
                exe_file = path
                break
            end
        end
        
        if not exe_file then
            print("错误: 请先运行 'xmake build button-demo' 构建项目")
            return
        end
        
        -- 创建分发目录
        os.mkdir("dist")
        
        -- 复制可执行文件
        os.cp(exe_file, "dist/button-demo.exe")
        print("复制: " .. path.filename(exe_file))
        
        -- 查找并复制 DLL 文件（避免重复）
        local dll_patterns = {
            "build/**/glfw*.dll",
            "build/**/glew*.dll", 
            "build/**/nanovg*.dll",
            "**/*glfw*.dll",
            "**/*glew*.dll",
            "**/*nanovg*.dll"
        }
        
        local copied_dlls = {}
        for _, pattern in ipairs(dll_patterns) do
            local files = os.files(pattern)
            for _, file in ipairs(files) do
                if os.isfile(file) then  -- 确保是文件不是目录
                    -- 复制逻辑
                end
            end
        end
            
        print("打包完成，分发文件在 dist 目录")
        print("可以将 dist 目录整体复制给其他用户")
    end)

