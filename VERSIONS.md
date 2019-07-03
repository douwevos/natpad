# 2.5

* Fixed occasional incorrect positioning in multi-byte UTF-8 characters.
* Fixed issue with low performance in overwrite mode.
* Handle opening of empty files and non-existing files correctly
* By default open an empty basic text editor
* Fixed layout sometimes not being updated when undoing.
* allow user to disable the 'mark occurrences' feature
![Mark occurrences configuration](/site/version-2.5/Allow_disabling_of_mark_occurrences.png)
* Replay macro now has a short-key
![Replacy macro shortkey](/site/version-2.5/Short_key_for_macro_replay.png)
* Added quick-toggle icons to status bar for toggling 'line-wrapping', 'show-whitespace-characters', 'mark-occurrences' and 'spelling'
![Quick toggle actions](/site/version-2.5/Quick_toggle_actions_in_status_bar.png)
* Fixed issues with crashes in big-file-mode.


# 2.4

* Allow the user to see the different types of line-endings.
![Line endings](/site/version-2.4/Visible_line_endings.png)
* Allow the user to change the line-endings of a file
![Change Line endings](/site/version-2.4/Change_line_endings.png)
* Greatly improved stability of loading large files
* Improve performance of moving cursor and editing in large files
* Added seperate configuration for large files. Large files now use monospace fonts only and force non-line-wraping. A option was added to force ascii mode when calculating line width.
![big-mode configuration](/site/version-2.4/Seperate_big_mode_config.png)
* Spelling capability enabled for raw-text editors. 
![Spelling in text files](/site/version-2.4/Spelling_in_text_files.png)
* Removed unstable Java decompiler and replaced it with a disassembled view.
* Fixed memory leakage.
