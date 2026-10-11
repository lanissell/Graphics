<#
.SYNOPSIS
Builds a submission archive that contains only the files needed to build and run the solution.

.DESCRIPTION
Takes every file that git tracks or would track, so everything matched by .gitignore is skipped,
drops the paths marked with the export-ignore attribute in .gitattributes
and writes the rest to dist\<Name>.zip inside a single top-level folder called <Name>.

.PARAMETER Name
Name of the archive and of its top-level folder.

.EXAMPLE
powershell -ExecutionPolicy Bypass -File tools\pack.ps1 -Name Lab1
#>
[CmdletBinding()]
param(
    [ValidateNotNullOrEmpty()]
    [string] $Name = 'Graphics'
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

Add-Type -AssemblyName System.IO.Compression
Add-Type -AssemblyName System.IO.Compression.FileSystem

$repositoryRoot = Split-Path -Parent $PSScriptRoot
$outputDirectory = Join-Path $repositoryRoot 'dist'
$archivePath = Join-Path $outputDirectory "$Name.zip"

# Git prints paths as UTF-8, the console default would corrupt non-ASCII file names.
[Console]::OutputEncoding = [System.Text.Encoding]::UTF8

$listing = & git -C $repositoryRoot ls-files --cached --others --exclude-standard -z -- ':(attr:!export-ignore)'
if ($LASTEXITCODE -ne 0)
{
    throw 'git ls-files failed. Run the script inside the git repository.'
}

$files = @(($listing -join '') -split "`0" |
    Where-Object { $_ -and (Test-Path -LiteralPath (Join-Path $repositoryRoot $_) -PathType Leaf) } |
    Sort-Object)
if ($files.Count -eq 0)
{
    throw 'There is nothing to pack.'
}

New-Item -ItemType Directory -Force -Path $outputDirectory | Out-Null
if (Test-Path -LiteralPath $archivePath)
{
    Remove-Item -LiteralPath $archivePath
}

$archive = [System.IO.Compression.ZipFile]::Open($archivePath, [System.IO.Compression.ZipArchiveMode]::Create)
try
{
    foreach ($file in $files)
    {
        [System.IO.Compression.ZipFileExtensions]::CreateEntryFromFile(
            $archive,
            (Join-Path $repositoryRoot $file),
            "$Name/$file",
            [System.IO.Compression.CompressionLevel]::Optimal) | Out-Null
        Write-Host "  $file"
    }
}
finally
{
    $archive.Dispose()
}

$sizeInKilobytes = [math]::Round((Get-Item -LiteralPath $archivePath).Length / 1KB)
Write-Host "$($files.Count) files, $sizeInKilobytes KB -> $archivePath"
