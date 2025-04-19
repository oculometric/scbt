#define STUI_IMPLEMENTATION
#include "stui_extensions.h"

using namespace stui;

TabDisplay tab_display({ "info", "input", "archive options", "extra options", "execute" });

Page info_configs;
TextArea info_block("here will go some usage information");
BorderedBox bb8(&info_block);
ImageView showoff_image(nullptr, Coordinate{ 20, 80 });
HorizontalSpacer hs(2);
HorizontalBox hb3({ &bb8, &hs, &showoff_image });
Label info_help_label(" [TAB] next field | [S] save config | [L] load config | [1-5] next step ");
VerticalBox vb8({ &tab_display, &hb3, &info_help_label });
// TODO: dialog for loading/saving configs
// TODO: make the demo image and assign it

Page input_selection;
ListView include_list;
BorderedBox bb0(&include_list, "included files & folders");
ListView exclude_list;
BorderedBox bb1(&exclude_list, "excluded patterns");
ToggleButton exclude_toggles({ { "version control", true }, { "backup files '.~'", false }, { ".godot directories", true } });
BorderedBox bb2(&exclude_toggles, "common exclude patterns");
VerticalBox vb0({ &bb1, &bb2 });
HorizontalBox hb1({ &bb0, &vb0 });
Label input_help_label(" [TAB] next field | [A] add entry | [R] remove entry | [1-5] next step ");
VerticalBox vb1({ &tab_display, &hb1, &input_help_label });
// TODO: dialog for adding files or excludes

Page archive_config;
RadioButton archive_mode({ "create", "add & update" }, 2);
BorderedBox bb3(&archive_mode, "archive mode");
RadioButton compression_type({ "none", "xz", "gz", "lz" }, 1);
BorderedBox bb4(&compression_type, "compression method");
VerticalBox vb3({ &bb3, &bb4 });
Slider compression_level(3.0f / 9.0f, 1.0f / 9.0f);
Label compression_level_label("3");
VerticalBox vb5({ &compression_level, &compression_level_label });
BorderedBox bb5(&vb5, "compression level");
Slider max_threads(4.0f / 16.0f);
Label max_threads_label("4");
VerticalBox vb6({ &max_threads, &max_threads_label });
BorderedBox bb6(&vb6, "max threads");
ToggleButton keep_uncompressed({ { "keep uncompressed tar", false } });
VerticalBox vb4({ &bb5, &bb6, &keep_uncompressed });
HorizontalBox hb2({ &vb3, &vb4 });
Label archive_help_label(" [TAB] next field | [1-5] next step ");
VerticalBox vb2({ &tab_display, &hb2, &archive_help_label });

Page additional_config;
RadioButton verbosity({ "normal", "verbose", "extra verbose" });
BorderedBox bb7(&verbosity, "verbosity");
TextInputBox additional_options("");
BorderedBox bb12(&additional_options, "custom options");
Label additional_help_label(" [TAB] next field | [1-5] next step ");
VerticalBox vb7({ &tab_display, &bb7, &bb12, &additional_help_label });
// TODO: additional copy operations

Page execution;
TextInputBox output_location("archive.tar");
BorderedBox bb9(&output_location, "output file");
Label command_label("tar something", 0);
BorderedBox bb10(&command_label, "backup command");
Button execute_button("execute");
TextArea command_capture("command output here");
BorderedBox bb11(&command_capture, "command output");
ProgressBar backup_progress(0.0f);
ProgressBar ratio_bar(1.0f);
Label ratio_label("compress ratio: ");
SizeLimiter sl0(&ratio_label, Coordinate{ 16, 1 });
HorizontalBox hb4({ &sl0, &ratio_bar });
VerticalBox vb9({ &execute_button, &bb11, &backup_progress, &hb4 });
ImageView image_animation(nullptr, Coordinate{ 20, 80 });
HorizontalBox hb5({ &vb9, &image_animation });
Label execution_help_label(" [TAB] next field | [Shift+X] execute | [Shift+C] cancel | [1-5] previous step ");
VerticalBox vb10({ &tab_display, &bb9, &bb10, &hb5, &execution_help_label });

// TODO: set command label to command to execute
// TODO: set output extension automatically
// TODO: disable output location input box when running
// TODO: set command capture to output of command when executing
// TODO: max execute button do something
// TODO: animation
// TODO: calculate progress calculator
// TODO: calculate compress ratio

Page* active_page = &info_configs;

