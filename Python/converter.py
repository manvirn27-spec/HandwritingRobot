import pymupdf
from svgpathtools import Path, svg2paths
import xml.etree.ElementTree as ET

# Map characters to their respective handwriting SVG files
SVGfiles = {
    "a": "Python/SVGs/a.svg",
    "b": "Python/SVGs/b.svg",
    "c": "Python/SVGs/c.svg",
    "d": "Python/SVGs/d.svg",
    "e": "Python/SVGs/e.svg",
    "f": "Python/SVGs/f.svg",
    "g": "Python/SVGs/g.svg",
    "h": "Python/SVGs/h.svg",
    "i": "Python/SVGs/i.svg",
    "j": "Python/SVGs/j.svg",
    "k": "Python/SVGs/k.svg",
    "l": "Python/SVGs/l.svg",
    "m": "Python/SVGs/m.svg",
    "n": "Python/SVGs/n.svg",
    "o": "Python/SVGs/o.svg",
    "p": "Python/SVGs/p.svg",
    "q": "Python/SVGs/q.svg",
    "r": "Python/SVGs/r.svg",
    "s": "Python/SVGs/s.svg",
    "t": "Python/SVGs/t.svg",
    "u": "Python/SVGs/u.svg",
    "v": "Python/SVGs/v.svg",
    "w": "Python/SVGs/w.svg",
    "x": "Python/SVGs/x.svg",
    "y": "Python/SVGs/y.svg",
    "z": "Python/SVGs/z.svg",
}

def extract_characters_with_positions(pdf_path):
    allCharsAndPos = []
    doc = pymupdf.open(pdf_path)
    
    for page_num, page in enumerate(doc):        
        page_dict = page.get_text("rawdict")
        
        for block in page_dict["blocks"]:
            if block.get("type") == 0:
                for line in block["lines"]:
                    for span in line["spans"]:
                        for char in span["chars"]:
                            char_text = char["c"]
                            bbox = char["bbox"]  # (x0, y0, x1, y1)
                            allCharsAndPos.append([page_num + 1, char_text, bbox])
    return allCharsAndPos

def charactersToSVG(allCharsAndPos):
    allSVGsAndPos = []
    for i in allCharsAndPos:
        pageNum, char, (x0, y0, x1, y1) = i
        if char in SVGfiles and SVGfiles[char]:
            # Keep pageNum so we know when to trigger page end commands
            allSVGsAndPos.append([pageNum, SVGfiles[char], x0, y0, x1, y1])
    return allSVGsAndPos

def round_and_compress_points(path_points):
    if not path_points:
        return []
    
    compressed = []
    for x, y in path_points:
        # Round to 1 decimal place
        rounded_point = (round(x, 1), round(y, 1))
        
        # Only add the point if it's different from the last added point
        if not compressed or rounded_point != compressed[-1]:
            compressed.append(rounded_point)
            
    return compressed

def svgToScaledPoints(allSVGsAndPos, subdivisions=10):
    all_transformed_paths = []
    PT_TO_MM = 25.4 / 72.0  # Conversion from PDF points to millimeters

    for svgs in allSVGsAndPos:
        page_num, svg_file, x0_pt, y0_pt, x1_pt, y1_pt = svgs
        paths, attributes = svg2paths(svg_file)
        if not paths:
            continue

        svgX0, svgX1, svgY0, svgY1 = Path(*paths).bbox()
        desiredWidth = x1_pt - x0_pt
        desiredHeight = y1_pt - y0_pt
        actualWidth = svgX1 - svgX0
        actualHeight = svgY1 - svgY0
        
        if actualWidth == 0 or actualHeight == 0:
            continue

        scaleFactor = min(desiredHeight / actualHeight, desiredWidth / actualWidth)
        
        translationX = x0_pt * PT_TO_MM - svgX0 * scaleFactor * PT_TO_MM
        translationY = y0_pt * PT_TO_MM - svgY0 * scaleFactor * PT_TO_MM

        for path in paths:
            path_points = []
            for segment in path:
                for i in range(subdivisions + 1):
                    t = i / subdivisions
                    point = segment.point(t)
                    
                    x_mm = (point.real * scaleFactor * PT_TO_MM) + translationX
                    y_mm = (point.imag * scaleFactor * PT_TO_MM) + translationY
                    
                    path_points.append((x_mm, y_mm)) # Keep full precision temporarily for accurate rounding
            
            # Compress and round to 1 decimal place here!
            compressed_points = round_and_compress_points(path_points)
                    
            if compressed_points:
                all_transformed_paths.append((page_num, compressed_points))

    return all_transformed_paths

def svgToCommands(all_transformed_paths):
    commands = []
    commands.append("; --- G-code with Custom Plotter Commands ---\n")
    commands.append("G2 ; Raise pen initially\n\n")
    
    current_page = None
    
    for page_num, path_points in all_transformed_paths:
        # If we transition to a new PDF page, trigger M1 (end page)
        if current_page is not None and page_num != current_page:
            commands.append("G2 ; Raise pen\n")
            commands.append(f"M1 ; End page {current_page}\n\n")
        current_page = page_num
        
        if not path_points:
            continue
        
        # 1. Raise pen and move to the start of the letter stroke using G1
        start_x, start_y = path_points[0]
        commands.append("G2 ; Raise pen\n")
        commands.append(f"G1 X{start_x} Y{start_y} ; Move to start\n")
        
        # 2. Lower pen to draw
        commands.append("G3 ; Lower pen\n")
        
        # 3. Trace the path using G1 moves
        for x, y in path_points[1:]:
            commands.append(f"G1 X{x} Y{y}\n")
            
        # 4. Raise pen after stroke completion
        commands.append("G2 ; Raise pen\n\n")
        
    # End the final page
    if current_page is not None:
        commands.append(f"M1 ; End page {current_page}\n")
        
    return commands

def createGcodeFile(name, commands):
    with open(name, "w") as file:
        file.writelines(commands)
    print(f"Successfully generated G-code file: {name}")




if __name__ == "__main__":
    pdf_path = "Python/HandWriting Robot test.pdf"
    
    print("1. Extracting characters from PDF...")
    chars_and_pos = extract_characters_with_positions(pdf_path)
    
    print("2. Matching characters with SVGs and tracking pages...")
    svgs_and_pos = charactersToSVG(chars_and_pos)
    
    print("3. Scaling and transforming paths to millimeters...")
    transformed_paths = svgToScaledPoints(svgs_and_pos, subdivisions=10)
    
    print("4. Generating custom G-code commands...")
    gcode_commands = svgToCommands(transformed_paths)
    
    createGcodeFile("output.gcode", gcode_commands)