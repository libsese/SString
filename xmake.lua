set_languages("c99", "cxx11")
add_includedirs("include")

target("SString")
	set_kind("static")
	add_files("src/*.cpp")

target("TestSString")
	set_kind("binary")
	if is_os("windows") then
		add_cxxflags("/utf-8")
	end
	add_deps("SString")
	add_files("test/TestSString.cpp")

target("TestSStringBuilder")
	set_enabled(false)
	set_kind("binary")
	if is_os("windows") then
		add_cxxflags("/utf-8")
	end
	add_cxxflags("/utf-8")
	add_deps("SString")
	add_files("test/TestSStringBuilder.cpp")

target("TestAlgol")
	set_enabled(false)
	set_kind("binary")
	if is_os("windows") then
		add_cxxflags("/utf-8")
	end
	add_cxxflags("/utf-8")
	add_deps("SString")
	add_files("test/TestAlgol.cpp")
