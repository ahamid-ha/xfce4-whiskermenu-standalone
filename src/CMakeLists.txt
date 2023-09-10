# SPDX-FileCopyrightText: 2013-2022 Graeme Gott <graeme@gottcode.org>
#
# SPDX-License-Identifier: GPL-2.0-or-later

find_package(PkgConfig REQUIRED)
pkg_check_modules(gio REQUIRED IMPORTED_TARGET gio-2.0)
pkg_check_modules(GTK3 REQUIRED IMPORTED_TARGET gtk+-3.0>=3.22)
pkg_check_modules(exo REQUIRED IMPORTED_TARGET exo-2>=0.12)
pkg_check_modules(garcon REQUIRED IMPORTED_TARGET garcon-1>=0.6.4)
pkg_check_modules(libxfce4panel REQUIRED IMPORTED_TARGET libxfce4panel-2.0>=4.14)
pkg_check_modules(libxfce4ui REQUIRED IMPORTED_TARGET libxfce4ui-2>=4.14)
pkg_check_modules(libxfce4util REQUIRED IMPORTED_TARGET libxfce4util-1.0>=4.14)
pkg_check_modules(libxfconf REQUIRED IMPORTED_TARGET libxfconf-0>=4.14)
if(ENABLE_ACCOUNTSSERVICE)
	pkg_check_modules(accountsservice IMPORTED_TARGET accountsservice>=0.6.45)
endif()

set(WHISKERMENU_PLUGIN_PATH "../external/xfce4-whiskermenu-plugin")

add_executable(whiskermenu_standalone
	main.c
	plugin_fake.cpp
	${WHISKERMENU_PLUGIN_PATH}/panel-plugin/applications-page.cpp
	${WHISKERMENU_PLUGIN_PATH}/panel-plugin/category.cpp
	${WHISKERMENU_PLUGIN_PATH}/panel-plugin/category-button.cpp
	${WHISKERMENU_PLUGIN_PATH}/panel-plugin/command.cpp
	${WHISKERMENU_PLUGIN_PATH}/panel-plugin/command-edit.cpp
	${WHISKERMENU_PLUGIN_PATH}/panel-plugin/element.cpp
	${WHISKERMENU_PLUGIN_PATH}/panel-plugin/favorites-page.cpp
	${WHISKERMENU_PLUGIN_PATH}/panel-plugin/icon-renderer.cpp
	${WHISKERMENU_PLUGIN_PATH}/panel-plugin/icon-size.cpp
	${WHISKERMENU_PLUGIN_PATH}/panel-plugin/image-menu-item.h
	${WHISKERMENU_PLUGIN_PATH}/panel-plugin/launcher.cpp
	${WHISKERMENU_PLUGIN_PATH}/panel-plugin/launcher-icon-view.cpp
	${WHISKERMENU_PLUGIN_PATH}/panel-plugin/launcher-tree-view.cpp
	${WHISKERMENU_PLUGIN_PATH}/panel-plugin/page.cpp
	${WHISKERMENU_PLUGIN_PATH}/panel-plugin/profile.cpp
	${WHISKERMENU_PLUGIN_PATH}/panel-plugin/query.cpp
	${WHISKERMENU_PLUGIN_PATH}/panel-plugin/recent-page.cpp
	${WHISKERMENU_PLUGIN_PATH}/panel-plugin/run-action.cpp
	${WHISKERMENU_PLUGIN_PATH}/panel-plugin/search-action.cpp
	${WHISKERMENU_PLUGIN_PATH}/panel-plugin/search-page.cpp
	${WHISKERMENU_PLUGIN_PATH}/panel-plugin/settings.cpp
	${WHISKERMENU_PLUGIN_PATH}/panel-plugin/settings-dialog.cpp
	${WHISKERMENU_PLUGIN_PATH}/panel-plugin/slot.h
	${WHISKERMENU_PLUGIN_PATH}/panel-plugin/window.cpp)

target_include_directories(whiskermenu_standalone PRIVATE ${WHISKERMENU_PLUGIN_PATH}/panel-plugin/)

target_link_libraries(whiskermenu_standalone PRIVATE
	PkgConfig::GTK3
	PkgConfig::exo
	PkgConfig::garcon
	PkgConfig::libxfce4panel
	PkgConfig::libxfce4ui
	PkgConfig::libxfce4util
	PkgConfig::libxfconf)

target_compile_options(whiskermenu_standalone PRIVATE -Wall -Wextra)

target_compile_definitions(whiskermenu_standalone PRIVATE
	GETTEXT_PACKAGE="xfce4-whiskermenu-plugin"
	PACKAGE_LOCALE_DIR="${CMAKE_INSTALL_FULL_LOCALEDIR}"
	PACKAGE_NAME="xfce4-whiskermenu-plugin"
	BINDIR="${CMAKE_INSTALL_FULL_BINDIR}"
	DATADIR="${CMAKE_INSTALL_FULL_DATADIR}"
	SETTINGS_MENUFILE="${CMAKE_INSTALL_FULL_SYSCONFDIR}/xdg/menus/xfce-settings-manager.menu"
	G_LOG_DOMAIN="whiskermenu"
	GSEAL_ENABLE
	GTK_MULTIHEAD_SAFE
	$<$<NOT:$<CONFIG:Debug>>:G_DISABLE_ASSERT G_DISABLE_CAST_CHECKS>)

target_compile_features(whiskermenu_standalone PRIVATE cxx_std_11)

if (accountsservice_FOUND)
	target_link_libraries(whiskermenu_standalone PRIVATE
		PkgConfig::accountsservice)
	target_compile_definitions(whiskermenu_standalone PRIVATE
		HAS_ACCOUNTSERVICE)
endif()

if(ENABLE_VISIBILITY)
	set_target_properties(whiskermenu_standalone PROPERTIES CXX_VISIBILITY_PRESET hidden)
endif()

if(ENABLE_AS_NEEDED)
	target_link_options(whiskermenu_standalone PRIVATE "LINKER:--as-needed")
endif()

if(ENABLE_LINKER_OPTIMIZED_HASH_TABLES)
	target_link_options(whiskermenu_standalone PRIVATE "LINKER:-O1")
endif()

if(ENABLE_DEVELOPER_MODE)
	target_link_options(whiskermenu_standalone PRIVATE "LINKER:-z,defs")
endif()

if(ENABLE_DEVELOPER_MODE)
	target_compile_options(whiskermenu_standalone PRIVATE -Werror)
	target_compile_definitions(whiskermenu_standalone PRIVATE
		XFCE_DISABLE_DEPRECATED
		G_DISABLE_DEPRECATED
		GDK_DISABLE_DEPRECATED
		GDK_PIXBUF_DISABLE_DEPRECATED
		GDK_DISABLE_SINGLE_INCLUDES
		GDK_PIXBUF_DISABLE_SINGLE_INCLUDES
		GTK_DISABLE_SINGLE_INCLUDES)
endif()

if(ENABLE_LINK_TIME_OPTIMIZATION)
	include(CheckIPOSupported)
	check_ipo_supported(RESULT result)
	if(result)
		set_target_properties(whiskermenu_standalone PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
	endif()
endif()

if(ENABLE_STRIP)
	add_custom_command(TARGET whiskermenu_standalone POST_BUILD
		COMMAND ${CMAKE_STRIP} $<TARGET_FILE:whiskermenu>)
endif()

install(TARGETS whiskermenu_standalone DESTINATION ${CMAKE_INSTALL_BINDIR})
