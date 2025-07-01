#ifndef HELPERS_H
#define HELPERS_H

#include "types.h"

/*
 * Helpers:
 * This file contains helpers for the main file and the helper file dedicated
 * to types handling (type-handlers).
 * The helpers are either variables to save data or functions that interact
 * with this data or with the user.
 */

// Authenticated user
extern User user;

/*
 * Prints options_count options provided as arguments.
 * Prompts the user to choose an option. Returns the index of the selected
 * option if the index is valid, else returns -1.
 */
int handle_option_choice(size_t options_count, ...);

#endif
