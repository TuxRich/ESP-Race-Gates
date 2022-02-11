const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>Settings</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  </head><body>
  <H2>LED Gate 1</H2>
  <form action="/get">
    Manual input: <input type="text" name="input3">
    <input type="submit" value="Submit">
  </form>
  <br>
  <br>
  <form action="/get" id="huts">
    <label for="hut1">Color:</label>
    <select id="hut1" name="hut1">
      <option value="red">Red</option>
      <option value="green">Green</option>
      <option value="blue">Blue</option>
      <option value="purple">Purple</option>
      <option value="off">off</option>
    </select>
    <label for="hut2">Patten:</label>
    <select id="hut2" name="hut2">
      <option value="Color">Color</option>
      <option value="Flash">Flash</option>
      <option value="Ranbow">Ranbow</option>
      <option value="Juggle">Juggle</option>
    </select>
  </form>
  <input type="submit" value="Submit" onClick="submit_col()">&nbsp;&nbsp;
  <input type="submit" value="Submit" onClick="submit_pat()">
  <br>
  <br>
  <label for="b">LED Brightness: &nbsp;&nbsp; </label>
  <input type="range" min="0" max="255" value="96" class="slider" id="b">
  <input type="submit" value="Set" onClick="submit_Brightness()">
  <br>

<div id="main"></div>

<script>
function submit_col()
{
  hut1 = document.getElementById("hut1").value;
 
  console.log('get?input3=c:'+hut1);
  fetch('get?input3=c:'+hut1)
    .then(function(response) {
      return response.text();
  })
  .then(function(body) {
    document.querySelector('#main').innerHTML = body;
  });
}
function submit_pat()
{
  hut2 = document.getElementById("hut2").value;
 
  console.log('get?input3=p:'+hut2);
  fetch('get?input3=p:'+hut2)
    .then(function(response) {
      return response.text();
  })
  .then(function(body) {
    document.querySelector('#main').innerHTML = body;
  });
}
function submit_Brightness()
{
  b = document.getElementById("b").value;
 
  console.log('get?input3=b:'+b);
  fetch('get?input3=b:'+b)
    .then(function(response) {
      return response.text();
  })
  .then(function(body) {
    document.querySelector('#main').innerHTML = body;
  });
}
</script>
</body></html>
)rawliteral";
