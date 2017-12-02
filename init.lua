wifi.setmode(wifi.STATION)  
wifi.sta.config("Demian","ustb1994") 
wifi.sta.connect()
print(wifi.sta.getip()) 
led = 3  
gpio.mode(led, gpio.OUTPUT)   
local mytimer = tmr.create()
mytimer = tmr.create()
mytimer:register(1000, tmr.ALARM_AUTO, function()
    if wifi.sta.getip() == nil then
        print('Waiting for IP ...')
    else
        print('IP is ' .. wifi.sta.getip())
        http.request("http://39.106.107.244:8080/springTest/user/id/1", "GET", "", "", function(code, data)
    if (code < 0) then
      print("HTTP request failed")
    else
      print(code, data)
      t = cjson.decode(data)
      for k,v in pairs(t) do print(k,v) end
      if (t["name"] == "on") then 
          gpio.write(led, gpio.HIGH) 
      elseif (t["name"] == "off") then 
          gpio.write(led, gpio.LOW) end    
    end
  end)
        tmr.stop(1)
    end
end)
mytimer:interval(1000) -- actually, 3 seconds is better!
mytimer:start()
