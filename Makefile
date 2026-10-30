.DEFAULT_GOAL := build

CMAKE ?= cmake
CMAKE_ARGS ?=
ARGS ?=

.PHONY: help configure build debug run release run-release format format-check lint check clean

help:
	@printf '%s\n' \
	  'make / make build  Configura e compila em Debug' \
	  'make run           Compila em Debug e executa a CLI' \
	  'make release       Configura e compila em Release' \
	  'make run-release   Compila em Release e executa a CLI' \
	  'make format        Formata os arquivos C++' \
	  'make format-check  Verifica a formatacao sem alterar arquivos' \
	  'make lint          Executa a analise estatica' \
	  'make check         Compila em Debug, verifica formatacao e executa lint' \
	  'make clean         Remove os builds Debug e Release' \
	  '' \
	  'Opcoes: CMAKE_ARGS="-DOPCAO=VALOR" e ARGS="argumentos da CLI"'

configure:
	$(CMAKE) --preset debug $(CMAKE_ARGS)

build: configure
	$(CMAKE) --build --preset debug

debug: build

run: build
	./build/debug/apps/cli/ytdl $(ARGS)

release:
	$(CMAKE) --preset release $(CMAKE_ARGS)
	$(CMAKE) --build --preset release

run-release: release
	./build/release/apps/cli/ytdl $(ARGS)

format: configure
	$(CMAKE) --build --preset format

format-check: configure
	$(CMAKE) --build --preset format-check

lint: configure
	$(CMAKE) --build --preset lint

check: build format-check lint

clean:
	$(CMAKE) -E rm -rf build/debug build/release
