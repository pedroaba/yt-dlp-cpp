# ytdl — C++

Base inicial em C++20 para um motor compartilhado entre desktop e mobile.
Por enquanto, o único comportamento é imprimir `Hello, world!`.

## Estrutura

```text
apps/cli/           Executável de demonstração
include/ytdl/core/  API pública da biblioteca
src/core/          Implementação do núcleo
cmake/             Opções de compilação e ferramentas de qualidade
.github/workflows/ Compilação e verificações no GitHub Actions
```

O alvo `ytdl::core` é uma biblioteca estática independente de Tauri, Expo e Kotlin.
A CLI apenas consome essa biblioteca. As futuras pontes nativas serão adicionadas
separadamente; este projeto ainda não inclui extratores, downloads ou integrações mobile.
A biblioteca usa código independente de posição para permitir sua futura ligação
com bibliotecas compartilhadas. O uso de C++ na API atual é interno: uma futura
ponte FFI deverá expor uma interface apropriada, como uma API C ou JNI.

## Pré-requisitos

- Compilador com suporte a C++20: Clang, GCC ou MSVC.
- CMake 3.24 ou superior.
- Ninja para os presets incluídos.
- `clang-format` e `clang-tidy` para as verificações de qualidade.

No macOS, com Homebrew e as ferramentas de linha de comando do Xcode instaladas:

```sh
brew install cmake ninja llvm
```

O CMake procura as ferramentas LLVM no PATH e nos caminhos usuais do Homebrew.
Para outra instalação, defina `LLVM_ROOT` antes de configurar ou passe
`-DYTDL_CLANG_FORMAT=/caminho/clang-format` e
`-DYTDL_CLANG_TIDY=/caminho/clang-tidy` ao CMake.
O CI utiliza LLVM 18 como referência para formatação e análise estática.

## Compilar e executar

### Atalhos com Make

No macOS e Linux, com `make` instalado, execute na raiz do projeto:

```sh
make              # Configura e compila em Debug
make run          # Compila e executa o Hello World
make release      # Compila em Release
make run-release  # Compila e executa em Release
make format       # Aplica a formatacao
make format-check # Verifica a formatacao
make lint         # Executa clang-tidy
make check        # Compila, verifica formatacao e executa lint
make clean        # Remove os builds Debug e Release
make help         # Lista os atalhos
```

O Makefile usa os mesmos presets CMake e configura o projeto automaticamente
antes de compilar ou executar as ferramentas de qualidade. A compilação continua
usando Ninja. É possível passar opções com
`make build CMAKE_ARGS="-DYTDL_WARNINGS_AS_ERRORS=ON"` e argumentos para a CLI
com `make run ARGS="..."` (o Hello World ainda não interpreta argumentos).

### Comandos CMake diretos

Execute os comandos a partir da raiz do projeto:

```sh
cmake --preset debug
cmake --build --preset debug
./build/debug/apps/cli/ytdl
```

No Windows, use um terminal de desenvolvimento do Visual Studio com Ninja
disponível e execute `build/debug/apps/cli/ytdl.exe`.

Para uma compilação otimizada:

```sh
cmake --preset release
cmake --build --preset release
```

Para compilar somente a biblioteca, configure com `-DYTDL_BUILD_CLI=OFF`.
Para tratar avisos do compilador como erros, use `-DYTDL_WARNINGS_AS_ERRORS=ON`.

## Formatação e análise estática

Após configurar o preset `debug`:

```sh
cmake --build --preset format        # Aplica clang-format
cmake --build --preset format-check  # Confere sem modificar arquivos
cmake --build --preset lint          # Executa clang-tidy
```

Esses alvos ficam disponíveis quando as respectivas ferramentas são encontradas.
O build comum não exige LLVM instalado. Ao adicionar arquivos C++, registre-os
nos alvos CMake correspondentes e nas listas de `cmake/CodeQuality.cmake`.

O arquivo `build/debug/compile_commands.json` é gerado para ferramentas como
clangd; `.clangd` já aponta para essa pasta. `.editorconfig` define as convenções
básicas de edição e `.clang-format` define o estilo C++.

O GitHub Actions está configurado para compilar e verificar a saída do Hello World
em Linux, macOS e Windows, além de executar formatação e análise estática em Linux.
As compilações Android/iOS serão configuradas junto às respectivas integrações.
