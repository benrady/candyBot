function animate() {  
  d3.select('#candy')
    .transition().duration(0)
    .style("fill", "green")
    .transition().duration(3000)
    .style("fill", "yellow")
    .transition().duration(3000)
    .style("fill", "red")
    .transition().duration(3000)
    .style("fill", "green");
};

//setInterval(animate, 10000);
$(function() {  
  animate();
})
