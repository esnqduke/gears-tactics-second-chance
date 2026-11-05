# Antivirus evidence - v0.2.2

## Exact payloads

| File | SHA-256 |
| --- | --- |
| InsaneDBNO.dll | aaff926e1315822157d308d05e4df8aa225f12faecc286fe5057b8849d7fc122 |
| InsaneDBNO-Launch.exe | 6c6dd34828ffdc5194597fdb39744618bba1bb33fab96a893100524bfc6c3c3b |

These are the gameplay-tested binaries recorded in manifest.json.

## Local Kaspersky scan

Installed Kaspersky 21.26 scanned both payloads on 2026-11-05:
- Processed: 2; OK: 2.
- Detected: 0; suspicious: 0.
- Skipped: 0; errors: 0.
- Antivirus database timestamp: 2026-11-04 20:51.

[Unmodified local scan report](Kaspersky-scan-0.2.2-final.txt).

This is a local Kaspersky result, not an online multi-engine scan. No antivirus exclusions or protection settings were changed.

## Online VirusTotal report links

The maintainer supplied these VirusTotal file-report links:

| Payload | Report |
| --- | --- |
| InsaneDBNO.dll | [View DLL report](https://www.virustotal.com/gui/file/aaff926e1315822157d308d05e4df8aa225f12faecc286fe5057b8849d7fc122) |
| InsaneDBNO-Launch.exe | [View launcher report](https://www.virustotal.com/gui/file/6c6dd34828ffdc5194597fdb39744618bba1bb33fab96a893100524bfc6c3c3b) |

The SHA-256 values in both URLs were checked against the exact release binaries and manifest.json.

**Verification limit:** the release assistant could not retrieve VirusTotal's dynamic report contents. Engine counts, detection names, analysis dates and completion status were not independently verified and are not asserted here. Open the reports to inspect the current results; a file-report URL alone does not prove that an analysis has completed or that a file is clean.

The local Kaspersky results above were verified separately. No online zero-detection or false-positive claim is made.

## What the executable does

The launcher deliberately loads the data DLL into GearsTactics.exe using standard Windows process/DLL-loading APIs. The DLL changes the documented fatigue fields. Source and tests are published for inspection.

A scan is evidence about exact files at a particular time, not a guarantee. A positive detection will not automatically be described as a false positive.
