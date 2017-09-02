function play() {
  d3.select('#splash').classed('hidden', true);
  d3.select('#game').classed('hidden', false);
}

function reset() {
  d3.select('#splash').classed('hidden', false);
  d3.select('#game').classed('hidden', true);
}

function buttonPress() {
  play();
  setTimeout(reset, 60000);
}

window.onload = function() {  
  var ws = new WebSocket('ws://' + window.location.host + '/ws');
  ws.onclose = function() { 
    setTimeout(function() {  
      window.location.reload();  
    }, 5000);
  }
  ws.onmessage = buttonPress;
};
