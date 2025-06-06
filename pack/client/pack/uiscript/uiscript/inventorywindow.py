import uiScriptLocale
import app
from utils import ReplaceElement, AppendChildren

EQUIPMENT_START_INDEX = 90

window = {
	"name" : "InventoryWindow",

	"x" : SCREEN_WIDTH - 176 - 200,
	"y" : SCREEN_HEIGHT - 37 - 565,

	"style" : ("movable", "float",),

	"width" : 176,
	"height" : 565,

	"children" :
	(
		{
			"name" : "board",
			"type" : "board",
			"style" : ("attach",),

			"x" : 0,
			"y" : 0,

			"width" : 176,
			"height" : 565,

			"children" :
			(
				## Title
				{
					"name" : "TitleBar",
					"type" : "titlebar",
					"style" : ("attach",),

					"x" : 8,
					"y" : 7,

					"width" : 161,
					"color" : "yellow",

					"children" :
					(
						{ "name":"TitleName", "type":"text", "x":77, "y":3, "text":uiScriptLocale.INVENTORY_PAGE_BUTTON_TOOLTIP_2, "text_horizontal_align":"center" },
					),
				},

				## Equipment Slot
				{
					"name" : "Equipment_Base",
					"type" : "image",

					"x" : 10,
					"y" : 33,

					"image" : "d:/ymir work/ui/game/windows/equipment_base.sub",

					"children" :
					(

						{
							"name" : "EquipmentSlot",
							"type" : "slot",

							"x" : 3,
							"y" : 3,

							"width" : 150,
							"height" : 182,

							"slot" : (
										{"index":item.EQUIPMENT_BODY, "x":39, "y":37, "width":32, "height":64},
										{"index":item.EQUIPMENT_HEAD, "x":39, "y":2, "width":32, "height":32},
										{"index":item.EQUIPMENT_SHOES, "x":39, "y":145, "width":32, "height":32},
										{"index":item.EQUIPMENT_WRIST, "x":75, "y":67, "width":32, "height":32},
										{"index":item.EQUIPMENT_WEAPON, "x":3, "y":3, "width":32, "height":96},
										{"index":item.EQUIPMENT_NECK, "x":114, "y":84, "width":32, "height":32},
										{"index":item.EQUIPMENT_EAR, "x":114, "y":52, "width":32, "height":32},
										{"index":item.EQUIPMENT_UNIQUE1, "x":2, "y":113, "width":32, "height":32},
										{"index":item.EQUIPMENT_UNIQUE2, "x":75, "y":113, "width":32, "height":32},
										{"index":item.EQUIPMENT_ARROW, "x":114, "y":1, "width":32, "height":32},
										{"index":item.EQUIPMENT_SHIELD, "x":75, "y":35, "width":32, "height":32},
									),
						},

						{
							"name" : "Equipment_Tab_01",
							"type" : "radio_button",

							"x" : 86,
							"y" : 161,

							"default_image" : "d:/ymir work/ui/game/windows/tab_button_small_01.sub",
							"over_image" : "d:/ymir work/ui/game/windows/tab_button_small_02.sub",
							"down_image" : "d:/ymir work/ui/game/windows/tab_button_small_03.sub",

							"children" :
							(
								{
									"name" : "Equipment_Tab_01_Print",
									"type" : "text",

									"x" : 0,
									"y" : 0,

									"all_align" : "center",

									"text" : "I",
								},
							),
						},
						{
							"name" : "Equipment_Tab_02",
							"type" : "radio_button",

							"x" : 86 + 32,
							"y" : 161,

							"default_image" : "d:/ymir work/ui/game/windows/tab_button_small_01.sub",
							"over_image" : "d:/ymir work/ui/game/windows/tab_button_small_02.sub",
							"down_image" : "d:/ymir work/ui/game/windows/tab_button_small_03.sub",

							"children" :
							(
								{
									"name" : "Equipment_Tab_02_Print",
									"type" : "text",

									"x" : 0,
									"y" : 0,

									"all_align" : "center",

									"text" : "II",
								},
							),
						},

					),
				},

				{
					"name" : "Inventory_Tab_01",
					"type" : "radio_button",

					"x" : 10,
					"y" : 33 + 191,

					"default_image" : "d:/ymir work/ui/game/windows/tab_button_large_01.sub",
					"over_image" : "d:/ymir work/ui/game/windows/tab_button_large_02.sub",
					"down_image" : "d:/ymir work/ui/game/windows/tab_button_large_03.sub",
					"tooltip_text" : uiScriptLocale.INVENTORY_PAGE_BUTTON_TOOLTIP_1,

					"children" :
					(
						{
							"name" : "Inventory_Tab_01_Print",
							"type" : "text",

							"x" : 0,
							"y" : 0,

							"all_align" : "center",

							"text" : "I",
						},
					),
				},
				{
					"name" : "Inventory_Tab_02",
					"type" : "radio_button",

					"x" : 10 + 78,
					"y" : 33 + 191,

					"default_image" : "d:/ymir work/ui/game/windows/tab_button_large_01.sub",
					"over_image" : "d:/ymir work/ui/game/windows/tab_button_large_02.sub",
					"down_image" : "d:/ymir work/ui/game/windows/tab_button_large_03.sub",
					"tooltip_text" : uiScriptLocale.INVENTORY_PAGE_BUTTON_TOOLTIP_2,

					"children" :
					(
						{
							"name" : "Inventory_Tab_02_Print",
							"type" : "text",

							"x" : 0,
							"y" : 0,

							"all_align" : "center",

							"text" : "II",
						},
					),
				},

				## Item Slot
				{
					"name" : "ItemSlot",
					"type" : "grid_table",

					"x" : 8,
					"y" : 246,

					"start_index" : 0,
					"x_count" : 5,
					"y_count" : 9,
					"x_step" : 32,
					"y_step" : 32,

					"image" : "d:/ymir work/ui/public/Slot_Base.sub"
				},

				## Print
				{
					"name":"Money_Slot",
					"type":"button",

					"x":8,
					"y":28,

					"horizontal_align":"center",
					"vertical_align":"bottom",

					"default_image" : "d:/ymir work/ui/public/parameter_slot_05.sub",
					"over_image" : "d:/ymir work/ui/public/parameter_slot_05.sub",
					"down_image" : "d:/ymir work/ui/public/parameter_slot_05.sub",

					"children" :
					(
						{
							"name":"Money_Icon",
							"type":"image",

							"x":-18,
							"y":20,

							"image":"d:/ymir work/ui/game/windows/money_icon.sub",
						},

						{
							"name" : "Money",
							"type" : "text",

							"x" : 3,
							"y" : 3,

							"horizontal_align" : "right",
							"text_horizontal_align" : "right",

							"text" : "123456789",
						},
					),
				},

			),
		},
	),
}

