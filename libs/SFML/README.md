# SFML Setup

This project uses **SFML 3.1.0 (64-bit)**.

## 1. Download SFML

Download SFML 3.1.0 (64-bit) from the official website:  
https://www.sfml-dev.org/download.php

> ⚠️ Make sure you choose the **64-bit version**, since the project is configured for it.

## 2. Extract required files

After downloading the archive, extract it.

Inside the archive, you will find multiple folders.  
You only need:

- `include`
- `lib`

All other folders can be ignored.

## 3. Project structure

In this repository, there is already a `libs` folder and `SFML` folder.

Create the following structure:

```
libs/
└── SFML/
    ├── include/
    └── lib/
```

Move the extracted `include` and `lib` folders into `libs/SFML/`.

## 4. Important

The project is already configured to use this exact folder structure.

If you follow the steps correctly:
- No additional setup is required
- You do NOT need to modify Visual Studio settings


## Notes

- This project was developed and tested with **SFML 3.1.0 (64-bit)**
- Using a different version may cause build or runtime issues

