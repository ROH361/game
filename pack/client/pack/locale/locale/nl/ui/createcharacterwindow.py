import uiScriptLocale
import app

ROOT_PATH = "d:/ymir work/ui/public/"
LOCALE_PATH = "locale/nl/ui/select/"
BOARD_X = SCREEN_WIDTH * (65) / 800
BOARD_Y = SCREEN_HEIGHT * (215) / 600

PLUS_BUTTON_WIDTH = 20
TEMPORARY_HEIGHT = 24 + 5

if app.ENABLE_WOLFMAN_CHARACTER:
	window = {
		"name" : "CreateCharacterWindow",

		"x" : 0,
		"y" : 0,

		"width" : SCREEN_WIDTH,
		"height" : SCREEN_HEIGHT,

		"children" :
		(
			## Board
			{
				"name" : "BackGround", "type" : "expanded_image", "x" : 0, "y" : 0,
				"x_scale" : float(SCREEN_WIDTH) / 1024.0, "y_scale" : float(SCREEN_HEIGHT) / 768.0,
				"image" : "locale/nl/ui/select.sub",
			},
			## Name
			{
				"name" : "name_warrior",
				"type" : "image",

				"x" : BOARD_X - 27,
				"y" : BOARD_Y - 174 + 25,

				"image" : LOCALE_PATH+"name_warrior.sub",
			},
			{
				"name" : "name_assassin",
				"type" : "image",

				"x" : BOARD_X - 27,
				"y" : BOARD_Y - 174 + 25,

				"image" : LOCALE_PATH+"name_assassin.sub",
			},
			{
				"name" : "name_sura",
				"type" : "image",

				"x" : BOARD_X - 27,
				"y" : BOARD_Y - 174 + 25,

				"image" : LOCALE_PATH+"name_sura.sub",
			},
			{
				"name" : "name_shaman",
				"type" : "image",

				"x" : BOARD_X - 27,
				"y" : BOARD_Y - 174 + 25,

				"image" : LOCALE_PATH+"name_shaman.sub",
			},
			{
				"name" : "name_wolfman",
				"type" : "image",

				"x" : BOARD_X - 27,
				"y" : BOARD_Y - 174 + 25,

				"image" : LOCALE_PATH+"name_wolfman.sub",
			},

			## Character Board
			{
				"name" : "character_board",
				"type" : "thinboard",

				"x" : BOARD_X,
				"y" : BOARD_Y,

				"width" : 208,
				"height" : 300 + TEMPORARY_HEIGHT,

				"children" :
				(
					{
						"name" : "text_board",
						"type" : "bar",

						"x" : 8,
						"y" : 10,

						"width" : 189,
						"height" : 122,

						"children" :
						(
							{
								"name" : "prev_button",
								"type" : "button",

								"x" : 95,
								"y" : 95,

								"text" : uiScriptLocale.CREATE_PREV,

								"default_image" : ROOT_PATH + "Small_Button_01.sub",
								"over_image" : ROOT_PATH + "Small_Button_02.sub",
								"down_image" : ROOT_PATH + "Small_Button_03.sub",
							},
							{
								"name" : "next_button",
								"type" : "button",

								"x" : 140,
								"y" : 95,

								"text" : uiScriptLocale.CREATE_NEXT,

								"default_image" : ROOT_PATH + "Small_Button_01.sub",
								"over_image" : ROOT_PATH + "Small_Button_02.sub",
								"down_image" : ROOT_PATH + "Small_Button_03.sub",
							},
							{
								"name" : "right_line",
								"type" : "line",

								"x" : 189-1,
								"y" : -1,

								"width" : 0,
								"height" : 122,

								"color" : 0xffAAA6A1,
							},
							{
								"name" : "bottom_line",
								"type" : "line",

								"x" : 0,
								"y" : 122-1,

								"width" : 189,
								"height" : 0,

								"color" : 0xffAAA6A1,
							},
							{
								"name" : "left_line",
								"type" : "line",

								"x" : 0,
								"y" : 0,

								"width" : 0,
								"height" : 122-1,

								"color" : 0xff2A2521,
							},
							{
								"name" : "top_line",
								"type" : "line",

								"x" : 0,
								"y" : 0,

								"width" : 189,
								"height" : 0,

								"color" : 0xff2A2521,
							},
						),
					},
					{
						"name" : "hth",
						"type" : "text",

						"x" : 15,
						"y" : 138,

						"text" : uiScriptLocale.CREATE_HP,

						"children" :
						(
							{
								"name" : "hth_gauge",
								"type" : "gauge",

								"x" : 30,
								"y" : 4,

								"width" : 100 + PLUS_BUTTON_WIDTH,
								"color" : "red",
							},
							{
								"name" : "hth_slot",
								"type" : "slotbar",

								"x" : 137 + PLUS_BUTTON_WIDTH,
								"y" : -1,
								"width" : 24,
								"height" : 16,

								"children" :
								(
									{
										"name" : "hth_value",
										"type" : "text",

										"x" : 0,
										"y" : 1,
										"all_align" : "center",

										"text" : "99",
									},
								),
							},
						),
					},
					{
						"name" : "int",
						"type" : "text",

						"x" : 15,
						"y" : 157,

						"text" : uiScriptLocale.CREATE_SP,

						"children" :
						(
							{
								"name" : "int_gauge",
								"type" : "gauge",

								"x" : 30,
								"y" : 4,

								"width" : 100 + PLUS_BUTTON_WIDTH,
								"color" : "pink",
							},
							{
								"name" : "int_slot",
								"type" : "slotbar",

								"x" : 137 + PLUS_BUTTON_WIDTH,
								"y" : -1,
								"width" : 24,
								"height" : 16,

								"children" :
								(
									{
										"name" : "int_value",
										"type" : "text",

										"x" : 0,
										"y" : 1,
										"all_align" : "center",

										"text" : "99",
									},
								),
							},
						),
					},
					{
						"name" : "str",
						"type" : "text",

						"x" : 15,
						"y" : 176,

						"text" : uiScriptLocale.CREATE_ATT_GRADE,

						"children" :
						(
							{
								"name" : "str_gauge",
								"type" : "gauge",

								"x" : 30,
								"y" : 4,

								"width" : 100 + PLUS_BUTTON_WIDTH,
								"color" : "purple",
							},
							{
								"name" : "str_slot",
								"type" : "slotbar",

								"x" : 137 + PLUS_BUTTON_WIDTH,
								"y" : -1,
								"width" : 24,
								"height" : 16,

								"children" :
								(
									{
										"name" : "str_value",
										"type" : "text",

										"x" : 0,
										"y" : 1,
										"all_align" : "center",

										"text" : "99",
									},
								),
							},
						),
					},
					{
						"name" : "dex",
						"type" : "text",

						"x" : 15,
						"y" : 195,

						"text" : uiScriptLocale.CREATE_DEX_GRADE,

						"children" :
						(
							{
								"name" : "dex_gauge",
								"type" : "gauge",

								"x" : 30,
								"y" : 4,

								"width" : 100 + PLUS_BUTTON_WIDTH,
								"color" : "blue",
							},
							{
								"name" : "dex_slot",
								"type" : "slotbar",

								"x" : 137 + PLUS_BUTTON_WIDTH,
								"y" : -1,
								"width" : 24,
								"height" : 16,

								"children" :
								(
									{
										"name" : "dex_value",
										"type" : "text",

										"x" : 0,
										"y" : 1,
										"all_align" : "center",

										"text" : "99",
									},
								),
							},
						),
					},

					{
						"name" : "hth_button",
						"type" : "button",

						"x" : 184,
						"y" : 139,

						"default_image" : "d:/ymir work/ui/game/windows/btn_plus_up.sub",
						"over_image" : "d:/ymir work/ui/game/windows/btn_plus_over.sub",
						"down_image" : "d:/ymir work/ui/game/windows/btn_plus_down.sub",
					},
					{
						"name" : "int_button",
						"type" : "button",

						"x" : 184,
						"y" : 158,

						"default_image" : "d:/ymir work/ui/game/windows/btn_plus_up.sub",
						"over_image" : "d:/ymir work/ui/game/windows/btn_plus_over.sub",
						"down_image" : "d:/ymir work/ui/game/windows/btn_plus_down.sub",
					},
					{
						"name" : "str_button",
						"type" : "button",

						"x" : 184,
						"y" : 177,

						"default_image" : "d:/ymir work/ui/game/windows/btn_plus_up.sub",
						"over_image" : "d:/ymir work/ui/game/windows/btn_plus_over.sub",
						"down_image" : "d:/ymir work/ui/game/windows/btn_plus_down.sub",
					},
					{
						"name" : "dex_button",
						"type" : "button",

						"x" : 184,
						"y" : 196,

						"default_image" : "d:/ymir work/ui/game/windows/btn_plus_up.sub",
						"over_image" : "d:/ymir work/ui/game/windows/btn_plus_over.sub",
						"down_image" : "d:/ymir work/ui/game/windows/btn_plus_down.sub",
					},


					{
						"name" : "character_name",
						"type" : "text",

						"x" : 43,
						"y" : 218,

						"text" : uiScriptLocale.CREATE_NAME,

						"text_horizontal_align" : "center",

						"children" :
						(
							{
								"name" : "character_name_slot",
								"type" : "image",

								"x" : 40 - 1,
								"y" : -2,

								"image" : "d:/ymir work/ui/public/parameter_slot_04.sub",
							},
							{
								"name" : "character_name_value",
								"type" : "editline",

								"x" : 40 - 1 + 3,
								"y" : 0,

								"input_limit" : 12,

								"width" : 90,
								"height" : 20,
							},
						),
					},

					{
						"name" : "character_gender",
						"type" : "text",

						"x" : 43,
						"y" : 247,

						"text" : uiScriptLocale.CREATE_SEX,

						"text_horizontal_align" : "center",
					},
					{
						"name" : "gender_button_01",
						"type" : "radio_button",

						"x" : 79,
						"y" : 247,

						"text" : uiScriptLocale.CREATE_MAN,

						"default_image" : ROOT_PATH + "Middle_Button_01.sub",
						"over_image"	: ROOT_PATH + "Middle_Button_02.sub",
						"down_image"	: ROOT_PATH + "Middle_Button_03.sub",
					},
					{
						"name" : "gender_button_02",
						"type" : "radio_button",

						"x" : 139,
						"y" : 247,

						"text" : uiScriptLocale.CREATE_WOMAN,

						"default_image" : ROOT_PATH + "Middle_Button_01.sub",
						"over_image"	: ROOT_PATH + "Middle_Button_02.sub",
						"down_image"	: ROOT_PATH + "Middle_Button_03.sub",
					},

					{
						"name" : "character_shape",
						"type" : "text",

						"x" : 43,
						"y" : 270,

						"text" : uiScriptLocale.CREATE_SHAPE,

						"text_horizontal_align" : "center",
					},
					{
						"name" : "shape_button_01",
						"type" : "radio_button",

						"x" : 79,
						"y" : 239 + TEMPORARY_HEIGHT,

						"text" : "1",

						"default_image" : ROOT_PATH + "Middle_Button_01.sub",
						"over_image" : ROOT_PATH + "Middle_Button_02.sub",
						"down_image" : ROOT_PATH + "Middle_Button_03.sub",
					},
					{
						"name" : "shape_button_02",
						"type" : "radio_button",

						"x" : 139,
						"y" : 239 + TEMPORARY_HEIGHT,

						"text" : "2",

						"default_image" : ROOT_PATH + "Middle_Button_01.sub",
						"over_image" : ROOT_PATH + "Middle_Button_02.sub",
						"down_image" : ROOT_PATH + "Middle_Button_03.sub",
					},

					{
						"name" : "create_button",
						"type" : "button",

						"x" : 11,
						"y" : 265 + TEMPORARY_HEIGHT,

						"text" : uiScriptLocale.CREATE_CREATE,

						"default_image" : ROOT_PATH + "Large_Button_01.sub",
						"over_image" : ROOT_PATH + "Large_Button_02.sub",
						"down_image" : ROOT_PATH + "Large_Button_03.sub",
					},
					{
						"name" : "cancel_button",
						"type" : "button",

						"x" : 109,
						"y" : 265 + TEMPORARY_HEIGHT,

						"text" : uiScriptLocale.CANCEL,

						"default_image" : ROOT_PATH + "Large_Button_01.sub",
						"over_image" : ROOT_PATH + "Large_Button_02.sub",
						"down_image" : ROOT_PATH + "Large_Button_03.sub",
					},
				),
			},

			## Buttons
			{
				"name" : "left_button",
				"type" : "button",

				"x" : SCREEN_WIDTH * (450 - 22*3) / 800,
				"y" : SCREEN_HEIGHT * (505) / 600,

				"default_image" : "d:/ymir work/ui/intro/select/dragon_left_button_01.sub",
				"over_image" : "d:/ymir work/ui/intro/select/dragon_left_button_02.sub",
				"down_image" : "d:/ymir work/ui/intro/select/dragon_left_button_03.sub",
			},
			{
				"name" : "right_button",
				"type" : "button",

				"x" : SCREEN_WIDTH * (580 - 22) / 800,
				"y" : SCREEN_HEIGHT * (505) / 600,

				"default_image" : "d:/ymir work/ui/intro/select/dragon_right_button_01.sub",
				"over_image" : "d:/ymir work/ui/intro/select/dragon_right_button_02.sub",
				"down_image" : "d:/ymir work/ui/intro/select/dragon_right_button_03.sub",
			},


		),
	}
