G90          ; Set to absolute positioning mode
G21          ; Set units to millimeters
M03 S12000   ; Turn spindle clockwise at 12,000 RPM

G00 X0 Y0 Z5 ; Rapid move to X0, Y0, and 5mm safe height above work

G01 Z-1 F500 ; Feed down into material 1mm at 500 mm/min
G01 X50 F1000; Cut right 50mm at 1000 mm/min
G01 Y50      ; Cut up 50mm
G01 X0       ; Cut left 50mm
G01 Y0       ; Cut down 50mm (back to start)

G00 Z5       ; Rapid move back up to safe height
M05          ; Turn off spindle
M30          ; Program end and rewind