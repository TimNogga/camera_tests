from pyorbbecsdk import Pipeline
import cv2

# 1. Create the pipeline
pipeline = Pipeline()

# 2. Start streaming
pipeline.start()

try:
    while True:
        # 3. Wait up to 100 ms for a new set of frames
        frames = pipeline.wait_for_frames(100)
        if frames is None:
            print("No frames received—check connection or power.")
            continue

        # 4. Grab color + depth
        color_frame = frames.get_color_frame()
        depth_frame = frames.get_depth_frame()

        # 5. Convert to numpy arrays (OpenCV friendly)
        color_img = color_frame.asarray()  # HxWx3 uint8
        depth_img = depth_frame.asarray()  # HxW uint16 (depth in mm)

        # 6. Display
        cv2.imshow("Color", color_img)
        # Normalize depth for visualization
        depth_vis = cv2.convertScaleAbs(depth_img, alpha=0.03)
        cv2.imshow("Depth", depth_vis)

        if cv2.waitKey(1) & 0xFF == ord("q"):
            break

finally:
    # 7. Stop streaming & clean up
    pipeline.stop()
    cv2.destroyAllWindows()
