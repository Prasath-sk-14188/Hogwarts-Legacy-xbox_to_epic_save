# Hogwarts Legacy – Game Pass to Epic Games Save Migration Tool

A safe, automated Windows tool to migrate **Hogwarts Legacy** save files from the **Xbox Game Pass (PC)** version to the **Epic Games** version.

This tool is based on a **community-verified manual migration method** and automates it with strict validation and safety checks to reduce human error.

---
## ⚠️ Disclaimer
- This tool only manipulates **save files**, not game binaries.
- Always **back up your saves** before running.
- The tool will **exit immediately** if anything looks unsafe or ambiguous.

---
## 📚 Source of the Migration Method

The migration process implemented by this tool is based on a community guide originally shared on Reddit:

> **“Xbox save to Epic Games (both on PC)”**  
> r/HogwartsLegacyGaming  
> https://www.reddit.com/r/HogwartsLegacyGaming/comments/1pknfra/xbox_save_to_epic_games_both_on_pc/

That guide describes how to manually extract save files from the Xbox Game Pass **WGS** format, rename them using the internal `HL-XX-XX` identifier, and place them into the Epic Games save directory.

This project **automates and hardens** those exact steps.

## 🛠 Requirements

- Hogwarts Legacy installed via:
  - Xbox App (must have played before)
  - Epic Games (launch game atleast once)
  
---
#### 1️⃣ Prepare Epic Games Save Folder
1. Launch Hogwarts Legacy from **Epic Games**
2. Create a **new character**
3. Play until the game **autosaves**
4. Exit the game
This ensures Epic creates the required save structure.
---
#### 2️⃣ Backup Saves (Strongly Recommended)
Back up these folders:
- Xbox save location - C:\Users\<username>\AppData\Local\Packages\WarnerBros.Interactive.PHX_ktmk1xygcecda
- Epic Games save location - C:\Users\<username>\AppData\Local\HogwartsLegacy\Saved\SaveGames\<user-id>
---
#### 3️⃣ Run the Tool
Using the EXE
1. Double-click the executable
2. When prompted, enter: C:\Users<your-username>.
Converted save files will be stored in `./ConvertedSaves/` folder
3. (Optional) Confirm when prompted for migration.
This will delete all previous hogwarts legacy epic games saves and copies converted xbox saves to the save location.
---
#### 4️⃣ Building from Source
use cmake build


### ✅ What This Tool Does

✔ Detects the **xbox Game Pass userId folder** using `container.index`
✔ Extracts the internal `HL-XX-XX` save identifier 
✔ Renames saves to `HL-XX-XX.sav`
(optional)
✔ Copies migrated saves into Epic Games Save location

---
### ❌ What This Tool Does NOT Do

✖ Does not modify registry
✖ Does not touch game installation files
✖ Does not bypass DRM
✖ Does not guess or brute-force paths
✖ Does not silently overwrite saves

---
### ⚠️ Precautions & Safety Notes
✖Do not run while the game is open
✖Do not interrupt the program
