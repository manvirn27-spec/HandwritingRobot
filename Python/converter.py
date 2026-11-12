import pymupdf
from svgpathtools import Path, svg2paths, parse_path
import xml.etree.ElementTree as ET

SVGfiles = {
    "a": "Python/letters-svgrepo-com.svg",
    "b": "",
    "c": "",
    "d": "",
    "e": "",
    "f": "",
    "g": "",
    "h": "",
    "i": "",
    "j": "",
    "k": "",
    "l": "",
    "m": "",
    "n": "",
    "o": "",
    "p": "",
    "q": "",
    "r": "",
    "s": "",
    "t": "",
    "u": "",
    "v": "",
    "w": "",
    "x": "",
    "y": "",
    "z": "",

    "A": "",
    "B": "",
    "C": "",
    "D": "",
    "E": "",
    "F": "",
    "G": "",
    "H": "",
    "I": "",
    "J": "",
    "K": "",
    "L": "",
    "M": "",
    "N": "",
    "O": "",
    "P": "",
    "Q": "",
    "R": "",
    "S": "",
    "T": "",
    "U": "",
    "V": "",
    "W": "",
    "X": "",
    "Y": "",
    "Z": ""
}

def extract_characters_with_positions(pdf_path):
    allCharsAndPos = []
    doc = pymupdf.open(pdf_path)
    
    for page_num, page in enumerate(doc):        
        # Get structured page dictionary containing character-level detail
        page_dict = page.get_text("rawdict")
        
        for block in page_dict["blocks"]:
            # Skip image blocks (only process text blocks)
            if block.get("type") == 0:
                for line in block["lines"]:
                    for span in line["spans"]:
                        for char in span["chars"]:
                            char_text = char["c"]
                            bbox = char["bbox"]  # (x0, y0, x1, y1)
                            
                            # Print character and its bounding box
                            allCharsAndPos.append([page_num + 1, char_text, bbox])
    return allCharsAndPos

def createGcodeFile(name, commands): #name must be .gcode; commands must be a list of valid gcode commands
    with open(name, "w") as file:
        file.writelines(commands)

def charactersToSVG(allCharsAndPos):
    allSVGsAndPos = []
    for i in allCharsAndPos:
        pageNum, char, (x0, y0, x1, y1) = i
        if char in SVGfiles:
            allSVGsAndPos.append([SVGfiles[char], x0, y0, x1, y1])
    return allSVGsAndPos

def svgToScaledPoints(allSVGsAndPos, subdivisions):
    all_transformed_paths = []
    for svgs in allSVGsAndPos:
        paths, attributes = svg2paths(svgs[0])

        svgX0, svgX1, svgY0, svgY1 = Path(*paths).bbox()
        desiredWidth = svgs[3] - svgs[1] #get x1-x0
        desiredHeight = svgs[4] - svgs[2] #get y1-y0
        actualWidth = svgX1 - svgX0
        actualHeight = svgY1 - svgY0
        scaleFactor = min(desiredHeight/actualHeight, desiredWidth/actualWidth)
        translationX = svgs[1] - svgX0
        translationY = svgs[2] - svgY0
        
        tree = ET.parse(svgs[0])
        root = tree.getroot()
        
        viewbox = root.attrib.get('viewBox', '').split()
        page_height = float(viewbox[3]) if len(viewbox) == 4 else 300.0

        for path in paths:
            path_points = []
            
            for segment in path:
                for i in range(subdivisions + 1):
                    t = i / subdivisions
                    point = segment.point(t)
                    
                    x = point.real * scaleFactor + translationX
                    y = (page_height - point.imag) * scaleFactor + translationY
                    
                    path_points.append((round(x, 3), round(y, 3)))
                    
            all_transformed_paths.append(path_points)

    return all_transformed_paths

def SVGtoCommands(allSVGsAndPos):
    commands = ""
    for letter in allSVGsAndPos:
        print()



path = "Python/HandWriting Robot test.pdf"
charactersToSVG(extract_characters_with_positions(path))
#Workflow: 
#createGcodeFile("output.gcode", SVGtoCommands(charactersToSVG(extract_characters_with_positions(path))))