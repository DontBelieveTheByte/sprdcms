
<html>
    <head>
        <meta http-equiv="content-type" content="text/html; charset=UTF-8" />
        <title>SprdCMS Login</title>
        <link rel="stylesheet" type="text/css" href="misc/css/cssreset-min.css" />
        <link rel="stylesheet" href="misc/jquery-ui-1.10.2.custom/css/custom-theme/jquery-ui-1.10.2.custom.min.css" type="text/css" />
        <link rel="stylesheet" href="misc/css/admin.css" type="text/css" />
        <link rel="shortcut icon" href="misc/favicon.png">
        <link rel="icon" href="misc/favicon.png">
        <script src="misc/js/jquery-1.9.1.min.js"></script>
        <script src="misc/jquery-ui-1.10.2.custom/js/jquery-ui-1.10.2.custom.min.js"></script>
        <script src="misc/js/ajaxerr.js"></script>
        <script>
            $(document).ready(function() {
                $("button").button();
                $("#login").dialog({
                    modal: true,
                    title: "Login to administer your shop.",
                    resizable: false,
                    draggable: false,
                    width: 475,
                    closeOnEscape: false
                });
                $('button').css("margin-left", "auto");
                $('button').css("margin-right", "auto");
                $('button').css("margin-top", "5px");
                $('button').css("margin-bottom", "0");
                $(".ui-icon-closethick").parent().remove();
            });
        </script>
    </head>
    <body style="width: 100%; height: 100% ;background : #37B;">
        <div id ="login">
            <form align="center" id="loginForm" method="POST" action="index.php?q=login">
                <input type="hidden" name="r" value="login">
                <input class="inputfield" type="text" name="username" placeholder="username"></br>
                <input class="inputfield" type="password" name="password" placeholder="password"></br>
                <button>Login</button>
            </form>
        </div>
    </body>
</html>