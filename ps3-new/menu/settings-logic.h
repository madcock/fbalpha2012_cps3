/******************************************************************************* 
 * settings-logic.h - Genesis Plus GX PS3
 *
 *  Created on: Aug 14, 2011
********************************************************************************/

#define toggle_settings_items() \
	for(int i = 0; i < MAX_NO_OF_CONTROLS_SETTINGS; i++) \
			items_generalsettings[i].enabled = 1;	 \
	\
	menu_generalvideosettings.items[SETTING_SHADER_2].enabled = Settings.ScaleEnabled; \
	menu_generalvideosettings.items[SETTING_SHADER_PRESETS].enabled = Settings.ScaleEnabled; \
	menu_generalvideosettings.items[SETTING_HW_TEXTURE_FILTER_2].enabled = Settings.ScaleEnabled; \
	menu_generalvideosettings.items[SETTING_SCALE_FACTOR].enabled = Settings.ScaleEnabled; \
	/* menu_generalvideosettings.items[SETTING_GAME_AWARE_SHADER].enabled = Settings.ScaleEnabled; */ \
	update_item_colors = false;

void apply_scaling()
{
   ps3graphics_set_fbo_scale(Settings.ScaleEnabled, Settings.ScaleFactor);
   ps3graphics_set_smooth(Settings.PS3Smooth, 0);
   ps3graphics_set_smooth(Settings.PS3Smooth2, 1);
}

