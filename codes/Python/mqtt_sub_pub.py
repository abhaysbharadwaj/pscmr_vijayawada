import paho.mqtt.client as mqtt
import datetime
from random import randint
switchTopic = "abhay/home/room1/switch"
tempTopic = "abhay/home/room1/temp"
humidityTopic = "abhay/home/room1/hum"

def on_connect(client, userdata, flags, rc):
    print("connected with result code: "+str(rc))
    client.subscribe(switchTopic,2)

def on_message(client, userdata,msg):
    print datetime.datetime.now()
    print(msg.topic+"= "+str(msg.payload))
    print"-----------------------------------"
    if (msg.topic == switchTopic):
        if(str(msg.payload) == "1"):
            print "Switch is ON"
        else:
            print "Switch is OFF"
    
    
client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.connect("52.204.21.160",1883)
print "connected to broker"
client.loop_forever()
