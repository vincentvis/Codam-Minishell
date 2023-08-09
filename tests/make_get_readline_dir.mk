# check for homebrew and if homebrew exists
# check for the readline path
ifneq (,$(strip $(shell which brew)))
READLINE_DIR := $(shell brew --prefix readline)
endif

# check if readline exists at the brew location
# this also captures if there was no brew found
ifeq (,$(wildcard $(READLINE_DIR)))
	ifneq (,$(wildcard /usr/local/opt/readline))
		READLINE_DIR := /usr/local/opt/readline
	else
		PLZ_INSTALL_MSG := Please install with `brew install readline`
		READLINE_DIR := $(error Could not find readline! $(PLZ_INSTALL_MSG))
	endif
endif
