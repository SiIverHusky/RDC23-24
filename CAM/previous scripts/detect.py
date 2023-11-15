# Import the required modules
import cv2
import numpy as np

# Define the color boundaries in HSV
lower_red = np.array([0, 168, 183])
higher_red = np.array([25, 255, 255])

lower_blue = np.array([90, 168,183])
higher_blue = np.array([150, 255, 255])

lower_green = np.array([40, 168, 183])
higher_green = np.array([90, 255, 255])

# Define a flag to switch between red and blue detection
red_flag = True

# Define a flag to toggle between raw and processed output
mask_flag = False
0
# Capture the video from the camera
cap = cv2.VideoCapture(0)

# Loop until the user presses 'q' key
while True:
    # Read a frame from the camera
    ret, frame = cap.read()

    # Check if the frame is valid
    if not ret:
        break

    # Convert the frame to HSV color space
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    # Apply a green mask to the frame to extract the region of interest
    green_mask = cv2.inRange(hsv, lower_green, higher_green)

    # Find the contours of the green regions
    green_contours, _ = cv2.findContours(green_mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    # Check if any green contour is found
    if green_contours:
        # Find the largest green contour
        max_area = 0
        max_contour = None
        for contour in green_contours:
            area = cv2.contourArea(contour)
            if area > max_area:
                max_area = area
                max_contour = contour

        # Compute the minimum bounding rectangle of the largest green contour
        x, y, w, h = cv2.boundingRect(max_contour)

        # Add some padding to the rectangle
        pad = 20
        x = max(0, x - pad)
        y = max(0, y - pad)
        w = min(frame.shape[1] - x, w + 2 * pad)
        h = min(frame.shape[0] - y, h + 2 * pad)

        # Draw the rectangle on the frame
        cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)

        # Crop the region of interest from the frame
        roi = frame[y:y + h, x:x + w]

        # Convert the region of interest to HSV color space
        roi_hsv = cv2.cvtColor(roi, cv2.COLOR_BGR2HSV)

        # Apply a red or blue mask to the region of interest depending on the flag
        if red_flag:
            color_mask = cv2.inRange(roi_hsv, lower_red, higher_red)
        else:
            color_mask = cv2.inRange(roi_hsv, lower_blue, higher_blue)

        # Find the contours of the red or blue regions
        color_contours, _ = cv2.findContours(color_mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

        # Check if any red or blue contour is found
        if color_contours:
            # Initialize a list to store the rectangles and their centers
            rects = []

            # Loop through each contour
            for contour in color_contours:
                # Compute the minimum bounding rectangle of the contour
                rect = cv2.minAreaRect(contour)

                # Convert the rectangle to a list of four vertices
                box = cv2.boxPoints(rect)
                box = np.int0(box)

                # Calculate the center point of the rectangle
                cx = int(rect[0][0])
                cy = int(rect[0][1])

                # Append the rectangle and its center to the list
                rects.append((box, (cx, cy)))

            # Sort the rectangles by their x-coordinates of the centers
            rects.sort(key=lambda x: x[1][0])

            # Initialize a boolean array to store the color encoding
            color_array = []

            # Loop through each rectangle
            for i, (box, center) in enumerate(rects):
                # Draw the rectangle on the region of interest
                cv2.drawContours(roi, [box], 0, (0, 0, 255), 2)

                # Annotate the rectangle with a number
                cv2.putText(roi, str(i + 1), center, cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 255, 255), 2)

                # Classify the color of the rectangle and append it to the array
                if red_flag:
                    color_array.append(1)
                else:
                    color_array.append(0)

            # Display the color array on the frame
            cv2.putText(frame, str(color_array), (10, 50), cv2.FONT_HERSHEY_SIMPLEX, 2, (255, 255, 255), 2)

    # Check if the user presses 'c' key to switch between red and blue detection
    if cv2.waitKey(1) & 0xFF == ord('c'):
        red_flag = not red_flag

    # Check if the user presses 'm' key to toggle between raw and processed output
    if cv2.waitKey(1) & 0xFF == ord('m'):
        mask_flag = not mask_flag

    # Check if the user presses 'q' key to quit the program
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

    # Display the frame or the mask depending on the flag
    if mask_flag:
        cv2.imshow('Mask', color_mask)
    else:
        cv2.imshow('Frame', frame)

# Release the camera and destroy the windows
cap.release()
cv2.destroyAllWindows()
