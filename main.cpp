#define STUI_IMPLEMENTATION
#include "stui_extensions.h"

using namespace stui;

void nextStep();

TabDisplay tab_display({ "input & output", "archive options", "other options", "operation monitor" });

// TODO: page 0 where you can read instructions and load saved configs

Page input_selection;
ListView include_list;
BorderedBox bb0(&include_list, "included files & folders");
ListView exclude_list;
BorderedBox bb1(&exclude_list, "excluded patterns");
ToggleButton exclude_toggles({ { "version control", true }, { "backup files '.~'", false }, { ".godot directories", true } });
BorderedBox bb2(&exclude_toggles, "common exclude patterns");
Button next_button("next step", nextStep);
VerticalBox vb0({ &bb1, &bb2, &next_button });
HorizontalBox hb1({ &bb0, &vb0 });
Label input_help_label(" [TAB] next field | [A] add entry | [R] remove entry | [S] save config | [L] load config | [1-4] next step ");
VerticalBox vb1({ &tab_display, &hb1, &input_help_label });
// TODO: dialog for adding files or excludes

Page archive_config;
RadioButton archive_mode({ "create", "append", "update & append" }, 2);
BorderedBox bb3(&archive_mode, "archive mode");
RadioButton compression_type({ "none", "xz", "gz", "lz" }, 1);
BorderedBox bb4(&compression_type, "compression method");
VerticalBox vb3({ &bb3, &bb4 });
Slider compression_level(3.0f / 9.0f, 1.0f / 9.0f);
Label compression_level_label("3");
VerticalBox vb5({ &compression_level, &compression_level_label });
BorderedBox bb5(&vb5, "compression level");
ToggleButton keep_uncompressed({ { "keep uncompressed tar", false } });
Slider max_threads(4.0f / 16.0f);
Label max_threads_label("4");
VerticalBox vb6({ &max_threads, &max_threads_label });
BorderedBox bb6(&vb6, "max threads");
VerticalBox vb4({ &bb5, &keep_uncompressed, &bb6, &next_button });
HorizontalBox hb2({ &vb3, &vb4 });
Label archive_help_label(" [TAB] next field | [1-4] next step ");
VerticalBox vb2({ &tab_display, &hb2, &archive_help_label });

Page additional_config;
RadioButton verbosity({ "normal", "verbose", "extra verbose" });
BorderedBox bb7(&verbosity, "verbosity");
Label additional_help_label(" [TAB] next field | [1-4] next step ");
VerticalBox vb7({ &tab_display, &bb7, &next_button, &additional_help_label });
// TODO: additional copy operations
// TODO: other command line toggles

Page execution;
// output location
// command to be executed
// capture of command output
// animation
// go! button
// progress calculator
// compress ratio information

Page* active_page = &input_selection;

void setPage(int page)
{
	switch(page)
	{
	case 0: active_page = &input_selection; break;
	case 1: active_page = &archive_config; break;
	case 2: active_page = &additional_config; break;
	case 3: active_page = &execution; break;
	}

	tab_display.current_tab = page;
}
void nextStep()
{
	if (active_page == &input_selection)
		setPage(1);
	else if (active_page == &archive_config)
		setPage(2);
	else if (active_page == &additional_config)
		setPage(3);
}

void jumpTab1()
{
	setPage(0);
}

void jumpTab2()
{
	setPage(1);
}

void jumpTab3()
{
	setPage(2);
}

void jumpTab4()
{
	setPage(3);
}

// TODO: add a bunch of size boxes
// TODO: add a bunch of help/info labels everywhere

int main()
{
	Terminal::configure("simple CLI backup tool", 1.0f);

	input_selection.setRoot(&vb1);
	input_selection.focusable_component_sequence = { &include_list, &exclude_list, &exclude_toggles, &next_button };
	input_selection.shortcuts.push_back(Input::Shortcut{ Input::Key{ '1' }, jumpTab1 });
	input_selection.shortcuts.push_back(Input::Shortcut{ Input::Key{ '2' }, jumpTab2 });
	input_selection.shortcuts.push_back(Input::Shortcut{ Input::Key{ '3' }, jumpTab3 });
	input_selection.shortcuts.push_back(Input::Shortcut{ Input::Key{ '4' }, jumpTab4 });
	// TODO: add shortcut for adding, removing elements
	// TODO: add shortcut for saving/loading config

	archive_config.setRoot(&vb2);
	archive_config.focusable_component_sequence = { &archive_mode, &compression_type, &compression_level, &keep_uncompressed, &max_threads, &next_button };
	archive_config.shortcuts.push_back(Input::Shortcut{ Input::Key{ '1' }, jumpTab1 });
	archive_config.shortcuts.push_back(Input::Shortcut{ Input::Key{ '2' }, jumpTab2 });
	archive_config.shortcuts.push_back(Input::Shortcut{ Input::Key{ '3' }, jumpTab3 });
	archive_config.shortcuts.push_back(Input::Shortcut{ Input::Key{ '4' }, jumpTab4 });
	static int system_max_threads = std::thread::hardware_concurrency();
	max_threads.step_size = 1.0f / system_max_threads;
	max_threads.value = 1.0f / system_max_threads;

	additional_config.setRoot(&vb7);
	additional_config.focusable_component_sequence = { &verbosity, &next_button };
	additional_config.shortcuts.push_back(Input::Shortcut{ Input::Key{ '1' }, jumpTab1 });
	additional_config.shortcuts.push_back(Input::Shortcut{ Input::Key{ '2' }, jumpTab2 });
	additional_config.shortcuts.push_back(Input::Shortcut{ Input::Key{ '3' }, jumpTab3 });
	additional_config.shortcuts.push_back(Input::Shortcut{ Input::Key{ '4' }, jumpTab4 });

	while(true)
	{
		active_page->checkInput();
		
		if (active_page == &input_selection)
			next_button.enabled = include_list.elements.size() > 0;
		else if (active_page == &archive_config)
		{
			next_button.enabled = true;
			compression_level.enabled = compression_type.selected_index != 0;
			max_threads.enabled = compression_level.enabled;
			keep_uncompressed.enabled = compression_level.enabled;

			compression_level.value = min(max(round(compression_level.value * 9.0f) / 9.0f, 0.0f), 1.0f);
			max_threads.value = min(max(round(max_threads.value * (float)system_max_threads) / (float)system_max_threads, 0.0f), 1.0f);

			compression_level_label.text = to_string((int)(compression_level.value * 9.0f));
			max_threads_label.text = to_string((int)(max_threads.value * (float)system_max_threads));
		}

		active_page->render();
		active_page->framerate(24);
	}

	Terminal::unConfigure(false);
	return 0;
}