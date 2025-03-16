.PHONY: all

all: build/lithium-language-support.vsix
	@echo > /dev/null

build/lithium-language-support.vsix: syntaxes/lithium.tmLanguage.json \
	lithium-configuration.json \
	syntaxes/lithium-asm.tmLanguage.json \
	lithium-asm-configuration.json \
	package.json \
	extension.js \
	hoverProvider.js 
	@npm install > /dev/null
	@npm run package > /dev/null