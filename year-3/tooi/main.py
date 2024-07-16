import cv2
import numpy as np

def getAntColorsCount(frame, rect):
    ans = 0
    threshold = 60
    (x0, y0, w, h) = rect
    # print(rect)
    for x in range (x0, x0 + w, 1):
        for y in range(y0, y0 + h, 1):
            color = frame[y , x]
            #print(color)
            (b, g, r) = color
            if b <= threshold and g <= threshold and r <= threshold:
                ans += 1
    return ans

def isAnt(contour, frame):
    perimeter = cv2.arcLength(contour, True)
    if perimeter < 100 or perimeter > 600:
        return False, None

    approx = cv2.approxPolyDP(contour, 0.04 * perimeter, True)
    if len(approx) < 4 or len(approx) > 10:
        return False, None

    # print("ant check")
    # print(perimeter)
    # print(len(approx))

    detected_box = cv2.boundingRect(approx)
    dark_count = getAntColorsCount(frame, detected_box)
    # print(dark_count)
    if dark_count < 10 or dark_count > 150:
        return False, None

    (x, y, w, h) = detected_box
    if not ((w < 85) and (w > 5) and (h < 85) and (h > 5) and (w * h < 2500)):
        return False, None

    return True, detected_box

def intersectionSquare(rect1, rect2):
    #print(rect1)
    #print(rect2)
    left1, bottom1, w1, h1 = rect1
    left2, bottom2, w2, h2 = rect2
    right1, top1 = left1 + w1, bottom1 + h1
    right2, top2 = left2 + w2, bottom1 + h2

    leftIntersection = max(left1, left2)
    topIntersection = min(top1, top2)
    rightIntersction = min(right1, right2)
    bottonIntersection = max(bottom1, bottom2)
    widhtIntersection = rightIntersction - leftIntersection
    heightIntersection = topIntersection - bottonIntersection

    if widhtIntersection < 0 or heightIntersection < 0:
        return 0
    else:
        return widhtIntersection * heightIntersection

video = cv2.VideoCapture("./resources/ants.avi")

frameN = 0

frame_width = int(video.get(cv2.CAP_PROP_FRAME_WIDTH))
frame_height = int(video.get(cv2.CAP_PROP_FRAME_HEIGHT))
fps = int(video.get(cv2.CAP_PROP_FPS))
video_output = cv2.VideoWriter("./output/output.avi", cv2.VideoWriter_fourcc(*"MJPG"), fps, (frame_width, frame_height))

trackers = []

while(True):
    success, frame = video.read()
    if not success:
        break

    frameN += 1
    if frameN < 12000:
        continue
    if frameN > 15000:
        break
    if frameN % 10 == 0:
        print(frameN)

    bboxs = []
    empty_trackers = []
    for tracker in trackers:
        success, bbox = tracker.update(frame)
        dark_count = getAntColorsCount(frame, bbox)
        if (not success) or  dark_count < 10 or dark_count > 200:
            empty_trackers.append(tracker)
            continue
        bboxs.append(bbox)

    for tracker in empty_trackers:
        trackers.remove(tracker)

    for i in range(len(bboxs)):
        for j in range(len(bboxs)):
            if (i <= j) or (bboxs[i] == None) or (bboxs[j] == None):
                continue

            s = intersectionSquare(bboxs[i], bboxs[j])
            if (s == 0):
                continue

            (x1, y1, w1, h1) = bboxs[i]
            (x2, y2, w2, h2) = bboxs[j]
            s1 = w1 * h1
            s2  = w2 * h2

            if (s1 <= s2):
                bboxs[i] = None
            else:
                bboxs[j] = None

    for i in reversed(range(len(bboxs))):
        if (bboxs[i] == None):
            bboxs.pop(i)
            trackers.pop(i)

    gray_image = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    denoised = cv2.fastNlMeansDenoising(gray_image, None, 10, 7, 21)

    sharpening_kernel = np.array([[-1, -1, -1], [-1, 9, -1], [-1, -1, -1]])
    sharpened_image = cv2.filter2D(denoised, -1, sharpening_kernel)

    ret, thresh = cv2.threshold(sharpened_image, 120, 255, cv2.THRESH_BINARY_INV)
    contours, _  = cv2.findContours(thresh, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

    for contour in contours:
        success, detected_box = isAnt(contour, frame)
        if not success:
            continue

        rect = detected_box
        (x, y, w, h) = rect

        have_cross = False
        swap_rect = -1
        s_max = 0
        for i in range(len(bboxs)):
            s = intersectionSquare(rect, bboxs[i])
            if (s > 0):
                have_cross = True
                (_, _, w1, h1) = rect
                (_, _, w2, h2) = bboxs[i]
                if (w1*h1 > w2*h2) and (s > s_max):
                    swap_rect = i
                    s_max = s

        if not have_cross:
            if (swap_rect != -1):
                bboxs.pop(i)
                trackers.pop(i)
            tracker = cv2.TrackerCSRT_create()
            tracker.init(frame, rect)
            trackers.append(tracker)
            bboxs.append(rect)

        for bbox in bboxs:
            x, y, w, h = bbox
            cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)

        # print("     len_approx = ", len(approx))
        # cv2.drawContours(image, [contour], -1, (0, 255, 0), 2)

    video_output.write(frame)
    # if not cv2.imwrite(f'./frames/frame_{frameN}.jpg', frame):
    #     raise Exception("Could not write image")

video.release()
video_output.release()

cv2.waitKey(0)
cv2.destroyAllWindows()
