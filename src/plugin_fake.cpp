#include "plugin.h"
#include <stdio.h>
#include "applications-page.h"
#include "command.h"
#include "settings.h"
#include "settings-dialog.h"
#include "slot.h"
#include "window.h"

#include <libxfce4ui/libxfce4ui.h>

using namespace WhiskerMenu;

//-----------------------------------------------------------------------------
extern "C" void whiskermenu_open()
{
	xfce_textdomain(GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR, "UTF-8");
	new Plugin((XfcePanelPlugin*)NULL);
}


//-----------------------------------------------------------------------------

Plugin::Plugin(XfcePanelPlugin* plugin) :
	m_plugin(plugin),
	m_window(nullptr),
	m_opacity(100),
	m_file_icon(false),
	m_hide_time(0)
{
	wm_settings = new Settings(this);
	wm_settings->load(xfce_resource_lookup(XFCE_RESOURCE_CONFIG, "xfce4/whiskermenu/defaults.rc"));
	wm_settings->m_button_title_default = wm_settings->button_title;
	wm_settings->load(xfce_resource_lookup(XFCE_RESOURCE_CONFIG, "whiskermenu-standalone.rc"));

	if (!wm_settings->button_icon_visible)
	{
		if (!wm_settings->button_title_visible)
		{
			wm_settings->button_icon_visible = true;
		}
		else if (wm_settings->button_title.empty())
		{
			wm_settings->button_title = get_button_title_default();
		}
	}

	m_window = new Window(this);
	connect(m_window->get_widget(), "hide",
		[this](GtkWidget*)
		{
			menu_hidden();
		});
	m_window->show(Window::PositionAtCursor);
}

Plugin::~Plugin()
{
	delete m_window;
	m_window = nullptr;

	gtk_widget_destroy(m_button);

	delete wm_settings;
	wm_settings = nullptr;
}

//-----------------------------------------------------------------------------

Plugin::ButtonStyle Plugin::get_button_style() const
{
	return ButtonStyle(0x1);
}

//-----------------------------------------------------------------------------

std::string Plugin::get_button_title_default()
{
	return "";
}

//-----------------------------------------------------------------------------

void Plugin::get_menu_position(int* x, int* y) const
{
	xfce_panel_plugin_position_widget(m_plugin, m_window->get_widget(), get_button(), x, y);
}

//-----------------------------------------------------------------------------

void Plugin::menu_hidden()
{
	// save();
	gtk_main_quit();
}

//-----------------------------------------------------------------------------

void Plugin::reload_button()
{
}

//-----------------------------------------------------------------------------

void Plugin::reload_menu()
{
}

//-----------------------------------------------------------------------------

void Plugin::set_button_style([[maybe_unused]] ButtonStyle style)
{
}

//-----------------------------------------------------------------------------

void Plugin::set_button_title([[maybe_unused]] const std::string& title)
{
}

//-----------------------------------------------------------------------------

void Plugin::set_button_icon_name([[maybe_unused]] const std::string& icon)
{
}

//-----------------------------------------------------------------------------

void Plugin::set_configure_enabled([[maybe_unused]] bool enabled)
{
}

//-----------------------------------------------------------------------------

void Plugin::set_loaded([[maybe_unused]] bool loaded)
{
}

//-----------------------------------------------------------------------------

void Plugin::configure()
{
}

//-----------------------------------------------------------------------------

void Plugin::icon_changed([[maybe_unused]] const gchar* icon)
{
}

//-----------------------------------------------------------------------------

void Plugin::mode_changed([[maybe_unused]] XfcePanelPluginMode mode)
{
}

//-----------------------------------------------------------------------------

gboolean Plugin::remote_event([[maybe_unused]] const gchar* name, [[maybe_unused]] const GValue* value)
{
	return true;
}

//-----------------------------------------------------------------------------

// void Plugin::save()
// {
// 	if (wm_settings->get_modified())
// 	{
// 		wm_settings->save(xfce_resource_lookup(XFCE_RESOURCE_CONFIG, "whiskermenu-standalone.rc"));
// 	}
// }

//-----------------------------------------------------------------------------

void Plugin::show_about()
{
}

//-----------------------------------------------------------------------------

gboolean Plugin::size_changed([[maybe_unused]] gint size)
{
	return true;
}

//-----------------------------------------------------------------------------

void Plugin::update_size()
{
}

//-----------------------------------------------------------------------------

void Plugin::show_menu([[maybe_unused]] int position)
{
}
