libsRoot = 'lib'
package.path = package.path .. ";" .. libsRoot

local utils    = require('lib.utils')
local tracking = require( 'IceAgeAdventure.Tracking.build' )

tracking.build( '.' )
--tracking.check( '.' )

newoption {
  trigger = "ios",
  description = "Enable iOS target (requires Xcode 4)"
}
newoption {
  trigger = "android",
  description = "Generates android solution"
}
newoption {
  trigger = "rebuild",
  description = "Rebuild static library, can be followed by library name or keywords 'linked' or 'all'",
  allowed = utils.getPossibleRebuildTargets(),
}

if _OPTIONS.android then
  platform = 'andr'
elseif _ACTION == "vs2010" then  
  platform = 'win'
elseif _ACTION == "xcode4" then
  platform = 'ios'
end

-- for available libraries see  'lib/libsDb.lua'
-- for available link types see 'lib/utils.lua'
staticLinkType = 'static'
projectLinkType = 'project'

-- projects linking not supported on android
if _OPTIONS.android then
  defaultLinkType = projectLinkType
else
  defaultLinkType = staticLinkType
end

libsConfig = {
  vox        = defaultLinkType,
  tracking   = defaultLinkType,
  protobuf   = defaultLinkType,
  glwebtools = defaultLinkType,
  glf        = defaultLinkType,
  lua        = defaultLinkType,
  jsoncpp    = defaultLinkType,
  zlib       = defaultLinkType,
  savegame   = defaultLinkType,
  gaia       = defaultLinkType,
  openssl    = defaultLinkType,
  iap        = defaultLinkType,
  crypto     = defaultLinkType,
  curl       = defaultLinkType,
  theora     = defaultLinkType,
  ogg        = defaultLinkType,
  glive      = defaultLinkType,
  glsocial   = defaultLinkType,
  slimxml    = defaultLinkType,
  elephant   = staticLinkType,
  png        = staticLinkType,
  freetype   = staticLinkType,
}

buildDefinesMsvc =  { 
            "_CRT_SECURE_NO_WARNINGS",
            "_CRT_SECURE_NO_DEPRECATE",
            "GAME_WIN32",
            "NOMINMAX",
            "WIN32",
            "_WIN32"
          }

buildDefinesXcode =  {  
            "GAME_IOS",
            "_IPHONE_OS",
            "OS_IPHONE",
            "IPHONEOS"
          }
          
utils.patchAllLibs(libsRoot, platform)

if _OPTIONS.rebuild ~= nil then
  utils.rebuildLib(libsRoot, platform, libsConfig, _OPTIONS.rebuild, buildDefinesMsvc, buildDefinesXcode)
end

utils.checkLibs(libsRoot, libsConfig)

solution "IceAgeAdventure"
language "C++"
flags {"Symbols", "StaticRuntime", "NoMinimalRebuild", "NoEditAndContinue", "FloatFast"}

-- external projects doesn't work when in release config
configurations {"Release", "Debug", "Retail"}

defines {
  "GLWEBTOOLS_GLWEBTOOLS_STANDALONE",
  "PREMAKE4",
}

configuration "Debug"
  defines {"_DEBUG"}

configuration "Release"
  flags {"Optimize"}
  defines {"NDEBUG"}

configuration "Retail"
  flags {"Optimize"}
  defines {"RKRETAIL", "RETAIL"}

configuration {"vs*"} 
  buildoptions {"/EHsc", "/MP"}
  if _OPTIONS.android then
    defines { 'GAME_ANDROID' }
  else
    defines (buildDefinesMsvc)
  end 

configuration {"xcode*"}
  defines (buildDefinesXcode)

  strongFrameworks = {
    'AVFoundation',
    'AddressBook',
    'AddressBookUI',
    'AudioToolbox',
    'CFNetwork',
    'CoreGraphics',
    'CoreAudio',
    'CoreFoundation',
    'CoreLocation',
    'CoreMedia',
    'CoreMotion',
    'CoreVideo',
    'Foundation',
    'MessageUI',
    'OpenAL',
    'OpenGLES',
    'QuartzCore',
    'StoreKit',
    'SystemConfiguration',
  }
  weakFrameworks = {
    'Accounts',
    'AdSupport',
    'CoreTelephony',
    'EventKit',
    'EventKitUI',
    'GameKit',
    'iAd',
    'MapKit',
    'MediaPlayer',
    'MobileCoreServices',
    'Security',
    'Social',
    'UIKit',
  }

  utils.addFrameworksToCurrentSolutionConfiguration(strongFrameworks, weakFrameworks)
  
configuration {}

if platform == 'andr' then
  solutionLocation = path.getabsolute("prj/AndroidTemp")
elseif platform == 'win' then  
  solutionLocation = path.getabsolute("prj/Win32")
elseif platform == 'ios' then
  solutionLocation = path.getabsolute("prj/iOS")

  xcodebuildsettings {
    'INFOPLIST_FILE = "Info.plist"',
    'CODE_SIGN_IDENTITY = "iPhone Developer"',
    "SDKROOT = iphoneos",
    'ARCHS = "armv7"',
    'TARGETED_DEVICE_FAMILY = "1,2"',
    'VALID_ARCHS = "armv7"',
    'IPHONEOS_DEPLOYMENT_TARGET = "5.0"',
    'FRAMEWORK_SEARCH_PATHS = "$(SRCROOT)/../../lib/GLSocialLib/SDK/Facebook_v3/iOS"',
    'CODE_SIGN_ENTITLEMENTS = "../../lib/GLIVE_HTML5_IPHONE_IPAD/KeychainAccessGroups.plist"',
  }
end
location(solutionLocation)

include "IceAgeAdventure"
include "CasualCore"
include "GameSWF"
if _OPTIONS.android then
  include "prj/android"
  project "Android"
else
  project "IceAgeAdventure"
end
utils.addLibsToCurrentProject(libsRoot, platform, libsConfig)
utils.addProjectsToCurrentSolution(libsRoot, platform, libsConfig)
