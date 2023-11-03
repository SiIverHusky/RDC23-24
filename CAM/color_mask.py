import cv2
import numpy as np

capture = cv2.VideoCapture(1)

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

        frame = cv2.GaussianBlur(frame, (5, 5), 0)  # Remove noise w/ Gaussian
        frame = cv2.medianBlur(frame, 5)            # Remove noise w/ Median

        # Converting to HSV
        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

        # Creating mask
        r_mask = cv2.inRange(hsv, lower_red, higher_red)
        b_mask = cv2.inRange(hsv, lower_blue, higher_blue)
        g_mask = cv2.inRange(hsv, lower_green, higher_green)
        p_mask = cv2.inRange(hsv, lower_purple, higher_purple)

        if color == "red":
            mask = r_mask
        elif color == "blue":
            mask = b_mask
        elif color == "green":
            mask = g_mask
        elif color == "purple":
            mask = p_mask
        elif color == "test":
            mask = cv2.bitwise_or(r_mask, b_mask)
            mask = cv2.bitwise_or(mask, g_mask)

        mask = cv2.erode(mask, (5,5), iterations=2)

        detected = cv2.bitwise_and(frame, frame, mask=mask)

        contours, hierarchy = cv2.findContours(mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

        for cnt in contours:
            m = cv2.moments(cnt)
            if m['m00'] > 10000:
                x, y, w, h = cv2.boundingRect(cnt)

                cv2.rectangle(detected, (x, y), (x+w, y+h), (255, 255, 0), 2)

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
        elif key == ord('t'):
            color = "test"

    capture.release()
    cv2.destroyAllWindows()

    cv2.waitKey(0)