cc_library(
	name="pixy_uart",
	srcs=["input/pixycam/PixyUART.cpp","input/pixycam/PixyImpl.cpp","input/pixycam/PixyBlock.cpp"],
	hdrs=["input/pixycam/PixyUART.h","input/pixycam/Pixy.h"],
	linkopts=["-pthread"]
)

cc_test(
	name="point_test",
	srcs=["util/point.cpp","util/point.h","util/interface.h","util/maybe.h","util/driver_station_interface.h","util/maybe_inline.h","util/checked_array.h","util/util.h","util/util.cpp"],
	deps=[":pixy_uart",":interface"],
	copts=["-DPOINT_TEST"],
	timeout="short"
)

cc_test(
	name="type_test",
	srcs=["util/type.cpp","util/type.h"],
	copts=["-DTYPE_TEST"],
	timeout="short"
)

cc_library(
	name="point",
	srcs=["util/point.cpp"],
	hdrs=["util/point.h","util/quick.h","util/interface.h","util/maybe.h","util/driver_station_interface.h","util/maybe_inline.h","util/checked_array.h","util/util.h"],
	deps=[":pixy_uart"]
)

cc_test(
	name="util_test",
	#srcs=["util/util.cpp","util/interface.h","util/maybe.h","util/driver_station_interface.h","util/maybe_inline.h","util/checked_array.h","util/util.h"],
	srcs=["util/util.cpp"],
	copts=["-DUTIL_TEST"],
	deps=[":point",":interface"],
	timeout="short"

)

cc_library(
	name="util",
	srcs=["util/util.cpp"],
	deps=[":point"]
)

cc_test(
	name="settable_constant_test",
	srcs=["util/settable_constant.cpp","util/settable_constant.h"],
	copts=["-DSETTABLE_CONSTANT_TEST"],
	deps=[],
	timeout="short"

)

cc_library(
	name="settable_constant",
	srcs=["util/settable_constant.cpp"],
	hdrs=["util/settable_constant.h"],
)


cc_test(
	name="driver_station_interface_test",
	srcs=["util/driver_station_interface.cpp"],
	copts=["-DDRIVER_STATION_INTERFACE_TEST"],
	deps=[":util",":interface"],
	timeout="short"

)

cc_library(
	name="driver_station_interface",
	srcs=["util/driver_station_interface.cpp"],
	hdrs=["util/quick.h"],
	deps=[":util"]
)

cc_test(
	name="interface_test",
	srcs=["util/interface.cpp","util/interface.h","util/quick.h"],
	copts=["-DINTERFACE_TEST"],
	deps=[":driver_station_interface",":pixy_uart"],
	timeout="short"
)

cc_library(
	name="posedge_toggle",
	srcs=["util/posedge_toggle.cpp"],
	hdrs=["util/posedge_toggle.h"],
)


cc_library(
	name="type",
	srcs=["util/type.cpp"],
	hdrs=["util/type.h"]
)

cc_library(
	name="countup_timer",
	srcs=["util/countup_timer.cpp"],
	hdrs=["util/countup_timer.h"],
	deps=[":interface"]
)

cc_test(
	name="countup_timer_test",
	srcs=["util/countup_timer.cpp","util/countup_timer.h"],
	copts=["-DCOUNTUP_TIMER_TEST"],
	deps=[":interface"],
	timeout="short"

)

cc_library(
	name="countdown_timer",
	srcs=["util/countdown_timer.cpp"],
	hdrs=["util/countdown_timer.h"],
	deps=[":util"]
)

cc_test(
	name="countdown_timer_test",
	srcs=["util/countdown_timer.cpp","util/countdown_timer.h"],
	copts=["-DCOUNTDOWN_TIMER_TEST"],
	deps=[":util"],
	timeout="short"

)

cc_library(
	name="network",
	srcs=["util/network.cpp"],
	hdrs=["util/network.h"]
)

cc_test(
	name="network_test",
	srcs=["util/network.cpp","util/network.h"],
	copts=["-DNETWORK_TEST"],
	deps=[],
	timeout="short"

)

