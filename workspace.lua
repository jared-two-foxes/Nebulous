

local workspace = {}

workspace.name = 'NebulaeEngine'
workspace.system = "premake5"

workspace.libraries = {
	nebulous_common = {
	    name = 'nebulous_common',
	    path = 'C:/Develop/Nebulous/Nebulae/Common',      
	    system = 'premake',
	    dependencies = { 'boost', 'brofiler', 'jsoncpp' },
	    naming = "standard",
	},
	nebulous_alpha = {
	    name = 'nebulous_alpha',
	    path = 'C:/Develop/Nebulous/Nebulae/Alpha',      
	    system = 'premake',
	    dependencies = { 'nebulous_common', 'libpng' },
	    naming = "standard",
	},
	nebulous_beta = {
	    name = 'nebulous_beta',
	    path = 'C:/Develop/Nebulous/Nebulae/Beta',      
	    system = 'premake',
	    dependencies = { 'nebulous_alpha', 'freetype', 'utf8' },
	    naming = "standard",
	},
	nebulous_audio = {
	    name = 'nebulous_audio',
	    path = 'C:/Develop/Nebulous/Nebulae/Audio',      
	    system = 'premake',
	    dependencies = { 'nebulous_common', 'openal' },
	    naming = "standard",
	},
	nebulous_gl3 = {
	    name = 'nebulous_gl3',
	    path = 'C:/Develop/Nebulous/RenderSystems/GL',      
	    system = 'premake',
	    dependencies = { 'nebulous_alpha', 'opengl' },
	    naming = "standard",
	},
}

workspace.binaries = {
	nebulous_tests = {
	    name = 'nebulous_tests',
	    path = 'C:/Develop/Nebulous/tests',      
	    system = 'premake',
	    dependencies = { 'nebulous_beta', 'gmock' },
	    defines = { 'GTEST_LINKED_AS_SHARED_LIBRARY' },
	    naming = "standard",
	},	
}

return workspace