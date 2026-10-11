# MSVC's custom init_seg suppresses CRT startup construction and redirects
# destructor registration. Writable data and local-static guards have their own
# sections, so a scene reload starts from the pre-constructor image.
# https://learn.microsoft.com/en-us/cpp/preprocessor/init-seg
function(dqiv_overlay_source id target)
    set(header "${CMAKE_CURRENT_BINARY_DIR}/overlays/module${id}.hpp")
    file(GENERATE OUTPUT "${header}" CONTENT "#pragma once\n#pragma warning(disable:4075) // Custom initialization is intentional.\nextern \"C\" int __cdecl pcOverlayExit${id}(void (__cdecl*)());\n#pragma init_seg(\".i$${id}m\", pcOverlayExit${id})\n#pragma data_seg(\".d$${id}m\")\n#pragma bss_seg(\".b$${id}m\")\n")
    foreach(source IN LISTS ARGN)
        set_property(SOURCE "${source}" APPEND PROPERTY COMPILE_OPTIONS
            "$<$<STREQUAL:$<TARGET_PROPERTY:NAME>,${target}>:/FI${header}>"
            "$<$<STREQUAL:$<TARGET_PROPERTY:NAME>,${target}>:/Gw->"
            "$<$<STREQUAL:$<TARGET_PROPERTY:NAME>,${target}>:/Zc:threadSafeInit->")
    endforeach()
endfunction()

function(dqiv_overlay_registry target registry)
    set(source "${CMAKE_CURRENT_BINARY_DIR}/overlays/${registry}.cpp")
    set(code "#include \"NitroOverlays.hpp\"\n#include <span>\n")
    set(entries "")
    foreach(id IN LISTS ARGN)
        string(APPEND code "extern \"C\" int __cdecl pcOverlayExit${id}(pc::OverlayFunction f) { return pcRegisterOverlayDestructor(${id},f); }\n")
        foreach(segment IN ITEMS d)
            string(APPEND code "#pragma section(\".${segment}$${id}a\",read,write)\n#pragma section(\".${segment}$${id}z\",read,write)\n__declspec(allocate(\".${segment}$${id}a\")) unsigned char ${segment}${id}Begin=0;\n__declspec(allocate(\".${segment}$${id}z\")) unsigned char ${segment}${id}End=0;\n")
        endforeach()
        string(APPEND code "#pragma bss_seg(\".b$${id}a\")\nunsigned char b${id}Begin;\n#pragma bss_seg(\".b$${id}z\")\nunsigned char b${id}End;\n#pragma bss_seg()\n")
        string(APPEND code "#pragma section(\".i$${id}a\",read)\n#pragma section(\".i$${id}z\",read)\n__declspec(allocate(\".i$${id}a\")) const pc::OverlayFunction i${id}Begin=nullptr;\n__declspec(allocate(\".i$${id}z\")) const pc::OverlayFunction i${id}End=nullptr;\n")
        string(APPEND entries "{${id},{&d${id}Begin+1,&d${id}End},{&b${id}Begin+1,&b${id}End},&i${id}Begin+1,&i${id}End},\n")
    endforeach()
    string(APPEND code "std::span<const pc::OverlayImage> ${registry}() { static const pc::OverlayImage images[]={\n${entries}};return images;}\n")
    file(GENERATE OUTPUT "${source}" CONTENT "${code}")
    target_sources(${target} PRIVATE "${source}")
endfunction()
