## README: Gomoku/Pente MCTS Bot

This project uses a C++ implementation of a Monte Carlo Tree Search (MCTS) algorithm, specialized for Gomoku and Pente, and exposes its core search function to Python using **Pybind11**.

### Dependencies

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
pip install "pybind11[global]"
```

### How to Build (Compile C++ to Python Module)



    ```bash
    mkdir build
    cd build
    cmake ..
    cmake --build .
    ```
### ▶️ How to Run

Once the `bot` module has been successfully compiled and is located in the root directory, you can run the Python script:

```bash
# From the project root directory
python main.py
