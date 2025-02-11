# TrackHer - Period Tracking Calendar

TrackHer is a lightweight, offline period tracking application developed in **C and GTK**. It helps users track menstrual cycles, predict ovulation, and estimate fertile windows while ensuring **data privacy** with secure **encryption**. 

## 🚀 Features
- **Menstrual Cycle Tracking** – Log period start dates and symptoms.
- **Cycle Prediction Algorithm** – Predict next period and fertile window.
- **Secure Data Storage** – File handling with encryption.
- **Interactive GTK UI** – User-friendly calendar and visual analytics.
- **Offline & Privacy-Focused** – No internet required, local storage only.

## 💻 Tech Stack
- **Programming Language:** C
- **UI Framework:** GTK (for graphical interface)
- **Data Storage:** CSV / JSON file handling
- **Security:** Basic encryption for user privacy

## 📂 Project Structure
```
📦 TrackHer
 ┣ 📂 src                 # Source Code
 ┃ ┣ 📜 main.c            # Main entry point
 ┃ ┣ 📜 backend.c         # Cycle prediction logic, file handling
 ┃ ┣ 📜 ui.c              # GTK-based UI
 ┃ ┣ 📜 encryption.c      # Secure user data storage
 ┣ 📂 assets              # Icons, images, and UI assets
 ┣ 📂 docs                # Documentation & Reports
 ┣ 📂 test                # Test cases & debugging scripts
 ┣ 📂 data                # Stores user cycle data
 ┣ 📜 README.md           # Project Overview
 ┣ 📜 Makefile            # Automate compilation
```

## 🛠 Installation & Setup
### **Prerequisites**
- **Git** (Optional, for version control)
- **GCC Compiler**
- **GTK Development Libraries**

### **Steps to Install**
1. **Clone the repository:**
   ```bash
   git clone https://github.com/yourusername/TrackHer.git
   cd TrackHer
   ```
2. **Install Dependencies:**
   - **Linux (Ubuntu/Debian)**:
     ```bash
     sudo apt update && sudo apt install build-essential libgtk-3-dev -y
     ```
   - **Windows (MinGW):** Install GTK+ manually ([Guide](https://www.gtk.org/)).
3. **Compile the Project:**
   ```bash
   make
   ```
4. **Run the Application:**
   ```bash
   ./trackher
   ```

## 🧪 Testing
To run test cases:
```bash
make test
./test_cycle
./test_storage
./test_ui
```

## 📜 License
This project is licensed under the **MIT License**.

## 🤝 Contributing
1. Fork the repository.
2. Create a feature branch (`git checkout -b feature-name`).
3. Commit changes (`git commit -m 'Add new feature'`).
4. Push to the branch (`git push origin feature-name`).
5. Create a pull request.

---
💡 **Developed by:** Prajwal Khanal, Ranjana Kumari Jha, Shakshi Gyawali, Shaunak Baniya 🚀
