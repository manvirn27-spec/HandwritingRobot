import pymupdf

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

# Path to your PDF file
path = "Python/HandWriting Robot test.pdf"
print(extract_characters_with_positions(path))