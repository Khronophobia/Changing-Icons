@ECHO OFF

set GEODE_MOD_ID=khronophobia.changing_icons

adb start-server
geode build -p %PLATFORM%
adb push build-%PLATFORM%/%GEODE_MOD_ID%.geode storage/emulated/0/Android/media/com.geode.launcher/game/geode/mods
adb kill-server
pause