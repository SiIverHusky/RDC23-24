# Libraries Needed
import cv2
import numpy as np
import os
import time
import serial

# Constants
com = 'COM4'        # Bluetooth Port

timeBetween = 5
track = 10//timeBetween           


# Define the color boundaries in HSV
lower_red_0 = np.array([0,120,120])
higher_red_0 = np.array([13, 255, 255])   

lower_red_180 = np.array([170,120,120])
higher_red_180 = np.array([180, 255, 255])

lower_blue = np.array([80,75,75])
higher_blue = np.array([130, 255, 255])

lower_green = np.array([25,20,20])
higher_green = np.array([75, 255, 255])

# Flag - Do not Touch for RDC
test_flag = True

# Capture Sources
cam = cv2.VideoCapture(1)
os.chdir(os.path.dirname(os.path.abspath(__file__)))
dft = cv2.imread("usb3.jpg")


img = dft

# Time Trackers
process_time = time.time()

# Functions
def searchGreen(frame):
    print("Searching for green...")
    green_mask = cv2.inRange(frame, lower_green, higher_green)
    kernel = np.ones((5,5), np.uint8)
    green_mask = cv2.morphologyEx(mask, cv2.MORPH_CLOSE, kernel)
    green_contours, _ = cv2.findContours(green_mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    if green_contours:
        max_contour = max(green_contours, key=cv2.contourArea)
        x, y, w, h = cv2.boundingRect(max_contour)
        if w > h:
            print('Two Green Boxes are next to each other...')
            print(f'Found green at {x}, {y} with width {w//2} and height {h}')
            return x, y, w//2, h
        print(f'Found green at {x}, {y} with width {w} and height {h}')
        return x, y, w, h
    else:
        return None, None, None, None

def destroyNonBoxes(mask, boxes):
    new_mask = np.zeros_like(mask)
    for box in boxes:
        if box is not None:
            x, y, w, h, _ = box
            np.copyto(new_mask[y:y+h, x:x+w], mask[y:y+h, x:x+w])
    return new_mask

def searchBoxes(mask, template):
    print('Searching for boxes...')
    _, _, tw, th = template
    boxes = []
    kernel = np.ones((5,5), np.uint8)
    mask = cv2.morphologyEx(mask, cv2.MORPH_CLOSE, kernel)
    contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    for cnt in contours:
        x, y, w, h = cv2.boundingRect(cnt)
        if ((tw * 0.75 < w and w < tw * 1.25) and (th * 0.75 < h and h < th * 1.25)) or ((tw * 2 * 0.75 < w and w < tw * 2 * 1.25) and (th * 0.75 < h and h < th * 1.25)):
            print('Within Template Size...')
            if w > h:
                print('Two Boxes are next to each other...')
                boxes.append((x, y, w//2, h))
                boxes.append((x + w//2, y, w//2, h))
                print(f'Two Boxes at {x}, {y} and {x + w//2}, {y} with width {w//2} and height {h}')
            else:
                print(f'Found box at {x}, {y} with width {w} and height {h}')
                boxes.append((x, y, w, h))
    return boxes

def classifyBoxes(boxes, cmask, gmask):
    print('Classifying boxes...')
    new_boxes = []
    boxes = sorted(boxes, key=lambda x: x[0])
    for box in boxes:
        print(f'Classifying box {box}...')
        x, y, w, h = box
        cmask_seen = np.any(cmask[int(y+(h*0.3)):int(y+h-(h*0.3)), int(x+(w*0.3)):int(x+w-(w*0.3))])
        gmask_seen = np.any(gmask[int(y+(h*0.3)):int(y+h-(h*0.3)), int(x+(w*0.3)):int(x+w-(w*0.3))])

        if cmask_seen and gmask_seen:
            continue
        elif gmask_seen:
            new_boxes.append((x, y, w, h, 0))
        elif cmask_seen:
            new_boxes.append((x, y, w, h, 1))
        
    return new_boxes

def are_boxes_similar(datBT, boxes, threshold):
    for box1, box2 in zip(datBT, boxes):
        if any(abs(val1 - val2) > threshold for val1, val2 in zip(box1[:5], box2[:5])):
            return False
    return True


if __name__ == '__main__':
    boxes = []
    datBT = []
    tracker = 0
    if test_flag:
        detected = img
    else:
        _, detected = cam.read()
    while True:
        if tracker >= track:
            break

        if test_flag:
            frame = img
            ret = True
        else:
            ret, frame = cam.read()

        if not ret:
            break

        if time.time() - process_time >= timeBetween:                       # Only process after timeBetween
            process_frame = frame

            process_frame = cv2.GaussianBlur(process_frame, (5, 5), 0)      # Remove noise w/ Gaussian
            process_frame = cv2.medianBlur(process_frame, 9)                # Remove noise w/ Median

            hsv = cv2.cvtColor(process_frame, cv2.COLOR_BGR2HSV)            # Convert to HSV

            # Creating masks to detect boxes
            lmask = cv2.inRange(hsv, lower_red_0, higher_red_0)             # Create mask for lower red
            umask = cv2.inRange(hsv, lower_red_180, higher_red_180)         # Create mask for upper red
            cmask = cv2.bitwise_or(lmask, umask)                            # Combine red masks
            bmask = cv2.inRange(hsv, lower_blue, higher_blue)               # Create mask for blue
            cmask = cv2.bitwise_or(cmask, bmask)                            # Combine blue and red masks

            gmask = cv2.inRange(hsv, lower_green, higher_green)             # Create mask for green
            mask = cv2.bitwise_or(cmask, gmask)                             # Combine green and red/blue masks
            kernel = np.ones((5,5), np.uint8)                   
            mask = cv2.erode(mask, kernel, iterations=2)                    # Erode mask

            detected = cv2.bitwise_and(process_frame, process_frame, mask=mask)     # Apply mask to frame
            gframe = cv2.bitwise_and(process_frame, process_frame, mask=gmask)      # Apply green mask to frame

            template = searchGreen(gframe)                                  # Search for green template
            boxes = searchBoxes(mask, template)                             # Search for boxes
            boxes = classifyBoxes(boxes, cmask, gmask)                      # Classify boxes

            if len(boxes) != 0:
                if tracker == 0:
                    datBT = boxes
                    tracker = 1
                elif tracker < track:
                    tracker += 1
                    if not are_boxes_similar(datBT, boxes, 30):
                        tracker = 0
                        print("Inconsistent, retracking...")
                else:
                    print("Consistent, sending data...")
                    if datBT == boxes:
                        break


            process_time = time.time()
        
        if len(boxes) > 0:
            # mask = destroyNonBoxes(mask, boxes)
            detected = cv2.bitwise_and(process_frame, process_frame, mask=mask)
            for box in boxes:
                x, y, w, h, c = box
                if c == 0:
                    cv2.rectangle(detected, (x, y), (x+w, y+h), (0, 255, 0), 2)
                    cv2.putText(detected, "0", (x + w//2 - 10, y - 10), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
                else:
                    cv2.rectangle(detected, (x, y), (x+w, y+h), (255, 0, 255), 2)
                    cv2.putText(detected, "1", (x + w//2 - 10, y - 10), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 0, 255), 2)
        

        cv2.imshow('CAM', frame)
        cv2.imshow('BOX', detected)


        if cv2.waitKey(1) & 0xFF == ord('q'):
            break


    cam.release()

    if (len(boxes) != 0):
        box_array = ''
        print(f'Sending {datBT}...')
        for dat in datBT:
            _, _, _, _, c = dat
            box_array += str(c)
        box_array = box_array[::-1]
        print(f'Final Data: {box_array}')
        check_array = [i for i, char in enumerate(box_array) if char == '1']
        check = int(check_array[0]) + 1
        check2 = int(check_array[1]) + 1
        checkStr = str(check) + str(check2)
        print(f'Sending {checkStr} to {com}...')

        attempts = 10  # Number of attempts to connect and write to the port
        for attempt in range(1, attempts + 1):
            try:
                ser = serial.Serial(port=com, baudrate=115200, bytesize=8, parity="N", stopbits=1, timeout=1)
                print(f'Writing to {com}...')
                ser.write(checkStr.encode())
                ser.close()
                print(f'Successfully wrote to {com}')
                break  # Exit the loop if successful
            except serial.SerialException:
                print(f'Attempt {attempt} failed to connect and write to {com}')
                if attempt == attempts:
                    print(f'All attempts failed. Unable to connect and write to {com}')
                else:
                    time.sleep(0.2)
    cv2.destroyAllWindows()