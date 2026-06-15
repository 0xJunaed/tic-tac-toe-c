

# 🎮 Tic-Tac-Toe with User Accounts

An advanced, terminal-based C game featuring secure user registration, persistent profile management, and a smart AI opponent.

## ✨ Features
* **User Authentication:** Integrated registration and login menus with hidden, masked password input (`*` characters).
* **Persistent Statistics:** Automatic tracking of total matches, wins, losses, and draws saved locally via binary file processing (`.dat`).
* **Dynamic UI Elements:** Custom-built loading screens complete with simulated progress bar animations.
* **Multiple Game Modes:**
  * **Solo:** Play against a deterministic AI featuring a strategic "God-Level" mode designed to be unbeatable.
  * **Duo:** Pass-and-play matches locally with a friend.

## ⚠️ System Requirements
* **Operating System:** Windows (Required due to dependencies on `<conio.h>` and Windows console commands).
* **Compiler:** GCC / MinGW

## 💻 How to Run

1. Open your Windows Command Prompt (CMD) or PowerShell.
2. Compile the game source file:
```cmd
   gcc tic-tac-toe.c -o tictactoe