static void producesettingentry(uint64_t switchvalue)
{
	uint64_t state = cell_pad_input_poll_device(0);

	switch(switchvalue)
	{
		case SETTING_EMU_CURRENT_SAVE_STATE_SLOT:
			if(CTRL_LEFT(state) || CTRL_LSTICK_LEFT(state) || CTRL_CROSS(state))
			{
				if(Settings.CurrentSaveStateSlot != MIN_SAVE_STATE_SLOT)
					Settings.CurrentSaveStateSlot--;

				sys_timer_usleep(FILEBROWSER_DELAY);
			}
			if(CTRL_RIGHT(state)  || CTRL_LSTICK_RIGHT(state) || CTRL_CROSS(state))
			{
				Settings.CurrentSaveStateSlot++;
				sys_timer_usleep(FILEBROWSER_DELAY);
			}

			if(CTRL_START(state))
				Settings.CurrentSaveStateSlot = MIN_SAVE_STATE_SLOT;

			break;
		case SETTING_CHANGE_RESOLUTION:
			if(CTRL_RIGHT(state) || CTRL_LSTICK_RIGHT(state) )
			{
				ps3graphics_next_resolution();
				sys_timer_usleep(SETTINGS_DELAY);
			}
			if(CTRL_LEFT(state) || CTRL_LSTICK_LEFT(state) )
			{
				ps3graphics_previous_resolution();
				sys_timer_usleep(SETTINGS_DELAY);
			}
			if(CTRL_CROSS(state))
			{
				ps3graphics_switch_resolution(ps3graphics_get_current_resolution(), Settings.PS3PALTemporalMode60Hz, Settings.TripleBuffering, Settings.ScaleEnabled, Settings.ScaleFactor);
				apply_scaling();
				emulator_implementation_set_texture(Settings.PS3CurrentBorder);
			}
			if(CTRL_START(state))
			{
				ps3graphics_switch_resolution(ps3graphics_get_initial_resolution(), Settings.PS3PALTemporalMode60Hz, Settings.TripleBuffering, Settings.ScaleEnabled, Settings.ScaleFactor);
				apply_scaling();
				emulator_implementation_set_texture(Settings.PS3CurrentBorder);
			}
			break;
			/*
			   case SETTING_PAL60_MODE:
			   if(CellInput->WasButtonPressed(0, CTRL_RIGHT) | CellInput->WasAnalogPressedLeft(0,CTRL_LSTICK) | CellInput->WasButtonPressed(0,CTRL_CROSS) | CellInput->WasButtonPressed(0, CTRL_LEFT) | CellInput->WasAnalogPressedLeft(0,CTRL_LSTICK))
			   {
			   if (ps3graphics_get_current_resolution() == CELL_VIDEO_OUT_RESOLUTION_576)
			   {
			   if(Graphics->CheckResolution(CELL_VIDEO_OUT_RESOLUTION_576))
			   {
			   Settings.PS3PALTemporalMode60Hz = !Settings.PS3PALTemporalMode60Hz;
			   Graphics->SetPAL60Hz(Settings.PS3PALTemporalMode60Hz);
			   ps3graphics_switch_resolution(ps3graphics_get_current_resolution(), Settings.PS3PALTemporalMode60Hz);
			   }
			   }

			   }
			   break;
			 */
		case SETTING_SHADER_PRESETS:
			if(Settings.ScaleEnabled)
			{
				if((CTRL_RIGHT(state) || CTRL_LSTICK_RIGHT(state) || CTRL_LEFT(state) || CTRL_LSTICK_LEFT(state) || CTRL_CROSS(state)) && Settings.ScaleEnabled)
				{
					menuStackindex++;
					menuStack[menuStackindex] = menu_filebrowser;
					menuStack[menuStackindex].enum_id = PRESET_CHOICE;
				set_initial_dir_tmpbrowser = true;
				}
			}
			if(CTRL_START(state) && Settings.ScaleEnabled)
			{
				strcpy(Settings.ShaderPresetPath, "");
				strcpy(Settings.ShaderPresetTitle, "None");
			}
			break;
		case SETTING_BORDER:
			if(CTRL_LEFT(state) || CTRL_LSTICK_LEFT(state) || CTRL_RIGHT(state) || CTRL_LSTICK_RIGHT(state) || CTRL_CROSS(state))
			{
				menuStackindex++;
				menuStack[menuStackindex] = menu_filebrowser;
				menuStack[menuStackindex].enum_id = BORDER_CHOICE;
				set_initial_dir_tmpbrowser = true;
			}
			if(CTRL_START(state))
			{
				strcpy(Settings.PS3CurrentBorder, DEFAULT_BORDER_FILE);
				emulator_implementation_set_texture(Settings.PS3CurrentBorder);
			}
			break;
		case SETTING_SHADER:
			if(CTRL_LEFT(state) || CTRL_LSTICK_LEFT(state) || CTRL_RIGHT(state) || CTRL_LSTICK_RIGHT(state) || CTRL_CROSS(state))
			{
				menuStackindex++;
				menuStack[menuStackindex] = menu_filebrowser;
				menuStack[menuStackindex].enum_id = SHADER_CHOICE;
				set_shader = 0;
				set_initial_dir_tmpbrowser = true;
			}
			if(CTRL_START(state))
			{
				ps3graphics_load_fragment_shader(DEFAULT_SHADER_FILE, 0);
			}
			break;
		case SETTING_SHADER_2:
			if(CTRL_LEFT(state) || CTRL_LSTICK_LEFT(state) || CTRL_RIGHT(state) || CTRL_LSTICK_RIGHT(state) || CTRL_CROSS(state))
			{
				if(Settings.ScaleEnabled)
				{
					set_shader = 1;
					menuStackindex++;
					menuStack[menuStackindex] = menu_filebrowser;
					menuStack[menuStackindex].enum_id = SHADER_CHOICE;
					set_initial_dir_tmpbrowser = true;
				}
			}
			if(CTRL_START(state))
			{
				if(Settings.ScaleEnabled)
					ps3graphics_load_fragment_shader(DEFAULT_SHADER_FILE, 1);
			}
			break;
		case SETTING_FONT_SIZE:
			if(CTRL_LEFT(state)  || CTRL_LSTICK_LEFT(state) || CTRL_CROSS(state))
			{
				if(Settings.PS3FontSize > -100)
				{
					Settings.PS3FontSize--;
					sys_timer_usleep(SETTINGS_DELAY);
				}
			}
			if(CTRL_RIGHT(state)  || CTRL_LSTICK_RIGHT(state) || CTRL_CROSS(state))
			{
				if((Settings.PS3FontSize < 200))
				{
					Settings.PS3FontSize++;
					sys_timer_usleep(SETTINGS_DELAY);
				}
			}
			if(CTRL_START(state))
			{
				Settings.PS3FontSize = 100;
			}
			break;
		case SETTING_KEEP_ASPECT_RATIO:
			if(CTRL_LEFT(state) || CTRL_LSTICK_LEFT(state))
			{
				if(Settings.PS3KeepAspect > 0)
				{
					Settings.PS3KeepAspect--;
					ps3graphics_set_aspect_ratio(Settings.PS3KeepAspect, bitmap_width, bitmap_height, 1);
					sys_timer_usleep(SETTINGS_DELAY);
				}
			}
			if(CTRL_RIGHT(state)  || CTRL_LSTICK_RIGHT(state) || CTRL_CROSS(state))
			{
				if(Settings.PS3KeepAspect < LAST_ASPECT_RATIO)
				{
					Settings.PS3KeepAspect++;
					ps3graphics_set_aspect_ratio(Settings.PS3KeepAspect, bitmap_width, bitmap_height, 1);
					sys_timer_usleep(SETTINGS_DELAY);
				}
			}
			if(CTRL_START(state))
			{
				Settings.PS3KeepAspect = ASPECT_RATIO_4_3;
				ps3graphics_set_aspect_ratio(Settings.PS3KeepAspect, bitmap_width, bitmap_height, 1);
				sys_timer_usleep(SETTINGS_DELAY);
			}
			break;
		case SETTING_HW_TEXTURE_FILTER:
			if(CTRL_LEFT(state) || CTRL_LSTICK_LEFT(state) || CTRL_RIGHT(state) || CTRL_LSTICK_RIGHT(state) || CTRL_CROSS(state))
			{
				Settings.PS3Smooth = !Settings.PS3Smooth;
				ps3graphics_set_smooth(Settings.PS3Smooth, 0);
				sys_timer_usleep(SETTINGS_DELAY);
			}
			if(CTRL_START(state))
			{
				Settings.PS3Smooth = 1;
				ps3graphics_set_smooth(Settings.PS3Smooth, 0);
				sys_timer_usleep(SETTINGS_DELAY);
			}
			break;
		case SETTING_HW_TEXTURE_FILTER_2:
			if(CTRL_LEFT(state) || CTRL_LSTICK_LEFT(state) || CTRL_RIGHT(state) || CTRL_LSTICK_RIGHT(state) || CTRL_CROSS(state))
			{
				Settings.PS3Smooth2 = !Settings.PS3Smooth2;
				ps3graphics_set_smooth(Settings.PS3Smooth2, 1);
				sys_timer_usleep(FILEBROWSER_DELAY);
			}
			if(CTRL_START(state))
			{
				Settings.PS3Smooth2 = 1;
				ps3graphics_set_smooth(Settings.PS3Smooth2, 1);
			}
			break;
		case SETTING_SCALE_ENABLED:
			if(CTRL_LEFT(state) || CTRL_LSTICK_LEFT(state) || CTRL_RIGHT(state) || CTRL_LSTICK_RIGHT(state) || CTRL_CROSS(state))
			{
				Settings.ScaleEnabled = !Settings.ScaleEnabled;
				if(Settings.ScaleEnabled)
					ps3graphics_set_fbo_scale(1, Settings.ScaleFactor);
				else
				{
					ps3graphics_load_fragment_shader(DEFAULT_SHADER_FILE, 1);
					Settings.ScaleFactor = 1;
					ps3graphics_set_fbo_scale(0, 2);
				}
				sys_timer_usleep(FILEBROWSER_DELAY);
			}
			if(CTRL_START(state))
			{
				Settings.ScaleEnabled = 0;
				ps3graphics_load_fragment_shader(DEFAULT_SHADER_FILE, 1);
				Settings.ScaleFactor = 1;
				ps3graphics_set_fbo_scale(0, 2);
			}
			break;
		case SETTING_SCALE_FACTOR:
			if(CTRL_LEFT(state) || CTRL_LSTICK_LEFT(state))
			{
				if((Settings.ScaleFactor > 1) && Settings.ScaleEnabled)
				{
					Settings.ScaleFactor -= 1;
					apply_scaling();
				}
				sys_timer_usleep(FILEBROWSER_DELAY);
				update_item_colors = 1;
			}
			if(CTRL_RIGHT(state) || CTRL_LSTICK_RIGHT(state) || CTRL_CROSS(state))
			{
				if((Settings.ScaleFactor < 5) && Settings.ScaleEnabled)
				{
					Settings.ScaleFactor += 1;
					apply_scaling();
				}
				sys_timer_usleep(FILEBROWSER_DELAY);
				update_item_colors = 1;
			}
			if(CTRL_START(state))
			{
				Settings.ScaleFactor = 1;
				apply_scaling();
				update_item_colors = 1;
			}
			break;
		case SETTING_HW_OVERSCAN_AMOUNT:
			if(CTRL_LEFT(state)  ||  CTRL_LSTICK_LEFT(state) || CTRL_CROSS(state))
			{
				Settings.PS3OverscanAmount--;
				Settings.PS3OverscanEnabled = 1;
				ps3graphics_set_overscan(Settings.PS3OverscanEnabled, (float)Settings.PS3OverscanAmount/100, 1);
				sys_timer_usleep(SETTINGS_DELAY);

				if(Settings.PS3OverscanAmount == 0)
				{
					Settings.PS3OverscanEnabled = 0;
					ps3graphics_set_overscan(Settings.PS3OverscanEnabled, (float)Settings.PS3OverscanAmount/100, 1);
					sys_timer_usleep(SETTINGS_DELAY);
				}
			}
			if(CTRL_RIGHT(state) || CTRL_LSTICK_RIGHT(state) || CTRL_CROSS(state))
			{
				Settings.PS3OverscanAmount++;
				Settings.PS3OverscanEnabled = 1;
				ps3graphics_set_overscan(Settings.PS3OverscanEnabled, (float)Settings.PS3OverscanAmount/100, 1);
				sys_timer_usleep(SETTINGS_DELAY);

				if(Settings.PS3OverscanAmount == 0)
				{
					Settings.PS3OverscanEnabled = 0;
					ps3graphics_set_overscan(Settings.PS3OverscanEnabled, (float)Settings.PS3OverscanAmount/100, 1);
					sys_timer_usleep(SETTINGS_DELAY);
				}
			}
			if(CTRL_START(state))
			{
				Settings.PS3OverscanAmount = 0;
				Settings.PS3OverscanEnabled = 0;
				ps3graphics_set_overscan(Settings.PS3OverscanEnabled, (float)Settings.PS3OverscanAmount/100, 1);
			}
			break;
		case SETTING_SOUND_MODE:
			if(CTRL_LEFT(state) ||  CTRL_LSTICK_LEFT(state))
			{
				if(Settings.SoundMode != SOUND_MODE_NORMAL)
				{
					Settings.SoundMode--;
					emulator_toggle_sound(Settings.SoundMode);
					sys_timer_usleep(FILEBROWSER_DELAY);
				}
			}
			if(CTRL_RIGHT(state) || CTRL_LSTICK_RIGHT(state) || CTRL_CROSS(state))
			{
				if(Settings.SoundMode < SOUND_MODE_RSOUND)
				{
					Settings.SoundMode++;
					emulator_toggle_sound(Settings.SoundMode);
					sys_timer_usleep(FILEBROWSER_DELAY);
				}
			}
			if(CTRL_START(state))
			{
				Settings.SoundMode = SOUND_MODE_NORMAL;
				emulator_toggle_sound(Settings.SoundMode);
				sys_timer_usleep(FILEBROWSER_DELAY);
			}
			break;
		case SETTING_RSOUND_SERVER_IP_ADDRESS:
			if(CTRL_LEFT(state) || CTRL_LSTICK_LEFT(state) || CTRL_RIGHT(state) || CTRL_CROSS(state) | CTRL_LSTICK_RIGHT(state) )
			{
				oskutil_write_initial_message(&oskutil_handle, L"192.168.1.1");
				oskutil_write_message(&oskutil_handle, L"Enter IP address for the RSound Server.");
				oskutil_start(&oskutil_handle);
				while(OSK_IS_RUNNING(oskutil_handle))
				{
					glClear(GL_COLOR_BUFFER_BIT);
					ps3graphics_draw_menu(1920, 1080);
					psglSwap();
					cellSysutilCheckCallback();
				}
				if(oskutil_handle.text_can_be_fetched)
					strcpy(Settings.RSoundServerIPAddress, OUTPUT_TEXT_STRING(oskutil_handle));
			}
			if(CTRL_START(state))
			{
				strcpy(Settings.RSoundServerIPAddress, "0.0.0.0");
			}
			break;
		case SETTING_THROTTLE_MODE:
			if(CTRL_LEFT(state)  || CTRL_LSTICK_LEFT(state) || CTRL_RIGHT(state) || CTRL_CROSS(state) || CTRL_LSTICK_RIGHT(state))
			{
				Settings.Throttled = !Settings.Throttled;
				ps3graphics_set_vsync(Settings.Throttled);
				sys_timer_usleep(FILEBROWSER_DELAY);
			}
			if(CTRL_START(state))
			{
				Settings.Throttled = 1;
				ps3graphics_set_vsync(Settings.Throttled);
				sys_timer_usleep(FILEBROWSER_DELAY);
			}
			break;
		case SETTING_TRIPLE_BUFFERING:
			if(CTRL_LEFT(state)  || CTRL_LSTICK_LEFT(state) || CTRL_RIGHT(state) || CTRL_LSTICK_RIGHT(state))
			{
				Settings.TripleBuffering = !Settings.TripleBuffering;
				ps3graphics_set_triple_buffering(Settings.TripleBuffering);
				ps3graphics_switch_resolution(ps3graphics_get_current_resolution(), Settings.PS3PALTemporalMode60Hz, Settings.TripleBuffering, Settings.ScaleEnabled, Settings.ScaleFactor);
				sys_timer_usleep(FILEBROWSER_DELAY);
			}
			if(CTRL_START(state))
			{
				if(Settings.TripleBuffering == 0)
				{
					Settings.TripleBuffering = 1;
					ps3graphics_set_triple_buffering(Settings.TripleBuffering);
					ps3graphics_switch_resolution(ps3graphics_get_current_resolution(), Settings.PS3PALTemporalMode60Hz, Settings.TripleBuffering, Settings.ScaleEnabled, Settings.ScaleFactor);
				}
			}
			break;
		case SETTING_ENABLE_SCREENSHOTS:
			if(CTRL_LEFT(state)  || CTRL_LSTICK_LEFT(state) || CTRL_RIGHT(state) || CTRL_LSTICK_RIGHT(state))
			{
#if(CELL_SDK_VERSION > 0x340000)
				Settings.ScreenshotsEnabled = !Settings.ScreenshotsEnabled;
				if(Settings.ScreenshotsEnabled)
				{
					cellSysmoduleLoadModule(CELL_SYSMODULE_SYSUTIL_SCREENSHOT);
					CellScreenShotSetParam screenshot_param = {0, 0, 0, 0};

					screenshot_param.photo_title = EMULATOR_NAME;
					screenshot_param.game_title = EMULATOR_NAME;
					cellScreenShotSetParameter (&screenshot_param);
					cellScreenShotEnable();
				}
				else
				{
					cellScreenShotDisable();
					cellSysmoduleUnloadModule(CELL_SYSMODULE_SYSUTIL_SCREENSHOT);
				}

				sys_timer_usleep(FILEBROWSER_DELAY);
#endif
			}
			if(CTRL_START(state))
			{
#if(CELL_SDK_VERSION > 0x340000)
				Settings.ScreenshotsEnabled = false;
#endif
			}
			break;
		case SETTING_SAVE_SHADER_PRESET:
			if(CTRL_LEFT(state)  || CTRL_LSTICK_LEFT(state)  || CTRL_RIGHT(state) | CTRL_LSTICK_RIGHT(state) || CTRL_START(state) || CTRL_CROSS(state))
			{
				emulator_save_settings(SHADER_PRESET_FILE);
			}
			break;
		case SETTING_APPLY_SHADER_PRESET_ON_STARTUP:
			if(CTRL_LEFT(state)  || CTRL_LSTICK_LEFT(state)  || CTRL_RIGHT(state) | CTRL_LSTICK_RIGHT(state) || CTRL_START(state) || CTRL_CROSS(state))
			{
				if(Settings.ApplyShaderPresetOnStartup)
					Settings.ApplyShaderPresetOnStartup = 0;
				else
					Settings.ApplyShaderPresetOnStartup = 1;
			}
			break;
		case SETTING_DEFAULT_VIDEO_ALL:
			if(CTRL_LEFT(state)  || CTRL_LSTICK_LEFT(state)  || CTRL_RIGHT(state) | CTRL_LSTICK_RIGHT(state) || CTRL_START(state) || CTRL_CROSS(state))
			{
				Settings.PS3FontSize = 100;
				strcpy(Settings.ShaderPresetPath, "");
				strcpy(Settings.ShaderPresetTitle, "None");

				Settings.PS3KeepAspect = 0;
				ps3graphics_set_aspect_ratio(Settings.PS3KeepAspect, SCREEN_RENDER_TEXTURE_WIDTH, SCREEN_RENDER_TEXTURE_HEIGHT, 1);

				Settings.PS3Smooth = 1;
				Settings.PS3Smooth2 = 1;
				ps3graphics_set_smooth(Settings.PS3Smooth, 0);
				ps3graphics_set_smooth(Settings.PS3Smooth2, 1);

				Settings.PS3OverscanEnabled = 0;
				Settings.PS3OverscanAmount = 0;
				ps3graphics_set_overscan(Settings.PS3OverscanEnabled, (float)Settings.PS3OverscanAmount/100, 1);

				Settings.PS3PALTemporalMode60Hz = 0;
				ps3graphics_set_pal60hz(Settings.PS3PALTemporalMode60Hz);

				if(!Settings.TripleBuffering)
				{
					Settings.TripleBuffering = 1;
					ps3graphics_set_triple_buffering(Settings.TripleBuffering);

					ps3graphics_switch_resolution(ps3graphics_get_current_resolution(), Settings.PS3PALTemporalMode60Hz, Settings.TripleBuffering, Settings.ScaleEnabled, Settings.ScaleFactor);

					Settings.ScaleEnabled = 0;
					Settings.ScaleFactor = 2;
					ps3graphics_set_fbo_scale(Settings.ScaleEnabled, Settings.ScaleFactor);
				}
				Settings.Throttled = 1;
				ps3graphics_set_vsync(Settings.Throttled);

				Settings.ScreenshotsEnabled = false;
				emulator_implementation_set_texture(DEFAULT_BORDER_FILE);
				Settings.ApplyShaderPresetOnStartup = 0;
				update_item_colors = 1;
			}
			break;
		case SETTING_DEFAULT_AUDIO_ALL:
			if(CTRL_LEFT(state)  || CTRL_LSTICK_LEFT(state)  || CTRL_RIGHT(state) | CTRL_LSTICK_RIGHT(state) || CTRL_START(state) || CTRL_CROSS(state))
			{
				strcpy(Settings.RSoundServerIPAddress, "0.0.0.0");
				if(Settings.SoundMode == SOUND_MODE_RSOUND)
				{
					Settings.SoundMode = SOUND_MODE_NORMAL;
					emulator_toggle_sound(Settings.SoundMode);
				}
				Settings.SoundMode = SOUND_MODE_NORMAL;
				emulator_toggle_sound(Settings.SoundMode);
			}
			break;
		case SETTING_EMU_DEFAULT_ALL:
			if(CTRL_LEFT(state) || CTRL_LSTICK_LEFT(state) || CTRL_RIGHT(state) || CTRL_LSTICK_RIGHT(state) || CTRL_CROSS(state))
			{
				Settings.CurrentSaveStateSlot = 0;
				Settings.SixButtonPad = 0;
				//Emulator_SetControllerMode();
			}
			break;
		case SETTING_EMU_VIDEO_DEFAULT_ALL:
				if(CTRL_LEFT(state) || CTRL_LSTICK_LEFT(state) || CTRL_RIGHT(state) || CTRL_LSTICK_RIGHT(state) || CTRL_START(state) || CTRL_CROSS(state))
				{
				}
				break;
		case SETTING_EMU_AUDIO_DEFAULT_ALL:
				if(CTRL_LEFT(state) || CTRL_LSTICK_LEFT(state) || CTRL_RIGHT(state) || CTRL_LSTICK_RIGHT(state) || CTRL_START(state) || CTRL_CROSS(state))
				{
					Settings.FMVolume     = 100;
					Settings.PSGVolume     = 150;
				}
				break;
		case SETTING_PATH_DEFAULT_ROM_DIRECTORY:
			if(CTRL_LEFT(state) || CTRL_LSTICK_LEFT(state) || CTRL_RIGHT(state) || CTRL_LSTICK_RIGHT(state) || CTRL_CROSS(state))
			{
				menuStackindex++;
				menuStack[menuStackindex] = menu_filebrowser;
				menuStack[menuStackindex].enum_id = PATH_DEFAULT_ROM_DIR_CHOICE;
				set_initial_dir_tmpbrowser = true;
			}
			if(CTRL_START(state))
			{
				strcpy(Settings.PS3PathROMDirectory, "/");
			}
			break;
		case SETTING_PATH_SAVESTATES_DIRECTORY:
			if(CTRL_LEFT(state) || CTRL_LSTICK_LEFT(state) || CTRL_RIGHT(state) || CTRL_LSTICK_RIGHT(state) || CTRL_CROSS(state))
			{
				menuStackindex++;
				menuStack[menuStackindex] = menu_filebrowser;
				menuStack[menuStackindex].enum_id = PATH_SAVESTATES_DIR_CHOICE;
				set_initial_dir_tmpbrowser = true;
			}
			if(CTRL_START(state))
			{
				strcpy(Settings.PS3PathSaveStates, usrDirPath);
			}
			break;
		case SETTING_PATH_SRAM_DIRECTORY:
			if(CTRL_LEFT(state) || CTRL_LSTICK_LEFT(state) || CTRL_RIGHT(state) || CTRL_LSTICK_RIGHT(state) || CTRL_CROSS(state))
			{
				menuStackindex++;
				menuStack[menuStackindex] = menu_filebrowser;
				menuStack[menuStackindex].enum_id = PATH_SRAM_DIR_CHOICE;
				set_initial_dir_tmpbrowser = true;
			}
			if(CTRL_START(state))
			{
				strcpy(Settings.PS3PathSRAM, usrDirPath);
			}
			break;
		case SETTING_PATH_DEFAULT_ALL:
			if(CTRL_LEFT(state) || CTRL_LSTICK_LEFT(state) ||  CTRL_RIGHT(state) || CTRL_LSTICK_RIGHT(state) || CTRL_CROSS(state) || CTRL_START(state))
			{
				strcpy(Settings.PS3PathROMDirectory, "/");
				strcpy(Settings.PS3PathSaveStates, usrDirPath);
				strcpy(Settings.PS3PathSRAM, usrDirPath);
			}
			break;
	}

}