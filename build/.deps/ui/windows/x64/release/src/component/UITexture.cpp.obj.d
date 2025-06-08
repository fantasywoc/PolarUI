{
    values = {
        [[E:\Program\VS2022\VS\VC\Tools\MSVC\14.43.34808\bin\HostX64\x64\cl.exe]],
        {
            "-nologo",
            "-MD",
            "-O2",
            "-std:c++17",
            "-Isrc",
            [[-Isrc\component]],
            [[-Isrc\animation]],
            "-DGLFW_INCLUDE_NONE",
            "-DGLEW_NO_GLU",
            "/EHsc",
            "-external:W0",
            [[-external:IC:\Users\CK\AppData\Local\.xmake\packages\g\glfw\3.3.8\7f33961a41e640bfa5bdb78428d3267d\include]],
            "-external:W0",
            [[-external:IC:\Users\CK\AppData\Local\.xmake\packages\n\nanovg\2023.8.27\cb0b287f2781439aaebe7fcc2bebc050\include]],
            "-external:W0",
            [[-external:IC:\Users\CK\AppData\Local\.xmake\packages\g\glew\2.2.0\7e77cab88e014bf2a64860fbbda75159\include]],
            "-DNDEBUG"
        }
    },
    depfiles = "{\
    \"Version\": \"1.2\",\
    \"Data\": {\
        \"Source\": \"e:\\\\program\\\\learning-progect\\\\ui\\\\src\\\\component\\\\uitexture.cpp\",\
        \"ProvidedModule\": \"\",\
        \"Includes\": [\
            \"e:\\\\program\\\\learning-progect\\\\ui\\\\src\\\\component\\\\uitexture.h\",\
            \"e:\\\\program\\\\learning-progect\\\\ui\\\\src\\\\component\\\\uicomponent.h\",\
            \"c:\\\\users\\\\ck\\\\appdata\\\\local\\\\.xmake\\\\packages\\\\n\\\\nanovg\\\\2023.8.27\\\\cb0b287f2781439aaebe7fcc2bebc050\\\\include\\\\nanovg.h\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\string\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\yvals_core.h\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\vcruntime.h\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\sal.h\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\concurrencysal.h\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\vadefs.h\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\xkeycheck.h\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\xstring\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\__msvc_sanitizer_annotate_container.hpp\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\__msvc_string_view.hpp\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\iosfwd\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\yvals.h\",\
            \"e:\\\\windows kits\\\\10\\\\include\\\\10.0.22621.0\\\\ucrt\\\\crtdbg.h\",\
            \"e:\\\\windows kits\\\\10\\\\include\\\\10.0.22621.0\\\\ucrt\\\\corecrt.h\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\vcruntime_new_debug.h\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\vcruntime_new.h\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\crtdefs.h\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\use_ansi.h\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\cstdio\",\
            \"e:\\\\windows kits\\\\10\\\\include\\\\10.0.22621.0\\\\ucrt\\\\stdio.h\",\
            \"e:\\\\windows kits\\\\10\\\\include\\\\10.0.22621.0\\\\ucrt\\\\corecrt_wstdio.h\",\
            \"e:\\\\windows kits\\\\10\\\\include\\\\10.0.22621.0\\\\ucrt\\\\corecrt_stdio_config.h\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\cstring\",\
            \"e:\\\\windows kits\\\\10\\\\include\\\\10.0.22621.0\\\\ucrt\\\\string.h\",\
            \"e:\\\\windows kits\\\\10\\\\include\\\\10.0.22621.0\\\\ucrt\\\\corecrt_memory.h\",\
            \"e:\\\\windows kits\\\\10\\\\include\\\\10.0.22621.0\\\\ucrt\\\\corecrt_memcpy_s.h\",\
            \"e:\\\\windows kits\\\\10\\\\include\\\\10.0.22621.0\\\\ucrt\\\\errno.h\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\vcruntime_string.h\",\
            \"e:\\\\windows kits\\\\10\\\\include\\\\10.0.22621.0\\\\ucrt\\\\corecrt_wstring.h\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\cwchar\",\
            \"e:\\\\windows kits\\\\10\\\\include\\\\10.0.22621.0\\\\ucrt\\\\wchar.h\",\
            \"e:\\\\windows kits\\\\10\\\\include\\\\10.0.22621.0\\\\ucrt\\\\corecrt_wconio.h\",\
            \"e:\\\\windows kits\\\\10\\\\include\\\\10.0.22621.0\\\\ucrt\\\\corecrt_wctype.h\",\
            \"e:\\\\windows kits\\\\10\\\\include\\\\10.0.22621.0\\\\ucrt\\\\corecrt_wdirect.h\",\
            \"e:\\\\windows kits\\\\10\\\\include\\\\10.0.22621.0\\\\ucrt\\\\corecrt_wio.h\",\
            \"e:\\\\windows kits\\\\10\\\\include\\\\10.0.22621.0\\\\ucrt\\\\corecrt_share.h\",\
            \"e:\\\\windows kits\\\\10\\\\include\\\\10.0.22621.0\\\\ucrt\\\\corecrt_wprocess.h\",\
            \"e:\\\\windows kits\\\\10\\\\include\\\\10.0.22621.0\\\\ucrt\\\\corecrt_wstdlib.h\",\
            \"e:\\\\windows kits\\\\10\\\\include\\\\10.0.22621.0\\\\ucrt\\\\corecrt_wtime.h\",\
            \"e:\\\\windows kits\\\\10\\\\include\\\\10.0.22621.0\\\\ucrt\\\\sys\\\\stat.h\",\
            \"e:\\\\windows kits\\\\10\\\\include\\\\10.0.22621.0\\\\ucrt\\\\sys\\\\types.h\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\xtr1common\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\xutility\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\__msvc_iter_core.hpp\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\utility\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\initializer_list\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\cstddef\",\
            \"e:\\\\windows kits\\\\10\\\\include\\\\10.0.22621.0\\\\ucrt\\\\stddef.h\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\type_traits\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\cstdint\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\stdint.h\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\climits\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\limits.h\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\cstdlib\",\
            \"e:\\\\windows kits\\\\10\\\\include\\\\10.0.22621.0\\\\ucrt\\\\math.h\",\
            \"e:\\\\windows kits\\\\10\\\\include\\\\10.0.22621.0\\\\ucrt\\\\corecrt_math.h\",\
            \"e:\\\\windows kits\\\\10\\\\include\\\\10.0.22621.0\\\\ucrt\\\\stdlib.h\",\
            \"e:\\\\windows kits\\\\10\\\\include\\\\10.0.22621.0\\\\ucrt\\\\corecrt_malloc.h\",\
            \"e:\\\\windows kits\\\\10\\\\include\\\\10.0.22621.0\\\\ucrt\\\\corecrt_search.h\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\xmemory\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\limits\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\cfloat\",\
            \"e:\\\\windows kits\\\\10\\\\include\\\\10.0.22621.0\\\\ucrt\\\\float.h\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\intrin0.h\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\intrin0.inl.h\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\new\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\exception\",\
            \"e:\\\\windows kits\\\\10\\\\include\\\\10.0.22621.0\\\\ucrt\\\\malloc.h\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\vcruntime_exception.h\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\eh.h\",\
            \"e:\\\\windows kits\\\\10\\\\include\\\\10.0.22621.0\\\\ucrt\\\\corecrt_terminate.h\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\xatomic.h\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\xpolymorphic_allocator.h\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\tuple\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\cctype\",\
            \"e:\\\\windows kits\\\\10\\\\include\\\\10.0.22621.0\\\\ucrt\\\\ctype.h\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\functional\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\typeinfo\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\vcruntime_typeinfo.h\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\unordered_map\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\xhash\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\cmath\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\list\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\vector\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\__msvc_bit_utils.hpp\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\xbit_ops.h\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\xnode_handle.h\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\memory\",\
            \"e:\\\\program\\\\learning-progect\\\\ui\\\\src\\\\component\\\\uievent.h\",\
            \"e:\\\\program\\\\learning-progect\\\\ui\\\\src\\\\animation\\\\uianimation.h\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\iostream\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\istream\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\__msvc_ostream.hpp\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\ios\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\xlocnum\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\iterator\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\streambuf\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\xiosbase\",\
            \"e:\\\\windows kits\\\\10\\\\include\\\\10.0.22621.0\\\\ucrt\\\\share.h\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\system_error\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\__msvc_system_error_abi.hpp\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\cerrno\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\stdexcept\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\xcall_once.h\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\xerrc.h\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\atomic\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\xthreads.h\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\__msvc_threads_core.hpp\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\xtimec.h\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\ctime\",\
            \"e:\\\\windows kits\\\\10\\\\include\\\\10.0.22621.0\\\\ucrt\\\\time.h\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\xlocale\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\xfacet\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\xlocinfo\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\__msvc_xlocinfo_types.hpp\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\clocale\",\
            \"e:\\\\windows kits\\\\10\\\\include\\\\10.0.22621.0\\\\ucrt\\\\locale.h\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\ostream\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\algorithm\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\__msvc_heap_algorithms.hpp\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\__msvc_minmax.hpp\",\
            \"e:\\\\program\\\\learning-progect\\\\ui\\\\src\\\\component\\\\stb_image.h\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\stdarg.h\",\
            \"e:\\\\windows kits\\\\10\\\\include\\\\10.0.22621.0\\\\ucrt\\\\math.h\",\
            \"e:\\\\windows kits\\\\10\\\\include\\\\10.0.22621.0\\\\ucrt\\\\assert.h\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\emmintrin.h\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\xmmintrin.h\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\mmintrin.h\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\intrin.h\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\setjmp.h\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\immintrin.h\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\wmmintrin.h\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\nmmintrin.h\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\smmintrin.h\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\tmmintrin.h\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\pmmintrin.h\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\zmmintrin.h\",\
            \"e:\\\\program\\\\vs2022\\\\vs\\\\vc\\\\tools\\\\msvc\\\\14.43.34808\\\\include\\\\ammintrin.h\"\
        ],\
        \"ImportedModules\": [],\
        \"ImportedHeaderUnits\": []\
    }\
}",
    files = {
        [[src\component\UITexture.cpp]]
    },
    depfiles_format = "cl_json"
}