R"(

<!DOCTYPE html>
<html>
  <head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css">
    <link rel="stylesheet" href="//code.jquery.com/ui/1.12.1/themes/base/jquery-ui.css">
  </head>
  <body>
    <div class="container-fluid">
      <div class="col-xs-12" >
        <button id="toggle" type="button" class="btn btn-default" style="width: 100%; margin-top: 2em" onClick='makeAjaxCall("toggle")' > Toggle LED </button>
      </div>
      <div class="col-xs-12" >
        <!-- <input id="brightness" style="width: 100%; margin-top: 2em" type="range" min="10" max="1020"> -->
        <div style="margin: 10px" id="slider"></div>
      </div>
    </div>
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/1.12.4/jquery.min.js"></script>
    <script src="https://code.jquery.com/ui/1.12.0/jquery-ui.min.js"></script>
    <script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js"></script>
    <script> function makeAjaxCall(url){$.ajax({"url": url})}</script>
    <script> 
      $( function() {
         $( "#slider" ).slider({
          value: 200,
          min: 10,
          max: 1020,
          step: 10,
          stop: function( event, ui ) {
            $.ajax({"url": "brightness?level=" + ui.value }) 
          }
      });
      });
    </script>
  </body>
</html>

)"