else:
	window = {
		"name" : "CreateCharacterWindow",

		"x" : 0,
		"y" : 0,

		"width" : SCREEN_WIDTH,
		"height" : SCREEN_HEIGHT,

		"children" :
		(
			## Board
			{
				"name" : "BackGround", "type" : "expanded_image", "x" : 0, "y" : 0,
				"x_scale" : float(SCREEN_WIDTH) / 1024.0, "y_scale" : float(SCREEN_HEIGHT) / 768.0,
				"image" : "locale/nl/ui/select.sub",
			},
			## Name
			{
				"name" : "name_warrior",
				"type" : "image",

				"x" : BOARD_X - 27,
				"y" : BOARD_Y - 174 + 25,

				"image" : LOCALE_PATH+"name_warrior.sub",
			},
			{
				"name" : "name_assassin",
				"type" : "image",

				"x" : BOARD_X - 27,
				"y" : BOARD_Y - 174 + 25,

				"image" : LOCALE_PATH+"name_assassin.sub",
			},
			{
				"name" : "name_sura",
				"type" : "image",

				"x" : BOARD_X - 27,
				"y" : BOARD_Y - 174 + 25,

				"image" : LOCALE_PATH+"name_sura.sub",
			},
			{
				"name" : "name_shaman",
				"type" : "image",

				"x" : BOARD_X - 27,
				"y" : BOARD_Y - 174 + 25,

				"image" : LOCALE_PATH+"name_shaman.sub",
			},

			## Character Board
			{
				"name" : "character_board",
				"type" : "thinboard",

				"x" : BOARD_X,
				"y" : BOARD_Y,

				"width" : 208,
				"height" : 300 + TEMPORARY_HEIGHT,

				"children" :
				(
					{
						"name" : "text_board",
						"type" : "bar",

						"x" : 8,
						"y" : 10,

						"width" : 189,
						"height" : 122,

						"children" :
						(
							{
								"name" : "prev_button",
								"type" : "button",

								"x" : 95,
								"y" : 95,

								"text" : uiScriptLocale.CREATE_PREV,

								"default_image" : ROOT_PATH + "Small_Button_01.sub",
								"over_image" : ROOT_PATH + "Small_Button_02.sub",
								"down_image" : ROOT_PATH + "Small_Button_03.sub",
							},
							{
								"name" : "next_button",
								"type" : "button",

								"x" : 140,
								"y" : 95,

								"text" : uiScriptLocale.CREATE_NEXT,

								"default_image" : ROOT_PATH + "Small_Button_01.sub",
								"over_image" : ROOT_PATH + "Small_Button_02.sub",
								"down_image" : ROOT_PATH + "Small_Button_03.sub",
							},
							{
								"name" : "right_line",
								"type" : "line",

								"x" : 189-1,
								"y" : -1,

								"width" : 0,
								"height" : 122,

								"color" : 0xffAAA6A1,
							},
							{
								"name" : "bottom_line",
								"type" : "line",

								"x" : 0,
								"y" : 122-1,

								"width" : 189,
								"height" : 0,

								"color" : 0xffAAA6A1,
							},
							{
								"name" : "left_line",
								"type" : "line",

								"x" : 0,
								"y" : 0,

								"width" : 0,
								"height" : 122-1,

								"color" : 0xff2A2521,
							},
							{
								"name" : "top_line",
								"type" : "line",

								"x" : 0,
								"y" : 0,

								"width" : 189,
								"height" : 0,

								"color" : 0xff2A2521,
							},
						),
					},
					{
						"name" : "hth",
						"type" : "text",

						"x" : 15,
						"y" : 138,

						"text" : uiScriptLocale.CREATE_HP,

						"children" :
						(
							{
								"name" : "hth_gauge",
								"type" : "gauge",

								"x" : 30,
								"y" : 4,

								"width" : 100 + PLUS_BUTTON_WIDTH,
								"color" : "red",
							},
							{
								"name" : "hth_slot",
								"type" : "slotbar",

								"x" : 137 + PLUS_BUTTON_WIDTH,
								"y" : -1,
								"width" : 24,
								"height" : 16,

								"children" :
								(
									{
										"name" : "hth_value",
										"type" : "text",

										"x" : 0,
										"y" : 1,
										"all_align" : "center",

										"text" : "99",
									},
								),
							},
						),
					},
					{
						"name" : "int",
						"type" : "text",

						"x" : 15,
						"y" : 157,

						"text" : uiScriptLocale.CREATE_SP,

						"children" :
						(
							{
								"name" : "int_gauge",
								"type" : "gauge",

								"x" : 30,
								"y" : 4,

								"width" : 100 + PLUS_BUTTON_WIDTH,
								"color" : "pink",
							},
							{
								"name" : "int_slot",
								"type" : "slotbar",

								"x" : 137 + PLUS_BUTTON_WIDTH,
								"y" : -1,
								"width" : 24,
								"height" : 16,

								"children" :
								(
									{
										"name" : "int_value",
										"type" : "text",

										"x" : 0,
										"y" : 1,
										"all_align" : "center",

										"text" : "99",
									},
								),
							},
						),
					},
					{
						"name" : "str",
						"type" : "text",

						"x" : 15,
						"y" : 176,

						"text" : uiScriptLocale.CREATE_ATT_GRADE,

						"children" :
						(
							{
								"name" : "str_gauge",
								"type" : "gauge",

								"x" : 30,
								"y" : 4,

								"width" : 100 + PLUS_BUTTON_WIDTH,
								"color" : "purple",
							},
							{
								"name" : "str_slot",
								"type" : "slotbar",

								"x" : 137 + PLUS_BUTTON_WIDTH,
								"y" : -1,
								"width" : 24,
								"height" : 16,

								"children" :
								(
									{
										"name" : "str_value",
										"type" : "text",

										"x" : 0,
										"y" : 1,
										"all_align" : "center",

										"text" : "99",
									},
								),
							},
						),
					},
					{
						"name" : "dex",
						"type" : "text",

						"x" : 15,
						"y" : 195,

						"text" : uiScriptLocale.CREATE_DEX_GRADE,

						"children" :
						(
							{
								"name" : "dex_gauge",
								"type" : "gauge",

								"x" : 30,
								"y" : 4,

								"width" : 100 + PLUS_BUTTON_WIDTH,
								"color" : "blue",
							},
							{
								"name" : "dex_slot",
								"type" : "slotbar",

								"x" : 137 + PLUS_BUTTON_WIDTH,
								"y" : -1,
								"width" : 24,
								"height" : 16,

								"children" :
								(
									{
										"name" : "dex_value",
										"type" : "text",

										"x" : 0,
										"y" : 1,
										"all_align" : "center",

										"text" : "99",
									},
								),
							},
						),
					},

					{
						"name" : "hth_button",
						"type" : "button",

						"x" : 184,
						"y" : 139,

						"default_image" : "d:/ymir work/ui/game/windows/btn_plus_up.sub",
						"over_image" : "d:/ymir work/ui/game/windows/btn_plus_over.sub",
						"down_image" : "d:/ymir work/ui/game/windows/btn_plus_down.sub",
					},
					{
						"name" : "int_button",
						"type" : "button",

						"x" : 184,
						"y" : 158,

						"default_image" : "d:/ymir work/ui/game/windows/btn_plus_up.sub",
						"over_image" : "d:/ymir work/ui/game/windows/btn_plus_over.sub",
						"down_image" : "d:/ymir work/ui/game/windows/btn_plus_down.sub",
					},
					{
						"name" : "str_button",
						"type" : "button",

						"x" : 184,
						"y" : 177,

						"default_image" : "d:/ymir work/ui/game/windows/btn_plus_up.sub",
						"over_image" : "d:/ymir work/ui/game/windows/btn_plus_over.sub",
						"down_image" : "d:/ymir work/ui/game/windows/btn_plus_down.sub",
					},
					{
						"name" : "dex_button",
						"type" : "button",

						"x" : 184,
						"y" : 196,

						"default_image" : "d:/ymir work/ui/game/windows/btn_plus_up.sub",
						"over_image" : "d:/ymir work/ui/game/windows/btn_plus_over.sub",
						"down_image" : "d:/ymir work/ui/game/windows/btn_plus_down.sub",
					},


					{
						"name" : "character_name",
						"type" : "text",

						"x" : 43,
						"y" : 218,

						"text" : uiScriptLocale.CREATE_NAME,

						"text_horizontal_align" : "center",

						"children" :
						(
							{
								"name" : "character_name_slot",
								"type" : "image",

								"x" : 40 - 1,
								"y" : -2,

								"image" : "d:/ymir work/ui/public/parameter_slot_04.sub",
							},
							{
								"name" : "character_name_value",
								"type" : "editline",

								"x" : 40 - 1 + 3,
								"y" : 0,

								"input_limit" : 12,

								"width" : 90,
								"height" : 20,
							},
						),
					},

					{
						"name" : "character_gender",
						"type" : "text",

						"x" : 43,
						"y" : 247,

						"text" : uiScriptLocale.CREATE_SEX,

						"text_horizontal_align" : "center",
					},
					{
						"name" : "gender_button_01",
						"type" : "radio_button",

						"x" : 79,
						"y" : 247,

						"text" : uiScriptLocale.CREATE_MAN,

						"default_image" : ROOT_PATH + "Middle_Button_01.sub",
						"over_image"	: ROOT_PATH + "Middle_Button_02.sub",
						"down_image"	: ROOT_PATH + "Middle_Button_03.sub",
					},
					{
						"name" : "gender_button_02",
						"type" : "radio_button",

						"x" : 139,
						"y" : 247,

						"text" : uiScriptLocale.CREATE_WOMAN,

						"default_image" : ROOT_PATH + "Middle_Button_01.sub",
						"over_image"	: ROOT_PATH + "Middle_Button_02.sub",
						"down_image"	: ROOT_PATH + "Middle_Button_03.sub",
					},

					{
						"name" : "character_shape",
						"type" : "text",

						"x" : 43,
						"y" : 270,

						"text" : uiScriptLocale.CREATE_SHAPE,

						"text_horizontal_align" : "center",
					},
					{
						"name" : "shape_button_01",
						"type" : "radio_button",

						"x" : 79,
						"y" : 239 + TEMPORARY_HEIGHT,

						"text" : "1",

						"default_image" : ROOT_PATH + "Middle_Button_01.sub",
						"over_image" : ROOT_PATH + "Middle_Button_02.sub",
						"down_image" : ROOT_PATH + "Middle_Button_03.sub",
					},
					{
						"name" : "shape_button_02",
						"type" : "radio_button",

						"x" : 139,
						"y" : 239 + TEMPORARY_HEIGHT,

						"text" : "2",

						"default_image" : ROOT_PATH + "Middle_Button_01.sub",
						"over_image" : ROOT_PATH + "Middle_Button_02.sub",
						"down_image" : ROOT_PATH + "Middle_Button_03.sub",
					},

					{
						"name" : "create_button",
						"type" : "button",

						"x" : 11,
						"y" : 265 + TEMPORARY_HEIGHT,

						"text" : uiScriptLocale.CREATE_CREATE,

						"default_image" : ROOT_PATH + "Large_Button_01.sub",
						"over_image" : ROOT_PATH + "Large_Button_02.sub",
						"down_image" : ROOT_PATH + "Large_Button_03.sub",
					},
					{
						"name" : "cancel_button",
						"type" : "button",

						"x" : 109,
						"y" : 265 + TEMPORARY_HEIGHT,

						"text" : uiScriptLocale.CANCEL,

						"default_image" : ROOT_PATH + "Large_Button_01.sub",
						"over_image" : ROOT_PATH + "Large_Button_02.sub",
						"down_image" : ROOT_PATH + "Large_Button_03.sub",
					},
				),
			},

			## Buttons
			{
				"name" : "left_button",
				"type" : "button",

				"x" : SCREEN_WIDTH * (450 - 22*3) / 800,
				"y" : SCREEN_HEIGHT * (505) / 600,

				"default_image" : "d:/ymir work/ui/intro/select/dragon_left_button_01.sub",
				"over_image" : "d:/ymir work/ui/intro/select/dragon_left_button_02.sub",
				"down_image" : "d:/ymir work/ui/intro/select/dragon_left_button_03.sub",
			},
			{
				"name" : "right_button",
				"type" : "button",

				"x" : SCREEN_WIDTH * (580 - 22) / 800,
				"y" : SCREEN_HEIGHT * (505) / 600,

				"default_image" : "d:/ymir work/ui/intro/select/dragon_right_button_01.sub",
				"over_image" : "d:/ymir work/ui/intro/select/dragon_right_button_02.sub",
				"down_image" : "d:/ymir work/ui/intro/select/dragon_right_button_03.sub",
			},


		),
	}