cc_library(
	name="negedge_trigger",
	srcs=["util/negedge_trigger.cpp"],
	hdrs=["util/negedge_trigger.h"]
)

cc_test(
	name="negedge_trigger_test",
	srcs=["util/negedge_trigger.cpp","util/negedge_trigger.h"],
	copts=["-DNEGEDGE_TRIGGER_TEST"],
	deps=[],
	timeout="short"

)

cc_library(
	name="string_utils",
	srcs=["util/string_utils.cpp"],
	hdrs=["util/string_utils.h"]
)

cc_test(
	name="string_utils_test",
	srcs=["util/string_utils.cpp","util/string_utils.h"],
	copts=["-DSTRING_UTILS_TEST"],
	deps=[],
	timeout="short"
)

cc_library(
	name="posedge_trigger",
	srcs=["util/posedge_trigger.cpp"],
	hdrs=["util/posedge_trigger.h"]
)

cc_test(
	name="posedge_trigger_test",
	srcs=["util/posedge_trigger.cpp","util/posedge_trigger.h"],
	copts=["-DPOSEDGE_TRIGGER_TEST"],
	deps=[],
	timeout="short"
)

cc_library(
	name="debounce",
	srcs=["util/debounce.cpp"],
	hdrs=["util/debounce.h"],
	deps=[":countdown_timer"]
)

cc_test(
	name="debounce_test",
	srcs=["util/debounce.cpp","util/debounce.h"],
	copts=["-DDEBOUNCE_TEST"],
	deps=[":countdown_timer"],
	timeout="short"
)

cc_library(
	name="interface",
	srcs=["util/interface.cpp"],
	hdrs=["util/interface.h","util/quick.h"],
	deps=[":driver_station_interface",":pixy_uart"]
)

cc_library(
	name="settable_toggle",
	srcs=["util/settable_toggle.cpp"],
	hdrs=["util/settable_toggle.h"],
	deps=[":posedge_trigger"]
)

cc_test(
	name="settable_toggle_test",
	srcs=["util/settable_toggle.cpp","util/settable_toggle.h"],
	copts=["-DSETTABLE_TOGGLE_TEST"],
	deps=[":posedge_trigger"],
	timeout="short"
)

cc_library(
	name="bound_tracker",
	srcs=["util/bound_tracker.cpp"],
	hdrs=["util/bound_tracker.h","util/maybe.h"],
)

cc_test(
	name="bound_tracker_test",
	srcs=["util/bound_tracker.cpp","util/bound_tracker.h","util/maybe.h"],
	copts=["-DBOUND_TRACKER_TEST"],
	deps=[],
	timeout="short"
)

cc_library(
	name="persistent_tracker",
	srcs=["util/persistent_tracker.cpp"],
	hdrs=["util/persistent_tracker.h","util/maybe.h"],
	deps=[":util"]
)

cc_test(
	name="persistent_tracker_test",
	srcs=["util/persistent_tracker.cpp","util/persistent_tracker.h","util/maybe.h"],
	copts=["-DPERSISTENT_TRACKER_TEST"],
	deps=[":util",":interface"],
	timeout="short"
)


cc_library(
	name="stall_monitor",
	srcs=["util/stall_monitor.cpp"],
	hdrs=["util/stall_monitor.h"],
	deps=[":interface",":persistent_tracker"]
)

cc_test(
	name="stall_monitor_test",
	srcs=["util/stall_monitor.cpp","util/stall_monitor.h"],
	copts=["-DSTALL_MONITOR_TEST"],
	deps=[":stall_monitor",":persistent_tracker"],
	timeout="short"
)

cc_library(
	name="fixVictor",
	srcs=["util/fixVictor.cpp"],
	hdrs=["util/fixVictor.h"]
)

cc_test(
	name="fixVictor_test",
	srcs=["util/fixVictor.cpp","util/fixVictor.h"],
	copts=["-DFIXVICTOR_TEST"],
	deps=[],
	timeout="short"
)

