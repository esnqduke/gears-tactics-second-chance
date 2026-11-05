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

## VirusTotal results verified through API

Retrieved on 2026-11-05 from the official VirusTotal v3 file API using a maintainer-provided API key. Returned SHA-256 values and sizes match the exact release payloads and manifest.json. The API key is not included in the repository or release. These are existing analysis results; this check did not request a new analysis.

| Payload | Malicious | Undetected | Unsupported | Analysis time (UTC) | Report |
| --- | ---: | ---: | ---: | --- | --- |
| InsaneDBNO.dll | 5 | 66 | 4 | 2026-11-05T04:09:42.000Z | [VirusTotal](https://www.virustotal.com/gui/file/aaff926e1315822157d308d05e4df8aa225f12faecc286fe5057b8849d7fc122) |
| InsaneDBNO-Launch.exe | 6 | 65 | 4 | 2026-11-05T04:10:34.000Z | [VirusTotal](https://www.virustotal.com/gui/file/6c6dd34828ffdc5194597fdb39744618bba1bb33fab96a893100524bfc6c3c3b) |

Each report contains 75 engine entries: 71 supported verdicts and 4 type-unsupported results. Suspicious, harmless, timeout, confirmed-timeout and failure counts are zero for both files. Undetected means the engine did not detect a threat; it is not a safety certification.

**These are positive antivirus detections, not a clean multi-engine scan. They have not been confirmed as false positives.** In particular, Microsoft reports Trojan:Win32/Wacatac.B!ml for the launcher. The clean local Kaspersky result above does not override these other engines.

### InsaneDBNO.dll

| Engine | Returned detection |
| --- | --- |
| Bkav | W32.Malware.7522396D |
| CrowdStrike | win/malicious_confidence_60% (D) |
| APEX | Malicious |
| Cynet | Malicious (score: 100) |
| McAfeeD | ti!AAFF926E1315 |

### InsaneDBNO-Launch.exe

| Engine | Returned detection |
| --- | --- |
| Bkav | W32.Malware.217D8F74 |
| CrowdStrike | win/malicious_confidence_90% (D) |
| APEX | Malicious |
| McAfeeD | ti!6C6DD34828FF |
| Microsoft | Trojan:Win32/Wacatac.B!ml |
| Malwarebytes | Malware.Heuristic.2518 |

[Sanitized machine-readable API results](VirusTotal-results-0.2.2.json). Report results can change over time; the table records the analysis timestamps returned by the API.

## What the executable does

The launcher deliberately loads the data DLL into GearsTactics.exe using standard Windows process/DLL-loading APIs. The DLL changes the documented fatigue fields. Source and tests are published for inspection.

A scan is evidence about exact files at a particular time, not a guarantee. A positive detection will not automatically be described as a false positive.
