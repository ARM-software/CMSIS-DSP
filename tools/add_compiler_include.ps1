# Inserts '#include "arm_compiler_specific.h"' immediately after the top comment block
# for datatype-specific .c files under the Source directory.
# Usage: run from repo root with PowerShell: .\tools\add_compiler_include.ps1

param()

$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Definition
# repoRoot is parent of the tools directory
$repoRoot = Split-Path -Parent $scriptDir
$sourcePath = Join-Path $repoRoot 'Source'

$pattern = '_(f16|f32|f64|q7|q15|q31|u8|u16|u32)\.c$'
$includeLine = '#include "arm_compiler_specific.h"'

$changed = @()

Get-ChildItem -Path $sourcePath -Recurse -Filter *.c | ForEach-Object {
    $file = $_.FullName
    if ($_.Name -match $pattern) {
        try {
            $lines = Get-Content -Encoding UTF8 -Path $file -ErrorAction Stop
        } catch {
            Write-Host "Skipping (read error): $file"
            return
        }

        # Remove any existing occurrences of the include first
        $origText = $lines -join "`n"
        $cleanLines = $lines | Where-Object { -not ($_ -match '^\s*#\s*include\s+"arm_compiler_specific.h"\s*$') }

        # Helper: find end of comment starting at index
        function FindCommentEnd($arr, $start) {
            for ($j = $start; $j -lt $arr.Count; $j++) {
                if ($arr[$j] -match '\*/') { return $j }
            }
            return -1
        }

        # Find first comment block (look in the first 40 lines)
        $firstStart = -1
        for ($i = 0; $i -lt [Math]::Min(40,$cleanLines.Count); $i++) {
            if ($cleanLines[$i].TrimStart().StartsWith('/*')) { $firstStart = $i; break }
        }

        $insertIndex = -1
        if ($firstStart -ne -1) {
            $firstEnd = FindCommentEnd $cleanLines $firstStart
            if ($firstEnd -ne -1) {
                # search for second comment block within next 60 lines
                $secondStart = -1
                for ($k = $firstEnd + 1; $k -lt [Math]::Min($cleanLines.Count, $firstEnd + 60); $k++) {
                    if ($cleanLines[$k].TrimStart().StartsWith('/*')) { $secondStart = $k; break }
                }
                if ($secondStart -ne -1) {
                    $secondEnd = FindCommentEnd $cleanLines $secondStart
                    if ($secondEnd -ne -1) { $insertIndex = $secondEnd + 1 }
                } else {
                    $insertIndex = $firstEnd + 1
                }
            }
        }

        if ($insertIndex -eq -1) {
            # fallback: after leading blank lines
            $insertIndex = 0
            for ($i = 0; $i -lt $cleanLines.Count; $i++) {
                if ($cleanLines[$i].Trim() -ne '') { $insertIndex = $i; break }
            }
        }

        # Build new content with include inserted at insertIndex
        $before = @()
        if ($insertIndex -gt 0) { $before = $cleanLines[0..($insertIndex-1)] }
        $after = @()
        if ($insertIndex -lt $cleanLines.Count) { $after = $cleanLines[$insertIndex..($cleanLines.Count-1)] }

        $newLines = $before + @($includeLine,'') + $after

        $newText = $newLines -join "`r`n"

        if ($newText -ne $origText) {
            try {
                Set-Content -Path $file -Value $newText -Encoding UTF8
                $changed += $file
                Write-Host "Updated: $file"
            } catch {
                Write-Host "Failed to update: $file : $_"
            }
        }

        try {
            Set-Content -Path $file -Value ($newLines -join "`r`n") -Encoding UTF8
            $changed += $file
            Write-Host "Updated: $file"
        } catch {
            Write-Host "Failed to update: $file : $_"
        }
    }
}

Write-Host "\nTotal files updated: $($changed.Count)"
if ($changed.Count -gt 0) { $changed | ForEach-Object { Write-Host $_ } }
else { Write-Host "No files needed updating." }
