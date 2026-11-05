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

## Online multi-engine scan

**No completed online result has been independently verified yet.** A hash-search URL is not being presented as a completed scan.

As of 2026-11-05, no VirusTotal API key or authenticated browser session was available in the release environment. VirusTotal requires an account-issued key for API submissions. A submission to the official public Jotti form was also attempted; the connection was reset during upload and no scan job or result URL was returned. These failures are not scan results.

The requested release remains a draft pending verified online reports for both payloads. No claim of an online clean result is made.

## What the executable does

The launcher deliberately loads the data DLL into GearsTactics.exe using standard Windows process/DLL-loading APIs. The DLL changes the documented fatigue fields. Source and tests are published for inspection.

A scan is evidence about exact files at a particular time, not a guarantee. A positive detection will not automatically be described as a false positive.