cc_library(
	name="input",
	srcs=["util/input.cpp"],
	hdrs=["util/input.h"],
	deps=[":interface"]
)

cc_test(
	name="input_test",
	srcs=["util/input.cpp","util/input.h"],
	copts=["-DINPUT_TEST"],
	deps=[":interface"],
	timeout="short"
)

cc_library(
	name="output",
	srcs=["util/output.cpp"],
	hdrs=["util/output.h"],
	deps=[":interface"]
)

cc_test(
	name="output_test",
	srcs=["util/output.cpp","util/output.h"],
	copts=["-DOUTPUT_TEST"],
	deps=[":interface"],
	timeout="short"
)

cc_library(
	name="motion_profile",
	srcs=["util/motion_profile.cpp"],
	hdrs=["util/motion_profile.h"],
	deps=[":interface",":util"]
)

cc_test(
	name="motion_profile_test",
	srcs=["util/motion_profile.cpp","util/motion_profile.h"],
	copts=["-DMOTION_PROFILE_TEST"],
	deps=[":interface",":util"],
	timeout="short"
)

cc_library(
	name="perf_tracker",
	srcs=["util/perf_tracker.cpp"],
	hdrs=["util/perf_tracker.h"],
	deps=[":interface"]
)

cc_test(
	name="perf_tracker_test",
	srcs=["util/perf_tracker.cpp","util/perf_tracker.h"],
	copts=["-DPERF_TRACKER_TEST"],
	deps=[":interface"],
	timeout="short"
)

cc_library(
	name="posedge_trigger_debounce",
	srcs=["util/posedge_trigger_debounce.cpp"],
	hdrs=["util/posedge_trigger_debounce.h"],
	deps=[":debounce",":posedge_trigger"]
)

cc_test(
	name="posedge_trigger_debounce_test",
	srcs=["util/posedge_trigger_debounce.cpp","util/posedge_trigger_debounce.h"],
	copts=["-DPOSEDGE_TRIGGER_DEBOUNCE_TEST"],
	deps=[":debounce",":posedge_trigger"],
	timeout="short"
)

cc_library(
	name="speed_tracker",
	srcs=["util/speed_tracker.cpp"],
	hdrs=["util/speed_tracker.h"],
	deps=[":interface",":countdown_timer",":drivebase"]
)

cc_test(
	name="speed_tracker_test",
	srcs=["util/speed_tracker.cpp","util/speed_tracker.h"],
	copts=["-DSPEED_TRACKER_TEST"],
	deps=[":interface",":countdown_timer",":drivebase"],
	timeout="short"
)

cc_library(
	name="meta",
	srcs=["util/meta.cpp"],
	hdrs=["util/meta.h"],
	deps=[":interface"]
)

cc_test(
	name="meta_test",
	srcs=["util/meta.cpp","util/meta.h"],
	copts=["-DMETA_TEST"],
	deps=[":interface"],
	timeout="short"
)

cc_library(
	name="auto_distances",
	srcs=["util/auto_distances.cpp", "util/quick.h"],
	hdrs=["util/auto_distances.h"],
	deps=[":interface"]
)

cc_test(
	name="auto_distances_test",
	srcs=["util/auto_distances.cpp","util/auto_distances.h", "util/quick.h"],
	copts=["-DAUTO_DISTANCES_TEST"],
	deps=[":interface"],
	timeout="short"
)

cc_library(
	name="monitor",
	srcs=["control/monitor.cpp"],
	hdrs=["control/monitor.h"],
	deps=[":main"]
)

cc_test(
	name="monitor_test",
	srcs=["control/monitor.cpp","control/monitor.h"],
	copts=["-DMONITOR_TEST"],
	deps=[":main"],
	timeout="short"
)

cc_library(
	name="motor_check",
	srcs=["control/motor_check.cpp"],
	hdrs=["control/motor_check.h"],
	deps=[":util"]
)

cc_test(
	name="motor_check_test",
	srcs=["control/motor_check.cpp","control/motor_check.h"],
	copts=["-DMOTOR_CHECK_TEST"],
	deps=[":util"],
	timeout="short"
)

