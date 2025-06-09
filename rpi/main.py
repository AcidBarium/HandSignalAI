from picamera2 import Picamera2
import cv2
import mediapipe as mp
import math
from chuankou import send_byte


# 初始化摄像头
picam2 = Picamera2()
picam2.preview_configuration.main.size = (640, 480)
picam2.preview_configuration.main.format = "RGB888"
picam2.configure("preview")
picam2.start()

# 初始化 MediaPipe Hands
mp_hands = mp.solutions.hands
hands = mp_hands.Hands(max_num_hands=2)
mp_draw = mp.solutions.drawing_utils

def calculate_distance(p1, p2):
    return math.sqrt((p2[0] - p1[0])**2 + (p2[1] - p1[1])**2)

while True:
    frame = picam2.capture_array()
    frame = cv2.flip(frame, 1)
    rgb_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

    result = hands.process(rgb_frame)

    if result.multi_hand_landmarks:
        for handLms in result.multi_hand_landmarks:
            h, w, c = frame.shape

            # 提取所需关键点
            wrist_coords = (int(handLms.landmark[0].x * w), int(handLms.landmark[0].y * h))
            thumb_tip_coords = (int(handLms.landmark[4].x * w), int(handLms.landmark[4].y * h))
            index_tip_coords = (int(handLms.landmark[8].x * w), int(handLms.landmark[8].y * h))
            index_mcp_coords = (int(handLms.landmark[5].x * w), int(handLms.landmark[5].y * h))

            # 计算两段距离
            d1 = calculate_distance(thumb_tip_coords, index_tip_coords)
            d2 = calculate_distance(index_mcp_coords, wrist_coords)

            # # 防止除零
            # if d2 != 0:
            #     ratio = d1 / d2
            #     print(f"Thumb-Index Tip Distance / Index MCP-Wrist Distance: {ratio:.2f}")
            # else:
            #     print("Index MCP and Wrist are at the same position!")
                
            radio_out = d1 / d2
            
            pwm_pin = max(0,radio_out - 0.2)
            pwm_pin = min(1.4,pwm_pin)
            
            pwm_pin = (pwm_pin / 1.4)  * 100
            
            # set_pwm(gpio_pin=18, frequency=int(pwm_pin), duty_cycle=500000)

            print(f"now pwn pin = : {pwm_pin:.2f}")
            
            send_byte(int(pwm_pin))

            # 可视化
            cv2.circle(frame, wrist_coords, 8, (0, 255, 255), -1)
            cv2.circle(frame, thumb_tip_coords, 8, (0, 255, 0), -1)
            cv2.circle(frame, index_tip_coords, 8, (255, 0, 0), -1)
            cv2.circle(frame, index_mcp_coords, 8, (0, 0, 255), -1)

            # 画连接线
            cv2.line(frame, thumb_tip_coords, index_tip_coords, (0, 255, 0), 2)
            cv2.line(frame, index_mcp_coords, wrist_coords, (255, 0, 0), 2)

            mp_draw.draw_landmarks(frame, handLms, mp_hands.HAND_CONNECTIONS)

    cv2.imshow("Hand Tracking", frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

picam2.close()
cv2.destroyAllWindows()
