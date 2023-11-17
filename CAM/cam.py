# Libraries Needed
import cv2
import numpy as np
import os
import time
import serial

# Constants
com = 'COM7'
track = 5


# Define the color boundaries in HSV
lower_red_0 = np.array([0,120,120])
higher_red_0 = np.array([13, 255, 255])   

lower_red_180 = np.array([170,120,120])
higher_red_180 = np.array([180, 255, 255])

lower_blue = np.array([80,75,75])
higher_blue = np.array([110, 255, 255])

lower_green = np.array([30,20,20])
higher_green = np.array([75, 255, 255])

# Flags
red_flag = True
img_flag = True
mask_flag = True

# Capture Sources
cam = cv2.VideoCapture(1)
os.chdir(os.path.dirname(os.path.abspath(__file__)))
dft = cv2.imread("usb1.jpg")
dif = cv2.imread("bluegreen.jpg")
ali = cv2.imread("notaligned.jpg")
tlt = cv2.imread("tilted.jpg")

img = dft

# Time Trackers
last_time = time.time()
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
    if img_flag:
        detected = img
    else:
        _, detected = cam.read()
    while True:
        if tracker >= track:
            break

        if img_flag:
            frame = img
            ret = True
        else:
            ret, frame = cam.read()

        if not ret:
            break

        if time.time() - process_time >= 1:                         # Only process every seconds
            process_frame = frame

            process_frame = cv2.GaussianBlur(process_frame, (5, 5), 0)  # Remove noise w/ Gaussian
            process_frame = cv2.medianBlur(process_frame, 9)            # Remove noise w/ Median

            hsv = cv2.cvtColor(process_frame, cv2.COLOR_BGR2HSV)        # Convert to HSV

            if red_flag:
                lmask = cv2.inRange(hsv, lower_red_0, higher_red_0)
                umask = cv2.inRange(hsv, lower_red_180, higher_red_180)
                cmask = cv2.bitwise_or(lmask, umask)
            else:
                cmask = cv2.inRange(hsv, lower_blue, higher_blue)

            gmask = cv2.inRange(hsv, lower_green, higher_green)
            mask = cv2.bitwise_or(cmask, gmask)
            kernel = np.ones((5,5), np.uint8)
            mask = cv2.erode(mask, kernel, iterations=2)

            detected = cv2.bitwise_and(process_frame, process_frame, mask=mask)
            gframe = cv2.bitwise_and(process_frame, process_frame, mask=gmask)

            template = searchGreen(gframe)
            boxes = searchBoxes(mask, template)
            boxes = classifyBoxes(boxes, cmask, gmask)

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
            mask = destroyNonBoxes(mask, boxes)
            detected = cv2.bitwise_and(process_frame, process_frame, mask=mask)
            for box in boxes:
                x, y, w, h, c = box
                if c == 0:
                    cv2.rectangle(detected, (x, y), (x+w, y+h), (0, 255, 0), 2)
                    cv2.putText(detected, "0", (x, y), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
                else:
                    cv2.rectangle(detected, (x, y), (x+w, y+h), (255, 0, 255), 2)
                    cv2.putText(detected, "1", (x, y), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 0, 255), 2)
        
        if mask_flag:
            cv2.imshow('CAM', detected)
        else:
            cv2.imshow('CAM', frame)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
        if cv2.waitKey(1) & 0xFF == ord('m'):
            mask_flag = not mask_flag
        if cv2.waitKey(1) & 0xFF == ord('t'):
            test_flag = not test_flag
        if cv2.waitKey(1) & 0xFF == ord('r'):
            red_flag = True
        if cv2.waitKey(1) & 0xFF == ord('b'):
            red_flag = False
        if cv2.waitKey(1) & 0xFF == ord('t'):
            test_flag = not test_flag
        if cv2.waitKey(1) & 0xFF == ord('0'):
            img = dft
        if cv2.waitKey(1) & 0xFF == ord('1'):
            img = dif
        if cv2.waitKey(1) & 0xFF == ord('2'):
            img = ali
        if cv2.waitKey(1) & 0xFF == ord('3'):
            img = tlt

    cam.release()
    cv2.destroyAllWindows()

    if (len(boxes) != 0):
        box_array = ''
        print(f'Sending {datBT}...')
        for dat in datBT:
            _, _, _, _, c = dat
            box_array += str(c)
        box_array = box_array[::-1]
        print(f'Final Data: {box_array}')
        ser = serial.Serial(port=com, baudrate=38400, bytesize=8, parity="N", stopbits=1, timeout=10)
        print(f'Writing to {com}...')
        ser.write(box_array.encode())

        ser.close()