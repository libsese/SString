set_project("SString")
set_version("0.1", {build = "%Y%m%d%H%M"})
set_allowedplats("windows", "linux", "macosx")
set_allowedarchs("x64", "x86")
set_languages("c99", "cxx11")
add_includedirs("include")

if is_os("windows") then
	add_cxxflags("/utf-8")
end

target("SString")
	set_kind("static")
	add_files("src/*.cpp")

target("TestSString")
	set_enabled(false)
	set_kind("binary")
	add_deps("SString")
	add_files("test/TestSString.cpp")

target("TestSStringBuilder")
	set_enabled(false)
	set_kind("binary")
	add_deps("SString")
	add_files("test/TestSStringBuilder.cpp")

target("TestAlgol")
	set_enabled(false)
	set_kind("binary")
	add_deps("SString")
	add_files("test/TestAlgol.cpp")
