# Feature Documentation

This document explains the features available in this mod menu, covering both standard features and advanced ones like Web Server and Room Info.

## 1. Floating Mod Menu (ImGui)
The main feature is a floating menu drawn over the game using the **ImGui** library.
- **Access:** The menu can be toggled (minimized/maximized), typically by touching an icon or a specific button (depending on input config).
- **Function:** Contains checkboxes and buttons to enable cheats/features.
- **Interface:**
  - **"Zygisk MLBS (White)" Menu:** Main window.
  - **Features:** Section to enable Web Server, Room Info, and Save Config button.
  - **Player Info:** Collapsible section displaying the list of players in the current room.

## 2. Web Server (HTTP API)
This unique feature runs a local HTTP server on the Android device while the game is running. It allows other devices (or a browser on the same phone) to view game data in real-time.

- **How to Enable:** Check "Web Server" in the menu.
- **Status:** Shows "Running" (Green) or "Stopped" (Red) in the menu.
- **Port:** Server runs on port `2626`.
- **Access:** Open a browser and go to `http://<YOUR_PHONE_IP>:2626/<endpoint>`.

### API Endpoints
Available endpoints to retrieve data (JSON format):

| Endpoint | Description | Returned Data |
| :--- | :--- | :--- |
| `/state` | Brief game status. | Battle state, feature status, basic player list. |
| `/inforoom` | Detailed Room/Lobby info. | Complete data for every player (ID, Rank, Hero, Skin, etc.). |
| `/infobattle` | Real-time battle stats. | Kills, Gold, EXP, Tower Kills, Lord/Turtle Kills for both teams. |
| `/timebattle` | Match duration. | Elapsed time in seconds. |
| `/config` | Configuration (POST). | Change menu settings via HTTP request. |

## 3. Room Info & Player Info
This feature reads game memory to obtain detailed information about other players in the match or lobby.

- **Menu Display:** Table containing Camp (Team), Name, and Rank.
- **Detailed Data (via Web Server):**
  - **Basic:** Name, UID, Rank, Hero, Spell.
  - **Advanced:** Win rate (implied), Hero Power, MMR, Equipped Skin, Country, etc.
  - **Benefit:** Know enemy strength before the match starts (if data is available during the draft pick phase).

## 4. Battle Stats
Monitors game statistics in real-time which might not always be visible in the standard game UI.
- **Team A vs Team B:**
  - Total Kills.
  - Total Gold & EXP.
  - Number of Towers destroyed.
  - Objectives (Lord/Turtle/LingZhu/ShenGui) killed.
- **Usage:** In-depth team performance analysis or for casting/streaming overlays via Web Server.

## 5. Save Config
- **Function:** Saves menu settings (window position, active cheats) to internal storage.
- **File Location:** `/data/data/<game_package>/files/imgui.ini` and internal JSON config.
- **Auto-Load:** Settings are automatically loaded when the game restarts.
