import jetson.inference
import jetson.utils
import time
import serial
from centroidtracker import CentroidTracker

net = jetson.inference.detectNet("ssd-mobilenet-v2", threshold=0.6)
# display = jetson.utils.videoOutput("display://0") 
# camera = jetson.utils.videoSource("walking.mp4")      
camera = jetson.utils.videoSource("csi://0")
arduino = serial.Serial(port='/dev/ttyUSB0', baudrate=115200, timeout=.1)

font = jetson.utils.cudaFont()
font = jetson.utils.cudaFont( size=32 )
ct = CentroidTracker(maxDisappeared=50, maxDistance=50)
object_id_list = []

SEND_FREQ = 300 # how long in seconds to wait before sending next count
next_time = time.time() + SEND_FREQ # send every 5 minutes
previous_count = 0

def send_lora(input):
	arduino.write(bytes(str(input), 'utf-8'))
	for i in range(15):
		arduino.readline() # clear response buffer


#while display.IsStreaming():
while True:
	img = camera.Capture()
	rects = []
	detections = net.Detect(img)
	for obj in detections:
		rects.append((int(obj.Left), int(obj.Bottom), int(obj.Right), int(obj.Top)))
	
	objects = ct.update(rects)
	for (objectId, bbox) in objects.items():
		
		if objectId not in object_id_list:
			object_id_list.append(objectId)
	
	#font.OverlayText(img, img.width, img.height, "Count: {}".format(len(object_id_list)), 5, 5, (255, 0, 0), (0, 0, 0))
	# display.Render(img)
	# display.SetStatus("Object Detection | Network {:.0f} FPS".format(net.GetNetworkFPS()))

	if time.time() > next_time:
		if ((len(object_id_list) - previous_count) > 255):
			send_lora(255)
			previous_count = previous_count + 255
			next_time += SEND_FREQ
		else:
			send_lora(len(object_id_list) - previous_count)
			previous_count = len(object_id_list)
			next_time += SEND_FREQ

