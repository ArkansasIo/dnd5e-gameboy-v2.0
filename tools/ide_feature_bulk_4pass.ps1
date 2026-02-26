param(
    [string]$RepoRoot = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path,
    [string]$SourceFile = "ide_editor/ide_editor.c",
    [string]$OutDir = "tools/out/ide_feature_passes",
    [switch]$RunBuild
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

function Write-Step([string]$text) {
    Write-Host "[4PASS] $text"
}

function Parse-QuotedPairs {
    param([string[]]$Lines, [string]$Pattern)
    $items = New-Object System.Collections.Generic.List[object]
    foreach ($line in $Lines) {
        $m = [regex]::Match($line, $Pattern)
        if ($m.Success) {
            if ($m.Groups.Count -ge 3) {
                $items.Add([pscustomobject]@{
                    A = $m.Groups[1].Value
                    B = $m.Groups[2].Value
                    Raw = $line.Trim()
                })
            } elseif ($m.Groups.Count -ge 2) {
                $items.Add([pscustomobject]@{
                    A = $m.Groups[1].Value
                    B = ""
                    Raw = $line.Trim()
                })
            }
        }
    }
    return $items
}

$sourcePath = Join-Path $RepoRoot $SourceFile
if (!(Test-Path $sourcePath)) {
    throw "Source file not found: $sourcePath"
}

$outPath = Join-Path $RepoRoot $OutDir
New-Item -ItemType Directory -Path $outPath -Force | Out-Null

$lines = Get-Content -Path $sourcePath

Write-Step "Pass 1: Audit command coverage and registry/menu mismatches"
$registered = Parse-QuotedPairs -Lines $lines -Pattern 'command_registry_add\([^"]*"([^"]+)",\s*"([^"]+)"\)'
$handled = Parse-QuotedPairs -Lines $lines -Pattern 'strcmp\(command_id,\s*"([^"]+)"\)\s*==\s*0'
$menus = Parse-QuotedPairs -Lines $lines -Pattern 'add_menu\([^"]*"([^"]+)",\s*"([^"]+)"\)'

$registeredIds = @($registered | ForEach-Object { $_.A } | Sort-Object -Unique)
$handledIds = @($handled | ForEach-Object { $_.A } | Sort-Object -Unique)
$menuCmdIds = @($menus | ForEach-Object { $_.B } | Sort-Object -Unique)

$missingHandlers = @($registeredIds | Where-Object { $_ -notin $handledIds })
$unregisteredMenuCommands = @($menuCmdIds | Where-Object { $_ -notin $registeredIds -and $_ -notlike "ui.menu.top.*" })

$pass1 = [pscustomobject]@{
    generated_at = (Get-Date).ToString("s")
    source_file = $SourceFile
    totals = [pscustomobject]@{
        registered = $registeredIds.Count
        handled = $handledIds.Count
        menu_commands = $menuCmdIds.Count
        missing_handlers = $missingHandlers.Count
        unregistered_menu_commands = $unregisteredMenuCommands.Count
    }
    missing_handlers = $missingHandlers
    unregistered_menu_commands = $unregisteredMenuCommands
}
$pass1 | ConvertTo-Json -Depth 6 | Set-Content -Path (Join-Path $outPath "pass1_audit.json")

Write-Step "Pass 2: Generate command-handler scaffolds for missing features"
$stubPath = Join-Path $outPath "pass2_missing_command_stubs.inc"
$stubLines = New-Object System.Collections.Generic.List[string]
$stubLines.Add("/* Auto-generated handler stubs for missing commands */")
$stubLines.Add("/* Copy selected blocks into execute_command(...) chain in ide_editor.c */")
$stubLines.Add("")
if ($missingHandlers.Count -eq 0) {
    $stubLines.Add("/* No missing handlers detected. */")
} else {
    foreach ($cmd in $missingHandlers) {
        $stubLines.Add("} else if (strcmp(command_id, `"$cmd`") == 0) {")
        $stubLines.Add("    ui_api_push_notification(&app->ui, UI_NOTIFY_WARN, `"TODO: implement $cmd`", 220);")
        $stubLines.Add("    printf(`[IDE] TODO handler: %s\n`, command_id);")
    }
}
$stubLines.Add("")
$stubLines | Set-Content -Path $stubPath

Write-Step "Pass 3: Generate gap report for menu intents and likely placeholder logic"
$placeholderPattern = 'add_menu\([^"]*"([^"]+)",\s*"(file\.open_project|file\.save_project|edit\.undo|edit\.redo)"\)'
$placeholderRows = Parse-QuotedPairs -Lines $lines -Pattern $placeholderPattern
$placeholderRows = @(
    $placeholderRows | Where-Object {
        $menuPath = $_.A
        $cmd = $_.B
        if ($cmd -eq "file.open_project" -and $menuPath -match "(^|/)Open(\s|$|/)") { return $false }
        if ($cmd -eq "file.save_project" -and $menuPath -match "(^|/)Save(\s|$|/)") { return $false }
        if ($cmd -eq "edit.undo" -and $menuPath -match "(^|/)Undo(\s|$|/)") { return $false }
        if ($cmd -eq "edit.redo" -and $menuPath -match "(^|/)Redo(\s|$|/)") { return $false }
        return $true
    }
)
$pass3Md = Join-Path $outPath "pass3_menu_logic_gap_report.md"
$md = New-Object System.Collections.Generic.List[string]
$md.Add("# Pass 3 - Menu/Logic Gap Report")
$md.Add("")
$md.Add("- Source: $SourceFile")
$md.Add("- Generated: $(Get-Date -Format s)")
$md.Add("")
$md.Add("## Likely Placeholder Menu Bindings")
if ($placeholderRows.Count -eq 0) {
    $md.Add("No obvious placeholder menu mappings detected.")
} else {
    $md.Add("| Menu Path | Current Command | Suggested Action |")
    $md.Add("|---|---|---|")
    foreach ($r in $placeholderRows) {
        $suggest = "Create dedicated command id + execute_command branch"
        $md.Add("| $($r.A) | $($r.B) | $suggest |")
    }
}
$md.Add("")
$md.Add("## Unregistered Menu Commands")
if ($unregisteredMenuCommands.Count -eq 0) {
    $md.Add("None")
} else {
    foreach ($c in $unregisteredMenuCommands) {
        $md.Add("- $c")
    }
}
$md | Set-Content -Path $pass3Md

Write-Step "Pass 4: Verification summary and optional build"
$buildStatus = "skipped"
$buildLog = Join-Path $outPath "pass4_build.log"
if ($RunBuild) {
    $ideDir = Join-Path $RepoRoot "ide_editor"
    if (Test-Path (Join-Path $ideDir "build.bat")) {
        Push-Location $ideDir
        try {
            cmd /c build.bat *>&1 | Tee-Object -FilePath $buildLog
            if ($LASTEXITCODE -eq 0) { $buildStatus = "success" } else { $buildStatus = "failed($LASTEXITCODE)" }
        } finally {
            Pop-Location
        }
    } else {
        $buildStatus = "build.bat not found"
    }
}

$summaryPath = Join-Path $outPath "pass4_summary.md"
$summary = @(
    "# IDE Feature 4-Pass Summary",
    "",
    "- Generated: $(Get-Date -Format s)",
    "- Source: $SourceFile",
    "- Registered Commands: $($registeredIds.Count)",
    "- Implemented Handlers: $($handledIds.Count)",
    "- Missing Handlers: $($missingHandlers.Count)",
    "- Unregistered Menu Commands: $($unregisteredMenuCommands.Count)",
    "- Build: $buildStatus",
    "",
    "## Outputs",
    "- pass1_audit.json",
    "- pass2_missing_command_stubs.inc",
    "- pass3_menu_logic_gap_report.md",
    "- pass4_summary.md",
    "",
    "Run with build verification:",
    "powershell -ExecutionPolicy Bypass -File tools/ide_feature_bulk_4pass.ps1 -RunBuild"
)
$summary | Set-Content -Path $summaryPath

Write-Step "Completed all 4 passes."
Write-Host "Output directory: $outPath"
