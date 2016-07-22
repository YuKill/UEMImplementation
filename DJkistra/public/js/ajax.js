

window.setInterval(function() {
  $.get("/queue/content", function(data, status) {
    $("#contentDiv").html(function(i, old) {
      return data; 
    }); 
  }); 

  var playerHtml = $("#playerDiv").html().trim();
  if (playerHtml === "") {
    $.get("/queue/player", function(data, status) {
      if (playerHtml != data.trim()) {
        $("#playerDiv").html(function(i, old) {
          return data;
        });
      }
    });
  }
}, 1000);

function remover(Tuple) {
  alert('removendo');
  $.get("/rem/" + Tuple, function(data, status) {
    $("contentDiv").html(function(i, old) {
      return data; 
    }); 
  });
  return false;
}

