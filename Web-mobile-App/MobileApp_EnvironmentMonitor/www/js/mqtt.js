var client = mqtt.connect('ws://23.20.0.61:9001');

var temptopic = "abhay/home/room1/temp";
var humdtopic = "abhay/home/room1/humd";
var lwttopic = "abhay/home/room1/lwt";
var statusCheck = "abhay/home/room1/status";
var switchtopic = "abhay/home/room1/switch";

client.subscribe(temptopic);
client.subscribe(humdtopic);
client.subscribe(lwttopic);
client.subscribe(statusCheck);

client.publish(statusCheck, '0');

client.on("message", function(topic, payload) {
  console.log(topic, payload.toString());
  if(topic === lwttopic){
    if(payload.toString() === '1'){
      $('#statusOO').attr('fill','#ff3333');
      $('#statusValue').text('OFFLINE');
    }
  }
  if(topic === statusCheck){
    if(payload.toString() === '1'){
      $('#statusOO').attr('fill','#4ac35b');
      $('#statusValue').text('ONLINE');
    }
  }
  if(topic === temptopic){
    document.getElementById('tempValue').innerHTML = payload.toString();
    var date = new Date();
    var ddmmyyyy = date.getDate() + "/" + (date.getMonth()+1) + "/"  + date.getFullYear();
    var mn = date.getMinutes();
    var hr = date.getHours();
    var min = mn > 9 ? "" + mn: "0" + mn;
    var hour = hr > 9 ? "" + hr: "0" + hr;
    document.getElementById('tempday').innerHTML = ddmmyyyy;
    document.getElementById('temphour').innerHTML = hour;
    document.getElementById('tempmin').innerHTML = " <b>:</b>"+ min;
    document.getElementById('tempsec').innerHTML = " <b>:</b>"+ date.getSeconds();
  }
  if(topic === humdtopic){
    document.getElementById('humdValue').innerHTML = payload.toString();
    var date = new Date();
    var ddmmyyyy = date.getDate() + "/" + (date.getMonth()+1) + "/"  + date.getFullYear();
    var mn = date.getMinutes();
    var hr = date.getHours();
    var min = mn > 9 ? "" + mn: "0" + mn;
    var hour = hr > 9 ? "" + hr: "0" + hr;
    document.getElementById('humdday').innerHTML = ddmmyyyy;
    document.getElementById('humdhour').innerHTML = hour;
    document.getElementById('humdmin').innerHTML = " <b>:</b>"+ min;
    document.getElementById('humdsec').innerHTML = " <b>:</b>"+ date.getSeconds();
  }
});

$(function(argument) {
  $('[name="actuator-switch"]').bootstrapSwitch();
});
$('input[name="actuator-switch"]').on('switchChange.bootstrapSwitch', function(event, state) {

   console.log(state); // true | false

   if(state==true)
     status='1';
   if(state==false)
     status='0';

  //Publish the switch status
  client.publish(switchtopic,status);
});