cc_library(
	name="main",
	srcs=["control/main.cpp"],
	hdrs=["control/main.h"],
	deps=[":force_interface",":posedge_toggle",":perf_tracker",":countdown_timer",":countup_timer",":toplevel",":panel",":log",":posedge_trigger_debounce",":motion_profile",":executive_impl"]
)

cc_test(
	name="main_test",
	srcs=["control/main.cpp","control/main.h"],
	copts=["-DMAIN_TEST"],
	deps=[":force_interface",":toplevel",":posedge_toggle",":perf_tracker",":countup_timer",":panel",":log",":posedge_trigger_debounce",":motion_profile",":monitor"],
	timeout="short"
)

cc_library(
	name="log",
	srcs=["control/log.cpp"],
	hdrs=["control/log.h"],
	deps=[":interface",":toplevel"]
)

cc_test(
	name="log_test",
	srcs=["control/log.cpp","control/log.h","control/formal.h"],
	copts=["-DLOG_TEST"],
	deps=[":interface",":toplevel"],
	timeout="short"
)

cc_library(
	name="drivebase",
	srcs=["control/drivebase.cpp"],
	hdrs=["control/drivebase.h","util/quick.h","util/robot_constants.h"],
	deps=[":interface",":motor_check",":motion_profile",":countdown_timer",":stall_monitor"]
)

cc_test(
	name="drivebase_test",
	srcs=["control/drivebase.cpp","control/drivebase.h","util/quick.h","util/robot_constants.h","control/formal.h"],
	copts=["-DDRIVEBASE_TEST"],
	deps=[":interface",":motion_profile",":motor_check",":countdown_timer",":stall_monitor"],
	timeout="short"
)

cc_library(
	name="lights",
	srcs=["control/lights.cpp"],
	hdrs=["control/lights.h"],
	deps=[":interface",":nop"]
)

cc_test(
	name="lights_test",
	srcs=["control/lights.cpp","control/lights.h","control/formal.h"],
	copts=["-DLIGHTS_TEST"],
	deps=[":lights"],
	timeout="short"
)

cc_library(
	name="force",
	srcs=["control/force.cpp"],
	hdrs=["control/force.h"],
	deps=[":interface"]
)

cc_library(
	name="force_interface",
	srcs=["control/force_interface.cpp"],
	hdrs=["control/force_interface.h"],
	deps=[":posedge_trigger",":force"]
)

cc_test(
	name="force_interface_test",
	srcs=["control/force_interface.cpp","control/force_interface.h"],
	copts=["-DFORCE_INTERFACE_TEST"],
	deps=[":posedge_trigger",":force",":interface"],
	timeout="short"
)


cc_library(
	name="toplevel",
	srcs=["control/toplevel.cpp"],
	hdrs=["control/toplevel.h"],
	deps=[":nop",":pump",":drivebase",":lights",":input"]
)

cc_test(
	name="toplevel_test",
	srcs=["control/toplevel.cpp","control/toplevel.h","control/formal.h"],
	copts=["-DTOPLEVEL_TEST"],
	deps=[":nop",":pump",":drivebase",":lights",":input",":output"],
	timeout="short"
)

cc_library(
	name="sim",
	srcs=["control/sim.cpp"],
	hdrs=["control/nop.h"],
	deps=[":nop",":step",":point"]
)

cc_test(
	name="sim_test",
	srcs=["control/sim.cpp"],
	copts=["-DSIM_TEST","-g"],
	deps=[
		":chain",
		":step",
		":teleop",
		":toplevel",
		":point",
		":type",
		":nop",
		":main",
		":util",
		":align"
	],
	timeout="short"
)

cc_library(
	name="pump",
	srcs=["control/pump.cpp"],
	hdrs=["control/pump.h","util/maybe.h"],
	deps=[":interface"]
)

cc_test(
	name="pump_test",
	srcs=["control/pump.cpp","control/pump.h","util/maybe.h","control/formal.h"],
	copts=["-DPUMP_TEST"],
	deps=[":interface"],
	timeout="short"
)

