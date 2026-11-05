# Gears Tactics: Second Chance

![Second Chance - AI-generated promotional artwork](https://github.com/esnqduke/gears-tactics-second-chance/releases/download/v0.2.2/Second-Chance-cover.png)

An open-source mod that gives **fresh player-controlled units one Down But Not Out (DBNO) opportunity on Insane difficulty**.

The first lethal hit downs a fresh unit; after revival, the next lethal hit kills that unit. This behavior was tested on ordinary soldiers and Gabe. Gabe's actual death still causes the expected mission failure.

## Why use it?

Keep Insane's combat pressure while retaining a chance to rescue a teammate. The mod changes the existing fatigue budget to **FatigueSteps = 2**. It does not add a separate revival counter.

- Ordinary soldiers and active heroes are handled automatically.
- No per-mission script or manual memory edit is needed.
- Damage, accuracy, AI, rewards and other combat settings are not patched.
- **Fatigue-related maximum-health penalties remain part of this change.** An already fatigued unit may have less DBNO capacity.
- Executions can still kill a downed unit; normal mission-failure rules remain active.

## Supported version

Windows x64, Steam edition of Gears Tactics. Tested executable PE timestamp: **6036BD2C**; image size: **0B57AA00**.

The launcher checks the executable build and stops if unsupported. Game Pass/Microsoft Store, other executable builds and compatibility with other mods have not been verified.

## Install and launch through Steam

1. Close Gears Tactics.
2. Download the ZIP from [Releases](https://github.com/esnqduke/gears-tactics-second-chance/releases) and extract the **entire** folder.
3. Run **Install.cmd**. It verifies the payload hashes and installs the two files into:

       %LOCALAPPDATA%\GearsTacticsInsaneDBNO\Mod

4. In Steam, open **Gears Tactics > Properties > General > Launch Options**.
5. Paste the **exact command printed by the installer**. Its format is:

       "FULL_PATH_TO\InsaneDBNO-Launch.exe" %command%

   Your exact command is also saved in **Steam-Launch-Options.txt** in the installation folder. Keep any pre-existing game arguments after **%command%**.

6. Start the game with Steam's normal **Play** button.

The launcher starts the original game, waits **30 seconds**, loads the data DLL and exits. Allow at least 35 seconds from game startup before an initial combat test. Nothing needs to be run again when loading another mission.

**Do not copy the mod, old XInput proxies or backup DLLs into the game's folder.** This release installs externally and leaves the original game XInput DLL, EXE, archives and JSON cache untouched.

## How it works

Steam runs a small native launcher before the original game command. Original arguments, environment and working directory are retained. The launcher uses Windows **CreateRemoteThread / LoadLibraryW** to load **InsaneDBNO.dll** after startup, verifies its presence and exits.

The DLL remains inside that game process until it closes. Once per second it checks the supported build, current mission, Insane difficulty and object identities. It changes only **Default_FatigueSteps.Insane** and positive **CharacterData.FatigueSteps** values in player roster/hero lists. It installs no game-function detours.

## Logs and verification

    %LOCALAPPDATA%\GearsTacticsInsaneDBNO\Launcher.log
    %LOCALAPPDATA%\GearsTacticsInsaneDBNO\InsaneDBNO.log

**READY** means the DLL loaded and is waiting for a suitable mission. **APPLIED** means it updated fatigue fields in an active Insane mission.

Testing included native automated tests, an independent read of all four active player pawns showing an effective budget of 2, repeated mission loading, and human gameplay tests:

| Case | Observed result |
| --- | --- |
| Ordinary soldier, first lethal hit | DBNO; mission continued |
| Revived soldier, second lethal hit | Death |
| Gabe, first lethal hit | DBNO; mission continued |
| Gabe, second lethal/DBNO attempt | Death and expected mission failure |

See [VALIDATION.txt](VALIDATION.txt) for the evidence. These observations apply to the supported build and exact release binaries.

## Antivirus results

**VirusTotal reports positive detections: 5 for the DLL and 6 for the launcher. These have not been confirmed as false positives.** Microsoft flags the launcher as Trojan:Win32/Wacatac.B!ml.

Verified through the VirusTotal API on 2026-11-05: [DLL report](https://www.virustotal.com/gui/file/aaff926e1315822157d308d05e4df8aa225f12faecc286fe5057b8849d7fc122) (5 malicious, 66 undetected, 4 unsupported), [launcher report](https://www.virustotal.com/gui/file/6c6dd34828ffdc5194597fdb39744618bba1bb33fab96a893100524bfc6c3c3b) (6 malicious, 65 undetected, 4 unsupported). The returned hashes and sizes match the exact release files.

Local Kaspersky separately reported both files OK. See [ANTIVIRUS.md](ANTIVIRUS.md) for all returned detection names, analysis timestamps, the unmodified local report and [sanitized API results](VirusTotal-results-0.2.2.json). No API key is published.

The manifest and published checksums identify the actual tested DLL and launcher. No Microsoft or game DLL is included.

## Uninstall and saves

1. Close the game.
2. Remove the added launcher command from Steam Launch Options, preserving your previous game arguments.
3. Run **Uninstall.cmd**.

Logs and recovery records are retained. Although the mod does not edit save files directly, changed fatigue data **can be serialized by the game into a save**. Uninstalling or changing difficulty does not rewrite already-modified saves. Keep a pre-mod save if you need a clean comparison or complete rollback.

## Build from source

Install MSVC x64 **14.44** and the Windows SDK, then run:

    source\build.cmd

This builds the DLL and launcher and runs core-behavior and isolated late-loading tests. Outputs are placed in **source\build**. No third-party hooking library is required.

## AI-assisted development and measured usage

Created by **esnqduke with OpenAI Codex assistance**. Codex assisted with game-data/code investigation, implementation, debugging, tests and packaging. esnqduke performed the actual in-game combat tests and confirmed the final soldier/hero behavior.

The local Codex logs record model identifier **gpt-6.1-sol**, reasoning effort **medium**, for the main coding agent and its code-review agent.

| Recorded agent | Input (cached included) | Cached input subset | Output (reasoning included) | Reasoning subset | Total |
| --- | ---: | ---: | ---: | ---: | ---: |
| main | 94,735,887 | 93,723,904 | 341,130 | 160,309 | 95,077,017 |
| code_review | 1,189,805 | 1,092,608 | 7,124 | 3,823 | 1,196,929 |
| **Combined** | **95,925,692** | **94,816,512** | **348,254** | **164,132** | **96,273,946** |

These are **processed token counts**, not 96 million tokens of unique written code: repeated cached prompts are included in input; reasoning is included in output. Do not add the subset columns again.

The frozen measurement ends at final gameplay acceptance, before later Git/GitHub release work. It includes failed experiments, debugging, testing and waits in the main conversation and one explicit code-review agent. Infrastructure and automatic approval-review services are outside this measurement. Unique response-usage records were summed and checked against their recorded thread counters.

Wall-clock time from session start to final gameplay confirmation: **5 hours, 8 minutes, 33 seconds**, including human playtests and waiting. Monetary cost was not measured; telemetry is not an invoice.

See [development-usage.json](development-usage.json) for exact counts, cutoff and scope. No raw conversations, reasoning text, session identifiers or credentials are published. [OpenAI's usage documentation](https://developers.openai.com/api/docs/guides/agents-api/observability) explains the cached-input and reasoning-output subsets.

[Russian instructions](README-RU.txt).

## License

[MIT](LICENSE.txt). Gears Tactics and related trademarks belong to their respective owners.
