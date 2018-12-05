solution "AdventOfCode"
	configurations {"debug", "release", "final"}
	language("c++")
	includedirs{
		"include",
	}
	platforms{"x64"}
	location("build")
	debugdir("build")
	targetdir("bin")
  buildoptions_cpp {"-std=c++14" }
  linkoptions {}
	flags{"ExtraWarnings", "NoExceptions"}
	defines {}

	configuration {"final"}
		flags {"OptimizeSpeed"}
		targetsuffix("_f")
	
	configuration {"release"}
		flags {"OptimizeSpeed", "Symbols"}
		targetsuffix("_r")
		
	configuration {"debug"}
		flags {"FullSymbols", "Symbols"}
		targetsuffix("_d")
		
	configuration{"windows"}
		flags{}
		includedirs{}

    configuration {"macosx"}
    		includedirs{}

project("Puzzles")
	kind("ConsoleApp")
	flags{"ExtraWarnings", "FloatFast", "No64BitChecks"}
  buildoptions_cpp {"-std=c++14" }
	includedirs{"include" }
	files {
	  "src/**.cpp",
	}		
  excludes {
    "include/example.h",
    "src/example.cpp"
  }
	configuration "windows"
	  defines {"WIN32"}

	configuration "macosx"
	  defines {"__apple__"}
