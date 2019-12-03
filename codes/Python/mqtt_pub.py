import paho.mqtt.client as mqtt
import time
from random import randint

statusTopic = 'abhay/home/room1/status'
tempTopic = 'abhay/home/room1/temp'
humdTopic = 'abhay/home/room1/humd'
mqttBrokerIp = '23.20.0.61'
mqttBrokerPort = 1883

client = mqtt.Client()
client.connect(mqttBrokerIp, mqttBrokerPort)#52.204.21.160
print "Connected to MQTT"
client.publish(statusTopic, '1')
while 1:
        x = randint(10,26)
        print x
        client.publish(tempTopic, str(x))
        y = randint(0,100)
        print y
        client.publish(humdTopic, str(y))
        print "Publish done"
        time.sleep(5)

    