void setPage(int page)
{
	switch(page)
	{
	case 0: active_page = &info_configs; break;
	case 1: active_page = &input_selection; break;
	case 2: active_page = &archive_config; break;
	case 3: active_page = &additional_config; break;
	case 4: active_page = &execution; break;
	}

	tab_display.current_tab = page;
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

void jumpTab5()
{
	setPage(4);
}

// TODO: add a bunch of help/info labels everywhere

string generateCommand()
{
	string command_text = "tar ";

	// verbosity control
	switch (verbosity.selected_index)
	{
	case 0: break;
	case 1: command_text += "-v "; break;
	case 2: command_text += "-vv "; break;
	}


}

// WHEN RUN:
// - if creating, go ahead and run the command
// - if updating, check if the tar file exists
//   - if it doesnt, check if the tar.xz file exists
//     - if it doesnt, go into create mode
//     - if it does, decompress it into just a tar
//   - go ahead and update

// will this even work?

int main()
{
	Terminal::configure("simple CLI backup tool", 1.0f);

	info_configs.setRoot(&vb8);
	info_configs.focusable_component_sequence = { &info_block };
	info_configs.shortcuts.push_back(Input::Shortcut{ Input::Key{ '1' }, jumpTab1 });
	info_configs.shortcuts.push_back(Input::Shortcut{ Input::Key{ '2' }, jumpTab2 });
	info_configs.shortcuts.push_back(Input::Shortcut{ Input::Key{ '3' }, jumpTab3 });
	info_configs.shortcuts.push_back(Input::Shortcut{ Input::Key{ '4' }, jumpTab4 });
	info_configs.shortcuts.push_back(Input::Shortcut{ Input::Key{ '5' }, jumpTab5 });
	// TODO: add shortcut for saving/loading config

	input_selection.setRoot(&vb1);
	input_selection.focusable_component_sequence = { &include_list, &exclude_list, &exclude_toggles };
	input_selection.shortcuts.push_back(Input::Shortcut{ Input::Key{ '1' }, jumpTab1 });
	input_selection.shortcuts.push_back(Input::Shortcut{ Input::Key{ '2' }, jumpTab2 });
	input_selection.shortcuts.push_back(Input::Shortcut{ Input::Key{ '3' }, jumpTab3 });
	input_selection.shortcuts.push_back(Input::Shortcut{ Input::Key{ '4' }, jumpTab4 });
	input_selection.shortcuts.push_back(Input::Shortcut{ Input::Key{ '5' }, jumpTab5 });
	// TODO: add shortcut for adding, removing elements

	archive_config.setRoot(&vb2);
	archive_config.focusable_component_sequence = { &archive_mode, &compression_type, &compression_level, &max_threads, &keep_uncompressed };
	archive_config.shortcuts.push_back(Input::Shortcut{ Input::Key{ '1' }, jumpTab1 });
	archive_config.shortcuts.push_back(Input::Shortcut{ Input::Key{ '2' }, jumpTab2 });
	archive_config.shortcuts.push_back(Input::Shortcut{ Input::Key{ '3' }, jumpTab3 });
	archive_config.shortcuts.push_back(Input::Shortcut{ Input::Key{ '4' }, jumpTab4 });
	archive_config.shortcuts.push_back(Input::Shortcut{ Input::Key{ '5' }, jumpTab5 });
	static int system_max_threads = std::thread::hardware_concurrency();
	max_threads.step_size = 1.0f / system_max_threads;
	max_threads.value = 1.0f / system_max_threads;

	additional_config.setRoot(&vb7);
	additional_config.focusable_component_sequence = { &verbosity, &additional_options };
	additional_config.shortcuts.push_back(Input::Shortcut{ Input::Key{ '1' }, jumpTab1 });
	additional_config.shortcuts.push_back(Input::Shortcut{ Input::Key{ '2' }, jumpTab2 });
	additional_config.shortcuts.push_back(Input::Shortcut{ Input::Key{ '3' }, jumpTab3 });
	additional_config.shortcuts.push_back(Input::Shortcut{ Input::Key{ '4' }, jumpTab4 });
	additional_config.shortcuts.push_back(Input::Shortcut{ Input::Key{ '5' }, jumpTab5 });

	execution.setRoot(&vb10);
	execution.focusable_component_sequence = { &output_location, &execute_button, &command_capture };
	execution.shortcuts.push_back(Input::Shortcut{ Input::Key{ '1' }, jumpTab1 });
	execution.shortcuts.push_back(Input::Shortcut{ Input::Key{ '2' }, jumpTab2 });
	execution.shortcuts.push_back(Input::Shortcut{ Input::Key{ '3' }, jumpTab3 });
	execution.shortcuts.push_back(Input::Shortcut{ Input::Key{ '4' }, jumpTab4 });
	execution.shortcuts.push_back(Input::Shortcut{ Input::Key{ '5' }, jumpTab5 });
	// TODO: cancel and execute shortcuts
	
	while(true)
	{
		active_page->checkInput();
		
		if (active_page == &archive_config)
		{
			compression_level.enabled = compression_type.selected_index != 0;
			max_threads.enabled = compression_level.enabled;
			keep_uncompressed.enabled = compression_level.enabled;

			compression_level.value = min(max(round(compression_level.value * 9.0f) / 9.0f, 0.0f), 1.0f);
			max_threads.value = min(max(round(max_threads.value * (float)system_max_threads) / (float)system_max_threads, 0.0f), 1.0f);

			compression_level_label.text = to_string((int)(compression_level.value * 9.0f));
			max_threads_label.text = to_string((int)(max_threads.value * (float)system_max_threads));
		}
		else if (active_page == &execution)
		{
			command_label = generateCommand();
		}

		active_page->render();
		active_page->framerate(24);
	}

	Terminal::unConfigure(false);
	return 0;
}