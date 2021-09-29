#include <strings.h>
#include "log.h"
#include "sway/commands.h"
#include "sway/config.h"
#include "sway/tree/arrange.h"
#include "sway/tree/container.h"
#include "sway/tree/view.h"
#include "sway/tree/workspace.h"
#include "util.h"

// maximize [enable|disable|toggle]
struct cmd_results *cmd_maximize(int argc, char **argv) {
	struct cmd_results *error = NULL;
	if ((error = checkarg(argc, "maximize", EXPECTED_AT_MOST, 1))) {
		return error;
	}
	if (!root->outputs->length) {
		return cmd_results_new(CMD_FAILURE,
				"Can't run this command while there's no outputs connected.");
	}
	struct sway_container *container = config->handler_context.container;

	if (!container) {
		// If the focus is not a container, do nothing successfully
		return cmd_results_new(CMD_SUCCESS, NULL);
	} else if (!container->pending.workspace) {
		// If in the scratchpad, operate on the highest container
		while (container->pending.parent) {
			container = container->pending.parent;
		}
	}

	bool is_maximized = container->pending.maximized;
	bool enable = !is_maximized;

	if (argc >= 1) {
		enable = parse_boolean(argv[0], is_maximized);
	}

	container_set_maximized(container, enable);
	arrange_root();

	return cmd_results_new(CMD_SUCCESS, NULL);
}
