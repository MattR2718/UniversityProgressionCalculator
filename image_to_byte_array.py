from PIL import Image
import numpy as np

file_name = 'WindowIcon'

# Load the image
image_path = file_name + '.png'
image = Image.open(image_path)
image = image.convert("RGBA")  # Ensure the image is in RGBA format
image_data = np.array(image)

# Flatten the image data to a byte array
byte_array = image_data.flatten().tolist()

# Create a C++ header file
header_file_content = f"""#pragma once

const int icon_width = {image.width};
const int icon_height = {image.height};
unsigned char icon_data[] = {{ {', '.join(map(str, byte_array))} }};
"""

# Save the header file
with open(file_name + '.h', 'w') as file:
    file.write(header_file_content)

print("Header file 'icon.h' generated successfully.")
