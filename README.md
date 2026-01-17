# Mini Version Control System (Mini-VCS)

A lightweight snapshot-based version control system built using C++, MySQL, and basic web technologies.

---

## 🚀 Features
- Create snapshots of a project directory
- Store commit messages and metadata in MySQL
- View version history via command line
- Export version history to JSON
- Display version history using a simple web interface

---

## 🛠 Tech Stack
- C++
- MySQL
- HTML
- JavaScript
- Python (for local HTTP server)

---

## 📂 Project Structure

mini-vcs/
├── vcs_core/
│ ├── main.cpp
│ ├── vcs
├── web/
│ ├── index.html
│ ├── versions.json
├── README.md
└── .gitignore


---

## ▶️ How to Run

### 1️⃣ Compile the program
```bash
g++ -std=c++17 main.cpp -o vcs $CPPFLAGS $LDFLAGS -lmysqlclient

2️⃣ Run the CLI
./vcs

3️⃣ Serve the Web UI
cd web
python3 -m http.server 8000

Open in browser:
http://localhost:8000