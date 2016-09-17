Param
(
    [Switch] $Summary
)

$po = @()
foreach ($f in (Get-Item lcd_*.cpp))
{
    $pc = @()
    $pc += Get-Content $f `
        | findstr "struct p*" `
        | Where-Object { $_ -match "struct p(\d+)" } `
        | ForEach-Object { [int]$Matches[1] }
    
    
    $po += New-Object PSObject -Property @{
        Name = $f.Name
        Count = $pc.Count
        LoadFactor = ($pc.Count / 50) * 100
        Items = $pc
    }
}

if ($Summary)
{
    $p = @()
    $po | ForEach-Object { $p += $po.Items }
    $p += [Int]::MaxValue

    $rs = $p[0]
    foreach ($i in 1..($p.Count - 1))
    {
        if (($p[$i] - $p[$i - 1]) -gt 1)
        {
            $re = $p[$i - 1]
            if (($re - $rs) -gt 1) {
                "${rs}-${re}"
            } else {
                "$rs"
            }
            
            $rs = $p[$i]
        }
    }
}
else
{
    $po | Select-Object Name,Count,LoadFactor | Format-Table
    ($po | Select-Object -ExpandProperty Items | Measure-Object).Count
}