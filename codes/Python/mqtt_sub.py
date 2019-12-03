import paho.mqtt.client as mqtt
import datetime

subscribeTopic = 'abhay/home/room1/#'
mqttBrokerIp = '23.20.0.61'
mqttBrokerPort = 1883

def on_connect(client, userdata, flags, rc):
    print("connected with result code: "+str(rc))
    client.subscribe("abhay/home/room1/#",2)
def on_message(client, userdata,msg):
    print datetime.datetime.now()
    print(msg.topic+"= "+str(msg.payload))
    print"------------------------"
    
client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.connect(mqttBrokerIp,mqttBrokerPort)#52.204.21.160
client.loop_forever()
