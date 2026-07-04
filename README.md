# 🚗 Vehicle Maintenance Record System (VMRS)

A lightweight, menu-driven CRUD application for tracking vehicle maintenance history, built in C. Records are stored in binary files using an **indexed storage design** for fast search, sorting, and retrieval.

> Keep a clean, persistent log of every service performed on a fleet of vehicles — who owns it, what was done, when, and how much it cost.

---

## ✨ Features

| # | Operation | Description |
|---|-----------|-------------|
| 1 | **Register** | Add one or more new maintenance records |
| 2 | **Display** | List every stored maintenance record |
| 3 | **Search** | Look up a vehicle by its license plate |
| 4 | **Update** | Modify the service details or cost of a record |
| 5 | **Delete** | Remove a specific record (with confirmation) |
| 6 | **Sort** | Order records by cost, ascending or descending |
| 0 | **Exit** | Close the program |

Additional highlights:

- 🔒 **Validated input everywhere** — plates, models, dates, and costs are all range-checked, with graceful re-prompts on bad input.
- 🚫 **Duplicate protection** — a plate can only be registered once.
- 🏷️ **Standardized plate format** — `LLL-NNN` (3 uppercase letters + 3 digits).
- 🔧 **Predefined service catalog** — Paint, Body Work, Paint & Body Work, or Maintenance.

---

## 🧠 How It Works

VMRS uses a **two-file indexed storage** model under `data/`:

- **`data.bin`** — the full vehicle records (owner, model, plate, service, date, cost).
- **`index.bin`** — a compact index of `{ plate, cost, byte-position }` pointing into `data.bin`.

Searching, sorting, and listing all iterate the small **index** file, then jump straight to the exact byte offset in `data.bin`. This keeps expensive operations off the heavier record data — sorting, for example, only rearranges the index while the actual records stay put.

```
index.bin                         data.bin
┌────────────────────────┐        ┌──────────────────────────────┐
│ ABC-123 | 1500 | 0     │──────▶ │ owner, model, service, date… │
│ XYZ-789 |  800 | 232   │──────▶ │ owner, model, service, date… │
└────────────────────────┘        └──────────────────────────────┘
```

---

## 🛠️ Build & Run

You need a C/C++ compiler (`g++` recommended).

```bash
# Compile
g++ main.cpp -o main

# Run (from the project root, so the data/ folder is found)
./main
```

> ⚠️ The program reads and writes to `data/`, so always launch it from the project root directory.

On the very first run with no records, VMRS walks you through creating a starter record to get going.

---

## 📁 Project Structure

```
VMRS/
├── main.cpp          # Complete application source
├── data/
│   ├── data.bin      # Full vehicle records
│   └── index.bin     # Index of plate / cost / position
└── README.md
```

---

## 📋 Data Model

**Vehicle record**

| Field | Type | Notes |
|-------|------|-------|
| Owner | text | Vehicle owner's name |
| Model | integer | Manufacturing year (1885–2026) |
| Plate | text | Format `LLL-NNN`, unique |
| Service | text | From the service catalog |
| Date | day / month / year | Service date |
| Cost | integer | Non-negative |

---

## 👤 Author

**Jose Eduardo Meneses Hernández**

Developed with Dev-C++ 5.11 / VSCode.