cc_library(
	name="nop",
	srcs=["control/nop.cpp"],
	hdrs=["control/nop.h"],
	deps=[":interface"]
)

cc_test(
	name="nop_test",
	srcs=["control/nop.cpp","control/nop.h","control/formal.h"],
	copts=["-DNOP_TEST"],
	deps=[":interface"],
	timeout="short"
)

cc_library(
	name="input_util",
	srcs=["input/util.cpp"],
	hdrs=["input/util.h"],
	deps=[":interface"]
)

cc_library(
	name="panel",
	srcs=["input/panel.cpp"],
	hdrs=["input/panel.h","util/maybe.h"],
	deps=[":interface",":input_util"]
)

cc_test(
	name="panel_test",
	srcs=["input/panel.cpp","input/panel.h"],
	copts=["-DPANEL_TEST"],
	deps=[":interface",":input_util"],
	timeout = "short"
)

cc_library(
	name="executive_impl",
	srcs=[
		"executive/autonomous.cpp",
		"executive/teleop.cpp",
		"executive/chain.cpp",
		"executive/align.cpp"
	],
	hdrs=[
		"executive/autonomous.h",
		"executive/teleop.h",
		"executive/chain.h",
		"executive/align.h"
	],
	deps=[":executive",":posedge_trigger_debounce",":posedge_toggle",":motion_profile",":step"]
)

cc_library(
	name="teleop",
	srcs=["executive/teleop.cpp"],
	hdrs=["executive/teleop.h"],
	deps=[":executive",":posedge_trigger_debounce",":posedge_toggle",":test"]
)

cc_test(
	name="teleop_test",
	srcs=["executive/teleop.cpp","executive/teleop.h"],
	copts=["-DTELEOP_TEST"],
	deps=[":executive",":posedge_trigger_debounce",":posedge_toggle",":test"],
	size="medium"
)

cc_test(
	name="autonomous_test",
	srcs=["executive/autonomous.cpp","executive/autonomous.h"],
	copts=["-DAUTONOMOUS_TEST"],
	deps=[
		":executive",":executive_impl",
		":test",":align"
	],
	timeout="short"
)

cc_library(
	name="executive",
	srcs=["executive/executive.cpp"],
	hdrs=["executive/executive.h"],
	deps=[":toplevel",":panel",":type"]
)

cc_test(
	name="executive_test",	srcs=["executive/executive.cpp","executive/executive.h"],
	copts=["-DEXECUTIVE_TEST"],
	deps=[":executive_impl",":panel",":type"],
	timeout="short"
)

cc_library(
	name="test",
	srcs=["executive/test.cpp"],
	hdrs=["executive/test.h"],
	deps=[":executive",":executive_impl"]
)

cc_library(
	name="step",
	srcs=["executive/step.cpp"],
	hdrs=["executive/step.h","util/robot_constants.h"],
	deps=[":executive",":motion_profile",":settable_constant"]
)

cc_library(
	name="chain",
	srcs=["executive/chain.cpp"],
	hdrs=["executive/chain.h"],
	deps=[":step",":executive_impl"]
)

cc_library(
	name="align",
	srcs=["executive/align.cpp"],
	hdrs=["executive/align.h"],
	deps=[":executive",":motion_profile",":step"]
)

cc_test(
	name="align_test",
	srcs=["executive/align.cpp","executive/align.h"],
	copts=["-DALIGN_TEST"],
	deps=[
		":executive",
		":step",
		":test",
		":teleop",
		":chain",
		":motion_profile"
	],
	timeout="short"
)

cc_test(
	name="step_test",
	srcs=["executive/step.cpp","executive/step.h","util/robot_constants.h"],
	copts=["-DSTEP_TEST"],
	deps=[":executive_impl",":motion_profile",":settable_constant"],
	timeout="short"
)

cc_test(
	name="chain_test",
	srcs=["executive/chain.cpp","executive/chain.h"],
	copts=["-DCHAIN_TEST"],
	deps=[":step",":executive_impl"],
	timeout="short"
)
