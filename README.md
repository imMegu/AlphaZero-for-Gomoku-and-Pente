## README: Gomoku/Pente MCTS Bot

This project uses a C++ implementation of a Monte Carlo Tree Search (MCTS) algorithm, specialized for Gomoku (and partially Pente), and exposes its core search function to Python using **Pybind11**.

The C++ code is optimized for speed using the `-O2` compiler flag.

-----

### 📦 Dependencies

To build and run this project, you need the following tools and libraries:

| Category | Dependency | Notes |
| :--- | :--- | :--- |
| **C++** | C++11 or higher compatible compiler (GCC/Clang on Linux/macOS, MSVC on Windows). | |
| **Build System** | **CMake** (v3.14 or newer recommended) | Used for cross-platform compilation. |
| **Python** | **Python 3** (3.8+ recommended) | Python interpreter and development headers. |
| **Python Bindings** | **Pybind11** | The library used to bind C++ code to Python. |

#### **Installing Pybind11**

The easiest way to install Pybind11 for use with CMake is via `pip` (preferably in a virtual environment):

```bash
# Recommended installation for Pybind11 to be found by CMake
pip install "pybind11[global]"
```

-----

### ⚙️ How to Build (Compile C++ to Python Module)

The C++ code in `PythonConvert.cpp` is compiled into a Python module named `bot`.

1.  **Configure CMake:**
    Create a `build` directory and run CMake from inside it.

    ```bash
    mkdir build
    cd build
    cmake ..
    ```

2.  **Compile the Module:**
    Run the build command, explicitly specifying the `Release` configuration to enable the required **`-O2` optimization flag**.

    | Operating System | Command | Output Module |
    | :--- | :--- | :--- |
    | **Linux/macOS** | `cmake --build . --config Release` | `bot.so` (e.g., `bot.cpython-310-x86_64-linux-gnu.so`) |
    | **Windows** | `cmake --build . --config Release` | `bot.pyd` |

    The compiled output file (`bot.so` or `bot.pyd`) will be placed in the project's root directory (next to `main.py`).

-----

### ▶️ How to Run

Once the `bot` module has been successfully compiled and is located in the root directory, you can run the Python script:

```bash
# From the project root directory
python main.py
```

The `main.py` script will import the `bot` module and call the high-performance C++ search function:

```python
# Snippet from main.py
import bot
# ...
move = bot.run_mcts_search(
    self.board_size,
    player1,
    player2,
    turn_number
)
```

-----

If you need any further assistance with the CMake configuration or troubleshooting the compilation, let me know\!
