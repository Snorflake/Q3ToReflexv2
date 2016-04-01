workspace "Q3ReflexMapConverter"
	configurations {"Debug", "Release"}

project "Q3ReflexMapConverter"
	kind "ConsoleApp"
	language "C++"
	characterset "MBCS"
	targetdir "bin/%{cfg.buildcfg}"
	buildoptions { "-std=c++14" }
	files {"src/**.h", "src/**.hpp", "src/**.cpp"}
	filter "configurations:Debug"
		flags{"Symbols"}
		defines {"_CRT_SECURE_NO_WARNINGS"}
	filter "configurations:Release"
		defines {"NDEBUG", "_CRT_SECURE_NO_WARNINGS"}
