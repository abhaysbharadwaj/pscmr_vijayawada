import requests
import paho.mqtt.client as mqtt
import datetime
threshold = 25
subTopic = "abhay/room1/temp"
def on_connect(client, userdata, flags, rc):
    print("connected with result code: "+str(rc))
    client.subscribe(subTopic,2)
    print "subscribed to: " + subTopic
    print "Threshold = " + str(threshold)

def on_message(client, userdata,msg):
    print datetime.datetime.now()
    print(msg.topic+"= "+str(msg.payload))
    print"-----------------------------------"
    if (msg.topic == subTopic):
        if(float(msg.payload) > threshold):
            print "Email triggered"
            report = {}
            report["value1"] = msg.payload
            requests.post("https://maker.ifttt.com/trigger/dht11-email/with/key/cC_wsSzEFvrIT7kRyX-su6", data=report)
    
client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.connect("platform.myflip.io",1883)
client.loop_forever()
