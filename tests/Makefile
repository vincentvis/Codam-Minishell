TEST_DIRS := ./status/ \
				./lexer/ \
				./parser/ \
				./env/ \
				./utils/ \
				./builtins/ \
				./expansion/

# called here so it's only done once, since `brew --prefix` can sometimes be slow
CRITERION_BREW_DIR := $(shell brew --prefix criterion)
export CRITERION_BREW_DIR

include ./make_get_readline_dir.mk

export READLINE_DIR

# EXPLANATION OF THE "DONE" file
#
# make stops on error, we do not want that..
# so we can add a single dash in front of the rule to make it continue on error
# but then we can't catch error for automated testing.. :S
# so each sub-makefile writes an error script to the "done" file on a sub-error
# this file is then executed after all tests are run and should give this
# "parent"-makefile a non-zero exit status if any of the tests failed

.PHONY: all
all: setup
	@$(MAKE) -j 4 $(TEST_DIRS)
	@./done

.PHONY: setup
# make empty executable file for later
setup:
	@echo "#!/bin/bash" > done
	@echo "rm done" >> done
	@chmod +x done

.PHONY: $(TEST_DIRS)
$(TEST_DIRS):
	-@$(MAKE) -C $@ run
