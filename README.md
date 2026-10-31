# vidx

Projeto experimental para portar progressivamente o [yt-dlp](https://github.com/yt-dlp/yt-dlp)
para C++20, construindo uma biblioteca que possa ser integrada a aplicações desktop
e mobile.

## Intuito do projeto

O objetivo é disponibilizar as funcionalidades de extração de informações e download
de mídia do yt-dlp em um núcleo C++ reutilizável. A intenção é facilitar a integração
com outras linguagens de sistemas, como Rust, e com as APIs nativas do Android e iOS,
sem exigir um interpretador Python embarcado para executar o futuro núcleo portado.

O núcleo será independente das interfaces dos aplicativos. As integrações planejadas
incluem uma API C para interoperabilidade com Rust, JNI para Kotlin/Java no Android
e uma ponte nativa para Swift no iOS. Isso também permitirá explorar o uso em
aplicações Tauri e módulos nativos para Expo/React Native.

A portabilidade será construída e validada gradualmente. O processamento de mídia
e outras funcionalidades poderão exigir dependências adicionais conforme forem
implementados; a escolha de C++ não elimina essas necessidades.

## Estado atual

O projeto está na fase inicial de configuração. Atualmente há uma biblioteca estática
e uma CLI que imprime apenas `Hello, world!`, além das ferramentas de compilação,
formatação e análise estática.

Ainda não há extração de informações, downloads, API C, bindings Rust, pontes JNI/Swift
ou suporte validado a Android/iOS. Este repositório ainda não substitui o yt-dlp.

## Próximos passos

- Implementar a consulta de informações de uma URL para um primeiro site.
- Adicionar seleção de formatos e download básico.
- Expor progresso e cancelamento de downloads.
- Criar uma API de interoperabilidade e validar o consumo por Rust.
- Configurar builds e integrações nativas para Android e iOS.
- Ampliar gradualmente a cobertura de sites e funcionalidades.

## Estrutura

```text
include/vidx/      Headers públicos da biblioteca (greeting.hpp)
src/               Implementação da biblioteca (greeting.cpp) e CLI (main.cpp)
CMakeLists.txt     Alvos, lista de fontes e ferramentas de qualidade
Makefile           Atalhos para compilar, executar e verificar
CMakePresets.json  Presets Debug e Release
.github/workflows/ Compilação e verificações no GitHub Actions
```

O alvo `vidx::core` é uma biblioteca estática independente de Tauri, Expo e Kotlin.
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
`-DVIDX_CLANG_FORMAT=/caminho/clang-format` e
`-DVIDX_CLANG_TIDY=/caminho/clang-tidy` ao CMake.
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
`make build CMAKE_ARGS="-DVIDX_WARNINGS_AS_ERRORS=ON"` e argumentos para a CLI
com `make run ARGS="..."` (o Hello World ainda não interpreta argumentos).

### Comandos CMake diretos

Execute os comandos a partir da raiz do projeto:

```sh
cmake --preset debug
cmake --build --preset debug
./build/debug/vidx
```

No Windows, use um terminal de desenvolvimento do Visual Studio com Ninja
disponível e execute `build/debug/vidx.exe`.

Para uma compilação otimizada:

```sh
cmake --preset release
cmake --build --preset release
```

Para compilar somente a biblioteca, configure com `-DVIDX_BUILD_CLI=OFF`.
Para tratar avisos do compilador como erros, use `-DVIDX_WARNINGS_AS_ERRORS=ON`.

## Formatação e análise estática

Após configurar o preset `debug`:

```sh
cmake --build --preset format        # Aplica clang-format
cmake --build --preset format-check  # Confere sem modificar arquivos
cmake --build --preset lint          # Executa clang-tidy
```

Esses alvos ficam disponíveis quando as respectivas ferramentas são encontradas.
O build comum não exige LLVM instalado. Ao adicionar arquivos `.cpp` à biblioteca,
registre-os em `VIDX_SOURCES` no `CMakeLists.txt` da raiz. Headers públicos entram
em `VIDX_HEADERS`. Os linters reutilizam essas listas, sem cadastro duplicado.
Headers internos podem ficar junto da implementação em `src/`; registre-os em
`VIDX_HEADERS` apenas se também quiser incluí-los na formatação, sem movê-los
para `include/`.

Por exemplo, uma implementação interna em `src/protocols/http.cpp` entra em
`VIDX_SOURCES`. A pasta não precisa ter seu próprio `CMakeLists.txt`.

O arquivo `build/debug/compile_commands.json` é gerado para ferramentas como
clangd; `.clangd` já aponta para essa pasta. `.editorconfig` define as convenções
básicas de edição e `.clang-format` define o estilo C++.

O GitHub Actions está configurado para compilar e verificar a saída do Hello World
em Linux, macOS e Windows, além de executar formatação e análise estática em Linux.
As compilações Android/iOS serão configuradas junto às respectivas integrações.

## Referência e licença

O [yt-dlp](https://github.com/yt-dlp/yt-dlp) é o projeto de referência para este port.
Esta é uma iniciativa independente, sem vínculo oficial com seus mantenedores.
O código C++ atual contém somente o exemplo inicial; nenhum extrator foi portado ainda.

O código original deste repositório é distribuído sob a [licença MIT](LICENSE).
O código de referência do yt-dlp é distribuído sob a
[Unlicense](https://github.com/yt-dlp/yt-dlp/blob/master/LICENSE).
Código de terceiros incorporado futuramente deverá manter os respectivos avisos
e condições de licença.
