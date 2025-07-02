-- -- 全局配置
-- set_policy("package.install_locally", true) -- 强制将依赖库复制到输出目录
-- set_policy("build.merge_archive", true)    -- 合并静态库依赖

-- 设置项目基本信息
add_rules("mode.debug", "mode.release")    -- 添加调试和发布模式
set_project("LiteUI")                      -- 设置项目名称
set_languages("c++17")                    -- 设置 C++ 标准
set_version("1.0.0")                      -- 设置版本号

includes("@builtin/xpack")  -- 引入XPack模块


-- 添加第三方库依赖
-- shared = true 表示使用动态链接库
add_requires("glfw 3.3.8", {configs = {shared = true}})
add_requires("nanovg", {configs = {shared = true}})
add_requires("glew", {configs = {shared = true}})

-- 定义 UI 静态库目标
target("ui")
    set_kind("static")                     -- 设置为静态库
    add_files("src/UIWindow.cpp")         -- 添加窗口相关源文件
    add_files("src/component/*.cpp")      -- 添加所有组件源文件
    add_files("src/animation/*.cpp")      -- 添加动画系统源文件
    add_files("src/utils/*.cpp")         -- 添加图像处理文件
    add_includedirs("src", "src/component", "src/animation","src/utils")  -- 添加头文件搜索路径
    add_packages("glfw", "nanovg", "glew") -- 添加依赖包

-- 定义按钮演示程序目标
target("button-demo")
    set_kind("binary")                     -- 设置为可执行文件
    add_files("src/main.cpp")             -- 添加主程序源文件
    add_deps("ui")                         -- 添加对 UI 库的依赖
    add_packages("glfw", "nanovg", "glew") -- 添加第三方依赖包
    
    -- 添加头文件搜索路径
    add_includedirs("src", "src/component", "src/widget", "src/animation")
    
    -- 根据平台添加不同的编译选项和链接库
    if is_plat("windows") then
        add_cxflags("/utf-8")             -- Windows 下启用 UTF-8 编码
        add_links("opengl32", "gdi32", "user32", "kernel32")  -- Windows 系统库
    else
        add_links("GL", "X11", "pthread", "Xrandr", "Xi", "dl")  -- Linux 系统库
    end
    
    set_rundir("$(projectdir)")           -- 设置运行目录为项目根目录
    
    -- 调试模式配置
    if is_mode("debug") then
        set_symbols("debug")              -- 启用调试符号
        set_optimize("none")              -- 禁用优化
        add_defines("DEBUG")              -- 定义 DEBUG 宏
    else
        set_optimize("fast")              -- 发布模式启用最快优化
    end
    
    add_cxxflags("/EHsc")                 -- 启用 C++ 异常处理

-- 定义图片查看器演示程序目标
target("vimag-demo")
    set_kind("binary")                     -- 设置为可执行文件
    add_rpathdirs("$ORIGIN")              -- 设置运行时库搜索路径
    add_files("src/Vimag.cpp","src/TinyEXIF/TinyEXIF.cpp")  -- 添加源文件
    add_deps("ui")                         -- 添加对 UI 库的依赖
    add_packages("glfw", "nanovg", "glew") -- 添加第三方依赖包
    
    -- 添加头文件搜索路径
    add_includedirs("src", "src/component", "src/widget", "src/animation", "src/TinyEXIF")
    
    -- 平台相关配置
    if is_plat("windows") then
        add_cxflags("/utf-8")
        add_links("opengl32", "gdi32", "user32", "kernel32")
    else
        add_links("GL", "X11", "pthread", "Xrandr", "Xi", "dl","m")
    end
    
    set_rundir("$(projectdir)")
    
    -- 调试/发布模式配置
    if is_mode("debug") then
        set_symbols("debug")
        set_optimize("none")
        add_defines("DEBUG")
    else
        set_optimize("fast")
    end
    
    add_cxxflags("/EHsc")

-- 构建后自动复制依赖 DLL 到目标目录
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



target("dist_package")
    set_kind("phony")
    add_deps("button-demo", "vimag-demo")
    on_build(function (target)
        -- 1. 复制可执行文件
        if is_plat("windows") then
            os.cp("build/windows/x64/release/*.exe", "dist")
        else
            os.cp("build/linux/x86_64/release/button-demo", "dist")
            os.cp("build/linux/x86_64/release/vimag-demo", "dist")
        end

        -- 2. 修复依赖库复制逻辑
        local packages = {"glfw", "nanovg", "glew"}
        for _, pkg_name in ipairs(packages) do
            -- ✅ 正确方式：通过 Xmake 包管理接口获取依赖
            local pkg = import("package.manager").find_package(pkg_name)
            if pkg then
                -- 获取库安装目录
                local libdir = pkg:installdir() .. "/lib"
                if is_plat("windows") then
                    os.cp(path.join(libdir, "*.dll"), "dist")  -- Windows 复制 DLL
                else
                    os.cp(path.join(libdir, "*.so*"), "dist")   -- Linux 复制 SO
                end
                print("✅ 已复制依赖库: " .. pkg_name)
            else
                print("⚠️ 警告: 依赖包未找到 - " .. pkg_name)
            end
        end

        -- 3. 复制资源文件
        os.cp("res/fonts/*.ttf", "dist")
        os.cp("res/images/*.png", "dist")
        print("🚀 打包完成！所有文件已复制到 dist 目录")
    end)