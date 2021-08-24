#
# Main component makefile.
#
# This Makefile can be left empty. By default, it will take the sources in the
# src/ directory, compile them and link them into lib(subdirectory_name).a
# in the build directory. This behaviour is entirely configurable,
# please read the ESP-IDF documents if you need to do this.
#

COMMON_SRC_DIR := common/debug \
                  common/bsp \
                  common/ssl \
                  common/utils \
                  common/initialize \
                  common/hardware \
                  common/hardware/hal \
                  common/hardware/component/esp32 \
                  common/hardware/component/motor

ALL_SRC_DIR := main \
              app \
	      app/lcd \
	      app/lcd/modeAutomatic \
	      app/lcd/modeSemiAuto \
	      app/lcd/modeCalibration \
	      app/lcd/modeTests \
	      app/lcd/modeUpdate \
	      app/lcd/modeManual \
	      app/lcd/modeSystem \
	      app/midi \
	      app/sensors \
	      app/heater \
	      app/worm \
	      app/speedVinyl \
	      app/servoMotor $(COMMON_SRC_DIR)

COMPONENT_ADD_INCLUDEDIRS := $(ALL_SRC_DIR)

COMPONENT_SRCDIRS := $(ALL_SRC_DIR)

CFLAGS += -DESP32 -DFREERTOS_V9 -DESP32_4MB

