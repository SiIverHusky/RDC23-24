import cv2
import numpy as np
import os
import time

# Define the color boundaries in HSV
lower_red = np.array([0,100,100])
higher_red = np.array([13, 255, 255])   

lower_blue = np.array([80,100,100])
higher_blue = np.array([110, 255, 255])

lower_green = np.array([40,100,100])
higher_green = np.array([75, 255, 255])


# Flags
red_flag = False
test_flag = True
mask_flag = False

# Capture Sources
cam = cv2.VideoCapture(1)
os.chdir(os.path.dirname(os.path.abspath(__file__)))
dft = cv2.imread("bluegreen.jpg")
dif = cv2.imread("redgreen.jpg")
ali = cv2.imread("notaligned.jpg")
tlt = cv2.imread("tilted.jpg")

img = dft


last_time = time.time()

# Functions
def searchGreen(frame):
    print("Searching for green...")
    green_mask = cv2.inRange(frame, lower_green, higher_green)
    green_contours, _ = cv2.findContours(green_mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    if green_contours:
        max_contour = max(green_contours, key=cv2.contourArea)
        x, y, w, h = cv2.boundingRect(max_contour)
        print(f'Found green at {x}, {y} with width {w} and height {h}')
        return x, y, w, h
    else:
        return None, None, None, None

def searchBoxes(cnt, templateBox, mask):
    print("Searching for boxes...")
    if templateBox is not None:
        _, _, tw, th = templateBox
        if tw is not None and th is not None:
            contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
            boxes = []
            for cnt in contours:
                x, y, w, h = cv2.boundingRect(cnt)
                if 0.75 * tw * th < w * h < 1.25 * tw * th:
                    print(f'Found box at {x}, {y} with width {w} and height {h}')
                    boxes.append((x, y, w, h))
            return boxes
    return None

def classifyBoxes(frame, boxes, cmask, gmask):
    print("Classifying boxes...")
    classified_boxes = []
    boxes = [box for box in boxes if box]
    boxes.sort(key=lambda box: box[0])

    for box in boxes:
        print(box)
        x, y, w, h = box

        # Check if box is seen on cmask or gmask within the inner 40 pixels
        cmask_seen = np.any(cmask[int(y+(h*0.3)):int(y+h-(h*0.3)), int(x+(w*0.3)):int(x+w-(w*0.3))])
        gmask_seen = np.any(gmask[int(y+(h*0.3)):int(y+h-(h*0.3)), int(x+(w*0.3)):int(x+w-(w*0.3))])

        # Assign 1 if seen on cmask, 0 if seen on gmask
        if cmask_seen:
            classified_boxes.append((x, y, w, h, 1))
        elif gmask_seen:
            classified_boxes.append((x, y, w, h, 0))

    return classified_boxes

if __name__ == "__main__":
    boxes = []
    while True:
        if test_flag:
            frame = img
            ret = True
        else:
            ret, frame = cam.read()

        if not ret:
            break

        frame = cv2.GaussianBlur(frame, (5, 5), 0)  # Remove noise w/ Gaussian
        frame = cv2.medianBlur(frame, 9)            # Remove noise w/ Median
        frame = cv2.GaussianBlur(frame, (5, 5), 0)  # Remove noise w/ Gaussian
        frame = cv2.medianBlur(frame, 9)            # Remove noise w/ Median


        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)


        if red_flag:
            cmask = cv2.inRange(hsv, lower_red, higher_red)
        else:
            cmask = cv2.inRange(hsv, lower_blue, higher_blue)

        gmask = cv2.inRange(hsv, lower_green, higher_green)
        mask = cv2.bitwise_or(cmask, gmask)
        mask = cv2.erode(mask, (5,5), iterations=10)

        detected = cv2.bitwise_and(frame, frame, mask=mask)
        gframe = cv2.bitwise_and(frame, frame, mask=gmask)
        
        if time.time() - last_time >= 5:
            last_time = time.time()
            template = searchGreen(gframe)
    #
            if template is not None:
                contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
                for cnt in contours:
                    dx, dy, dw, dh = cv2.boundingRect(cnt)
                    cv2.rectangle(detected, (dx, dy), (dx+dw, dy+dh), (255, 255, 255), 2)
                    foundBox = searchBoxes(cnt, template, mask)
                    if foundBox is None:
                        continue
                    print(f'Search boxes raw returned {foundBox}')
                    foundBox = [[boxes[0], boxes[1], boxes[2], boxes[3]] for boxes in foundBox if len(boxes) >= 4]
                    print(f'Search boxes returned {foundBox}')
                    if foundBox is not None:
                        boxes = foundBox
                        print(f'Found {len(boxes)} boxes')
                        print(boxes)
                        boxes = [box for box in boxes if box is not None]
                        print(f'Found {len(boxes)} boxes not none')
                        print(boxes)
                        boxes = classifyBoxes(frame, boxes, cmask, gmask)
                        print(f'Found {len(boxes)} boxes classified')

        for box in boxes:
            print(f'Box: {box}')
            x, y, w, h, color = box
            if color == 1:
                cv2.putText(detected, "1", (x + w//2, y + h//2), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 255, 255), 2, cv2.LINE_AA)
            elif color == 0:
                cv2.putText(detected, "0", (x + w//2, y + h//2), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 255, 255), 2, cv2.LINE_AA)

        
        if mask_flag:
            cv2.imshow("frame", detected)
        else:
            cv2.imshow("frame", frame)

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