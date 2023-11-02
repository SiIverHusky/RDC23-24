import cv2
import numpy as np

capture = cv2.VideoCapture(0)

# red = rgb(225, 81, 86)

# In HSV
lower_red = np.array([0, 100, 20])
higher_red = np.array([25, 255, 255])

lower_blue = np.array([90, 50, 50])
higher_blue = np.array([150, 255, 255])

lower_green = np.array([40, 50, 50])
higher_green = np.array([90, 255, 255])

lower_purple = np.array([150, 50, 50])
higher_purple = np.array([180, 255, 255])

show_detected = False
color = "red"

if __name__ == "__main__":
    while True:
        ret, frame = capture.read()

        # Converting to HSV
        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

        if color == "red":
            mask = cv2.inRange(hsv, lower_red, higher_red)
        elif color == "blue":
            mask = cv2.inRange(hsv, lower_blue, higher_blue)
        elif color == "green":
            mask = cv2.inRange(hsv, lower_green, higher_green)
        elif color == "purple":
            mask = cv2.inRange(hsv, lower_purple, higher_purple)

        detected = cv2.bitwise_and(frame, frame, mask=mask)

        if show_detected:
            cv2.imshow('frame', detected)
        else:
            cv2.imshow('frame', frame)

        key = cv2.waitKey(20)
        
        if key == ord('q'):
            break
        elif key == ord('m'):
            show_detected = not show_detected
        elif key == ord('r'):
            color = "red"
        elif key == ord('b'):
            color = "blue"
        elif key == ord('g'):
            color = "green"
        elif key == ord('p'):
            color = "purple"


    capture.release()
    cv2.destroyAllWindows()

    cv2.waitKey(0)