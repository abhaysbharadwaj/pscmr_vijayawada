/******************        USER INPUT     *******************************/

var temperature_topic = 'abhay/home/room1/temp';
var humidity_topic = 'abhay/home/room1/hum';
var switch_topic = 'abhay/home/room1/switch';
var lastWill_topic = 'abhay/home/room1/lwt';
var subscribe_topic = 'abhay/home/room1/#';

/************************************************************************/
var client = mqtt.connect('ws://23.20.0.61:9001');
var dt = {}; //Object to store date realted stuff

/* Callback received whenever the client is connected to the MQTT broker*/
client.on('connect', function () {
  console.log('connected');
  client.subscribe(subscribe_topic);
});

client.on('message', function(topic, payload){
    if(topic == temperature_topic){
      dt.date = getDate(new Date());
      document.getElementById('temperatureValue').innerHTML = payload;
      document.getElementById('temperatureDay').innerHTML = dt.date.ddmmyyyy;
      document.getElementById('temperatureHour').innerHTML = dt.date.hour;
      document.getElementById('temperatureMinute').innerHTML = " <b>:</b>"+ dt.date.min;
      document.getElementById('temperatureSecond').innerHTML = " <b>:</b>"+ dt.date.seconds;
    }
    if(topic == humidity_topic){
      dt.date = getDate(new Date());
      document.getElementById('humidityValue').innerHTML = payload;
      document.getElementById('humidityDay').innerHTML = dt.date.ddmmyyyy;
      document.getElementById('humidityHour').innerHTML = dt.date.hour;
      document.getElementById('humidityMinute').innerHTML = " <b>:</b>"+ dt.date.min;
      document.getElementById('humiditySecond').innerHTML = " <b>:</b>"+ dt.date.seconds;
    }
    if(topic == lastWill_topic){
      dt.date = getDate(new Date());
      $('#lwt-cirlce').attr('fill', '#ff0000');
      document.getElementById('lwtDay').innerHTML = dt.date.ddmmyyyy;
      document.getElementById('lwtHour').innerHTML = dt.date.hour;
      document.getElementById('lwtMinute').innerHTML = " <b>:</b>"+ dt.date.min;
      document.getElementById('lwtSecond').innerHTML = " <b>:</b>"+ dt.date.seconds;
    }
});

/*
 * Function to generate the current timestamp in the formart of dd/mm/yyyy hr min sec
 */
function getDate(date){
  this.dt.ddmmyyyy = date.getDate() + "/" + (date.getMonth()+1) + "/"  + date.getFullYear();
  this.dt.mn = date.getMinutes();
  this.dt.hr = date.getHours();
  this.dt.min = this.dt.mn > 9 ? "" + this.dt.mn: "0" + this.dt.mn;
  this.dt.hour = this.dt.hr > 9 ? "" + this.dt.hr : "0" + this.dt.hr ;
  this.dt.seconds = date.getSeconds();
  return this.dt;
}
/* Switch Function and initalization*/
$(function(argument) {
  $('#relay').bootstrapSwitch();
});
$('#relay').on('switchChange.bootstrapSwitch', function(event, state) {
   if(state==true)
     status='1';
   if(state==false)
     status='0';
  //Publish the switch status
  client.publish(switch_topic,status);
});
