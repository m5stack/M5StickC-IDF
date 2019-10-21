set(COMPONENT_SRCS 	"util/tftspi.c"
					"util/tft.c"
					"util/spi_master_lobo.c"
					"m5event.c"
					"m5power.c"
					"m5display.c"
					"m5button.c"
					"AXP192.c"
					"wire.c"
					"main.c")
set(COMPONENT_ADD_INCLUDEDIRS "."
							 "util"
							 "util/font" )

register_component()
