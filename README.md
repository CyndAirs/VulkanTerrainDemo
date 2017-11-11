# VulkanTerrainDemo
Height map-based terrain modeling demo in Vulkan API.

=========================================================

Instrukcja dotycząca kompilacji kodu źródłowego:

1. Utworzyć solucję w Visual Studio 2015 lub wykorzystać załączoną solucję poprawiając jedynie
ścieżki do bibliotek.

2. Należy upewnić się, że na komputerze są zainstalowane prawidłowo sterowniki do Vulkana.

3. Należy zainstalować na komputerze:
- VulkanSDK od LunarG w wersji 1.0.30.0 lub nowszej
- Pobrać biblioteki STB
- Pobrać biblioteki GLM w wersji 0.9.8.3 lub nowszej
- Pobrać biblioteki GLFW w wersji 3.2.1 lub nowszej
Załączona solucja zakłada że wszystko znajduje się na „C:\”

4. W ustawieniach projektu (prawym na projekt - > Properties →Configuration Properties) należy:

- W General -> Character Set ustawić „Use Unicode Character Set”

-VC++ Directories → Include Directories dodać:
{ścieżka do Vulkan SDK}\VulkanSDK\{numer wersji}\Include

-VC++ Directories → Library Directories dodać:
{ścieżka do Vulkan SDK}\VulkanSDK\{numer wersji}\Bin
dla wersji 64-bitowej lub
{ścieżka do Vulkan SDK}\VulkanSDK\{numer wersji}\Bin32
dla wersji 32-bitowej

-C/C++→General→Additional Include Directories dodać:
{ścieżka do STB}\stb-master;{ścieżka do GLM}\glm-0.9.8.3\glm;{ścieżka do GLFW}\glfw-3.2.1.bin.WIN64\include;{ścieżka do Vulkan SDK}\VulkanSDK\1.0.30.0\Include;
dla wersji 64-bitowej lub
{ścieżka do STB}\stb-master;{ścieżka do GLM}\glm-0.9.8.3\glm;{ścieżka do GLFW}\glfw-3.2.1.bin.WIN32\include;{ścieżka do Vulkan SDK}\VulkanSDK\1.0.30.0\Include;
dla wersji 32-bitowej

-C/C++→Preprocessor→Preprocessor Definitions
WIN32;_DEBUG;_MBCS;
Dla Debug
WIN32;NDEBUG;_CONSOLE;_LIB;_MBCS;
Dla Release

-Linker→General→Additional Library Dependencies
{ścieżka do GLFW}\glfw-3.2.1.bin.WIN64\lib-vc2015;{ścieżka do Vulkan SDK}\VulkanSDK\1.0.30.0\Bin;
dla wersji 64-bitowej lub
{ścieżka do GLFW}\glfw-3.2.1.bin.WIN32\lib-vc2015;{ścieżka do Vulkan SDK}\VulkanSDK\1.0.30.0\Bin32;
dla wersji 32-bitowej

- Linker→Input→Additional Dependencies:
vulkan-1.lib;glfw3.lib;

5. Aby kompilować programy cieniujące należy wykorzystać plik .bat załączony w ich folderze. Bat
wykorzystuje GLSL w folderze Vulkan SDK, ścieżkę do folderu należy podać wewnątrz pliku bat.