if app.ENABLE_CHEQUE_SYSTEM:
	slot = {
		"name":"Money_Slot",
		"type":"button",

		"x":75,
		"y":28,

		#"horizontal_align":"center",
		"vertical_align":"bottom",

		"default_image" : "d:/ymir work/ui/public/gold_slot.sub",
		"over_image" : "d:/ymir work/ui/public/gold_slot.sub",
		"down_image" : "d:/ymir work/ui/public/gold_slot.sub",

		"children" :
		(
			{
				"name" : "Money",
				"type" : "text",

				"x" : 3,
				"y" : 3,

				"horizontal_align" : "right",
				"text_horizontal_align" : "right",

				"text" : "123456789",
			},
		),
	}

	children = (
		{
			"name":"Money_Icon",
			"type":"image",
			"vertical_align":"bottom",

			"x":57,
			"y":26,

			"image":"d:/ymir work/ui/game/windows/money_icon.sub",
		},

		{
			"name":"Cheque_Icon",
			"type":"image",
			"vertical_align":"bottom",

			"x":10,
			"y":26,

			"image":"d:/ymir work/ui/game/windows/cheque_icon.sub",
		},
		{
			"name":"Cheque_Slot",
			"type":"button",

			"x":28,
			"y":28,

			#"horizontal_align":"center",
			"vertical_align":"bottom",

			"default_image" : "d:/ymir work/ui/public/cheque_slot.sub",
			"over_image" : "d:/ymir work/ui/public/cheque_slot.sub",
			"down_image" : "d:/ymir work/ui/public/cheque_slot.sub",

			"children" :
			(
				{
					"name" : "Cheque",
					"type" : "text",

					"x" : 3,
					"y" : 3,

					"horizontal_align" : "right",
					"text_horizontal_align" : "right",

					"text" : "99",
				},
			),
		},
	)

	ReplaceElement("Money_Slot", slot, window)
	AppendChildren("board", children, window)
