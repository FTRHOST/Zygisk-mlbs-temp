#pragma once

const char* panel_html_content = R"raw(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>MLBS Mod Panel</title>
    <style>
        body {
            font-family: sans-serif;
            margin: 20px;
            background-color: #2e2e2e;
            color: #eee;
        }
        .container {
            max-width: 800px;
            margin: 0 auto;
            background-color: #3e3e3e;
            padding: 20px;
            border-radius: 8px;
            box-shadow: 0 0 10px rgba(0, 0, 0, 0.5);
        }
        h1, h2 {
            color: #4CAF50;
        }
        .status, .config {
            margin-bottom: 20px;
            padding: 15px;
            border: 1px solid #555;
            border-radius: 5px;
            background-color: #4a4a4a;
        }
        button {
            background-color: #4CAF50;
            color: white;
            padding: 10px 15px;
            border: none;
            border-radius: 5px;
            cursor: pointer;
            font-size: 16px;
            margin-right: 10px;
        }
        button:hover {
            background-color: #45a049;
        }
        .players-list, .draft-list {
            list-style: none;
            padding: 0;
        }
        .players-list li, .draft-list li {
            background-color: #5a5a5a;
            margin-bottom: 5px;
            padding: 8px;
            border-radius: 3px;
        }
        pre {
            background-color: #1e1e1e;
            padding: 10px;
            border-radius: 5px;
            overflow-x: auto;
        }
        .label {
            font-weight: bold;
            color: #888;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>MLBS Mod Control Panel</h1>

        <div class="status">
            <h2>Current Game State</h2>
            <p><span class="label">Battle State:</span> <span id="battleState">Loading...</span></p>
            <p><span class="label">Bypass Enabled:</span> <span id="bypassEnabled">Loading...</span></p>
            <p><span class="label">Players Count:</span> <span id="playersCount">Loading...</span></p>
            <p><span class="label">Draft Events Count:</span> <span id="draftEventsCount">Loading...</span></p>
        </div>

        <div class="config">
            <h2>Configuration</h2>
            <button id="toggleBypass">Toggle Bypass</button>
            <span id="bypassStatus">Loading...</span>
            <br><br>
            <button id="toggleRoomInfo">Toggle Room Info</button>
            <span id="roomInfoStatus">Loading...</span>
        </div>

        <div class="players">
            <h2>Players Info</h2>
            <ul id="playersList" class="players-list">
                <li>No player data available.</li>
            </ul>
        </div>

        <div class="draft">
            <h2>Draft Events</h2>
            <ul id="draftList" class="draft-list">
                <li>No draft data available.</li>
            </ul>
        </div>

        <script>
            const API_BASE_URL = window.location.protocol + "//" + window.location.hostname + ":8080";

            async function fetchState() {
                try {
                    const response = await fetch(`${API_BASE_URL}/state`);
                    if (!response.ok) {
                        const errorText = await response.text();
                        throw new Error(`HTTP error! Status: ${response.status}, Message: ${errorText}`);
                    }
                    
                    const state = await response.json();
                    
                    document.getElementById('battleState').textContent = state.battleState;
                    document.getElementById('bypassStatus').textContent = state.bypassEnabled ? 'ON' : 'OFF';
                    document.getElementById('roomInfoStatus').textContent = state.roomInfoEnabled ? 'ON' : 'OFF';
                    
                    // Only update player/draft info if the feature is on
                    if (state.roomInfoEnabled) {
                        document.getElementById('playersCount').textContent = state.players.length;
                        document.getElementById('draftEventsCount').textContent = state.draftEvents.length;
                        updatePlayersList(state.players);
                        updateDraftList(state.draftEvents);
                    } else {
                        document.getElementById('playersCount').textContent = 'DISABLED';
                        document.getElementById('draftEventsCount').textContent = 'DISABLED';
                        document.getElementById('playersList').innerHTML = '<li>Room Info is disabled.</li>';
                        document.getElementById('draftList').innerHTML = '<li>Room Info is disabled.</li>';
                    }

                } catch (error) {
                    console.error('Error fetching state:', error);
                    document.getElementById('battleState').textContent = 'Error';
                    document.getElementById('bypassStatus').textContent = 'Error';
                    document.getElementById('roomInfoStatus').textContent = 'Error';
                    document.getElementById('playersCount').textContent = 'Error';
                    document.getElementById('draftEventsCount').textContent = 'Error';
                    document.getElementById('playersList').innerHTML = '<li>Error loading player data.</li>';
                    document.getElementById('draftList').innerHTML = '<li>Error loading draft data.</li>';
                }
            }

            function updatePlayersList(players) {
                const list = document.getElementById('playersList');
                list.innerHTML = '';
                if (players.length === 0) {
                    list.innerHTML = '<li>No player data available.</li>';
                    return;
                }
                players.forEach(p => {
                    const li = document.createElement('li');
                    li.innerHTML = `<span class="label">Name:</span> ${p.name}<br>
                                    <span class="label">UID:</span> ${p.uid}<br>
                                    <span class="label">Rank:</span> ${p.rank} (${p.rankLevel})<br>
                                    <span class="label">Hero:</span> ${p.hero} (${p.heroId})<br>
                                    <span class="label">Spell:</span> ${p.spell} (${p.spellId})<br>
                                    <span class="label">Camp:</span> ${p.camp}`;
                    list.appendChild(li);
                });
            }

            function updateDraftList(draftEvents) {
                const list = document.getElementById('draftList');
                list.innerHTML = '';
                if (draftEvents.length === 0) {
                    list.innerHTML = '<li>No draft data available.</li>';
                    return;
                }
                draftEvents.forEach(e => {
                    const li = document.createElement('li');
                    li.innerHTML = `<span class="label">Player:</span> ${e.player}<br>
                                    <span class="label">Hero:</span> ${e.hero}<br>
                                    <span class="label">Event:</span> ${e.event}`;
                    list.appendChild(li);
                });
            }

            document.getElementById('toggleBypass').addEventListener('click', async () => {
                const currentStatus = document.getElementById('bypassStatus').textContent === 'ON';
                const newStatus = !currentStatus;
                try {
                    const response = await fetch(`${API_BASE_URL}/config`, {
                        method: 'POST',
                        headers: { 'Content-Type': 'application/json' },
                        body: JSON.stringify({ enableBypass: newStatus })
                    });
                    if (response.ok) {
                        fetchState();
                    }
                } catch (error) {
                    console.error('Error toggling bypass:', error);
                }
            });

            document.getElementById('toggleRoomInfo').addEventListener('click', async () => {
                const currentStatus = document.getElementById('roomInfoStatus').textContent === 'ON';
                const newStatus = !currentStatus;
                try {
                    const response = await fetch(`${API_B_URL}/config`, {
                        method: 'POST',
                        headers: { 'Content-Type': 'application/json' },
                        body: JSON.stringify({ enableRoomInfo: newStatus })
                    });
                    if (response.ok) {
                        fetchState();
                    }
                } catch (error) {
                    console.error('Error toggling room info:', error);
                }
            });

            // Fetch state every 1 second
            setInterval(fetchState, 1000);
            fetchState(); // Initial fetch
        </script>
    </div>
</body>
</html>
)raw";
